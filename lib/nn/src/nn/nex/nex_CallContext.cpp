#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SpinTest.h"
#include "nn/nex/nex_SystemError.h"

namespace nn {
namespace nex {
namespace {
const wchar_t* const FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/CallContext.cpp";
// the result of a call that did not complete
const int RESULT_FAILURE = qResult::FAILURE;
// the time between two tests of Wait (ms)
const u32 SPIN_SLEEP_TIME = 10;

bool IsCompleted(u8 state)
{
    return state == CallContext::STATE_SUCCESS || state == CallContext::STATE_FAILURE || state == CallContext::STATE_CANCELLED;
}
} // namespace

// 0x00356988 | fefates:bytes-fuzzy [tier B]
bool nn::nex::CallContext::CancelImpl(nn::nex::CallContext::State state)
{
    u8 currentState = m_State;
    if (IsCompleted(currentState)) {
        return true;
    }
    if (currentState != STATE_CALL_IN_PROGRESS) {
        return false;
    }
    if (!IsCompleted(state)) {
        return false;
    }
    AcquireRef();
    SetStateImpl(state, qResult(RESULT_FAILURE), false);
    if (m_pCancelHandler != nullptr) {
        m_pCancelHandler->OnCancel();
    }
    ReleaseRef();
    return true;
}

// 0x00356A84 | fefates:callgraph [tier C]
bool nn::nex::CallContext::InitiateCall()
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    if (!SetCallInProgressState()) {
        return false;
    }
    m_Id = Core::GetInstance()->m_pCallContextRegister->Register(this);
    return true;
}

// 0x00356B00 | fefates:callgraph [tier C]
void nn::nex::CallContext::SetStateImpl(nn::nex::CallContext::State state, nn::nex::qResult result, bool isSignaled)
{
    {
        ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
        if (state != STATE_CALL_IN_PROGRESS && state == STATE_AVAILABLE) {
            m_Result = qResult(RESULT_FAILURE);
        }
        if (m_State == STATE_CALL_IN_PROGRESS && m_Id != 0) {
            Core::GetInstance()->m_pCallContextRegister->Unregister(m_Id);
            m_Id = 0;
        }
        BeginTransition(state, result, isSignaled);
        m_State = state;
        if (isSignaled) {
            m_Result = result;
        }
    }
    if (!IsCompleted(state)) {
        return;
    }
    for (qList<CompletionCallback>::Node* pNode = m_Callbacks.GetFirst(); !m_Callbacks.IsEnd(pNode); pNode = pNode->m_pNext) {
        CompletionCallback callback = pNode->m_Value;
        callback.m_pFunction(this, &callback.m_Context);
    }
    ProcessCallCompletion();
    if (m_Flags & FLAG_RELEASE_ON_COMPLETION) {
        ReleaseRef();
    }
}

// 0x00356CB8 | fefates:bytes [tier B]
void nn::nex::CallContext::SignalFailure(nn::nex::qResult result)
{
    SetStateImpl(STATE_FAILURE, result, true);
}

// 0x00356CE8 | fefates:bytes [tier B]
void nn::nex::CallContext::SignalSuccess(nn::nex::qResult result)
{
    SetStateImpl(STATE_SUCCESS, result, true);
}

// 0x00356D18
void nn::nex::CallContext::BeginTransition(nn::nex::CallContext::State state, nn::nex::qResult result, bool isSignaled)
{
    // nothing to do in this build
}

// 0x00356D1C
void nn::nex::CallContext::ProcessCallCompletion()
{
    // nothing to do in this build
}

// 0x00356D24 | fefates:callgraph [tier C]
bool nn::nex::CallContext::SetCallInProgressState()
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    if (m_State == STATE_CALL_IN_PROGRESS) {
        return false;
    }
    if (IsCompleted(m_State)) {
        Reset();
    }
    SetStateImpl(STATE_CALL_IN_PROGRESS, qResult(RESULT_FAILURE), false);
    return true;
}

// 0x00356DD8 (name is ours)
void nn::nex::CallContext::SetOwner(void* pOwner, u32 ownerId)
{
    m_pOwner = pOwner;
    m_OwnerId = ownerId;
}

