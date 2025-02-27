/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FontFaceSetLoadEvent_h
#define FontFaceSetLoadEvent_h

#include "core/css/FontFace.h"
#include "core/dom/DOMError.h"
#include "core/events/Event.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

struct FontFaceSetLoadEventInit : public EventInit {
    FontFaceArray fontfaces;
};

class FontFaceSetLoadEvent FINAL : public Event {
public:
    static PassRefPtrWillBeRawPtr<FontFaceSetLoadEvent> create()
    {
        return adoptRefWillBeNoop(new FontFaceSetLoadEvent());
    }

    static PassRefPtrWillBeRawPtr<FontFaceSetLoadEvent> create(const AtomicString& type, const FontFaceSetLoadEventInit& initializer)
    {
        return adoptRefWillBeNoop(new FontFaceSetLoadEvent(type, initializer));
    }

    static PassRefPtrWillBeRawPtr<FontFaceSetLoadEvent> createForFontFaces(const AtomicString& type, const FontFaceArray& fontfaces = FontFaceArray())
    {
        return adoptRefWillBeNoop(new FontFaceSetLoadEvent(type, fontfaces));
    }

    virtual ~FontFaceSetLoadEvent();

    FontFaceArray fontfaces() const { return m_fontfaces; }

    virtual const AtomicString& interfaceName() const OVERRIDE;

    virtual void trace(Visitor*) OVERRIDE;

private:
    FontFaceSetLoadEvent();
    FontFaceSetLoadEvent(const AtomicString&, const FontFaceArray&);
    FontFaceSetLoadEvent(const AtomicString&, const FontFaceSetLoadEventInit&);

    FontFaceArray m_fontfaces;
};

} // namespace blink

#endif // FontFaceSetLoadEvent_h
