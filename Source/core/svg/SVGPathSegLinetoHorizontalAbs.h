/*
 * Copyright (C) 2004, 2005, 2006 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2008 Rob Buis <buis@kde.org>
 * Copyright (C) 2013 Samsung Electronics. All rights reserved.
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

#ifndef SVGPathSegLinetoHorizontalAbs_h
#define SVGPathSegLinetoHorizontalAbs_h

#include "core/svg/SVGPathSegLinetoHorizontal.h"

namespace blink {

class SVGPathSegLinetoHorizontalAbs FINAL : public SVGPathSegLinetoHorizontal {
public:
    static PassRefPtr<SVGPathSegLinetoHorizontalAbs> create(SVGPathElement* element, float x)
    {
        return adoptRef(new SVGPathSegLinetoHorizontalAbs(element, x));
    }

private:
    SVGPathSegLinetoHorizontalAbs(SVGPathElement* element, float x)
        : SVGPathSegLinetoHorizontal(element, x)
    {
        ScriptWrappable::init(this);
    }

    virtual unsigned short pathSegType() const OVERRIDE { return PATHSEG_LINETO_HORIZONTAL_ABS; }
    virtual String pathSegTypeAsLetter() const OVERRIDE { return "H"; }
};

} // namespace blink

#endif
