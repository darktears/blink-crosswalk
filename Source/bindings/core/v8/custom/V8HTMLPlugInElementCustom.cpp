/*
* Copyright (C) 2009 Google Inc. All rights reserved.
* Copyright (C) 2014 Opera Software ASA. All rights reserved.
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

#include "bindings/core/v8/NPV8Object.h"
#include "bindings/core/v8/SharedPersistent.h"
#include "bindings/core/v8/V8Binding.h"
#include "bindings/core/v8/V8HTMLAppletElement.h"
#include "bindings/core/v8/V8HTMLEmbedElement.h"
#include "bindings/core/v8/V8HTMLObjectElement.h"
#include "bindings/core/v8/V8NPObject.h"
#include "core/frame/UseCounter.h"
#include "wtf/OwnPtr.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

namespace {

template <typename ElementType, typename PropertyType>
void getScriptableObjectProperty(PropertyType property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    HTMLPlugInElement* impl = ElementType::toNative(info.Holder());
    RefPtr<SharedPersistent<v8::Object> > wrapper = impl->pluginWrapper();
    if (!wrapper)
        return;

    v8::Local<v8::Object> instance = wrapper->newLocal(info.GetIsolate());
    if (instance.IsEmpty())
        return;

    TONATIVE_VOID(v8::Local<v8::Value>, value, instance->Get(property));

    // We quit here to allow the binding code to look up general HTMLObjectElement properties
    // if they are not overriden by plugin.
    if (value->IsUndefined())
        return;

    v8SetReturnValue(info, value);
}

namespace {
void callNpObjectSetter(v8::Local<v8::Object> self, v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    npObjectSetNamedProperty(self, name, value, info);
}

void callNpObjectSetter(v8::Local<v8::Object> self, uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    npObjectSetIndexedProperty(self, index, value, info);
}
}

template <typename ElementType, typename PropertyType>
void setScriptableObjectProperty(PropertyType property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    HTMLPlugInElement* impl = ElementType::toNative(info.Holder());
    RefPtr<SharedPersistent<v8::Object> > wrapper = impl->pluginWrapper();
    if (!wrapper)
        return;

    v8::Local<v8::Object> instance = wrapper->newLocal(info.GetIsolate());
    if (instance.IsEmpty())
        return;

    // We need to directly call setter on NPObject to be able to detect
    // situation where NPObject notifies it does not possess the property
    // to be able to lookup standard DOM property.
    // This information is lost when retrieving it through v8::Object.
    if (isWrappedNPObject(instance)) {
        callNpObjectSetter(instance, property, value, info);
        return;
    }

    if (instance->Set(property, value))
        v8SetReturnValue(info, value);
}
} // namespace

void V8HTMLAppletElement::namedPropertyGetterCustom(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    getScriptableObjectProperty<V8HTMLAppletElement>(name, info);
}

void V8HTMLEmbedElement::namedPropertyGetterCustom(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    getScriptableObjectProperty<V8HTMLEmbedElement>(name, info);
}

void V8HTMLObjectElement::namedPropertyGetterCustom(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    getScriptableObjectProperty<V8HTMLObjectElement>(name, info);
}

void V8HTMLAppletElement::namedPropertySetterCustom(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    setScriptableObjectProperty<V8HTMLAppletElement>(name, value, info);
}

void V8HTMLEmbedElement::namedPropertySetterCustom(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    setScriptableObjectProperty<V8HTMLEmbedElement>(name, value, info);
}

void V8HTMLObjectElement::namedPropertySetterCustom(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    setScriptableObjectProperty<V8HTMLObjectElement>(name, value, info);
}

void V8HTMLAppletElement::indexedPropertyGetterCustom(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    getScriptableObjectProperty<V8HTMLAppletElement>(index, info);
}

void V8HTMLEmbedElement::indexedPropertyGetterCustom(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    getScriptableObjectProperty<V8HTMLEmbedElement>(index, info);
}

void V8HTMLObjectElement::indexedPropertyGetterCustom(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    getScriptableObjectProperty<V8HTMLObjectElement>(index, info);
}

void V8HTMLAppletElement::indexedPropertySetterCustom(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    setScriptableObjectProperty<V8HTMLAppletElement>(index, value, info);
}

void V8HTMLEmbedElement::indexedPropertySetterCustom(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    setScriptableObjectProperty<V8HTMLEmbedElement>(index, value, info);
}

void V8HTMLObjectElement::indexedPropertySetterCustom(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    setScriptableObjectProperty<V8HTMLObjectElement>(index, value, info);
}

namespace {

template <typename ElementType>
void invokeOnScriptableObject(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    HTMLPlugInElement* impl = ElementType::toNative(info.Holder());
    RefPtr<SharedPersistent<v8::Object> > wrapper = impl->pluginWrapper();
    if (!wrapper)
        return;

    v8::Local<v8::Object> instance = wrapper->newLocal(info.GetIsolate());
    if (instance.IsEmpty())
        return;

    WTF::OwnPtr<v8::Handle<v8::Value>[] > arguments = adoptArrayPtr(new v8::Handle<v8::Value>[info.Length()]);
    for (int i = 0; i < info.Length(); ++i)
        arguments[i] = info[i];

    TONATIVE_VOID(v8::Local<v8::Value>, retVal, instance->CallAsFunction(info.This(), info.Length(), arguments.get()));
    v8SetReturnValue(info, retVal);
}

} // namespace

void V8HTMLAppletElement::legacyCallCustom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    invokeOnScriptableObject<V8HTMLAppletElement>(info);
    UseCounter::count(V8HTMLAppletElement::toNative(info.Holder())->document(), UseCounter::HTMLAppletElementLegacyCall);
}

void V8HTMLEmbedElement::legacyCallCustom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    invokeOnScriptableObject<V8HTMLEmbedElement>(info);
    UseCounter::count(V8HTMLEmbedElement::toNative(info.Holder())->document(), UseCounter::HTMLEmbedElementLegacyCall);
}

void V8HTMLObjectElement::legacyCallCustom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    invokeOnScriptableObject<V8HTMLObjectElement>(info);
    UseCounter::count(V8HTMLObjectElement::toNative(info.Holder())->document(), UseCounter::HTMLObjectElementLegacyCall);
}

} // namespace blink
