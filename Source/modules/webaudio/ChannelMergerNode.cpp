/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
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

#include "config.h"

#if ENABLE(WEB_AUDIO)

#include "modules/webaudio/ChannelMergerNode.h"

#include "modules/webaudio/AudioContext.h"
#include "modules/webaudio/AudioNodeInput.h"
#include "modules/webaudio/AudioNodeOutput.h"

const unsigned DefaultNumberOfOutputChannels = 1;

namespace blink {

PassRefPtrWillBeRawPtr<ChannelMergerNode> ChannelMergerNode::create(AudioContext* context, float sampleRate, unsigned numberOfInputs)
{
    if (!numberOfInputs || numberOfInputs > AudioContext::maxNumberOfChannels())
        return nullptr;

    return adoptRefWillBeNoop(new ChannelMergerNode(context, sampleRate, numberOfInputs));
}

ChannelMergerNode::ChannelMergerNode(AudioContext* context, float sampleRate, unsigned numberOfInputs)
    : AudioNode(context, sampleRate)
    , m_desiredNumberOfOutputChannels(DefaultNumberOfOutputChannels)
{
    ScriptWrappable::init(this);
    // Create the requested number of inputs.
    for (unsigned i = 0; i < numberOfInputs; ++i)
        addInput();

    addOutput(AudioNodeOutput::create(this, 1));
    setNodeType(NodeTypeChannelMerger);
    initialize();
}

void ChannelMergerNode::process(size_t framesToProcess)
{
    AudioNodeOutput* output = this->output(0);
    ASSERT(output);
    ASSERT_UNUSED(framesToProcess, framesToProcess == output->bus()->length());

    // Output bus not updated yet, so just output silence.
    if (m_desiredNumberOfOutputChannels != output->numberOfChannels()) {
        output->bus()->zero();
        return;
    }

    // Merge all the channels from all the inputs into one output.
    unsigned outputChannelIndex = 0;
    unsigned maxAllowedOutputChannels = output->numberOfChannels();

    for (unsigned i = 0; i < numberOfInputs(); ++i) {
        AudioNodeInput* input = this->input(i);
        if (input->isConnected()) {
            unsigned numberOfInputChannels = input->bus()->numberOfChannels();

            // Merge channels from this particular input, but be careful not to exceed the number of
            // output channels.  (This can happen if there are many inputs with each input
            // containing many channels.)
            for (unsigned j = 0; j < numberOfInputChannels; ++j) {
                if (outputChannelIndex < maxAllowedOutputChannels) {
                    AudioChannel* inputChannel = input->bus()->channel(j);
                    AudioChannel* outputChannel = output->bus()->channel(outputChannelIndex);
                    outputChannel->copyFrom(inputChannel);

                    ++outputChannelIndex;
                }
            }
        }
        if (outputChannelIndex >= maxAllowedOutputChannels)
            break;
    }

    ASSERT(outputChannelIndex == output->numberOfChannels());
}

// Any time a connection or disconnection happens on any of our inputs, we potentially need to change the
// number of channels of our output.
void ChannelMergerNode::checkNumberOfChannelsForInput(AudioNodeInput* input)
{
    ASSERT(context()->isAudioThread() && context()->isGraphOwner());

    // Count how many channels we have all together from all of the inputs.
    unsigned numberOfOutputChannels = 0;
    for (unsigned i = 0; i < numberOfInputs(); ++i) {
        AudioNodeInput* input = this->input(i);
        if (input->isConnected())
            numberOfOutputChannels += input->numberOfChannels();
    }

    // If the actual number of channels exceeds the max allowed, just drop the excess.
    numberOfOutputChannels = std::min(numberOfOutputChannels, AudioContext::maxNumberOfChannels());

    // Set the correct number of channels on the output
    AudioNodeOutput* output = this->output(0);
    ASSERT(output);
    output->setNumberOfChannels(numberOfOutputChannels);
    // There can in rare cases be a slight delay before the output bus is updated to the new number of
    // channels because of tryLocks() in the context's updating system. So record the new number of
    // output channels here.
    m_desiredNumberOfOutputChannels = numberOfOutputChannels;

    AudioNode::checkNumberOfChannelsForInput(input);
}

} // namespace blink

#endif // ENABLE(WEB_AUDIO)
