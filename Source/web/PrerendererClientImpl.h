/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
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
 *
 */

#ifndef PrerendererClientImpl_h
#define PrerendererClientImpl_h

#include "core/loader/PrerendererClient.h"
#include "wtf/Noncopyable.h"
#include "wtf/PassRefPtr.h"

namespace blink {
class Prerender;
}

namespace blink {

class WebPrerendererClient;

class PrerendererClientImpl FINAL : public NoBaseWillBeGarbageCollected<PrerendererClientImpl>, public blink::PrerendererClient {
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(PrerendererClientImpl);
    WTF_MAKE_NONCOPYABLE(PrerendererClientImpl);
public:
    explicit PrerendererClientImpl(WebPrerendererClient*);

    void willAddPrerender(blink::Prerender*) OVERRIDE;

    virtual void trace(blink::Visitor* visitor) OVERRIDE { blink::PrerendererClient::trace(visitor); }

private:
    WebPrerendererClient* m_client;
};

}

#endif // PrerendererClientImpl_h
