/*
 * Copyright (C) 2010, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(WEB_AUDIO)

#include "modules/webaudio/AudioNodeOutput.h"

#include "modules/webaudio/AudioContext.h"
#include "modules/webaudio/AudioNodeInput.h"
#include "wtf/Threading.h"

namespace blink {

inline AudioNodeOutput::AudioNodeOutput(AudioNode* node, unsigned numberOfChannels)
    : m_node(node)
    , m_numberOfChannels(numberOfChannels)
    , m_desiredNumberOfChannels(numberOfChannels)
    , m_isInPlace(false)
    , m_isEnabled(true)
    , m_renderingFanOutCount(0)
    , m_renderingParamFanOutCount(0)
{
    ASSERT(numberOfChannels <= AudioContext::maxNumberOfChannels());

    m_internalBus = AudioBus::create(numberOfChannels, AudioNode::ProcessingSizeInFrames);
}

PassOwnPtrWillBeRawPtr<AudioNodeOutput> AudioNodeOutput::create(AudioNode* node, unsigned numberOfChannels)
{
    return adoptPtrWillBeNoop(new AudioNodeOutput(node, numberOfChannels));
}

void AudioNodeOutput::trace(Visitor* visitor)
{
#if ENABLE(OILPAN)
    visitor->trace(m_node);
    visitor->trace(m_inputs);
    visitor->trace(m_params);
#endif
}

void AudioNodeOutput::setNumberOfChannels(unsigned numberOfChannels)
{
    ASSERT(numberOfChannels <= AudioContext::maxNumberOfChannels());
    ASSERT(context()->isGraphOwner());

    m_desiredNumberOfChannels = numberOfChannels;

    if (context()->isAudioThread()) {
        // If we're in the audio thread then we can take care of it right away (we should be at the very start or end of a rendering quantum).
        updateNumberOfChannels();
    } else {
        // Let the context take care of it in the audio thread in the pre and post render tasks.
        context()->markAudioNodeOutputDirty(this);
    }
}

void AudioNodeOutput::updateInternalBus()
{
    if (numberOfChannels() == m_internalBus->numberOfChannels())
        return;

    m_internalBus = AudioBus::create(numberOfChannels(), AudioNode::ProcessingSizeInFrames);
}

void AudioNodeOutput::updateRenderingState()
{
    updateNumberOfChannels();
    m_renderingFanOutCount = fanOutCount();
    m_renderingParamFanOutCount = paramFanOutCount();
}

void AudioNodeOutput::updateNumberOfChannels()
{
    ASSERT(context()->isAudioThread() && context()->isGraphOwner());

    if (m_numberOfChannels != m_desiredNumberOfChannels) {
        m_numberOfChannels = m_desiredNumberOfChannels;
        updateInternalBus();
        propagateChannelCount();
    }
}

void AudioNodeOutput::propagateChannelCount()
{
    ASSERT(context()->isAudioThread() && context()->isGraphOwner());

    if (isChannelCountKnown()) {
        // Announce to any nodes we're connected to that we changed our channel count for its input.
        for (InputsIterator i = m_inputs.begin(); i != m_inputs.end(); ++i)
            i->value->checkNumberOfChannelsForInput(i->key);
    }
}

AudioBus* AudioNodeOutput::pull(AudioBus* inPlaceBus, size_t framesToProcess)
{
    ASSERT(context()->isAudioThread());
    ASSERT(m_renderingFanOutCount > 0 || m_renderingParamFanOutCount > 0);

    // Causes our AudioNode to process if it hasn't already for this render quantum.
    // We try to do in-place processing (using inPlaceBus) if at all possible,
    // but we can't process in-place if we're connected to more than one input (fan-out > 1).
    // In this case pull() is called multiple times per rendering quantum, and the processIfNecessary() call below will
    // cause our node to process() only the first time, caching the output in m_internalOutputBus for subsequent calls.

    m_isInPlace = inPlaceBus && inPlaceBus->numberOfChannels() == numberOfChannels() && (m_renderingFanOutCount + m_renderingParamFanOutCount) == 1;

    m_inPlaceBus = m_isInPlace ? inPlaceBus : 0;

    node()->processIfNecessary(framesToProcess);
    return bus();
}

AudioBus* AudioNodeOutput::bus() const
{
    ASSERT(const_cast<AudioNodeOutput*>(this)->context()->isAudioThread());
    return m_isInPlace ? m_inPlaceBus.get() : m_internalBus.get();
}

unsigned AudioNodeOutput::fanOutCount()
{
    ASSERT(context()->isGraphOwner());
    return m_inputs.size();
}

unsigned AudioNodeOutput::paramFanOutCount()
{
    ASSERT(context()->isGraphOwner());
    return m_params.size();
}

unsigned AudioNodeOutput::renderingFanOutCount() const
{
    return m_renderingFanOutCount;
}

void AudioNodeOutput::addInput(AudioNodeInput& input)
{
    ASSERT(context()->isGraphOwner());
    m_inputs.add(&input, &input.node());
    input.node().makeConnection();
}

void AudioNodeOutput::removeInput(AudioNodeInput& input)
{
    ASSERT(context()->isGraphOwner());
    input.node().breakConnection();
    m_inputs.remove(&input);
}

void AudioNodeOutput::disconnectAllInputs()
{
    ASSERT(context()->isGraphOwner());

    // AudioNodeInput::disconnect() changes m_inputs by calling removeInput().
    while (!m_inputs.isEmpty())
        m_inputs.begin()->key->disconnect(*this);
}

void AudioNodeOutput::addParam(AudioParam& param)
{
    ASSERT(context()->isGraphOwner());
    m_params.add(&param);
}

void AudioNodeOutput::removeParam(AudioParam& param)
{
    ASSERT(context()->isGraphOwner());
    m_params.remove(&param);
}

void AudioNodeOutput::disconnectAllParams()
{
    ASSERT(context()->isGraphOwner());

    // AudioParam::disconnect() changes m_params by calling removeParam().
    while (!m_params.isEmpty())
        (*m_params.begin())->disconnect(*this);
}

void AudioNodeOutput::disconnectAll()
{
    disconnectAllInputs();
    disconnectAllParams();
}

void AudioNodeOutput::disable()
{
    ASSERT(context()->isGraphOwner());

    if (m_isEnabled) {
        for (InputsIterator i = m_inputs.begin(); i != m_inputs.end(); ++i)
            i->key->disable(*this);
        m_isEnabled = false;
    }
}

void AudioNodeOutput::enable()
{
    ASSERT(context()->isGraphOwner());

    if (!m_isEnabled) {
        for (InputsIterator i = m_inputs.begin(); i != m_inputs.end(); ++i)
            i->key->enable(*this);
        m_isEnabled = true;
    }
}

} // namespace blink

#endif // ENABLE(WEB_AUDIO)
