#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_UserContext.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// The state of an asynchronous call: in progress until it is signalled (success, failure) or
// cancelled; then the completion callbacks run. Running calls are in the CallContextRegister. The
// names of the members, of the states other than CallInProgress (from an assert text) and of the
// functions marked so are ours.
// RTTI N2nn3nex11CallContextE @ 0x008CDFF4
// vtable 0x008FC254 (vptr 0x008FC25C), offset_to_top 0, 5 entries
class CallContext : public ::nn::nex::RefCountedObject
{
public:
    enum State
    {
        STATE_AVAILABLE = 0,
        STATE_CALL_IN_PROGRESS = 1,
        STATE_SUCCESS = 2,
        STATE_FAILURE = 3,
        STATE_CANCELLED = 4,
    };
    // the flags (m_Flags)
    // the call context releases a reference to itself when the call completes
    static const u32 FLAG_RELEASE_ON_COMPLETION = 1 << 1;
    static const u32 FLAG_UNKNOWN_4 = 1 << 2;
    static const u32 FLAG_UNKNOWN_8 = 1 << 3;

    // a function that runs when the call completes
    typedef void (*CompletionFunction)(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext);
    struct CompletionCallback
    {
        CompletionCallback(CompletionFunction pFunction, const UserContext& context) : m_pFunction(pFunction), m_Context(context) {}

        CompletionFunction m_pFunction; // 0x0
        UserContext m_Context;          // 0x4
    };
    // the object of m_pCancelHandler (its type is not known; the names are ours)
    class CancelHandler
    {
    public:
        virtual ~CancelHandler() {}
        // called when the call is cancelled
        virtual void OnCancel() = 0; // slot 0x08
    };

    CallContext(); // 0x00357284 | mk7dlp:callseq-callee [tier A]
    virtual ~CallContext(); // 0x003573B0 slot 0x00 | fefates:callgraph [tier C]
    // 0x00357380 slot 0x04 (deleting dtor)
    // (inline in the original, with a copy out of line)
    virtual bool vf_0x08(); // 0x0072A0D8 slot 0x08
    // the derived classes see each state change before it is made
    virtual void BeginTransition(nn::nex::CallContext::State state, nn::nex::qResult result, bool isSignaled); // 0x00356D18 slot 0x0C
    // after the completion callbacks
    virtual void ProcessCallCompletion(); // 0x00356D1C slot 0x10

    // cancels a running call with the state; true if it was completed or cancelled
    bool CancelImpl(nn::nex::CallContext::State state); // 0x00356988 | fefates:bytes-fuzzy [tier B]
    // the call starts: it gets in progress and into the CallContextRegister
    bool InitiateCall(); // 0x00356A84 | fefates:callgraph [tier C]
    void SetStateImpl(nn::nex::CallContext::State state, nn::nex::qResult result, bool isSignaled); // 0x00356B00 | fefates:callgraph [tier C]
    void SignalFailure(nn::nex::qResult result); // 0x00356CB8 | fefates:bytes [tier B]
    void SignalSuccess(nn::nex::qResult result); // 0x00356CE8 | fefates:bytes [tier B]
    bool SetCallInProgressState(); // 0x00356D24 | fefates:callgraph [tier C]
    // (name is ours)
    void SetOwner(void* pOwner, u32 ownerId); // 0x00356DD8
    void RegisterCompletionCallback(CompletionFunction pFunction, const nn::nex::UserContext& context, bool isAppended); // 0x00356DE4 | fefates:bytes [tier B]
    // Wait for Scheduler::PreventBlockCall (armlink made it a nop in front of Wait; name is ours)
    static bool WaitCallback(nn::nex::RootObject* pObject, unsigned int timeout); // 0x00356F54
    // waits (dispatching the scheduler) until the call is not in progress; true if it completed
    bool Wait(unsigned int timeout); // 0x00356F58 | fefates:bytes-fuzzy [tier B]
    // makes the call context available again (false while the call runs)
    bool Reset(); // 0x00357078 | fefates:bytes-fuzzy [tier B]
    // cancels the call and the calls associated with it
    bool Cancel(nn::nex::CallContext::State state); // 0x00357188 | fefates:callgraph [tier C]
    // (name is ours)
    void SetFlag(u32 flag); // 0x00357274

    u32 m_Flags;                                // 0x0C
    u8 m_State;                                 // 0x10
    qList<CompletionCallback> m_Callbacks;      // 0x14
    qVector<u32> m_Unknown0x2C;                 // 0x2C
    qResult m_Result;                           // 0x38
    u32 m_Id;                                   // 0x44 (in the CallContextRegister)
    void* m_pOwner;                             // 0x48 (CallContextRegister::CancelCallContexts)
    u32 m_OwnerId;                              // 0x4C
    CancelHandler* m_pCancelHandler;            // 0x50
    Time m_Deadline;                            // 0x58
};
ASSERT_OFFSET(CallContext, m_State, 0x10);
ASSERT_OFFSET(CallContext, m_Result, 0x38);
ASSERT_OFFSET(CallContext, m_pCancelHandler, 0x50);
ASSERT_SIZE(CallContext, 0x60);
} // namespace nex
} // namespace nn
