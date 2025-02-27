// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Headers_h
#define Headers_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "modules/serviceworkers/FetchHeaderList.h"
#include "wtf/Forward.h"
#include "wtf/PassOwnPtr.h"
#include "wtf/RefCounted.h"

namespace blink {

class Dictionary;
class ExceptionState;
class HeadersForEachCallback;
class ScriptValue;

// http://fetch.spec.whatwg.org/#headers-class
class Headers FINAL : public RefCountedWillBeGarbageCollected<Headers>, public ScriptWrappable {
    DECLARE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(Headers);
public:
    enum Guard { ImmutableGuard, RequestGuard, RequestNoCORSGuard, ResponseGuard, NoneGuard };

    static PassRefPtrWillBeRawPtr<Headers> create();
    static PassRefPtrWillBeRawPtr<Headers> create(ExceptionState&);
    static PassRefPtrWillBeRawPtr<Headers> create(const Headers*, ExceptionState&);
    static PassRefPtrWillBeRawPtr<Headers> create(const Dictionary&, ExceptionState&);

    // Shares the FetchHeaderList. Called when creating a Request or Response.
    static PassRefPtrWillBeRawPtr<Headers> create(FetchHeaderList*);

    PassRefPtrWillBeRawPtr<Headers> createCopy() const;

    // Headers.idl implementation.
    void append(const String& name, const String& value, ExceptionState&);
    void remove(const String& key, ExceptionState&);
    String get(const String& key, ExceptionState&);
    Vector<String> getAll(const String& key, ExceptionState&);
    bool has(const String& key, ExceptionState&);
    void set(const String& key, const String& value, ExceptionState&);
    unsigned long size() const;
    void forEach(PassOwnPtr<HeadersForEachCallback>, ScriptValue&);
    void forEach(PassOwnPtr<HeadersForEachCallback>);

    void setGuard(Guard guard) { m_guard = guard; }
    Guard guard() const { return m_guard; }

    // These methods should only be called when size() would return 0.
    void fillWith(const Headers*, ExceptionState&);
    void fillWith(const Dictionary&, ExceptionState&);

    void trace(Visitor*);

private:
    Headers();
    // Shares the FetchHeaderList. Called when creating a Request or Response.
    explicit Headers(FetchHeaderList*);
    void forEachInternal(PassOwnPtr<HeadersForEachCallback>, ScriptValue*);

    RefPtrWillBeMember<FetchHeaderList> m_headerList;
    Guard m_guard;
};

} // namespace blink

#endif // Headers_h
