/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2010 Apple Inc. All rights reserved.
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

#ifndef HTMLLabelElement_h
#define HTMLLabelElement_h

#include "core/html/HTMLElement.h"
#include "core/html/LabelableElement.h"

namespace blink {

class HTMLLabelElement FINAL : public HTMLElement {
public:
    DECLARE_NODE_FACTORY(HTMLLabelElement);

    LabelableElement* control() const;

    virtual bool willRespondToMouseClickEvents() OVERRIDE;

    virtual HTMLFormElement* formOwner() const OVERRIDE;

private:
    explicit HTMLLabelElement(Document&);
    bool isInInteractiveContent(Node*) const;

    virtual bool rendererIsFocusable() const OVERRIDE;
    virtual bool isInteractiveContent() const OVERRIDE;
    virtual void accessKeyAction(bool sendMouseEvents) OVERRIDE;

    virtual void attributeWillChange(const QualifiedName&, const AtomicString& oldValue, const AtomicString& newValue) OVERRIDE;
    virtual InsertionNotificationRequest insertedInto(ContainerNode*) OVERRIDE;
    virtual void removedFrom(ContainerNode*) OVERRIDE;

    // Overridden to update the hover/active state of the corresponding control.
    virtual void setActive(bool = true) OVERRIDE;
    virtual void setHovered(bool = true) OVERRIDE;

    // Overridden to either click() or focus() the corresponding control.
    virtual void defaultEventHandler(Event*) OVERRIDE;

    virtual void focus(bool restorePreviousSelection, FocusType) OVERRIDE;

    void updateLabel(TreeScope&, const AtomicString& oldForAttributeValue, const AtomicString& newForAttributeValue);
};

} //namespace

#endif
