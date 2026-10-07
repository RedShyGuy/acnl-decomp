#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/session/session_UpdateSessionSettingJob.h"

namespace nn {
namespace pia {
namespace inet {
class NexMatchmakeSession;
// RTTI N2nn3pia4inet31NexMatchUpdateSessionSettingJobE @ 0x008CFA80
// vtable 0x00900778 (vptr 0x00900780), offset_to_top 0, 8 entries
//
// Changes the setting of the matchmake session through NexMatchmakeSession. The step names are from
// the strings; the member name is ours.
class NexMatchUpdateSessionSettingJob : public ::nn::pia::session::UpdateSessionSettingJob
{
public:
    NexMatchUpdateSessionSettingJob(); // 0x004118D0
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchUpdateSessionSettingJob(); // 0x00442BE8 slot 0x00
    // 0x004118F0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072FA68 slot 0x14
    virtual void Cleanup(); // 0x004118B8 slot 0x18
    virtual nn::Result vf_0x1C(const nn::pia::session::CreateSessionSetting* pSetting); // 0x004115A8 slot 0x1C

    // the steps
    common::ExecuteResult UpdateSessionSetting(); // 0x0041162C
    common::ExecuteResult WaitUpdateSessionSetting(); // 0x0041176C

    NexMatchmakeSession* m_pSession; // 0x5C, the current matchmake session of Session
};
ASSERT_OFFSET(NexMatchUpdateSessionSettingJob, m_pSession, 0x5C);
ASSERT_SIZE(NexMatchUpdateSessionSettingJob, 0x60);
} // namespace inet
} // namespace pia
} // namespace nn
