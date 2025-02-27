/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
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
#include "public/web/WebCryptoNormalize.h"

#include "bindings/core/v8/Dictionary.h"
#include "modules/crypto/CryptoResultImpl.h"
#include "modules/crypto/NormalizeAlgorithm.h"
#include "platform/CryptoResult.h"
#include "public/platform/WebString.h"
#include <v8.h>

using namespace blink;

namespace blink {


WebCryptoAlgorithm normalizeCryptoAlgorithm(v8::Handle<v8::Object> algorithmObject, WebCryptoOperation operation, int* exceptionCode, WebString* errorDetails, v8::Isolate* isolate)
{
    blink::Dictionary algorithmDictionary(algorithmObject, isolate);
    if (!algorithmDictionary.isUndefinedOrNull() && !algorithmDictionary.isObject())
        return WebCryptoAlgorithm();
    WebCryptoAlgorithm algorithm;
    blink::AlgorithmError error;
    if (!normalizeAlgorithm(algorithmDictionary, operation, algorithm, &error)) {
        *exceptionCode = blink::webCryptoErrorToExceptionCode(error.errorType);
        *errorDetails = error.errorDetails;
        return WebCryptoAlgorithm();
    }

    return algorithm;
}

} // namespace blink
