/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
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

#ifndef PerformanceNavigation_h
#define PerformanceNavigation_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "core/frame/DOMWindowProperty.h"
#include "platform/heap/Handle.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"

namespace blink {

class LocalFrame;

class PerformanceNavigation FINAL : public RefCountedWillBeGarbageCollectedFinalized<PerformanceNavigation>, public ScriptWrappable, public DOMWindowProperty {
public:
    static PassRefPtrWillBeRawPtr<PerformanceNavigation> create(LocalFrame* frame)
    {
        return adoptRefWillBeNoop(new PerformanceNavigation(frame));
    }

    enum PerformanceNavigationType {
        TYPE_NAVIGATE,
        TYPE_RELOAD,
        TYPE_BACK_FORWARD,
        TYPE_RESERVED = 255
    };

    unsigned short type() const;
    unsigned short redirectCount() const;

    void trace(Visitor*) { }

private:
    explicit PerformanceNavigation(LocalFrame*);
};

}

#endif // !defined(PerformanceNavigation_h)
