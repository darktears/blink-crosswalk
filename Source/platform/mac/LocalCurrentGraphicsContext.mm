/*
 * Copyright (C) 2006 Apple Computer, Inc.
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
#include "platform/mac/LocalCurrentGraphicsContext.h"

#include <AppKit/NSGraphicsContext.h>
#include "platform/graphics/GraphicsContext.h"
#include "platform_canvas.h"

namespace blink {

LocalCurrentGraphicsContext::LocalCurrentGraphicsContext(GraphicsContext* graphicsContext)
    : m_didSetGraphicsContext(false)
    , m_skiaBitLocker(graphicsContext->canvas())
{
    m_savedGraphicsContext = graphicsContext;
    graphicsContext->save();

    CGContextRef cgContext = this->cgContext();
    if (cgContext == [[NSGraphicsContext currentContext] graphicsPort]) {
        m_savedNSGraphicsContext = 0;
        return;
    }

    m_savedNSGraphicsContext = [[NSGraphicsContext currentContext] retain];
    NSGraphicsContext* newContext = [NSGraphicsContext graphicsContextWithGraphicsPort:cgContext flipped:YES];
    [NSGraphicsContext setCurrentContext:newContext];
    m_didSetGraphicsContext = true;
}

LocalCurrentGraphicsContext::~LocalCurrentGraphicsContext()
{
    if (m_didSetGraphicsContext) {
        [NSGraphicsContext setCurrentContext:m_savedNSGraphicsContext];
        [m_savedNSGraphicsContext release];
    }

    m_savedGraphicsContext->restore();
}

CGContextRef LocalCurrentGraphicsContext::cgContext()
{
    // This synchronizes the CGContext to reflect the current SkCanvas state.
    // The implementation may not return the same CGContext each time.
    CGContextRef cgContext = m_skiaBitLocker.cgContext();

    return cgContext;
}

ContextContainer::ContextContainer(GraphicsContext* graphicsContext) 
    : m_skiaBitLocker(graphicsContext->canvas())
{
}

}
