/*
 * Copyright (C) 2007, 2008, 2009 Google Inc. All rights reserved.
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

#include "config.h"
#include "bindings/core/v8/V8WebKitPoint.h"

#include "bindings/core/v8/V8Binding.h"
#include "bindings/core/v8/V8DOMWrapper.h"
#include "bindings/core/v8/WrapperTypeInfo.h"
#include "wtf/MathExtras.h"

namespace blink {

void V8WebKitPoint::constructorCustom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    float x = 0;
    float y = 0;
    if (info.Length() > 1) {
        if (!info[0]->IsUndefined()) {
            x = toFloat(info[0]);
            if (std::isnan(x))
                x = 0;
        }
        if (!info[1]->IsUndefined()) {
            y = toFloat(info[1]);
            if (std::isnan(y))
                y = 0;
        }
    }
    RefPtrWillBeRawPtr<WebKitPoint> point = WebKitPoint::create(x, y);
    v8::Handle<v8::Object> wrapper = info.Holder();
    V8DOMWrapper::associateObjectWithWrapper<V8WebKitPoint>(point.release(), &wrapperTypeInfo, wrapper, info.GetIsolate(), WrapperConfiguration::Dependent);
    info.GetReturnValue().Set(wrapper);
}

} // namespace blink
