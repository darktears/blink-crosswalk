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

#ifndef AudioBasicProcessorNode_h
#define AudioBasicProcessorNode_h

#include "modules/webaudio/AudioNode.h"
#include "wtf/Forward.h"
#include "wtf/Threading.h"

namespace blink {

class AudioBus;
class AudioNodeInput;
class AudioProcessor;

// AudioBasicProcessorNode is an AudioNode with one input and one output where the input and output have the same number of channels.
class AudioBasicProcessorNode : public AudioNode {
public:
    AudioBasicProcessorNode(AudioContext*, float sampleRate);
    virtual ~AudioBasicProcessorNode();
    virtual void trace(Visitor*) OVERRIDE;

    // AudioNode
    virtual void dispose() OVERRIDE FINAL;
    virtual void process(size_t framesToProcess) OVERRIDE FINAL;
    virtual void pullInputs(size_t framesToProcess) OVERRIDE FINAL;
    virtual void initialize() OVERRIDE FINAL;
    virtual void uninitialize() OVERRIDE FINAL;

    // Called in the main thread when the number of channels for the input may have changed.
    virtual void checkNumberOfChannelsForInput(AudioNodeInput*) OVERRIDE FINAL;

    // Returns the number of channels for both the input and the output.
    unsigned numberOfChannels();

protected:
    virtual double tailTime() const OVERRIDE FINAL;
    virtual double latencyTime() const OVERRIDE FINAL;

    AudioProcessor* processor() { return m_processor.get(); }
    OwnPtrWillBeMember<AudioProcessor> m_processor;
};

} // namespace blink

#endif // AudioBasicProcessorNode_h
