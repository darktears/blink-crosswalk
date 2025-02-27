// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Geofencing_h
#define Geofencing_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "platform/heap/Handle.h"
#include "wtf/text/WTFString.h"

namespace blink {

class ExecutionContext;
class GeofencingRegion;
class ScriptPromise;
class ScriptState;

class Geofencing FINAL : public GarbageCollected<Geofencing>, public ScriptWrappable {
public:
    static Geofencing* create()
    {
        return new Geofencing();
    }

    ScriptPromise registerRegion(ScriptState*, GeofencingRegion*);
    ScriptPromise unregisterRegion(ScriptState*, const String& regionId);
    ScriptPromise getRegisteredRegions(ScriptState*) const;

    virtual void trace(Visitor*) { }

private:
    Geofencing();
};

} // namespace blink

#endif // Geofencing_h
