/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
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
#include "modules/filesystem/EntrySync.h"

#include "bindings/core/v8/ExceptionMessages.h"
#include "bindings/core/v8/ExceptionState.h"
#include "core/dom/ExceptionCode.h"
#include "modules/filesystem/DOMFilePath.h"
#include "modules/filesystem/DirectoryEntry.h"
#include "modules/filesystem/DirectoryEntrySync.h"
#include "modules/filesystem/FileEntrySync.h"
#include "modules/filesystem/Metadata.h"
#include "modules/filesystem/SyncCallbackHelper.h"

namespace blink {

EntrySync* EntrySync::create(EntryBase* entry)
{
    if (entry->isFile())
        return FileEntrySync::create(entry->m_fileSystem, entry->m_fullPath);
    return DirectoryEntrySync::create(entry->m_fileSystem, entry->m_fullPath);
}

Metadata* EntrySync::getMetadata(ExceptionState& exceptionState)
{
    RefPtr<MetadataSyncCallbackHelper> helper = MetadataSyncCallbackHelper::create();
    m_fileSystem->getMetadata(this, helper->successCallback(), helper->errorCallback(), DOMFileSystemBase::Synchronous);
    return helper->getResult(exceptionState);
}

EntrySync* EntrySync::moveTo(DirectoryEntrySync* parent, const String& name, ExceptionState& exceptionState) const
{
    RefPtr<EntrySyncCallbackHelper> helper = EntrySyncCallbackHelper::create();
    m_fileSystem->move(this, parent, name, helper->successCallback(), helper->errorCallback(), DOMFileSystemBase::Synchronous);
    return helper->getResult(exceptionState);
}

EntrySync* EntrySync::copyTo(DirectoryEntrySync* parent, const String& name, ExceptionState& exceptionState) const
{
    RefPtr<EntrySyncCallbackHelper> helper = EntrySyncCallbackHelper::create();
    m_fileSystem->copy(this, parent, name, helper->successCallback(), helper->errorCallback(), DOMFileSystemBase::Synchronous);
    return helper->getResult(exceptionState);
}

void EntrySync::remove(ExceptionState& exceptionState) const
{
    RefPtr<VoidSyncCallbackHelper> helper = VoidSyncCallbackHelper::create();
    m_fileSystem->remove(this, helper->successCallback(), helper->errorCallback(), DOMFileSystemBase::Synchronous);
    helper->getResult(exceptionState);
}

EntrySync* EntrySync::getParent() const
{
    // Sync verion of getParent doesn't throw exceptions.
    String parentPath = DOMFilePath::getDirectory(fullPath());
    return DirectoryEntrySync::create(m_fileSystem, parentPath);
}

EntrySync::EntrySync(DOMFileSystemBase* fileSystem, const String& fullPath)
    : EntryBase(fileSystem, fullPath)
{
    ScriptWrappable::init(this);
}

void EntrySync::trace(Visitor* visitor)
{
    EntryBase::trace(visitor);
}

}
