/*
 * Copyright (C) 2004, 2005, 2006 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006 Rob Buis <buis@kde.org>
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

#ifndef SVGPathSegClosePath_h
#define SVGPathSegClosePath_h

#include "core/svg/SVGPathSegWithContext.h"

namespace blink {

class SVGPathSegClosePath FINAL : public SVGPathSegWithContext {
public:
    static PassRefPtr<SVGPathSegClosePath> create(SVGPathElement* element)
    {
        return adoptRef(new SVGPathSegClosePath(element));
    }

private:
    SVGPathSegClosePath(SVGPathElement* element)
        : SVGPathSegWithContext(element)
    {
        ScriptWrappable::init(this);
    }

    virtual unsigned short pathSegType() const OVERRIDE { return PATHSEG_CLOSEPATH; }
    virtual String pathSegTypeAsLetter() const OVERRIDE { return "Z"; }
};

} // namespace blink

#endif
