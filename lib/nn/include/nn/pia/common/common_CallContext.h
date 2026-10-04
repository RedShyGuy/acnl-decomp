#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
// The state of an asynchronous call of pia: the job that does the work signals success, failure
// or cancel, and the optional callback gets the result. Layout from the constructor; the member
// names and the state names are ours.
class CallContext
{
public:
    enum State : u8
    {
        STATE_NOT_CALLED = 0,
        STATE_CALL_IN_PROGRESS = 1,
        STATE_CALL_SUCCESS = 2,
        STATE_CALL_FAILURE = 3,
        STATE_CALL_CANCEL = 4,
    };

    typedef void (*Callback)(nn::Result result, void* pArg);

    CallContext(); // 0x00426944 | fefates:callgraph [tier C]

    void Reset(); // 0x0042691C | fefates:callgraph [tier C]
    // the call begins (always true)
    bool InitiateCall(); // 0x004267CC | fefates:bytes [tier B]
    // asks the job to cancel the call
    void Cancel(); // 0x00426938 | fefates:callgraph [tier C]
    void SignalCancel(); // 0x004267E4 | fefates:bytes [tier B]
    void SignalFailure(nn::Result result); // 0x00426814 | fefates:bytes [tier B]
    void SignalSuccess(nn::Result result); // 0x00426860 | fefates:bytes [tier B]
    // (name is ours)
    void RegisterCallback(Callback callback, void* pArg); // 0x004268AC

    State GetState() const { return m_State; }
    bool IsCancelRequested() const { return m_IsCancelRequested; }

    State m_State;              // 0x00
    nn::Result m_Result;        // 0x04
    Callback m_Callback;        // 0x08
    void* m_pCallbackArg;       // 0x0C
    bool m_IsCancelRequested;   // 0x10
};
ASSERT_SIZE(CallContext, 0x14);
} // namespace common
} // namespace pia
} // namespace nn
