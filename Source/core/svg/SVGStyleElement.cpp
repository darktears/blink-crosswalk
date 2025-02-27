/*
 * Copyright (C) 2004, 2005 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2007 Rob Buis <buis@kde.org>
 * Copyright (C) 2006 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Cameron McCormack <cam@mcc.id.au>
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
#include "core/svg/SVGStyleElement.h"

#include "core/css/CSSStyleSheet.h"
#include "wtf/StdLibExtras.h"

namespace blink {

inline SVGStyleElement::SVGStyleElement(Document& document, bool createdByParser)
    : SVGElement(SVGNames::styleTag, document)
    , StyleElement(&document, createdByParser)
    , m_svgLoadEventTimer(this, &SVGElement::svgLoadEventTimerFired)
{
    ScriptWrappable::init(this);
}

SVGStyleElement::~SVGStyleElement()
{
#if !ENABLE(OILPAN)
    StyleElement::clearDocumentData(document(), this);
#endif
}

PassRefPtrWillBeRawPtr<SVGStyleElement> SVGStyleElement::create(Document& document, bool createdByParser)
{
    return adoptRefWillBeNoop(new SVGStyleElement(document, createdByParser));
}

bool SVGStyleElement::disabled() const
{
    if (!m_sheet)
        return false;

    return m_sheet->disabled();
}

void SVGStyleElement::setDisabled(bool setDisabled)
{
    if (CSSStyleSheet* styleSheet = sheet())
        styleSheet->setDisabled(setDisabled);
}

const AtomicString& SVGStyleElement::type() const
{
    DEFINE_STATIC_LOCAL(const AtomicString, defaultValue, ("text/css", AtomicString::ConstructFromLiteral));
    const AtomicString& n = getAttribute(SVGNames::typeAttr);
    return n.isNull() ? defaultValue : n;
}

void SVGStyleElement::setType(const AtomicString& type)
{
    setAttribute(SVGNames::typeAttr, type);
}

const AtomicString& SVGStyleElement::media() const
{
    DEFINE_STATIC_LOCAL(const AtomicString, defaultValue, ("all", AtomicString::ConstructFromLiteral));
    const AtomicString& n = fastGetAttribute(SVGNames::mediaAttr);
    return n.isNull() ? defaultValue : n;
}

void SVGStyleElement::setMedia(const AtomicString& media)
{
    setAttribute(SVGNames::mediaAttr, media);
}

String SVGStyleElement::title() const
{
    return fastGetAttribute(SVGNames::titleAttr);
}

void SVGStyleElement::setTitle(const AtomicString& title)
{
    setAttribute(SVGNames::titleAttr, title);
}

bool SVGStyleElement::isSupportedAttribute(const QualifiedName& attrName)
{
    DEFINE_STATIC_LOCAL(HashSet<QualifiedName>, supportedAttributes, ());
    if (supportedAttributes.isEmpty())
        supportedAttributes.add(SVGNames::titleAttr);
    return supportedAttributes.contains<SVGAttributeHashTranslator>(attrName);
}

void SVGStyleElement::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    if (!isSupportedAttribute(name)) {
        SVGElement::parseAttribute(name, value);
        return;
    }

    if (name == SVGNames::titleAttr) {
        if (m_sheet)
            m_sheet->setTitle(value);
        return;
    }

    ASSERT_NOT_REACHED();
}

void SVGStyleElement::finishParsingChildren()
{
    StyleElement::finishParsingChildren(this);
    SVGElement::finishParsingChildren();
}

Node::InsertionNotificationRequest SVGStyleElement::insertedInto(ContainerNode* rootParent)
{
    SVGElement::insertedInto(rootParent);
    return InsertionShouldCallDidNotifySubtreeInsertions;
}

void SVGStyleElement::didNotifySubtreeInsertionsToDocument()
{
    StyleElement::processStyleSheet(document(), this);
}

void SVGStyleElement::removedFrom(ContainerNode* rootParent)
{
    SVGElement::removedFrom(rootParent);
    if (rootParent->inDocument())
        StyleElement::removedFromDocument(document(), this);
}

void SVGStyleElement::childrenChanged(const ChildrenChange& change)
{
    SVGElement::childrenChanged(change);
    StyleElement::childrenChanged(this);
}

void SVGStyleElement::trace(Visitor* visitor)
{
    StyleElement::trace(visitor);
    SVGElement::trace(visitor);
}

}
