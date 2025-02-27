/*
 * Copyright (C) Research In Motion Limited 2010. All rights reserved.
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

#include "core/rendering/svg/RenderSVGResourceSolidColor.h"

#include "core/frame/FrameView.h"
#include "core/frame/LocalFrame.h"
#include "core/rendering/style/RenderStyle.h"
#include "core/rendering/svg/RenderSVGShape.h"
#include "core/rendering/svg/SVGRenderSupport.h"
#include "platform/graphics/GraphicsContext.h"

namespace blink {

const RenderSVGResourceType RenderSVGResourceSolidColor::s_resourceType = SolidColorResourceType;

RenderSVGResourceSolidColor::RenderSVGResourceSolidColor()
{
}

RenderSVGResourceSolidColor::~RenderSVGResourceSolidColor()
{
}

bool RenderSVGResourceSolidColor::applyResource(RenderObject* object, RenderStyle* style, GraphicsContext*& context, unsigned short resourceMode)
{
    // We are NOT allowed to ASSERT(object) here, unlike all other resources.
    // RenderSVGResourceSolidColor is the only resource which may be used from HTML, when rendering
    // SVG Fonts for a HTML document. This will be indicated by a null RenderObject pointer.
    ASSERT(context);
    ASSERT(resourceMode != ApplyToDefaultMode);

    const SVGRenderStyle* svgStyle = style ? &style->svgStyle() : 0;

    bool isRenderingMask = false;
    if (object->frame() && object->frame()->view())
        isRenderingMask = object->frame()->view()->paintBehavior() & PaintBehaviorRenderingSVGMask;

    if (resourceMode & ApplyToFillMode) {
        if (!isRenderingMask && svgStyle)
            context->setAlphaAsFloat(svgStyle->fillOpacity());
        else
            context->setAlphaAsFloat(1);
        context->setFillColor(m_color);
        if (!isRenderingMask)
            context->setFillRule(svgStyle ? svgStyle->fillRule() : RULE_NONZERO);

        if (resourceMode & ApplyToTextMode)
            context->setTextDrawingMode(TextModeFill);
    } else if (resourceMode & ApplyToStrokeMode) {
        // When rendering the mask for a RenderSVGResourceClipper, the stroke code path is never hit.
        ASSERT(!isRenderingMask);
        context->setAlphaAsFloat(svgStyle ? svgStyle->strokeOpacity() : 1);
        context->setStrokeColor(m_color);

        if (style)
            SVGRenderSupport::applyStrokeStyleToContext(context, style, object);

        if (resourceMode & ApplyToTextMode)
            context->setTextDrawingMode(TextModeStroke);
    }

    return true;
}

void RenderSVGResourceSolidColor::postApplyResource(RenderObject*, GraphicsContext*& context, unsigned short resourceMode, const Path* path, const RenderSVGShape* shape)
{
    ASSERT(context);
    ASSERT(resourceMode != ApplyToDefaultMode);

    if (resourceMode & ApplyToFillMode) {
        if (path)
            context->fillPath(*path);
        else if (shape)
            shape->fillShape(context);
    }
    if (resourceMode & ApplyToStrokeMode) {
        if (path)
            context->strokePath(*path);
        else if (shape)
            shape->strokeShape(context);
    }
}

}
