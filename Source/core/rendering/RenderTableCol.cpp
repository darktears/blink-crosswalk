/*
 * Copyright (C) 1997 Martin Jones (mjones@kde.org)
 *           (C) 1997 Torben Weis (weis@kde.org)
 *           (C) 1998 Waldo Bastian (bastian@kde.org)
 *           (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
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
#include "core/rendering/RenderTableCol.h"

#include "core/HTMLNames.h"
#include "core/html/HTMLTableColElement.h"
#include "core/rendering/RenderTable.h"
#include "core/rendering/RenderTableCell.h"

namespace blink {

using namespace HTMLNames;

RenderTableCol::RenderTableCol(Element* element)
    : RenderBox(element)
    , m_span(1)
{
    // init RenderObject attributes
    setInline(true); // our object is not Inline
    updateFromElement();
}

void RenderTableCol::trace(Visitor* visitor)
{
    visitor->trace(m_children);
    RenderBox::trace(visitor);
}

void RenderTableCol::styleDidChange(StyleDifference diff, const RenderStyle* oldStyle)
{
    RenderBox::styleDidChange(diff, oldStyle);

    // If border was changed, notify table.
    if (parent()) {
        RenderTable* table = this->table();
        if (table && !table->selfNeedsLayout() && !table->normalChildNeedsLayout() && oldStyle && oldStyle->border() != style()->border())
            table->invalidateCollapsedBorders();
    }
}

void RenderTableCol::updateFromElement()
{
    unsigned oldSpan = m_span;
    Node* n = node();
    if (isHTMLTableColElement(n)) {
        HTMLTableColElement& tc = toHTMLTableColElement(*n);
        m_span = tc.span();
    } else
        m_span = !(style() && style()->display() == TABLE_COLUMN_GROUP);
    if (m_span != oldSpan && style() && parent())
        setNeedsLayoutAndPrefWidthsRecalcAndFullPaintInvalidation();
}

void RenderTableCol::insertedIntoTree()
{
    RenderBox::insertedIntoTree();
    table()->addColumn(this);
}

void RenderTableCol::willBeRemovedFromTree()
{
    RenderBox::willBeRemovedFromTree();
    table()->removeColumn(this);
}

bool RenderTableCol::isChildAllowed(RenderObject* child, RenderStyle* style) const
{
    // We cannot use isTableColumn here as style() may return 0.
    return child->isRenderTableCol() && style->display() == TABLE_COLUMN;
}

bool RenderTableCol::canHaveChildren() const
{
    // Cols cannot have children. This is actually necessary to fix a bug
    // with libraries.uc.edu, which makes a <p> be a table-column.
    return isTableColumnGroup();
}

LayoutRect RenderTableCol::clippedOverflowRectForPaintInvalidation(const RenderLayerModelObject* paintInvalidationContainer, const PaintInvalidationState* paintInvalidationState) const
{
    // For now, just repaint the whole table.
    // FIXME: Find a better way to do this, e.g., need to repaint all the cells that we
    // might have propagated a background color or borders into.
    // FIXME: check for paintInvalidationContainer each time here?

    RenderTable* parentTable = table();
    if (!parentTable)
        return LayoutRect();
    return parentTable->clippedOverflowRectForPaintInvalidation(paintInvalidationContainer, paintInvalidationState);
}

void RenderTableCol::imageChanged(WrappedImagePtr, const IntRect*)
{
    // FIXME: Repaint only the rect the image paints in.
    paintInvalidationForWholeRenderer();
}

void RenderTableCol::clearPreferredLogicalWidthsDirtyBits()
{
    clearPreferredLogicalWidthsDirty();

    for (RenderObject* child = firstChild(); child; child = child->nextSibling())
        child->clearPreferredLogicalWidthsDirty();
}

RenderTable* RenderTableCol::table() const
{
    RenderObject* table = parent();
    if (table && !table->isTable())
        table = table->parent();
    return table && table->isTable() ? toRenderTable(table) : 0;
}

RenderTableCol* RenderTableCol::enclosingColumnGroup() const
{
    if (!parent()->isRenderTableCol())
        return 0;

    RenderTableCol* parentColumnGroup = toRenderTableCol(parent());
    ASSERT(parentColumnGroup->isTableColumnGroup());
    ASSERT(isTableColumn());
    return parentColumnGroup;
}

RenderTableCol* RenderTableCol::nextColumn() const
{
    // If |this| is a column-group, the next column is the colgroup's first child column.
    if (RenderObject* firstChild = this->firstChild())
        return toRenderTableCol(firstChild);

    // Otherwise it's the next column along.
    RenderObject* next = nextSibling();

    // Failing that, the child is the last column in a column-group, so the next column is the next column/column-group after its column-group.
    if (!next && parent()->isRenderTableCol())
        next = parent()->nextSibling();

    for (; next && !next->isRenderTableCol(); next = next->nextSibling()) { }

    return toRenderTableCol(next);
}

const BorderValue& RenderTableCol::borderAdjoiningCellStartBorder(const RenderTableCell*) const
{
    return style()->borderStart();
}

const BorderValue& RenderTableCol::borderAdjoiningCellEndBorder(const RenderTableCell*) const
{
    return style()->borderEnd();
}

const BorderValue& RenderTableCol::borderAdjoiningCellBefore(const RenderTableCell* cell) const
{
    ASSERT_UNUSED(cell, table()->colElement(cell->col() + cell->colSpan()) == this);
    return style()->borderStart();
}

const BorderValue& RenderTableCol::borderAdjoiningCellAfter(const RenderTableCell* cell) const
{
    ASSERT_UNUSED(cell, table()->colElement(cell->col() - 1) == this);
    return style()->borderEnd();
}

}
