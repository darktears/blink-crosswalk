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

#if ENABLE(SVG_FONTS)
#include "core/svg/SVGVKernElement.h"

#include "core/svg/SVGParserUtilities.h"

namespace blink {

inline SVGVKernElement::SVGVKernElement(Document& document)
    : SVGElement(SVGNames::vkernTag, document)
{
    ScriptWrappable::init(this);
}

DEFINE_NODE_FACTORY(SVGVKernElement)

Node::InsertionNotificationRequest SVGVKernElement::insertedInto(ContainerNode* rootParent)
{
    if (rootParent->inDocument()) {
        ContainerNode* fontNode = parentNode();
        if (isSVGFontElement(fontNode))
            toSVGFontElement(*fontNode).invalidateGlyphCache();
    }

    return SVGElement::insertedInto(rootParent);
}

void SVGVKernElement::removedFrom(ContainerNode* rootParent)
{
    ContainerNode* fontNode = parentNode();
    if (isSVGFontElement(fontNode))
        toSVGFontElement(*fontNode).invalidateGlyphCache();

    SVGElement::removedFrom(rootParent);
}

void SVGVKernElement::buildVerticalKerningPair(KerningPairVector& kerningPairs)
{
    String u1 = fastGetAttribute(SVGNames::u1Attr);
    String g1 = fastGetAttribute(SVGNames::g1Attr);
    String u2 = fastGetAttribute(SVGNames::u2Attr);
    String g2 = fastGetAttribute(SVGNames::g2Attr);
    if ((u1.isEmpty() && g1.isEmpty()) || (u2.isEmpty() && g2.isEmpty()))
        return;

    SVGKerningPair kerningPair;
    if (parseGlyphName(g1, kerningPair.glyphName1)
        && parseGlyphName(g2, kerningPair.glyphName2)
        && parseKerningUnicodeString(u1, kerningPair.unicodeRange1, kerningPair.unicodeName1)
        && parseKerningUnicodeString(u2, kerningPair.unicodeRange2, kerningPair.unicodeName2)) {
        kerningPair.kerning = fastGetAttribute(SVGNames::kAttr).string().toFloat();
        kerningPairs.append(kerningPair);
    }
}

}

#endif // ENABLE(SVG_FONTS)
