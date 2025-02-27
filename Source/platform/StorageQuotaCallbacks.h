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

#ifndef StorageQuotaCallbacks_h
#define StorageQuotaCallbacks_h

#include "platform/PlatformExport.h"
#include "public/platform/WebStorageQuotaError.h"
#include "wtf/Assertions.h"
#include "wtf/Noncopyable.h"

namespace blink {

class PLATFORM_EXPORT StorageQuotaCallbacks {
    WTF_MAKE_NONCOPYABLE(StorageQuotaCallbacks);
public:
    StorageQuotaCallbacks() { }
    virtual ~StorageQuotaCallbacks() { }

    virtual void didQueryStorageUsageAndQuota(unsigned long long usageInBytes, unsigned long long quotaInBytes) { ASSERT_NOT_REACHED(); };
    virtual void didGrantStorageQuota(unsigned long long usageInBytes, unsigned long long grantedQuotaInBytes) { ASSERT_NOT_REACHED(); };
    virtual void didFail(WebStorageQuotaError) { ASSERT_NOT_REACHED(); };
};

} // namespace blink

#endif // StorageQuotaCallbacks_h
