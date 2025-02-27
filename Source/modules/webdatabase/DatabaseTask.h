/*
 * Copyright (C) 2007, 2008, 2013 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DatabaseTask_h
#define DatabaseTask_h

#include "modules/webdatabase/DatabaseBackend.h"
#include "modules/webdatabase/DatabaseBasicTypes.h"
#include "modules/webdatabase/DatabaseError.h"
#include "modules/webdatabase/SQLTransactionBackend.h"
#include "platform/Task.h"
#include "platform/TaskSynchronizer.h"
#include "platform/heap/Handle.h"
#include "wtf/OwnPtr.h"
#include "wtf/PassOwnPtr.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefPtr.h"
#include "wtf/Threading.h"
#include "wtf/Vector.h"
#include "wtf/text/WTFString.h"

namespace blink {

class DatabaseTask : public WebThread::Task {
    WTF_MAKE_NONCOPYABLE(DatabaseTask); WTF_MAKE_FAST_ALLOCATED;
public:
    virtual ~DatabaseTask();

    virtual void run() OVERRIDE FINAL;

    DatabaseBackend* database() const { return m_database.get(); }
#if ENABLE(ASSERT)
    bool hasSynchronizer() const { return m_synchronizer; }
    bool hasCheckedForTermination() const { return m_synchronizer->hasCheckedForTermination(); }
#endif

protected:
    DatabaseTask(DatabaseBackend*, TaskSynchronizer*);

private:
    virtual void doPerformTask() = 0;
    virtual void taskCancelled() { }

    RefPtrWillBeCrossThreadPersistent<DatabaseBackend> m_database;
    TaskSynchronizer* m_synchronizer;

#if !LOG_DISABLED
    virtual const char* debugTaskName() const = 0;
    bool m_complete;
#endif
};

class DatabaseBackend::DatabaseOpenTask FINAL : public DatabaseTask {
public:
    static PassOwnPtr<DatabaseOpenTask> create(DatabaseBackend* db, bool setVersionInNewDatabase, TaskSynchronizer* synchronizer, DatabaseError& error, String& errorMessage, bool& success)
    {
        return adoptPtr(new DatabaseOpenTask(db, setVersionInNewDatabase, synchronizer, error, errorMessage, success));
    }

private:
    DatabaseOpenTask(DatabaseBackend*, bool setVersionInNewDatabase, TaskSynchronizer*, DatabaseError&, String& errorMessage, bool& success);

    virtual void doPerformTask() OVERRIDE;
#if !LOG_DISABLED
    virtual const char* debugTaskName() const OVERRIDE;
#endif

    bool m_setVersionInNewDatabase;
    DatabaseError& m_error;
    String& m_errorMessage;
    bool& m_success;
};

class DatabaseBackend::DatabaseCloseTask FINAL : public DatabaseTask {
public:
    static PassOwnPtr<DatabaseCloseTask> create(DatabaseBackend* db, TaskSynchronizer* synchronizer)
    {
        return adoptPtr(new DatabaseCloseTask(db, synchronizer));
    }

private:
    DatabaseCloseTask(DatabaseBackend*, TaskSynchronizer*);

    virtual void doPerformTask() OVERRIDE;
#if !LOG_DISABLED
    virtual const char* debugTaskName() const OVERRIDE;
#endif
};

class DatabaseBackend::DatabaseTransactionTask FINAL : public DatabaseTask {
public:
    virtual ~DatabaseTransactionTask();

    // Transaction task is never synchronous, so no 'synchronizer' parameter.
    static PassOwnPtr<DatabaseTransactionTask> create(PassRefPtrWillBeRawPtr<SQLTransactionBackend> transaction)
    {
        return adoptPtr(new DatabaseTransactionTask(transaction));
    }

    SQLTransactionBackend* transaction() const { return m_transaction.get(); }

private:
    explicit DatabaseTransactionTask(PassRefPtrWillBeRawPtr<SQLTransactionBackend>);

    virtual void doPerformTask() OVERRIDE;
    virtual void taskCancelled() OVERRIDE;
#if !LOG_DISABLED
    virtual const char* debugTaskName() const OVERRIDE;
#endif

    RefPtrWillBeCrossThreadPersistent<SQLTransactionBackend> m_transaction;
};

class DatabaseBackend::DatabaseTableNamesTask FINAL : public DatabaseTask {
public:
    static PassOwnPtr<DatabaseTableNamesTask> create(DatabaseBackend* db, TaskSynchronizer* synchronizer, Vector<String>& names)
    {
        return adoptPtr(new DatabaseTableNamesTask(db, synchronizer, names));
    }

private:
    DatabaseTableNamesTask(DatabaseBackend*, TaskSynchronizer*, Vector<String>& names);

    virtual void doPerformTask() OVERRIDE;
#if !LOG_DISABLED
    virtual const char* debugTaskName() const OVERRIDE;
#endif

    Vector<String>& m_tableNames;
};

} // namespace blink

#endif // DatabaseTask_h
