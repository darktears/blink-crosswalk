/*
 * Copyright (C) 2012 Samsung Electronics. All Rights Reserved.
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

#ifndef NavigatorContentUtilsClient_h
#define NavigatorContentUtilsClient_h

#include "platform/weborigin/KURL.h"
#include "wtf/text/WTFString.h"

namespace blink {

class Page;

class NavigatorContentUtilsClient {
public:
    virtual ~NavigatorContentUtilsClient() { }
    virtual void registerProtocolHandler(const String& scheme, const KURL& baseURL, const KURL&, const String& title) = 0;

    enum CustomHandlersState {
        CustomHandlersNew,
        CustomHandlersRegistered,
        CustomHandlersDeclined
    };

    virtual CustomHandlersState isProtocolHandlerRegistered(const String& scheme, const KURL& baseURL, const KURL&) = 0;
    virtual void unregisterProtocolHandler(const String& scheme, const KURL& baseURL, const KURL&) = 0;
};

void provideNavigatorContentUtilsTo(Page&, PassOwnPtr<NavigatorContentUtilsClient>);

}

#endif // NavigatorContentUtilsClient_h
