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

#ifndef MIDIOutput_h
#define MIDIOutput_h

#include "modules/webmidi/MIDIPort.h"
#include "wtf/Uint8Array.h"

namespace blink {

class ExceptionState;
class MIDIAccess;

class MIDIOutput FINAL : public MIDIPort {
public:
    static MIDIOutput* create(MIDIAccess*, unsigned portIndex, const String& id, const String& manufacturer, const String& name, const String& version);
    virtual ~MIDIOutput();

    void send(Uint8Array*, double timestamp, ExceptionState&);
    void send(Vector<unsigned>, double timestamp, ExceptionState&);

    // send() without optional |timestamp|.
    void send(Uint8Array*, ExceptionState&);
    void send(Vector<unsigned>, ExceptionState&);

    virtual void trace(Visitor*) OVERRIDE;

private:
    MIDIOutput(MIDIAccess*, unsigned portIndex, const String& id, const String& manufacturer, const String& name, const String& version);

    unsigned m_portIndex;
};

typedef HeapVector<Member<MIDIOutput> > MIDIOutputVector;

} // namespace blink

#endif // MIDIOutput_h
