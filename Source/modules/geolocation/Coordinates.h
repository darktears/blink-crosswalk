/*
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef Coordinates_h
#define Coordinates_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "modules/EventModules.h"
#include "platform/heap/Handle.h"

namespace blink {

class Coordinates : public GarbageCollected<Coordinates>, public ScriptWrappable {
public:
    static Coordinates* create(double latitude, double longitude, bool providesAltitude, double altitude, double accuracy, bool providesAltitudeAccuracy, double altitudeAccuracy, bool providesHeading, double heading, bool providesSpeed, double speed)
    {
        return new Coordinates(latitude, longitude, providesAltitude, altitude, accuracy, providesAltitudeAccuracy, altitudeAccuracy, providesHeading, heading, providesSpeed, speed);
    }

    void trace(Visitor*) { }

    double latitude() const { return m_latitude; }
    double longitude() const { return m_longitude; }
    double altitude(bool& isNull) const;
    double accuracy() const { return m_accuracy; }
    double altitudeAccuracy(bool& isNull) const;
    double heading(bool& isNull) const;
    double speed(bool& isNull) const;

private:
    Coordinates(double latitude, double longitude, bool providesAltitude, double altitude, double accuracy, bool providesAltitudeAccuracy, double altitudeAccuracy, bool providesHeading, double heading, bool providesSpeed, double speed)
        : m_latitude(latitude)
        , m_longitude(longitude)
        , m_altitude(altitude)
        , m_accuracy(accuracy)
        , m_altitudeAccuracy(altitudeAccuracy)
        , m_heading(heading)
        , m_speed(speed)
        , m_canProvideAltitude(providesAltitude)
        , m_canProvideAltitudeAccuracy(providesAltitudeAccuracy)
        , m_canProvideHeading(providesHeading)
        , m_canProvideSpeed(providesSpeed)
    {
        ScriptWrappable::init(this);
    }

    double m_latitude;
    double m_longitude;
    double m_altitude;
    double m_accuracy;
    double m_altitudeAccuracy;
    double m_heading;
    double m_speed;

    bool m_canProvideAltitude;
    bool m_canProvideAltitudeAccuracy;
    bool m_canProvideHeading;
    bool m_canProvideSpeed;
};

} // namespace blink

#endif // Coordinates_h
