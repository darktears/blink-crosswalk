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
 */

#include "config.h"

#include "public/platform/WebRTCVoidRequest.h"

#include "platform/mediastream/RTCVoidRequest.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

namespace {

class ExtraDataContainer : public RTCVoidRequest::ExtraData {
public:
    ExtraDataContainer(PassOwnPtr<WebRTCVoidRequest::ExtraData> extraData) : m_extraData(extraData) { }

    WebRTCVoidRequest::ExtraData* extraData() { return m_extraData.get(); }

private:
    OwnPtr<WebRTCVoidRequest::ExtraData> m_extraData;
};

} // namespace

WebRTCVoidRequest::WebRTCVoidRequest(const PassRefPtr<RTCVoidRequest>& constraints)
    : m_private(constraints)
{
}

void WebRTCVoidRequest::assign(const WebRTCVoidRequest& other)
{
    m_private = other.m_private;
}

void WebRTCVoidRequest::reset()
{
    m_private.reset();
}

void WebRTCVoidRequest::requestSucceeded() const
{
    ASSERT(m_private.get());
    m_private->requestSucceeded();
}

void WebRTCVoidRequest::requestFailed(const WebString& error) const
{
    ASSERT(m_private.get());
    m_private->requestFailed(error);
}

WebRTCVoidRequest::ExtraData* WebRTCVoidRequest::extraData() const
{
    RTCVoidRequest::ExtraData* data = m_private->extraData();
    if (!data)
        return 0;
    return static_cast<ExtraDataContainer*>(data)->extraData();
}

void WebRTCVoidRequest::setExtraData(ExtraData* extraData)
{
    m_private->setExtraData(adoptPtr(new ExtraDataContainer(adoptPtr(extraData))));
}

} // namespace blink

