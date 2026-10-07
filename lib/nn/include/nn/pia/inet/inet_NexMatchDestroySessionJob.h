#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_DestroySessionJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet25NexMatchDestroySessionJobE @ 0x008CF9FC
// vtable 0x009005B0 (vptr 0x009005B8), offset_to_top 0, 10 entries
//
// The nex part of DestroySessionJob: after the mesh the matchmake sessions (first the other one
// of a joint session) are unregistered. The step names are from the strings; no members.
class NexMatchDestroySessionJob : public ::nn::pia::session::DestroySessionJob
{
public:
    NexMatchDestroySessionJob(); // 0x0040C360
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchDestroySessionJob(); // 0x00438F30 slot 0x00
    // 0x0040C378 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F850 slot 0x14
    // (armlink placed it in front of the one of the base)
    virtual void Cleanup(); // 0x00438D38 slot 0x18
    virtual nn::Result vf_0x1C(); // 0x0040BCFC slot 0x1C
    virtual void vf_0x20(); // 0x0040BD9C slot 0x20
    virtual void vf_0x24(); // 0x0040BD48 slot 0x24

    // the steps
    common::ExecuteResult CompleteProcess(); // 0x0040BD04
    common::ExecuteResult UnregisterBufferMatchmakeSession(); // 0x0040BE00
    common::ExecuteResult UnregisterCurrentMatchmakeSession(); // 0x0040BFB0
    common::ExecuteResult WaitUnregisterBufferMatchmakeSession(); // 0x0040C0CC
    common::ExecuteResult WaitUnregisterCurrentMatchmakeSession(); // 0x0040C230

    // the result of an unregistering (inline in the Wait steps; name is ours)
    void SetUnregisterResult()
    {
        if (m_CallContext.m_State != common::CallContext::STATE_CALL_FAILURE) {
            return;
        }
        nn::Result result = m_CallContext.m_Result;
        if (result == common::RESULT_MATCHMAKE_SESSION_GONE) {
            // the session is gone already
        } else if (result == common::RESULT_INVALID_STATE) {
            m_Result = common::RESULT_UNREGISTER_FAILED;
        } else if (result == common::RESULT_FATAL_196) {
            m_Result = result;
        }
    }
};
} // namespace inet
} // namespace pia
} // namespace nn
