// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "config.h"
#include "V8TestDictionary.h"

#include "bindings/core/v8/Dictionary.h"
#include "bindings/tests/v8/V8TestInterface.h"
#include "bindings/tests/v8/V8TestInterfaceGarbageCollected.h"
#include "bindings/tests/v8/V8TestInterfaceWillBeGarbageCollected.h"

namespace blink {

TestDictionary* V8TestDictionary::toNative(v8::Isolate* isolate, v8::Handle<v8::Value> v8Value)
{
    TestDictionary* impl = TestDictionary::create();
    // FIXME: Do not use Dictionary and DictionaryHelper
    // https://crbug.com/321462
    Dictionary dictionary(v8Value, isolate);
    bool booleanMember;
    if (DictionaryHelper::get(dictionary, "booleanMember", booleanMember))
        impl->setBooleanMember(booleanMember);
    double doubleOrNullMember;
    if (DictionaryHelper::get(dictionary, "doubleOrNullMember", doubleOrNullMember))
        impl->setDoubleOrNullMember(doubleOrNullMember);
    int longMember;
    if (DictionaryHelper::get(dictionary, "longMember", longMember))
        impl->setLongMember(longMember);
    else
        impl->setLongMember(1);
    Vector<String> stringArrayMember;
    if (DictionaryHelper::get(dictionary, "stringArrayMember", stringArrayMember))
        impl->setStringArrayMember(stringArrayMember);
    String stringMember;
    if (DictionaryHelper::get(dictionary, "stringMember", stringMember))
        impl->setStringMember(stringMember);
    String stringOrNullMember;
    if (DictionaryHelper::get(dictionary, "stringOrNullMember", stringOrNullMember))
        impl->setStringOrNullMember(stringOrNullMember);
    else
        impl->setStringOrNullMember(String("default string value"));
    Vector<String> stringSequenceMember;
    if (DictionaryHelper::get(dictionary, "stringSequenceMember", stringSequenceMember))
        impl->setStringSequenceMember(stringSequenceMember);
    RawPtr<TestInterfaceGarbageCollected> testInterfaceGarbageCollectedMember;
    if (DictionaryHelper::get(dictionary, "testInterfaceGarbageCollectedMember", testInterfaceGarbageCollectedMember))
        impl->setTestInterfaceGarbageCollectedMember(testInterfaceGarbageCollectedMember);
    RawPtr<TestInterfaceGarbageCollected> testInterfaceGarbageCollectedOrNullMember;
    if (DictionaryHelper::get(dictionary, "testInterfaceGarbageCollectedOrNullMember", testInterfaceGarbageCollectedOrNullMember))
        impl->setTestInterfaceGarbageCollectedOrNullMember(testInterfaceGarbageCollectedOrNullMember);
    RefPtr<TestInterfaceImplementation> testInterfaceMember;
    if (DictionaryHelper::get(dictionary, "testInterfaceMember", testInterfaceMember))
        impl->setTestInterfaceMember(testInterfaceMember);
    RefPtr<TestInterfaceImplementation> testInterfaceOrNullMember;
    if (DictionaryHelper::get(dictionary, "testInterfaceOrNullMember", testInterfaceOrNullMember))
        impl->setTestInterfaceOrNullMember(testInterfaceOrNullMember);
    RefPtrWillBeRawPtr<TestInterfaceWillBeGarbageCollected> testInterfaceWillBeGarbageCollectedMember;
    if (DictionaryHelper::get(dictionary, "testInterfaceWillBeGarbageCollectedMember", testInterfaceWillBeGarbageCollectedMember))
        impl->setTestInterfaceWillBeGarbageCollectedMember(testInterfaceWillBeGarbageCollectedMember);
    RefPtrWillBeRawPtr<TestInterfaceWillBeGarbageCollected> testInterfaceWillBeGarbageCollectedOrNullMember;
    if (DictionaryHelper::get(dictionary, "testInterfaceWillBeGarbageCollectedOrNullMember", testInterfaceWillBeGarbageCollectedOrNullMember))
        impl->setTestInterfaceWillBeGarbageCollectedOrNullMember(testInterfaceWillBeGarbageCollectedOrNullMember);
    return impl;
}

v8::Handle<v8::Value> toV8(TestDictionary* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    v8::Handle<v8::Object> v8Object = v8::Object::New(isolate);
    if (impl->hasBooleanMember())
        v8Object->Set(v8String(isolate, "booleanMember"), v8Boolean(impl->booleanMember(), isolate));
    if (impl->hasDoubleOrNullMember())
        v8Object->Set(v8String(isolate, "doubleOrNullMember"), v8::Number::New(isolate, impl->doubleOrNullMember()));
    else
        v8Object->Set(v8String(isolate, "doubleOrNullMember"), v8::Null(isolate));
    if (impl->hasLongMember())
        v8Object->Set(v8String(isolate, "longMember"), v8::Integer::New(isolate, impl->longMember()));
    else
        v8Object->Set(v8String(isolate, "longMember"), v8::Integer::New(isolate, 1));
    if (impl->hasStringArrayMember())
        v8Object->Set(v8String(isolate, "stringArrayMember"), v8Array(impl->stringArrayMember(), creationContext, isolate));
    if (impl->hasStringMember())
        v8Object->Set(v8String(isolate, "stringMember"), v8String(isolate, impl->stringMember()));
    if (impl->hasStringOrNullMember())
        v8Object->Set(v8String(isolate, "stringOrNullMember"), impl->stringOrNullMember().isNull() ? v8::Handle<v8::Value>(v8::Null(isolate)) : v8String(isolate, impl->stringOrNullMember()));
    else
        v8Object->Set(v8String(isolate, "stringOrNullMember"), v8String(isolate, String("default string value")));
    if (impl->hasStringSequenceMember())
        v8Object->Set(v8String(isolate, "stringSequenceMember"), v8Array(impl->stringSequenceMember(), creationContext, isolate));
    if (impl->hasTestInterfaceGarbageCollectedMember())
        v8Object->Set(v8String(isolate, "testInterfaceGarbageCollectedMember"), toV8(impl->testInterfaceGarbageCollectedMember(), creationContext, isolate));
    if (impl->hasTestInterfaceGarbageCollectedOrNullMember())
        v8Object->Set(v8String(isolate, "testInterfaceGarbageCollectedOrNullMember"), toV8(impl->testInterfaceGarbageCollectedOrNullMember(), creationContext, isolate));
    if (impl->hasTestInterfaceMember())
        v8Object->Set(v8String(isolate, "testInterfaceMember"), toV8(impl->testInterfaceMember(), creationContext, isolate));
    if (impl->hasTestInterfaceOrNullMember())
        v8Object->Set(v8String(isolate, "testInterfaceOrNullMember"), toV8(impl->testInterfaceOrNullMember(), creationContext, isolate));
    if (impl->hasTestInterfaceWillBeGarbageCollectedMember())
        v8Object->Set(v8String(isolate, "testInterfaceWillBeGarbageCollectedMember"), toV8(impl->testInterfaceWillBeGarbageCollectedMember(), creationContext, isolate));
    if (impl->hasTestInterfaceWillBeGarbageCollectedOrNullMember())
        v8Object->Set(v8String(isolate, "testInterfaceWillBeGarbageCollectedOrNullMember"), toV8(impl->testInterfaceWillBeGarbageCollectedOrNullMember(), creationContext, isolate));
    return v8Object;
}

} // namespace blink