// 0x00356DE4 | fefates:bytes [tier B]
void nn::nex::CallContext::RegisterCompletionCallback(CompletionFunction pFunction, const nn::nex::UserContext& context, bool isAppended)
{
    if (isAppended) {
        m_Callbacks.push_back(CompletionCallback(pFunction, context));
    } else {
        m_Callbacks.push_front(CompletionCallback(pFunction, context));
    }
}

// 0x00356F54 (name is ours)
bool nn::nex::CallContext::WaitCallback(nn::nex::RootObject* pObject, unsigned int timeout)
{
    return static_cast<CallContext*>(pObject)->Wait(timeout);
}

// 0x00356F58 | fefates:bytes-fuzzy [tier B]
bool nn::nex::CallContext::Wait(unsigned int timeout)
{
    if (m_State == STATE_CALL_IN_PROGRESS) {
        bool isCompleted = false;
        if (Scheduler::PreventBlockCall(timeout != 0, timeout, false, WaitCallback, this, &isCompleted)) {
            return isCompleted;
        }
        SpinTest spinTest(SPIN_SLEEP_TIME, timeout);
        spinTest.m_Mode = SpinTest::MODE_STOP;
        while (m_State == STATE_CALL_IN_PROGRESS) {
            Scheduler::GlobalSingleThreadDispatch(spinTest.GetRemainingTime());
            if (!spinTest.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 420, const_cast<wchar_t*>(L"GetState()==CallContext::CallInProgress"))) {
                break;
            }
        }
    }
    return IsCompleted(m_State);
}

// 0x00357078 | fefates:bytes-fuzzy [tier B]
bool nn::nex::CallContext::Reset()
{
    if (m_State == STATE_CALL_IN_PROGRESS) {
        SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 219, SystemError::ERROR_INVALID_STATE, 0);
        return false;
    }
    if (m_State == STATE_AVAILABLE) {
        return true;
    }
    SetStateImpl(STATE_AVAILABLE, qResult(RESULT_FAILURE), false);
    m_Deadline = Time(0);
    m_Callbacks.clear();
    if (m_pCancelHandler != nullptr) {
        delete m_pCancelHandler;
        m_pCancelHandler = nullptr;
    }
    m_pOwner = nullptr;
    m_OwnerId = 0;
    return true;
}

// 0x00357188 | fefates:callgraph [tier C]
bool nn::nex::CallContext::Cancel(nn::nex::CallContext::State state)
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    qVector<u32> ids;
    Core::GetInstance()->m_pCallContextRegister->GetAssociatedId(m_Id, &ids);
    bool isCancelled = CancelImpl(state);
    for (u32* pId = ids.begin(); pId != ids.end(); pId++) {
        CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(*pId);
        if (pContext != nullptr) {
            pContext->Cancel(STATE_CANCELLED);
        }
    }
    return isCancelled;
}

// 0x00357274 (name is ours)
void nn::nex::CallContext::SetFlag(u32 flag)
{
    m_Flags |= flag;
}

// 0x00357284 | mk7dlp:callseq-callee [tier A]
nn::nex::CallContext::CallContext()
    : m_Flags(0), m_State(STATE_AVAILABLE), m_Result(RESULT_FAILURE), m_Id(0), m_pOwner(nullptr), m_OwnerId(0), m_pCancelHandler(nullptr), m_Deadline(0)
{
}

// 0x003573B0 | fefates:callgraph [tier C]
// 0x00357380 (deleting dtor)
nn::nex::CallContext::~CallContext()
{
    Core* pCore = Core::GetInstance();
    if (pCore != nullptr && pCore->m_pScheduler != nullptr) {
        ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
        if (m_State == STATE_CALL_IN_PROGRESS) {
            Cancel(STATE_CANCELLED);
        }
        if (m_Id != 0) {
            Core::GetInstance()->m_pCallContextRegister->Unregister(m_Id);
            m_Id = 0;
        }
    }
    if (m_pCancelHandler != nullptr) {
        delete m_pCancelHandler;
        m_pCancelHandler = nullptr;
    }
    m_Deadline = Time(0);
}

// 0x0072A0D8
bool nn::nex::CallContext::vf_0x08()
{
    if (!(m_Flags & FLAG_UNKNOWN_8)) {
        return true;
    }
    return (m_Flags & FLAG_UNKNOWN_4) != 0;
}

} // namespace nex
} // namespace nn
