/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2009 Apple Inc. All rights reserved.
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
 *
 */

#ifndef RenderListMarker_h
#define RenderListMarker_h

#include "core/rendering/RenderBox.h"

namespace blink {

class RenderListItem;

String listMarkerText(EListStyleType, int value);

// Used to render the list item's marker.
// The RenderListMarker always has to be a child of a RenderListItem.
class RenderListMarker FINAL : public RenderBox {
public:
    static RenderListMarker* createAnonymous(RenderListItem*);

    virtual ~RenderListMarker();
    virtual void trace(Visitor*) OVERRIDE;

    const String& text() const { return m_text; }
    String suffix() const;

    bool isInside() const;

    void updateMarginsAndContent();

private:
    RenderListMarker(RenderListItem*);

    virtual const char* renderName() const OVERRIDE { return "RenderListMarker"; }
    virtual void computePreferredLogicalWidths() OVERRIDE;

    virtual bool isListMarker() const OVERRIDE { return true; }

    virtual void paint(PaintInfo&, const LayoutPoint&) OVERRIDE;

    virtual void layout() OVERRIDE;

    virtual void imageChanged(WrappedImagePtr, const IntRect* = 0) OVERRIDE;

    virtual InlineBox* createInlineBox() OVERRIDE;

    virtual LayoutUnit lineHeight(bool firstLine, LineDirectionMode, LinePositionMode = PositionOnContainingLine) const OVERRIDE;
    virtual int baselinePosition(FontBaseline, bool firstLine, LineDirectionMode, LinePositionMode = PositionOnContainingLine) const OVERRIDE;

    virtual bool isImage() const OVERRIDE;
    bool isText() const { return !isImage(); }

    virtual void setSelectionState(SelectionState) OVERRIDE;
    virtual LayoutRect selectionRectForPaintInvalidation(const RenderLayerModelObject* paintInvalidationContainer, bool clipToVisibleContent = true) OVERRIDE;
    virtual bool canBeSelectionLeaf() const OVERRIDE { return true; }

    void updateMargins();
    void updateContent();

    virtual void styleWillChange(StyleDifference, const RenderStyle& newStyle) OVERRIDE;
    virtual void styleDidChange(StyleDifference, const RenderStyle* oldStyle) OVERRIDE;

    IntRect getRelativeMarkerRect();
    LayoutRect localSelectionRect();

    String m_text;
    RefPtr<StyleImage> m_image;
    RawPtrWillBeMember<RenderListItem> m_listItem;
};

DEFINE_RENDER_OBJECT_TYPE_CASTS(RenderListMarker, isListMarker());

} // namespace blink

#endif // RenderListMarker_h
