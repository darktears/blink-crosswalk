/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DeviceRotationRate_h
#define DeviceRotationRate_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "modules/device_orientation/DeviceMotionData.h"
#include "platform/heap/Handle.h"
#include "wtf/RefCounted.h"

namespace blink {

class DeviceRotationRate : public RefCountedWillBeGarbageCollected<DeviceRotationRate>, public ScriptWrappable {
public:
    static PassRefPtrWillBeRawPtr<DeviceRotationRate> create(PassRefPtrWillBeRawPtr<DeviceMotionData::RotationRate> rotationRate)
    {
        return adoptRefWillBeNoop(new DeviceRotationRate(rotationRate));
    }
    void trace(Visitor*);

    double alpha(bool& isNull) const;
    double beta(bool& isNull) const;
    double gamma(bool& isNull) const;

private:
    DeviceRotationRate(PassRefPtrWillBeRawPtr<DeviceMotionData::RotationRate>);

    RefPtrWillBeMember<DeviceMotionData::RotationRate> m_rotationRate;
};

} // namespace blink

#endif // DeviceRotationRate_h
