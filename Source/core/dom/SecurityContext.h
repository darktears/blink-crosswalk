/*
 * Copyright (C) 2011 Google Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY GOOGLE, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef SecurityContext_h
#define SecurityContext_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefPtr.h"
#include "wtf/text/WTFString.h"

namespace blink {

class SecurityOrigin;
class ContentSecurityPolicy;
class KURL;

class SecurityContext {
public:
    SecurityOrigin* securityOrigin() const { return m_securityOrigin.get(); }
    ContentSecurityPolicy* contentSecurityPolicy() { return m_contentSecurityPolicy.get(); }

    bool isSecureTransitionTo(const KURL&) const;

    // Explicitly override the security origin for this security context.
    // Note: It is dangerous to change the security origin of a script context
    //       that already contains content.
    void setSecurityOrigin(PassRefPtr<SecurityOrigin>);

protected:
    SecurityContext();
    virtual ~SecurityContext();

    void setContentSecurityPolicy(PassRefPtr<ContentSecurityPolicy>);

    void didFailToInitializeSecurityOrigin() { m_haveInitializedSecurityOrigin = false; }
    bool haveInitializedSecurityOrigin() const { return m_haveInitializedSecurityOrigin; }

private:
    bool m_haveInitializedSecurityOrigin;
    RefPtr<SecurityOrigin> m_securityOrigin;
    RefPtr<ContentSecurityPolicy> m_contentSecurityPolicy;
};

} // namespace blink

#endif // SecurityContext_h
