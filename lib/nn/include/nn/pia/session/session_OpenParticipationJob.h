#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
class CommonMatchmakeSession;
// RTTI N2nn3pia7session20OpenParticipationJobE @ 0x008D0068
// vtable 0x00901A14 (vptr 0x00901A1C), offset_to_top 0, 6 entries
//
// Session::OpenParticipationAsync: the matchmake session lets other stations join again.
class OpenParticipationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    OpenParticipationJob(); // 0x0043AD18
    virtual ~OpenParticipationJob(); // 0x0043AD6C slot 0x00
    // 0x0043AD48 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0073392C slot 0x14

    // (names are ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, u32 sessionId, nn::pia::session::CommonMatchmakeSession* pSession); // 0x0043AC50
    void Cleanup(); // 0x0043AC0C

    // the steps
    common::ExecuteResult OpenParticipation(); // 0x0043A9A0
    common::ExecuteResult WaitOpenParticipation(); // 0x0043AAE4

    common::CallContext* m_pCallContext;     // 0x40, of Session
    common::CallContext m_CallContext;       // 0x44, of the matchmake session
    u32 m_SessionId;                         // 0x58
    CommonMatchmakeSession* m_pSession;      // 0x5C
};
ASSERT_OFFSET(OpenParticipationJob, m_SessionId, 0x58);
ASSERT_SIZE(OpenParticipationJob, 0x60);
} // namespace session
} // namespace pia
} // namespace nn
