/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
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
#include "modules/mediastream/RTCStatsRequestImpl.h"

#include "modules/mediastream/MediaStreamTrack.h"
#include "modules/mediastream/RTCPeerConnection.h"
#include "modules/mediastream/RTCStatsCallback.h"

namespace blink {

PassRefPtr<RTCStatsRequestImpl> RTCStatsRequestImpl::create(ExecutionContext* context, RTCPeerConnection* requester, PassOwnPtr<RTCStatsCallback> callback, MediaStreamTrack* selector)
{
    RefPtr<RTCStatsRequestImpl> request = adoptRef(new RTCStatsRequestImpl(context, requester, callback, selector));
    request->suspendIfNeeded();
    return request.release();
}

RTCStatsRequestImpl::RTCStatsRequestImpl(ExecutionContext* context, RTCPeerConnection* requester, PassOwnPtr<RTCStatsCallback> callback, MediaStreamTrack* selector)
    : ActiveDOMObject(context)
    , m_successCallback(callback)
    , m_component(selector ? selector->component() : 0)
    , m_requester(requester)
{
    ASSERT(m_requester);
}

RTCStatsRequestImpl::~RTCStatsRequestImpl()
{
}

RTCStatsResponseBase* RTCStatsRequestImpl::createResponse()
{
    return RTCStatsResponse::create();
}

bool RTCStatsRequestImpl::hasSelector()
{
    return m_component;
}

MediaStreamComponent* RTCStatsRequestImpl::component()
{
    return m_component.get();
}

void RTCStatsRequestImpl::requestSucceeded(RTCStatsResponseBase* response)
{
    bool shouldFireCallback = m_requester ? m_requester->shouldFireGetStatsCallback() : false;
    if (shouldFireCallback && m_successCallback)
        m_successCallback->handleEvent(static_cast<RTCStatsResponse*>(response));
    clear();
}

void RTCStatsRequestImpl::stop()
{
    clear();
}

void RTCStatsRequestImpl::clear()
{
    m_successCallback.clear();
    m_requester.clear();
}

} // namespace blink
