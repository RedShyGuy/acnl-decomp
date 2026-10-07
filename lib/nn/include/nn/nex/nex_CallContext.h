#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11CallContextE @ 0x008CDFF4
// vtable 0x008FC254 (vptr 0x008FC25C), offset_to_top 0, 5 entries
class CallContext : public ::nn::nex::RefCountedObject
{
public:
    // the state of the call (only the values pia uses are known; the names are ours)
    enum State
    {
        STATE_CALL_IN_PROGRESS = 1,
        STATE_SUCCESS = 2,
        STATE_FAILURE = 3,
        STATE_CANCELLED = 4,
    };

    virtual ~CallContext(); // 0x003573B0 slot 0x00 | fefates:callgraph
    // 0x00357380 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void vf_0x08(); // 0x0072A0D8 slot 0x08 | virtual slot, introduced by nn::nex::CallContext
    virtual void BeginTransition(nn::nex::CallContext::State, nn::nex::qResult, bool); // 0x00356D18 slot 0x0C | slot vf_0x0C of nn::nex::CallContext
    virtual void ProcessCallCompletion(); // 0x00356D1C slot 0x10 | slot vf_0x10 of nn::nex::CallContext
    void CancelImpl(nn::nex::CallContext::State); // 0x00356988 | fefates:bytes-fuzzy [tier B]
    // cancels a running call with the state (name after CancelImpl)
    void Cancel(nn::nex::CallContext::State state); // 0x00357188 | fefates:callgraph [tier C]
    void SignalFailure(nn::nex::qResult); // 0x00356CB8 | fefates:bytes [tier B]
    void SignalSuccess(nn::nex::qResult); // 0x00356CE8 | fefates:bytes [tier B]
    void RegisterCompletionCallback(void (*)(nn::nex::CallContext*,const nn::nex::UserContext*), const nn::nex::UserContext&, bool); // 0x00356DE4 | fefates:bytes [tier B]
    void Wait(unsigned int); // 0x00356F58 | fefates:bytes-fuzzy [tier B]
    void Reset(); // 0x00357078 | fefates:bytes-fuzzy [tier B]
    CallContext(); // 0x00357284 | mk7dlp:callseq-callee [tier A]

    // (the size is from the constructor; only the members pia uses are named, the names are ours)
    u8 m_Unknown0x9[0x7];   // 0x09
    // 1 while the call runs (pia::inet::NatTraverser)
    u8 m_State;             // 0x10
    u8 m_Unknown0x11[0x27]; // 0x11
    // the result of the call (pia::inet::NexProcessHostMigrationJob)
    qResult m_Result;       // 0x38
    u8 m_Unknown0x44[0x14]; // 0x44
    Time m_Deadline;        // 0x58
};
ASSERT_OFFSET(CallContext, m_State, 0x10);
ASSERT_OFFSET(CallContext, m_Result, 0x38);
ASSERT_SIZE(CallContext, 0x60);
} // namespace nex
} // namespace nn
