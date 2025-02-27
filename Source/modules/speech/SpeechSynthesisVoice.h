/*
 * Copyright (C) 2013 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SpeechSynthesisVoice_h
#define SpeechSynthesisVoice_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "platform/heap/Handle.h"
#include "platform/speech/PlatformSpeechSynthesisVoice.h"
#include "wtf/text/WTFString.h"

namespace blink {

class SpeechSynthesisVoice FINAL : public GarbageCollected<SpeechSynthesisVoice>, public ScriptWrappable {
public:
    static SpeechSynthesisVoice* create(PlatformSpeechSynthesisVoice*);

    const String& voiceURI() const { return m_platformVoice->voiceURI(); }
    const String& name() const { return m_platformVoice->name(); }
    const String& lang() const { return m_platformVoice->lang(); }
    bool localService() const { return m_platformVoice->localService(); }
    bool isDefault() const { return m_platformVoice->isDefault(); }

    PlatformSpeechSynthesisVoice* platformVoice() const { return m_platformVoice; }

    void trace(Visitor*);

private:
    explicit SpeechSynthesisVoice(PlatformSpeechSynthesisVoice*);

    Member<PlatformSpeechSynthesisVoice> m_platformVoice;
};

} // namespace blink

#endif // SpeechSynthesisVoice_h
