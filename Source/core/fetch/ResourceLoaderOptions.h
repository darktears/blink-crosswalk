/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
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

#ifndef ResourceLoaderOptions_h
#define ResourceLoaderOptions_h

#include "core/fetch/FetchInitiatorInfo.h"
#include "platform/CrossThreadCopier.h"
#include "platform/weborigin/SecurityOrigin.h"

namespace blink {

enum ContentSniffingPolicy {
    SniffContent,
    DoNotSniffContent
};

enum DataBufferingPolicy {
    BufferData,
    DoNotBufferData
};

enum ContentSecurityPolicyCheck {
    CheckContentSecurityPolicy,
    DoNotCheckContentSecurityPolicy
};

enum RequestInitiatorContext {
    DocumentContext,
    WorkerContext,
};

enum StoredCredentials {
    AllowStoredCredentials,
    DoNotAllowStoredCredentials
};

// APIs like XMLHttpRequest and EventSource let the user decide
// whether to send credentials, but they're always sent for
// same-origin requests. Additional information is needed to handle
// cross-origin redirects correctly.
enum CredentialRequest {
    ClientRequestedCredentials,
    ClientDidNotRequestCredentials
};

enum MixedContentBlockingTreatment {
    TreatAsDefaultForType,
    TreatAsPassiveContent,
    TreatAsActiveContent,
    TreatAsAlwaysAllowedContent
};

enum SynchronousPolicy {
    RequestSynchronously,
    RequestAsynchronously
};

// A resource fetch can be marked as being CORS enabled. The loader
// must perform an access check upon seeing the response.
enum CORSEnabled {
    NotCORSEnabled,
    IsCORSEnabled
};

struct ResourceLoaderOptions {
    ResourceLoaderOptions()
        : sniffContent(DoNotSniffContent)
        , dataBufferingPolicy(BufferData)
        , allowCredentials(DoNotAllowStoredCredentials)
        , credentialsRequested(ClientDidNotRequestCredentials)
        , contentSecurityPolicyOption(CheckContentSecurityPolicy)
        , requestInitiatorContext(DocumentContext)
        , mixedContentBlockingTreatment(TreatAsDefaultForType)
        , synchronousPolicy(RequestAsynchronously)
        , corsEnabled(NotCORSEnabled)
    {
    }

    ResourceLoaderOptions(
        ContentSniffingPolicy sniffContent,
        DataBufferingPolicy dataBufferingPolicy,
        StoredCredentials allowCredentials,
        CredentialRequest credentialsRequested,
        ContentSecurityPolicyCheck contentSecurityPolicyOption,
        RequestInitiatorContext requestInitiatorContext)
        : sniffContent(sniffContent)
        , dataBufferingPolicy(dataBufferingPolicy)
        , allowCredentials(allowCredentials)
        , credentialsRequested(credentialsRequested)
        , contentSecurityPolicyOption(contentSecurityPolicyOption)
        , requestInitiatorContext(requestInitiatorContext)
        , mixedContentBlockingTreatment(TreatAsDefaultForType)
        , synchronousPolicy(RequestAsynchronously)
        , corsEnabled(NotCORSEnabled)
    {
    }

    // Answers the question "can a separate request with these
    // different options be re-used" (e.g. preload request)
    // The safe (but possibly slow) answer is always false.
    bool canReuseRequest(const ResourceLoaderOptions& other) const
    {
        // sniffContent is dead code.
        // dataBufferingPolicy differences are believed to be safe for re-use.
        // FIXME: check allowCredentials.
        // FIXME: check credentialsRequested.
        // FIXME: check contentSecurityPolicyOption.
        // initiatorInfo is purely informational and should be benign for re-use.
        // requestInitiatorContext is benign (indicates document vs. worker)
        // FIXME: check mixedContentBlockingTreatment.
        // synchronousPolicy (safe to re-use an async XHR response for sync, etc.)
        return corsEnabled == other.corsEnabled;
        // securityOrigin has more complicated checks which callers are responsible for.
    }

    // When adding members, CrossThreadResourceLoaderOptionsData should be
    // updated.
    ContentSniffingPolicy sniffContent; // FIXME: Dead code, please remove.
    DataBufferingPolicy dataBufferingPolicy;
    StoredCredentials allowCredentials; // Whether HTTP credentials and cookies are sent with the request.
    CredentialRequest credentialsRequested; // Whether the client (e.g. XHR) wanted credentials in the first place.
    ContentSecurityPolicyCheck contentSecurityPolicyOption;
    FetchInitiatorInfo initiatorInfo;
    RequestInitiatorContext requestInitiatorContext;
    MixedContentBlockingTreatment mixedContentBlockingTreatment;
    SynchronousPolicy synchronousPolicy;
    CORSEnabled corsEnabled; // If the resource is loaded out-of-origin, whether or not to use CORS.
    RefPtr<SecurityOrigin> securityOrigin;
};

// Encode AtomicString (in FetchInitiatorInfo) as String to cross threads.
struct CrossThreadResourceLoaderOptionsData {
    explicit CrossThreadResourceLoaderOptionsData(const ResourceLoaderOptions& options)
        : sniffContent(options.sniffContent)
        , dataBufferingPolicy(options.dataBufferingPolicy)
        , allowCredentials(options.allowCredentials)
        , credentialsRequested(options.credentialsRequested)
        , contentSecurityPolicyOption(options.contentSecurityPolicyOption)
        , initiatorInfo(options.initiatorInfo)
        , requestInitiatorContext(options.requestInitiatorContext)
        , mixedContentBlockingTreatment(options.mixedContentBlockingTreatment)
        , synchronousPolicy(options.synchronousPolicy)
        , corsEnabled(options.corsEnabled)
        , securityOrigin(options.securityOrigin) { }

    operator ResourceLoaderOptions() const
    {
        ResourceLoaderOptions options;
        options.sniffContent = sniffContent;
        options.dataBufferingPolicy = dataBufferingPolicy;
        options.allowCredentials = allowCredentials;
        options.credentialsRequested = credentialsRequested;
        options.contentSecurityPolicyOption = contentSecurityPolicyOption;
        options.initiatorInfo = initiatorInfo;
        options.requestInitiatorContext = requestInitiatorContext;
        options.mixedContentBlockingTreatment = mixedContentBlockingTreatment;
        options.synchronousPolicy = synchronousPolicy;
        options.corsEnabled = corsEnabled;
        options.securityOrigin = securityOrigin;
        return options;
    }

    ContentSniffingPolicy sniffContent;
    DataBufferingPolicy dataBufferingPolicy;
    StoredCredentials allowCredentials;
    CredentialRequest credentialsRequested;
    ContentSecurityPolicyCheck contentSecurityPolicyOption;
    CrossThreadFetchInitiatorInfoData initiatorInfo;
    RequestInitiatorContext requestInitiatorContext;
    MixedContentBlockingTreatment mixedContentBlockingTreatment;
    SynchronousPolicy synchronousPolicy;
    CORSEnabled corsEnabled;
    RefPtr<SecurityOrigin> securityOrigin;
};

template<> struct CrossThreadCopierBase<false, false, false, ResourceLoaderOptions> {
    typedef CrossThreadResourceLoaderOptionsData Type;
    static Type copy(const ResourceLoaderOptions& options)
    {
        return CrossThreadResourceLoaderOptionsData(options);
    }
};

} // namespace blink

#endif // ResourceLoaderOptions_h
