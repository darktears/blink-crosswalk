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

#ifndef WebRTCStatsRequest_h
#define WebRTCStatsRequest_h

#include "WebCommon.h"
#include "WebPrivatePtr.h"
#include "WebString.h"

namespace blink {

class RTCStatsRequest;
class WebMediaStreamTrack;
class WebMediaStream;
class WebRTCStatsResponse;

// The WebRTCStatsRequest class represents a JavaScript call on
// RTCPeerConnection.getStats(). The user of this API will use
// the calls on this class and WebRTCStatsResponse to fill in the
// data that will be returned via a callback to the user in an
// RTCStatsResponse structure.
//
// The typical usage pattern is:
// WebRTCStatsRequest request = <from somewhere>
// WebRTCStatsResponse response = request.createResponse();
//
// For each item on which statistics are going to be reported:
//   size_t reportIndex = response.addReport();
//   Add local information:
//   size_t elementIndex = response.addElement(reportIndex, true, dateNow());
//   For each statistic being reported on:
//     response.addStatistic(reportIndex, true, elementIndex,
//                           "name of statistic", "statistic value");
//   Remote information (typically RTCP-derived) is added in the same way.
// When finished adding information:
// request.requestSucceeded(response);

class WebRTCStatsRequest {
public:
    WebRTCStatsRequest() { }
    WebRTCStatsRequest(const WebRTCStatsRequest& other) { assign(other); }
    ~WebRTCStatsRequest() { reset(); }

    WebRTCStatsRequest& operator=(const WebRTCStatsRequest& other)
    {
        assign(other);
        return *this;
    }

    BLINK_PLATFORM_EXPORT void assign(const WebRTCStatsRequest&);

    BLINK_PLATFORM_EXPORT void reset();

    // This function returns true if a selector argument was given to getStats.
    BLINK_PLATFORM_EXPORT bool hasSelector() const;

    // The component() accessor give the information
    // required to look up a MediaStreamTrack implementation.
    // It is only useful to call it when hasSelector() returns true.
    BLINK_PLATFORM_EXPORT const WebMediaStreamTrack component() const;

    BLINK_PLATFORM_EXPORT void requestSucceeded(const WebRTCStatsResponse&) const;

    BLINK_PLATFORM_EXPORT WebRTCStatsResponse createResponse() const;

#if INSIDE_BLINK
    BLINK_PLATFORM_EXPORT WebRTCStatsRequest(const PassRefPtr<RTCStatsRequest>&);
#endif

private:
    WebPrivatePtr<RTCStatsRequest> m_private;
};

} // namespace blink

#endif // WebRTCStatsRequest_h
