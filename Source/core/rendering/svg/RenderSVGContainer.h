/*
 * Copyright (C) 2004, 2005, 2007 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2007 Rob Buis <buis@kde.org>
 * Copyright (C) 2009 Google, Inc.  All rights reserved.
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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

#ifndef RenderSVGContainer_h
#define RenderSVGContainer_h

#include "core/rendering/svg/RenderSVGModelObject.h"

namespace blink {

class SVGElement;

class RenderSVGContainer : public RenderSVGModelObject {
public:
    explicit RenderSVGContainer(SVGElement*);
    virtual ~RenderSVGContainer();
    virtual void trace(Visitor*) OVERRIDE;

    RenderObject* firstChild() const { ASSERT(children() == virtualChildren()); return children()->firstChild(); }
    RenderObject* lastChild() const { ASSERT(children() == virtualChildren()); return children()->lastChild(); }

    // If you have a RenderSVGContainer, use firstChild or lastChild instead.
    void slowFirstChild() const WTF_DELETED_FUNCTION;
    void slowLastChild() const WTF_DELETED_FUNCTION;

    const RenderObjectChildList* children() const { return &m_children; }
    RenderObjectChildList* children() { return &m_children; }

    virtual void paint(PaintInfo&, const LayoutPoint&) OVERRIDE;
    virtual void setNeedsBoundariesUpdate() OVERRIDE FINAL { m_needsBoundariesUpdate = true; }
    virtual bool didTransformToRootUpdate() { return false; }
    bool isObjectBoundingBoxValid() const { return m_objectBoundingBoxValid; }

protected:
    virtual RenderObjectChildList* virtualChildren() OVERRIDE FINAL { return children(); }
    virtual const RenderObjectChildList* virtualChildren() const OVERRIDE FINAL { return children(); }

    virtual bool isSVGContainer() const OVERRIDE FINAL { return true; }
    virtual const char* renderName() const OVERRIDE { return "RenderSVGContainer"; }

    virtual void layout() OVERRIDE;

    virtual void addChild(RenderObject* child, RenderObject* beforeChild = 0) OVERRIDE FINAL;
    virtual void removeChild(RenderObject*) OVERRIDE FINAL;
    virtual void addFocusRingRects(Vector<IntRect>&, const LayoutPoint& additionalOffset, const RenderLayerModelObject* paintContainer = 0) const OVERRIDE FINAL;

    virtual FloatRect objectBoundingBox() const OVERRIDE FINAL { return m_objectBoundingBox; }
    virtual FloatRect strokeBoundingBox() const OVERRIDE FINAL { return m_strokeBoundingBox; }
    virtual FloatRect paintInvalidationRectInLocalCoordinates() const OVERRIDE FINAL { return m_paintInvalidationBoundingBox; }

    virtual bool nodeAtFloatPoint(const HitTestRequest&, HitTestResult&, const FloatPoint& pointInParent, HitTestAction) OVERRIDE;

    // Allow RenderSVGTransformableContainer to hook in at the right time in layout()
    virtual bool calculateLocalTransform() { return false; }

    // Allow RenderSVGViewportContainer to hook in at the right times in layout(), paint() and nodeAtFloatPoint()
    virtual void calcViewport() { }
    virtual void applyViewportClip(PaintInfo&) { }
    virtual bool pointIsInsideViewportClip(const FloatPoint& /*pointInParent*/) { return true; }

    virtual void determineIfLayoutSizeChanged() { }

    bool selfWillPaint();
    void updateCachedBoundaries();

private:
    RenderObjectChildList m_children;
    FloatRect m_objectBoundingBox;
    bool m_objectBoundingBoxValid;
    FloatRect m_strokeBoundingBox;
    FloatRect m_paintInvalidationBoundingBox;
    bool m_needsBoundariesUpdate : 1;
};

DEFINE_RENDER_OBJECT_TYPE_CASTS(RenderSVGContainer, isSVGContainer());

} // namespace blink

#endif // RenderSVGContainer_h
