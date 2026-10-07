#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace session {
class CommonMatchmakeSession;
// RTTI N2nn3pia7session21CloseParticipationJobE @ 0x008D008C
// vtable 0x00901A64 (vptr 0x00901A6C), offset_to_top 0, 6 entries
//
// Session::CloseParticipationAsync: the matchmake session lets no more stations join; then the
// job waits until the stations of the session agree (WaitP2PStable).
class CloseParticipationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    // the times of WaitP2PStable (names are ours)
    static const s32 P2P_STABLE_WAIT_MSEC = 1000;
    static const s32 P2P_STABLE_TIMEOUT_MSEC = 15000;

    CloseParticipationJob(); // 0x0043F474
    virtual ~CloseParticipationJob(); // 0x0043F4D4 slot 0x00
    // 0x0043F4B0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00734054 slot 0x14

    // (names are ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, u32 sessionId, nn::pia::session::CommonMatchmakeSession* pSession); // 0x0043F384
    void Cleanup(); // 0x0043F340

    // the steps
    common::ExecuteResult WaitP2PStable(); // 0x0043EEE0
    common::ExecuteResult CloseParticipation(); // 0x0043F098
    common::ExecuteResult WaitCloseParticipation(); // 0x0043F1E0

    common::CallContext* m_pCallContext;     // 0x40, of Session
    common::CallContext m_CallContext;       // 0x44, of the matchmake session
    u32 m_SessionId;                         // 0x58
    CommonMatchmakeSession* m_pSession;      // 0x5C
    common::Time m_StartTime;                // 0x60, of WaitP2PStable
};
ASSERT_OFFSET(CloseParticipationJob, m_SessionId, 0x58);
ASSERT_OFFSET(CloseParticipationJob, m_StartTime, 0x60);
ASSERT_SIZE(CloseParticipationJob, 0x68);
} // namespace session
} // namespace pia
} // namespace nn
