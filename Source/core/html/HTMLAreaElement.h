/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2004, 2008, 2009, 2011 Apple Inc. All rights reserved.
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

#ifndef HTMLAreaElement_h
#define HTMLAreaElement_h

#include "core/html/HTMLAnchorElement.h"
#include "platform/geometry/LayoutRect.h"

namespace blink {

class HitTestResult;
class HTMLImageElement;
class Path;

class HTMLAreaElement FINAL : public HTMLAnchorElement {
public:
    DECLARE_NODE_FACTORY(HTMLAreaElement);

    bool isDefault() const { return m_shape == Default; }

    bool mapMouseEvent(LayoutPoint location, const LayoutSize&, HitTestResult&);

    LayoutRect computeRect(RenderObject*) const;
    Path computePath(RenderObject*) const;

    // The parent map's image.
    HTMLImageElement* imageElement() const;

private:
    explicit HTMLAreaElement(Document&);

    virtual void parseAttribute(const QualifiedName&, const AtomicString&) OVERRIDE;
    virtual bool isKeyboardFocusable() const OVERRIDE;
    virtual bool isMouseFocusable() const OVERRIDE;
    virtual bool rendererIsFocusable() const OVERRIDE;
    virtual void updateFocusAppearance(bool /*restorePreviousSelection*/) OVERRIDE;
    virtual void setFocus(bool) OVERRIDE;

    enum Shape { Default, Poly, Rect, Circle, Unknown };
    Path getRegion(const LayoutSize&) const;
    void invalidateCachedRegion();

    OwnPtr<Path> m_region;
    Vector<Length> m_coords;
    LayoutSize m_lastSize;
    Shape m_shape;
};

} //namespace

#endif
