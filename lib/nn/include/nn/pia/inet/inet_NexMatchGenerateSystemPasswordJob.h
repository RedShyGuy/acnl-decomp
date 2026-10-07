#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/session/session_GenerateMatchmakeSystemPasswordJob.h"

namespace nn {
namespace pia {
namespace session {
class CommonMatchmakeSession;
}
namespace inet {
class NexMatchmakeSession;
// RTTI N2nn3pia4inet33NexMatchGenerateSystemPasswordJobE @ 0x008CFA98
// vtable 0x009007C8 (vptr 0x009007D0), offset_to_top 0, 8 entries
//
// Makes a system password for the matchmake session through NexMatchmakeSession. The step names are from the strings; the member name is ours.
class NexMatchGenerateSystemPasswordJob : public ::nn::pia::session::GenerateMatchmakeSystemPasswordJob
{
public:
    NexMatchGenerateSystemPasswordJob(); // 0x004122E8
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchGenerateSystemPasswordJob(); // 0x00447B90 slot 0x00
    // 0x00412308 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072FA70 slot 0x14
    virtual void Cleanup(); // 0x004122D0 slot 0x18
    virtual nn::Result vf_0x1C(); // 0x00411FB4 slot 0x1C

    // the steps
    common::ExecuteResult GenerateMatchmakeSystemPassword(); // 0x00411FF8
    common::ExecuteResult WaitGenerateMatchmakeSystemPassword(); // 0x004120FC

    NexMatchmakeSession* m_pSession; // 0x60, the current matchmake session of Session
};
ASSERT_OFFSET(NexMatchGenerateSystemPasswordJob, m_pSession, 0x60);
ASSERT_SIZE(NexMatchGenerateSystemPasswordJob, 0x68);
} // namespace inet
} // namespace pia
} // namespace nn
