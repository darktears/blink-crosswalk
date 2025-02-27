/*
 * Copyright (C) 2004, 2005, 2007 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2008 Rob Buis <buis@kde.org>
 * Copyright (C) 2005, 2007 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2009 Google, Inc.
 * Copyright (C) 2009 Dirk Schulze <krit@webkit.org>
 * Copyright (C) Research In Motion Limited 2010. All rights reserved.
 * Copyright (C) 2009 Jeff Schiller <codedread@gmail.com>
 * Copyright (C) 2011 Renata Hodovan <reni@webkit.org>
 * Copyright (C) 2011 University of Szeged
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "core/rendering/svg/RenderSVGShape.h"

#include "core/rendering/GraphicsContextAnnotator.h"
#include "core/rendering/HitTestRequest.h"
#include "core/rendering/PointerEventsHitRules.h"
#include "core/rendering/svg/RenderSVGResourceMarker.h"
#include "core/rendering/svg/RenderSVGResourceSolidColor.h"
#include "core/rendering/svg/SVGPathData.h"
#include "core/rendering/svg/SVGRenderSupport.h"
#include "core/rendering/svg/SVGRenderingContext.h"
#include "core/rendering/svg/SVGResources.h"
#include "core/rendering/svg/SVGResourcesCache.h"
#include "core/svg/SVGGraphicsElement.h"
#include "platform/geometry/FloatPoint.h"
#include "platform/graphics/GraphicsContextStateSaver.h"
#include "wtf/MathExtras.h"

namespace blink {

RenderSVGShape::RenderSVGShape(SVGGraphicsElement* node)
    : RenderSVGModelObject(node)
    , m_needsBoundariesUpdate(false) // Default is false, the cached rects are empty from the beginning.
    , m_needsShapeUpdate(true) // Default is true, so we grab a Path object once from SVGGraphicsElement.
    , m_needsTransformUpdate(true) // Default is true, so we grab a AffineTransform object once from SVGGraphicsElement.
{
}

RenderSVGShape::~RenderSVGShape()
{
}

void RenderSVGShape::updateShapeFromElement()
{
    m_path.clear();
    m_path = adoptPtr(new Path);
    ASSERT(RenderSVGShape::isShapeEmpty());

    updatePathFromGraphicsElement(toSVGGraphicsElement(element()), path());
    processMarkerPositions();

    m_fillBoundingBox = calculateObjectBoundingBox();
    m_strokeBoundingBox = calculateStrokeBoundingBox();
}

void RenderSVGShape::fillShape(GraphicsContext* context) const
{
    context->fillPath(path());
}

void RenderSVGShape::strokeShape(GraphicsContext* context) const
{
    ASSERT(m_path);
    Path* usePath = m_path.get();

    if (hasNonScalingStroke())
        usePath = nonScalingStrokePath(usePath, nonScalingStrokeTransform());

    context->strokePath(*usePath);
}

bool RenderSVGShape::shapeDependentStrokeContains(const FloatPoint& point)
{
    ASSERT(m_path);
    StrokeData strokeData;
    SVGRenderSupport::applyStrokeStyleToStrokeData(&strokeData, style(), this);

    if (hasNonScalingStroke()) {
        AffineTransform nonScalingTransform = nonScalingStrokeTransform();
        Path* usePath = nonScalingStrokePath(m_path.get(), nonScalingTransform);

        return usePath->strokeContains(nonScalingTransform.mapPoint(point), strokeData);
    }

    return m_path->strokeContains(point, strokeData);
}

bool RenderSVGShape::shapeDependentFillContains(const FloatPoint& point, const WindRule fillRule) const
{
    return path().contains(point, fillRule);
}

bool RenderSVGShape::fillContains(const FloatPoint& point, bool requiresFill, const WindRule fillRule)
{
    if (!m_fillBoundingBox.contains(point))
        return false;

    bool hasFallback;
    if (requiresFill && !RenderSVGResource::fillPaintingResource(this, style(), hasFallback))
        return false;

    return shapeDependentFillContains(point, fillRule);
}

bool RenderSVGShape::strokeContains(const FloatPoint& point, bool requiresStroke)
{
    if (!strokeBoundingBox().contains(point))
        return false;

    bool hasFallback;
    if (requiresStroke && !RenderSVGResource::strokePaintingResource(this, style(), hasFallback))
        return false;

    return shapeDependentStrokeContains(point);
}

void RenderSVGShape::layout()
{
    bool updateCachedBoundariesInParents = false;

    if (m_needsShapeUpdate || m_needsBoundariesUpdate) {
        updateShapeFromElement();
        m_needsShapeUpdate = false;
        updatePaintInvalidationBoundingBox();
        m_needsBoundariesUpdate = false;
        updateCachedBoundariesInParents = true;
    }

    if (m_needsTransformUpdate) {
        m_localTransform =  toSVGGraphicsElement(element())->animatedLocalTransform();
        m_needsTransformUpdate = false;
        updateCachedBoundariesInParents = true;
    }

    // Invalidate all resources of this client if our layout changed.
    if (everHadLayout() && selfNeedsLayout())
        SVGResourcesCache::clientLayoutChanged(this);

    // If our bounds changed, notify the parents.
    if (updateCachedBoundariesInParents)
        RenderSVGModelObject::setNeedsBoundariesUpdate();

    clearNeedsLayout();
}

Path* RenderSVGShape::nonScalingStrokePath(const Path* path, const AffineTransform& strokeTransform) const
{
    DEFINE_STATIC_LOCAL(Path, tempPath, ());

    tempPath = *path;
    tempPath.transform(strokeTransform);

    return &tempPath;
}

bool RenderSVGShape::setupNonScalingStrokeContext(AffineTransform& strokeTransform, GraphicsContextStateSaver& stateSaver)
{
    if (!strokeTransform.isInvertible())
        return false;

    stateSaver.save();
    stateSaver.context()->concatCTM(strokeTransform.inverse());
    return true;
}

AffineTransform RenderSVGShape::nonScalingStrokeTransform() const
{
    return toSVGGraphicsElement(element())->getScreenCTM(SVGGraphicsElement::DisallowStyleUpdate);
}

bool RenderSVGShape::shouldGenerateMarkerPositions() const
{
    if (!style()->svgStyle().hasMarkers())
        return false;

    if (!SVGResources::supportsMarkers(*toSVGGraphicsElement(element())))
        return false;

    SVGResources* resources = SVGResourcesCache::cachedResourcesForRenderObject(this);
    if (!resources)
        return false;

    return resources->markerStart() || resources->markerMid() || resources->markerEnd();
}

void RenderSVGShape::fillShape(RenderStyle* style, GraphicsContext* context)
{
    bool hasFallback;
    if (RenderSVGResource* fillPaintingResource = RenderSVGResource::fillPaintingResource(this, style, hasFallback)) {
        if (fillPaintingResource->applyResource(this, style, context, ApplyToFillMode)) {
            fillPaintingResource->postApplyResource(this, context, ApplyToFillMode, 0, this);
        } else if (hasFallback) {
            RenderSVGResourceSolidColor* fallbackResource = RenderSVGResource::sharedSolidPaintingResource();
            if (fallbackResource->applyResource(this, style, context, ApplyToFillMode))
                fallbackResource->postApplyResource(this, context, ApplyToFillMode, 0, this);
        }
    }
}

void RenderSVGShape::strokeShape(RenderStyle* style, GraphicsContext* context)
{
    bool hasFallback;
    if (RenderSVGResource* strokePaintingResource = RenderSVGResource::strokePaintingResource(this, style, hasFallback)) {
        if (strokePaintingResource->applyResource(this, style, context, ApplyToStrokeMode)) {
            strokePaintingResource->postApplyResource(this, context, ApplyToStrokeMode, 0, this);
        } else if (hasFallback) {
            RenderSVGResourceSolidColor* fallbackResource = RenderSVGResource::sharedSolidPaintingResource();
            if (fallbackResource->applyResource(this, style, context, ApplyToStrokeMode))
                fallbackResource->postApplyResource(this, context, ApplyToStrokeMode, 0, this);
        }
    }
}

void RenderSVGShape::paint(PaintInfo& paintInfo, const LayoutPoint&)
{
    ANNOTATE_GRAPHICS_CONTEXT(paintInfo, this);
    if (paintInfo.phase != PaintPhaseForeground
        || style()->visibility() == HIDDEN
        || isShapeEmpty())
        return;

    FloatRect boundingBox = paintInvalidationRectInLocalCoordinates();
    if (!SVGRenderSupport::paintInfoIntersectsPaintInvalidationRect(boundingBox, m_localTransform, paintInfo))
        return;

    PaintInfo childPaintInfo(paintInfo);

    GraphicsContextStateSaver stateSaver(*childPaintInfo.context);
    childPaintInfo.applyTransform(m_localTransform);

    SVGRenderingContext renderingContext(this, childPaintInfo);

    if (renderingContext.isRenderingPrepared()) {
        const SVGRenderStyle& svgStyle = style()->svgStyle();
        if (svgStyle.shapeRendering() == SR_CRISPEDGES)
            childPaintInfo.context->setShouldAntialias(false);

        for (int i = 0; i < 3; i++) {
            switch (svgStyle.paintOrderType(i)) {
            case PT_FILL:
                fillShape(this->style(), childPaintInfo.context);
                break;
            case PT_STROKE:
                if (svgStyle.hasVisibleStroke()) {
                    GraphicsContextStateSaver stateSaver(*childPaintInfo.context, false);
                    AffineTransform nonScalingTransform;

                    if (hasNonScalingStroke()) {
                        AffineTransform nonScalingTransform = nonScalingStrokeTransform();
                        if (!setupNonScalingStrokeContext(nonScalingTransform, stateSaver))
                            return;
                    }

                    strokeShape(this->style(), childPaintInfo.context);
                }
                break;
            case PT_MARKERS:
                if (!m_markerPositions.isEmpty())
                    drawMarkers(childPaintInfo);
                break;
            default:
                ASSERT_NOT_REACHED();
                break;
            }
        }
    }

    if (style()->outlineWidth())
        paintOutline(childPaintInfo, IntRect(boundingBox));
}

// This method is called from inside paintOutline() since we call paintOutline()
// while transformed to our coord system, return local coords
void RenderSVGShape::addFocusRingRects(Vector<IntRect>& rects, const LayoutPoint&, const RenderLayerModelObject*) const
{
    IntRect rect = enclosingIntRect(paintInvalidationRectInLocalCoordinates());
    if (!rect.isEmpty())
        rects.append(rect);
}

bool RenderSVGShape::nodeAtFloatPoint(const HitTestRequest& request, HitTestResult& result, const FloatPoint& pointInParent, HitTestAction hitTestAction)
{
    // We only draw in the foreground phase, so we only hit-test then.
    if (hitTestAction != HitTestForeground)
        return false;

    FloatPoint localPoint;
    if (!SVGRenderSupport::transformToUserSpaceAndCheckClipping(this, m_localTransform, pointInParent, localPoint))
        return false;

    PointerEventsHitRules hitRules(PointerEventsHitRules::SVG_GEOMETRY_HITTESTING, request, style()->pointerEvents());
    if (nodeAtFloatPointInternal(request, localPoint, hitRules)) {
        updateHitTestResult(result, roundedLayoutPoint(localPoint));
        return true;
    }

    return false;
}

bool RenderSVGShape::nodeAtFloatPointInternal(const HitTestRequest& request, const FloatPoint& localPoint, PointerEventsHitRules hitRules)
{
    bool isVisible = (style()->visibility() == VISIBLE);
    if (isVisible || !hitRules.requireVisible) {
        const SVGRenderStyle& svgStyle = style()->svgStyle();
        WindRule fillRule = svgStyle.fillRule();
        if (request.svgClipContent())
            fillRule = svgStyle.clipRule();
        if ((hitRules.canHitBoundingBox && objectBoundingBox().contains(localPoint))
            || (hitRules.canHitStroke && (svgStyle.hasStroke() || !hitRules.requireStroke) && strokeContains(localPoint, hitRules.requireStroke))
            || (hitRules.canHitFill && (svgStyle.hasFill() || !hitRules.requireFill) && fillContains(localPoint, hitRules.requireFill, fillRule)))
            return true;
    }
    return false;
}

static inline RenderSVGResourceMarker* markerForType(SVGMarkerType type, RenderSVGResourceMarker* markerStart, RenderSVGResourceMarker* markerMid, RenderSVGResourceMarker* markerEnd)
{
    switch (type) {
    case StartMarker:
        return markerStart;
    case MidMarker:
        return markerMid;
    case EndMarker:
        return markerEnd;
    }

    ASSERT_NOT_REACHED();
    return 0;
}

FloatRect RenderSVGShape::markerRect(float strokeWidth) const
{
    ASSERT(!m_markerPositions.isEmpty());

    SVGResources* resources = SVGResourcesCache::cachedResourcesForRenderObject(this);
    ASSERT(resources);

    RenderSVGResourceMarker* markerStart = resources->markerStart();
    RenderSVGResourceMarker* markerMid = resources->markerMid();
    RenderSVGResourceMarker* markerEnd = resources->markerEnd();
    ASSERT(markerStart || markerMid || markerEnd);

    FloatRect boundaries;
    unsigned size = m_markerPositions.size();
    for (unsigned i = 0; i < size; ++i) {
        if (RenderSVGResourceMarker* marker = markerForType(m_markerPositions[i].type, markerStart, markerMid, markerEnd))
            boundaries.unite(marker->markerBoundaries(marker->markerTransformation(m_markerPositions[i].origin, m_markerPositions[i].angle, strokeWidth)));
    }
    return boundaries;
}

FloatRect RenderSVGShape::calculateObjectBoundingBox() const
{
    return path().boundingRect();
}

FloatRect RenderSVGShape::calculateStrokeBoundingBox() const
{
    ASSERT(m_path);
    FloatRect strokeBoundingBox = m_fillBoundingBox;

    if (style()->svgStyle().hasStroke()) {
        StrokeData strokeData;
        SVGRenderSupport::applyStrokeStyleToStrokeData(&strokeData, style(), this);
        if (hasNonScalingStroke()) {
            AffineTransform nonScalingTransform = nonScalingStrokeTransform();
            if (nonScalingTransform.isInvertible()) {
                Path* usePath = nonScalingStrokePath(m_path.get(), nonScalingTransform);
                FloatRect strokeBoundingRect = usePath->strokeBoundingRect(strokeData);
                strokeBoundingRect = nonScalingTransform.inverse().mapRect(strokeBoundingRect);
                strokeBoundingBox.unite(strokeBoundingRect);
            }
        } else {
            strokeBoundingBox.unite(path().strokeBoundingRect(strokeData));
        }
    }

    if (!m_markerPositions.isEmpty())
        strokeBoundingBox.unite(markerRect(strokeWidth()));

    return strokeBoundingBox;
}

void RenderSVGShape::updatePaintInvalidationBoundingBox()
{
    m_paintInvalidationBoundingBox = strokeBoundingBox();
    if (strokeWidth() < 1.0f && !m_paintInvalidationBoundingBox.isEmpty())
        m_paintInvalidationBoundingBox.inflate(1);
    SVGRenderSupport::intersectPaintInvalidationRectWithResources(this, m_paintInvalidationBoundingBox);
}

float RenderSVGShape::strokeWidth() const
{
    SVGLengthContext lengthContext(element());
    return style()->svgStyle().strokeWidth()->value(lengthContext);
}

bool RenderSVGShape::hasSmoothStroke() const
{
    const SVGRenderStyle& svgStyle = style()->svgStyle();
    return svgStyle.strokeDashArray()->isEmpty()
        && svgStyle.strokeMiterLimit() == SVGRenderStyle::initialStrokeMiterLimit()
        && svgStyle.joinStyle() == SVGRenderStyle::initialJoinStyle()
        && svgStyle.capStyle() == SVGRenderStyle::initialCapStyle();
}

void RenderSVGShape::drawMarkers(PaintInfo& paintInfo)
{
    ASSERT(!m_markerPositions.isEmpty());

    SVGResources* resources = SVGResourcesCache::cachedResourcesForRenderObject(this);
    if (!resources)
        return;

    RenderSVGResourceMarker* markerStart = resources->markerStart();
    RenderSVGResourceMarker* markerMid = resources->markerMid();
    RenderSVGResourceMarker* markerEnd = resources->markerEnd();
    if (!markerStart && !markerMid && !markerEnd)
        return;

    float strokeWidth = this->strokeWidth();
    unsigned size = m_markerPositions.size();
    for (unsigned i = 0; i < size; ++i) {
        if (RenderSVGResourceMarker* marker = markerForType(m_markerPositions[i].type, markerStart, markerMid, markerEnd))
            marker->draw(paintInfo, marker->markerTransformation(m_markerPositions[i].origin, m_markerPositions[i].angle, strokeWidth));
    }
}

void RenderSVGShape::processMarkerPositions()
{
    m_markerPositions.clear();

    if (!shouldGenerateMarkerPositions())
        return;

    ASSERT(m_path);

    SVGMarkerData markerData(m_markerPositions);
    m_path->apply(&markerData, SVGMarkerData::updateFromPathElement);
    markerData.pathIsDone();
}

}
