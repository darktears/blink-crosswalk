/*
 * Copyright (C) 2006 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) Research In Motion Limited 2011. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "core/svg/graphics/SVGImage.h"

#include "core/animation/AnimationTimeline.h"
#include "core/dom/NodeTraversal.h"
#include "core/dom/shadow/ComposedTreeWalker.h"
#include "core/frame/FrameView.h"
#include "core/frame/LocalFrame.h"
#include "core/frame/Settings.h"
#include "core/loader/FrameLoadRequest.h"
#include "core/page/Chrome.h"
#include "core/rendering/style/RenderStyle.h"
#include "core/rendering/svg/RenderSVGRoot.h"
#include "core/svg/SVGDocumentExtensions.h"
#include "core/svg/SVGFEImageElement.h"
#include "core/svg/SVGImageElement.h"
#include "core/svg/SVGSVGElement.h"
#include "core/svg/animation/SMILTimeContainer.h"
#include "core/svg/graphics/SVGImageChromeClient.h"
#include "platform/EventDispatchForbiddenScope.h"
#include "platform/LengthFunctions.h"
#include "platform/TraceEvent.h"
#include "platform/geometry/IntRect.h"
#include "platform/graphics/GraphicsContextStateSaver.h"
#include "platform/graphics/ImageBuffer.h"
#include "platform/graphics/ImageObserver.h"
#include "wtf/PassRefPtr.h"

namespace blink {

SVGImage::SVGImage(ImageObserver* observer)
    : Image(observer)
{
}

SVGImage::~SVGImage()
{
    if (m_page) {
        // Store m_page in a local variable, clearing m_page, so that SVGImageChromeClient knows we're destructed.
        OwnPtrWillBeRawPtr<Page> currentPage = m_page.release();
        // Break both the loader and view references to the frame
        currentPage->willBeDestroyed();
    }

    // Verify that page teardown destroyed the Chrome
    ASSERT(!m_chromeClient || !m_chromeClient->image());
}

bool SVGImage::isInSVGImage(const Node* node)
{
    ASSERT(node);

    Page* page = node->document().page();
    if (!page)
        return false;

    return page->chrome().client().isSVGImageChromeClient();
}

bool SVGImage::currentFrameHasSingleSecurityOrigin() const
{
    if (!m_page)
        return true;

    LocalFrame* frame = toLocalFrame(m_page->mainFrame());

    RELEASE_ASSERT(frame->document()->loadEventFinished());

    SVGSVGElement* rootElement = frame->document()->accessSVGExtensions().rootElement();
    if (!rootElement)
        return true;

    // Don't allow foreignObject elements or images that are not known to be
    // single-origin since these can leak cross-origin information.
    ComposedTreeWalker walker(rootElement);
    while (Node* node = walker.get()) {
        if (isSVGForeignObjectElement(*node))
            return false;
        if (isSVGImageElement(*node)) {
            if (!toSVGImageElement(*node).currentFrameHasSingleSecurityOrigin())
                return false;
        } else if (isSVGFEImageElement(*node)) {
            if (!toSVGFEImageElement(*node).currentFrameHasSingleSecurityOrigin())
                return false;
        }
        walker.next();
    }

    // Because SVG image rendering disallows external resources and links, these
    // images effectively are restricted to a single security origin.
    return true;
}

static SVGSVGElement* svgRootElement(Page* page)
{
    if (!page)
        return 0;
    LocalFrame* frame = toLocalFrame(page->mainFrame());
    return frame->document()->accessSVGExtensions().rootElement();
}

void SVGImage::setContainerSize(const IntSize& size)
{
    if (!usesContainerSize())
        return;

    SVGSVGElement* rootElement = svgRootElement(m_page.get());
    if (!rootElement)
        return;

    FrameView* view = frameView();
    view->resize(this->containerSize());

    RenderSVGRoot* renderer = toRenderSVGRoot(rootElement->renderer());
    if (!renderer)
        return;
    renderer->setContainerSize(size);
}

IntSize SVGImage::containerSize() const
{
    SVGSVGElement* rootElement = svgRootElement(m_page.get());
    if (!rootElement)
        return IntSize();

    RenderSVGRoot* renderer = toRenderSVGRoot(rootElement->renderer());
    if (!renderer)
        return IntSize();

    // If a container size is available it has precedence.
    IntSize containerSize = renderer->containerSize();
    if (!containerSize.isEmpty())
        return containerSize;

    // Assure that a container size is always given for a non-identity zoom level.
    ASSERT(renderer->style()->effectiveZoom() == 1);

    FloatSize intrinsicSize;
    double intrinsicRatio = 0;
    renderer->computeIntrinsicRatioInformation(intrinsicSize, intrinsicRatio);

    if (intrinsicSize.isEmpty() && intrinsicRatio) {
        if (!intrinsicSize.width() && intrinsicSize.height())
            intrinsicSize.setWidth(intrinsicSize.height() * intrinsicRatio);
        else if (intrinsicSize.width() && !intrinsicSize.height())
            intrinsicSize.setHeight(intrinsicSize.width() / intrinsicRatio);
    }

    if (!intrinsicSize.isEmpty())
        return expandedIntSize(intrinsicSize);

    // As last resort, use CSS replaced element fallback size.
    return IntSize(300, 150);
}

void SVGImage::drawForContainer(GraphicsContext* context, const FloatSize containerSize, float zoom, const FloatRect& dstRect,
    const FloatRect& srcRect, CompositeOperator compositeOp, blink::WebBlendMode blendMode)
{
    if (!m_page)
        return;

    // Temporarily disable the image observer to prevent changeInRect() calls due re-laying out the image.
    ImageObserverDisabler imageObserverDisabler(this);

    IntSize roundedContainerSize = roundedIntSize(containerSize);
    setContainerSize(roundedContainerSize);

    FloatRect scaledSrc = srcRect;
    scaledSrc.scale(1 / zoom);

    // Compensate for the container size rounding by adjusting the source rect.
    FloatSize adjustedSrcSize = scaledSrc.size();
    adjustedSrcSize.scale(roundedContainerSize.width() / containerSize.width(), roundedContainerSize.height() / containerSize.height());
    scaledSrc.setSize(adjustedSrcSize);

    draw(context, dstRect, scaledSrc, compositeOp, blendMode);
}

PassRefPtr<NativeImageSkia> SVGImage::nativeImageForCurrentFrame()
{
    if (!m_page)
        return nullptr;

    OwnPtr<ImageBuffer> buffer = ImageBuffer::create(size());
    if (!buffer)
        return nullptr;

    drawForContainer(buffer->context(), size(), 1, rect(), rect(), CompositeSourceOver, blink::WebBlendModeNormal);

    // FIXME: WK(Bug 113657): We should use DontCopyBackingStore here.
    return buffer->copyImage(CopyBackingStore)->nativeImageForCurrentFrame();
}

void SVGImage::drawPatternForContainer(GraphicsContext* context, const FloatSize containerSize, float zoom, const FloatRect& srcRect,
    const FloatSize& scale, const FloatPoint& phase, CompositeOperator compositeOp, const FloatRect& dstRect, blink::WebBlendMode blendMode, const IntSize& repeatSpacing)
{
    FloatRect zoomedContainerRect = FloatRect(FloatPoint(), containerSize);
    zoomedContainerRect.scale(zoom);

    // The ImageBuffer size needs to be scaled to match the final resolution.
    // FIXME: No need to get the full CTM here, we just need the scale.
    // FIXME: See crbug.com/382491. This scale does not reflect compositor applied
    // scale factors, such a High DPI or device zoom.
    AffineTransform transform = context->getCTM();
    FloatSize imageBufferScale = FloatSize(transform.xScale(), transform.yScale());
    ASSERT(imageBufferScale.width());
    ASSERT(imageBufferScale.height());

    FloatSize scaleWithoutCTM(scale.width() / imageBufferScale.width(), scale.height() / imageBufferScale.height());

    FloatRect imageBufferSize = zoomedContainerRect;
    imageBufferSize.scale(imageBufferScale.width(), imageBufferScale.height());

    OwnPtr<ImageBuffer> buffer = ImageBuffer::create(expandedIntSize(imageBufferSize.size()));
    if (!buffer) // Failed to allocate buffer.
        return;

    drawForContainer(buffer->context(), containerSize, zoom, imageBufferSize, zoomedContainerRect, CompositeSourceOver, blink::WebBlendModeNormal);
    RefPtr<Image> image = buffer->copyImage(DontCopyBackingStore, Unscaled);

    // Adjust the source rect and transform due to the image buffer's scaling.
    FloatRect scaledSrcRect = srcRect;
    scaledSrcRect.scale(imageBufferScale.width(), imageBufferScale.height());

    image->drawPattern(context, scaledSrcRect, scaleWithoutCTM, phase, compositeOp, dstRect, blendMode, repeatSpacing);
}

void SVGImage::draw(GraphicsContext* context, const FloatRect& dstRect, const FloatRect& srcRect, CompositeOperator compositeOp, blink::WebBlendMode blendMode)
{
    if (!m_page)
        return;

    GraphicsContextStateSaver stateSaver(*context);
    context->setCompositeOperation(compositeOp, blendMode);
    context->clip(enclosingIntRect(dstRect));

    bool compositingRequiresTransparencyLayer = compositeOp != CompositeSourceOver || blendMode != blink::WebBlendModeNormal;
    float opacity = context->getNormalizedAlpha() / 255.f;
    bool requiresTransparencyLayer = compositingRequiresTransparencyLayer || opacity < 1;
    if (requiresTransparencyLayer) {
        context->beginTransparencyLayer(opacity);
        if (compositingRequiresTransparencyLayer)
            context->setCompositeOperation(CompositeSourceOver, blink::WebBlendModeNormal);
    }

    FloatSize scale(dstRect.width() / srcRect.width(), dstRect.height() / srcRect.height());

    // We can only draw the entire frame, clipped to the rect we want. So compute where the top left
    // of the image would be if we were drawing without clipping, and translate accordingly.
    FloatSize topLeftOffset(srcRect.location().x() * scale.width(), srcRect.location().y() * scale.height());
    FloatPoint destOffset = dstRect.location() - topLeftOffset;

    context->translate(destOffset.x(), destOffset.y());
    context->scale(scale.width(), scale.height());

    FrameView* view = frameView();
    view->resize(containerSize());

    if (!m_url.isEmpty())
        view->scrollToFragment(m_url);

    view->updateLayoutAndStyleForPainting();
    view->paint(context, enclosingIntRect(srcRect));
    ASSERT(!view->needsLayout());

    if (requiresTransparencyLayer)
        context->endLayer();

    stateSaver.restore();

    if (imageObserver())
        imageObserver()->didDraw(this);

    // Start any (SMIL) animations if needed. This will restart or continue
    // animations if preceded by calls to resetAnimation or stopAnimation
    // respectively.
    startAnimation();
}

RenderBox* SVGImage::embeddedContentBox() const
{
    SVGSVGElement* rootElement = svgRootElement(m_page.get());
    if (!rootElement)
        return 0;
    return toRenderBox(rootElement->renderer());
}

FrameView* SVGImage::frameView() const
{
    if (!m_page)
        return 0;

    return toLocalFrame(m_page->mainFrame())->view();
}

void SVGImage::computeIntrinsicDimensions(Length& intrinsicWidth, Length& intrinsicHeight, FloatSize& intrinsicRatio)
{
    SVGSVGElement* rootElement = svgRootElement(m_page.get());
    if (!rootElement)
        return;

    intrinsicWidth = rootElement->intrinsicWidth();
    intrinsicHeight = rootElement->intrinsicHeight();
    if (rootElement->preserveAspectRatio()->currentValue()->align() == SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_NONE)
        return;

    intrinsicRatio = rootElement->viewBox()->currentValue()->value().size();
    if (intrinsicRatio.isEmpty() && intrinsicWidth.isFixed() && intrinsicHeight.isFixed())
        intrinsicRatio = FloatSize(floatValueForLength(intrinsicWidth, 0), floatValueForLength(intrinsicHeight, 0));
}

// FIXME: support CatchUpAnimation = CatchUp.
void SVGImage::startAnimation(CatchUpAnimation)
{
    SVGSVGElement* rootElement = svgRootElement(m_page.get());
    if (!rootElement || !rootElement->animationsPaused())
        return;
    rootElement->unpauseAnimations();
}

void SVGImage::stopAnimation()
{
    SVGSVGElement* rootElement = svgRootElement(m_page.get());
    if (!rootElement)
        return;
    rootElement->pauseAnimations();
}

void SVGImage::resetAnimation()
{
    SVGSVGElement* rootElement = svgRootElement(m_page.get());
    if (!rootElement)
        return;
    rootElement->pauseAnimations();
    rootElement->setCurrentTime(0);
}

bool SVGImage::hasAnimations() const
{
    SVGSVGElement* rootElement = svgRootElement(m_page.get());
    if (!rootElement)
        return false;
    return rootElement->timeContainer()->hasAnimations() || toLocalFrame(m_page->mainFrame())->document()->timeline().hasPendingUpdates();
}

bool SVGImage::dataChanged(bool allDataReceived)
{
    TRACE_EVENT0("blink", "SVGImage::dataChanged");

    // Don't do anything if is an empty image.
    if (!data()->size())
        return true;

    if (allDataReceived) {
        // SVGImage will fire events (and the default C++ handlers run) but doesn't
        // actually allow script to run so it's fine to call into it. We allow this
        // since it means an SVG data url can synchronously load like other image
        // types.
        EventDispatchForbiddenScope::AllowUserAgentEvents allowUserAgentEvents;

        static FrameLoaderClient* dummyFrameLoaderClient = new EmptyFrameLoaderClient;

        Page::PageClients pageClients;
        fillWithEmptyClients(pageClients);
        m_chromeClient = adoptPtr(new SVGImageChromeClient(this));
        pageClients.chromeClient = m_chromeClient.get();

        // FIXME: If this SVG ends up loading itself, we might leak the world.
        // The Cache code does not know about ImageResources holding Frames and
        // won't know to break the cycle.
        // This will become an issue when SVGImage will be able to load other
        // SVGImage objects, but we're safe now, because SVGImage can only be
        // loaded by a top-level document.
        OwnPtrWillBeRawPtr<Page> page;
        {
            TRACE_EVENT0("blink", "SVGImage::dataChanged::createPage");
            page = adoptPtrWillBeNoop(new Page(pageClients));
            page->settings().setScriptEnabled(false);
            page->settings().setPluginsEnabled(false);
            page->settings().setAcceleratedCompositingEnabled(false);
        }

        RefPtr<LocalFrame> frame;
        {
            TRACE_EVENT0("blink", "SVGImage::dataChanged::createFrame");
            frame = LocalFrame::create(dummyFrameLoaderClient, &page->frameHost(), 0);
            frame->setView(FrameView::create(frame.get()));
            frame->init();
        }

        FrameLoader& loader = frame->loader();
        loader.forceSandboxFlags(SandboxAll);

        frame->view()->setScrollbarsSuppressed(true);
        frame->view()->setCanHaveScrollbars(false); // SVG Images will always synthesize a viewBox, if it's not available, and thus never see scrollbars.
        frame->view()->setTransparent(true); // SVG Images are transparent.

        m_page = page.release();

        TRACE_EVENT0("blink", "SVGImage::dataChanged::load");
        loader.load(FrameLoadRequest(0, blankURL(), SubstituteData(data(), "image/svg+xml", "UTF-8", KURL(), ForceSynchronousLoad)));
        // Set the intrinsic size before a container size is available.
        m_intrinsicSize = containerSize();
    }

    return m_page;
}

String SVGImage::filenameExtension() const
{
    return "svg";
}

}
