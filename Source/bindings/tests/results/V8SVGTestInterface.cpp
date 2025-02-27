// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "config.h"
#include "V8SVGTestInterface.h"

#include "bindings/core/v8/ExceptionState.h"
#include "bindings/core/v8/V8DOMConfiguration.h"
#include "bindings/core/v8/V8HiddenValue.h"
#include "bindings/core/v8/V8ObjectConstructor.h"
#include "core/SVGNames.h"
#include "core/dom/ContextFeatures.h"
#include "core/dom/Document.h"
#include "core/dom/custom/CustomElementCallbackDispatcher.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "platform/TraceEvent.h"
#include "wtf/GetPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

static void initializeScriptWrappableForInterface(SVGTestInterface* object)
{
    if (ScriptWrappable::wrapperCanBeStoredInObject(object))
        ScriptWrappable::fromObject(object)->setTypeInfo(&V8SVGTestInterface::wrapperTypeInfo);
    else
        ASSERT_NOT_REACHED();
}

} // namespace blink

void webCoreInitializeScriptWrappableForInterface(blink::SVGTestInterface* object)
{
    blink::initializeScriptWrappableForInterface(object);
}

namespace blink {
const WrapperTypeInfo V8SVGTestInterface::wrapperTypeInfo = { gin::kEmbedderBlink, V8SVGTestInterface::domTemplate, V8SVGTestInterface::derefObject, 0, 0, 0, V8SVGTestInterface::installConditionallyEnabledMethods, 0, WrapperTypeObjectPrototype, RefCountedObject };

namespace SVGTestInterfaceV8Internal {

template <typename T> void V8_USE(T) { }

static void typeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Handle<v8::Object> holder = info.Holder();
    SVGTestInterface* impl = V8SVGTestInterface::toNative(holder);
    v8SetReturnValueString(info, impl->fastGetAttribute(SVGNames::typeAttr), info.GetIsolate());
}

static void typeAttributeGetterCallback(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("blink", "DOMGetter");
    SVGTestInterfaceV8Internal::typeAttributeGetter(info);
    TRACE_EVENT_SET_SAMPLING_STATE("v8", "V8Execution");
}

static void typeAttributeSetter(v8::Local<v8::Value> v8Value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Handle<v8::Object> holder = info.Holder();
    SVGTestInterface* impl = V8SVGTestInterface::toNative(holder);
    TOSTRING_VOID(V8StringResource<>, cppValue, v8Value);
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    impl->setAttribute(SVGNames::typeAttr, cppValue);
}

static void typeAttributeSetterCallback(v8::Local<v8::String>, v8::Local<v8::Value> v8Value, const v8::PropertyCallbackInfo<void>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("blink", "DOMSetter");
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    SVGTestInterfaceV8Internal::typeAttributeSetter(v8Value, info);
    TRACE_EVENT_SET_SAMPLING_STATE("v8", "V8Execution");
}

} // namespace SVGTestInterfaceV8Internal

static const V8DOMConfiguration::AttributeConfiguration V8SVGTestInterfaceAttributes[] = {
    {"type", SVGTestInterfaceV8Internal::typeAttributeGetterCallback, SVGTestInterfaceV8Internal::typeAttributeSetterCallback, 0, 0, 0, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), V8DOMConfiguration::ExposedToAllScripts, V8DOMConfiguration::OnInstance},
};

static void installV8SVGTestInterfaceTemplate(v8::Handle<v8::FunctionTemplate> functionTemplate, v8::Isolate* isolate)
{
    functionTemplate->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = V8DOMConfiguration::installDOMClassTemplate(functionTemplate, "SVGTestInterface", v8::Local<v8::FunctionTemplate>(), V8SVGTestInterface::internalFieldCount,
        V8SVGTestInterfaceAttributes, WTF_ARRAY_LENGTH(V8SVGTestInterfaceAttributes),
        0, 0,
        0, 0,
        isolate);
    v8::Local<v8::ObjectTemplate> instanceTemplate ALLOW_UNUSED = functionTemplate->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> prototypeTemplate ALLOW_UNUSED = functionTemplate->PrototypeTemplate();

    // Custom toString template
    functionTemplate->Set(v8AtomicString(isolate, "toString"), V8PerIsolateData::from(isolate)->toStringTemplate());
}

v8::Handle<v8::FunctionTemplate> V8SVGTestInterface::domTemplate(v8::Isolate* isolate)
{
    return V8DOMConfiguration::domClassTemplate(isolate, const_cast<WrapperTypeInfo*>(&wrapperTypeInfo), installV8SVGTestInterfaceTemplate);
}

bool V8SVGTestInterface::hasInstance(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->hasInstance(&wrapperTypeInfo, v8Value);
}

v8::Handle<v8::Object> V8SVGTestInterface::findInstanceInPrototypeChain(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->findInstanceInPrototypeChain(&wrapperTypeInfo, v8Value);
}

SVGTestInterface* V8SVGTestInterface::toNativeWithTypeCheck(v8::Isolate* isolate, v8::Handle<v8::Value> value)
{
    return hasInstance(value, isolate) ? fromInternalPointer(blink::toInternalPointer(v8::Handle<v8::Object>::Cast(value))) : 0;
}

v8::Handle<v8::Object> wrap(SVGTestInterface* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(!DOMDataStore::containsWrapper<V8SVGTestInterface>(impl, isolate));
    return V8SVGTestInterface::createWrapper(impl, creationContext, isolate);
}

v8::Handle<v8::Object> V8SVGTestInterface::createWrapper(PassRefPtr<SVGTestInterface> impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(!DOMDataStore::containsWrapper<V8SVGTestInterface>(impl.get(), isolate));
    if (ScriptWrappable::wrapperCanBeStoredInObject(impl.get())) {
        const WrapperTypeInfo* actualInfo = ScriptWrappable::fromObject(impl.get())->typeInfo();
        // Might be a XXXConstructor::wrapperTypeInfo instead of an XXX::wrapperTypeInfo. These will both have
        // the same object de-ref functions, though, so use that as the basis of the check.
        RELEASE_ASSERT_WITH_SECURITY_IMPLICATION(actualInfo->derefObjectFunction == wrapperTypeInfo.derefObjectFunction);
    }

    v8::Handle<v8::Object> wrapper = V8DOMWrapper::createWrapper(creationContext, &wrapperTypeInfo, toInternalPointer(impl.get()), isolate);
    if (UNLIKELY(wrapper.IsEmpty()))
        return wrapper;

    installConditionallyEnabledProperties(wrapper, isolate);
    V8DOMWrapper::associateObjectWithWrapper<V8SVGTestInterface>(impl, &wrapperTypeInfo, wrapper, isolate, WrapperConfiguration::Dependent);
    return wrapper;
}

void V8SVGTestInterface::derefObject(ScriptWrappableBase* internalPointer)
{
    fromInternalPointer(internalPointer)->deref();
}

template<>
v8::Handle<v8::Value> toV8NoInline(SVGTestInterface* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl, creationContext, isolate);
}

} // namespace blink
