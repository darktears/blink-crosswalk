/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
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
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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

#ifndef AudioParamTimeline_h
#define AudioParamTimeline_h

#include "modules/webaudio/AudioContext.h"
#include "wtf/Float32Array.h"
#include "wtf/Forward.h"
#include "wtf/Threading.h"
#include "wtf/Vector.h"

namespace blink {

class AudioParamTimeline {
public:
    AudioParamTimeline()
    {
    }

    void setValueAtTime(float value, double time);
    void linearRampToValueAtTime(float value, double time);
    void exponentialRampToValueAtTime(float value, double time, ExceptionState&);
    void setTargetAtTime(float target, double time, double timeConstant);
    void setValueCurveAtTime(Float32Array* curve, double time, double duration);
    void cancelScheduledValues(double startTime);

    // hasValue is set to true if a valid timeline value is returned.
    // otherwise defaultValue is returned.
    float valueForContextTime(AudioContext*, float defaultValue, bool& hasValue);

    // Given the time range, calculates parameter values into the values buffer
    // and returns the last parameter value calculated for "values" or the defaultValue if none were calculated.
    // controlRate is the rate (number per second) at which parameter values will be calculated.
    // It should equal sampleRate for sample-accurate parameter changes, and otherwise will usually match
    // the render quantum size such that the parameter value changes once per render quantum.
    float valuesForTimeRange(double startTime, double endTime, float defaultValue, float* values, unsigned numberOfValues, double sampleRate, double controlRate);

    bool hasValues() { return m_events.size(); }

private:
    class ParamEvent {
    public:
        enum Type {
            SetValue,
            LinearRampToValue,
            ExponentialRampToValue,
            SetTarget,
            SetValueCurve,
            LastType
        };

        ParamEvent(Type type, float value, double time, double timeConstant, double duration, PassRefPtr<Float32Array> curve)
            : m_type(type)
            , m_value(value)
            , m_time(time)
            , m_timeConstant(timeConstant)
            , m_duration(duration)
            , m_curve(curve)
        {
        }

        unsigned type() const { return m_type; }
        float value() const { return m_value; }
        double time() const { return m_time; }
        double timeConstant() const { return m_timeConstant; }
        double duration() const { return m_duration; }
        Float32Array* curve() { return m_curve.get(); }

    private:
        unsigned m_type;
        float m_value;
        double m_time;
        double m_timeConstant;
        double m_duration;
        RefPtr<Float32Array> m_curve;
    };

    void insertEvent(const ParamEvent&);
    float valuesForTimeRangeImpl(double startTime, double endTime, float defaultValue, float* values, unsigned numberOfValues, double sampleRate, double controlRate);

    Vector<ParamEvent> m_events;

    Mutex m_eventsLock;
};

} // namespace blink

#endif // AudioParamTimeline_h
