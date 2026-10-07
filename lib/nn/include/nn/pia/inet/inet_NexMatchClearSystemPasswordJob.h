#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/session/session_ClearMatchmakeSystemPasswordJob.h"

namespace nn {
namespace pia {
namespace session {
class CommonMatchmakeSession;
}
namespace inet {
class NexMatchmakeSession;
// RTTI N2nn3pia4inet30NexMatchClearSystemPasswordJobE @ 0x008CFA68
// vtable 0x0090073C (vptr 0x00900744), offset_to_top 0, 8 entries
//
// Clears the system password of the matchmake session through NexMatchmakeSession. The step names are from the strings; the member name is ours.
class NexMatchClearSystemPasswordJob : public ::nn::pia::session::ClearMatchmakeSystemPasswordJob
{
public:
    NexMatchClearSystemPasswordJob(); // 0x004114C4
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchClearSystemPasswordJob(); // 0x00447A2C slot 0x00
    // 0x004114E4 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072FA64 slot 0x14
    virtual void Cleanup(); // 0x004114AC slot 0x18
    virtual nn::Result vf_0x1C(); // 0x00411158 slot 0x1C

    // the steps
    common::ExecuteResult ClearMatchmakeSystemPassword(); // 0x004111DC
    common::ExecuteResult WaitClearMatchmakeSystemPassword(); // 0x004112E0

    NexMatchmakeSession* m_pSession; // 0x5C, the current matchmake session of Session
};
ASSERT_OFFSET(NexMatchClearSystemPasswordJob, m_pSession, 0x5C);
ASSERT_SIZE(NexMatchClearSystemPasswordJob, 0x60);
} // namespace inet
} // namespace pia
} // namespace nn
