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

#ifndef WorkerInspectorController_h
#define WorkerInspectorController_h

#include "core/inspector/InspectorBaseAgent.h"
#include "wtf/FastAllocBase.h"
#include "wtf/Forward.h"
#include "wtf/Noncopyable.h"
#include "wtf/OwnPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

class InjectedScriptManager;
class InspectorBackendDispatcher;
class InspectorFrontend;
class InspectorFrontendChannel;
class InspectorState;
class InspectorStateClient;
class InstrumentingAgents;
class WorkerDebuggerAgent;
class WorkerGlobalScope;
class WorkerScriptDebugServer;

class WorkerInspectorController : public RefCountedWillBeGarbageCollectedFinalized<WorkerInspectorController> {
    WTF_MAKE_NONCOPYABLE(WorkerInspectorController);
    WTF_MAKE_FAST_ALLOCATED_WILL_BE_REMOVED;
public:
    explicit WorkerInspectorController(WorkerGlobalScope*);
    ~WorkerInspectorController();
    void trace(Visitor*);

    void connectFrontend();
    void disconnectFrontend();
    void restoreInspectorStateFromCookie(const String& inspectorCookie);
    void dispatchMessageFromFrontend(const String&);
    void resume();
    void dispose();
    void interruptAndDispatchInspectorCommands();

private:
    friend InstrumentingAgents* instrumentationForWorkerGlobalScope(WorkerGlobalScope*);

    RawPtrWillBeMember<WorkerGlobalScope> m_workerGlobalScope;
    OwnPtr<InspectorStateClient> m_stateClient;
    OwnPtrWillBeMember<InspectorCompositeState> m_state;
    RefPtrWillBeMember<InstrumentingAgents> m_instrumentingAgents;
    OwnPtrWillBeMember<InjectedScriptManager> m_injectedScriptManager;
    OwnPtr<WorkerScriptDebugServer> m_debugServer;
    InspectorAgentRegistry m_agents;
    OwnPtr<InspectorFrontendChannel> m_frontendChannel;
    OwnPtr<InspectorFrontend> m_frontend;
    RefPtr<InspectorBackendDispatcher> m_backendDispatcher;
    RawPtrWillBeMember<WorkerDebuggerAgent> m_workerDebuggerAgent;
};

}

#endif // !defined(WorkerInspectorController_h)
