// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RegistrationOptionList_h
#define RegistrationOptionList_h

#include "bindings/core/v8/Dictionary.h"

namespace blink {

struct RegistrationOptionList  {
    explicit RegistrationOptionList(const Dictionary& options)
        : scope("/")
    {
        // FIXME: Should be ScalarValueString. http://crbug.com/379009
        DictionaryHelper::get(options, "scope", scope);
    }

    String scope;
};

} // namespace blink

#endif // RegistrationOptionList_h
