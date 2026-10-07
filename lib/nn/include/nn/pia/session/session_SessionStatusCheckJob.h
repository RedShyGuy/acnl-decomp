#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session21SessionStatusCheckJobE @ 0x008D00BC
// vtable 0x00901AC4 (vptr 0x00901ACC), offset_to_top 0, 6 entries
//
// Watches the session while Session runs: the network, the transport and the mesh set
// Session::m_DisconnectState; with Session::GlobalSetting::m_Unknown0x1 the host also lets the
// matchmake session check its status (slots 0x78 / 0x7C).
class SessionStatusCheckJob : public ::nn::pia::common::StepSequenceJob
{
public:
    SessionStatusCheckJob(); // 0x00440E80
    virtual ~SessionStatusCheckJob(); // 0x00440ED0 slot 0x00
    // 0x00440EAC slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00734064 slot 0x14

    // (names are ours)
    nn::Result Startup(); // 0x00440D88
    void Cleanup(); // 0x00440D38

    // the steps (the second one with the station id table)
    common::ExecuteResult CheckSessionStatus(); // 0x00440720
    common::ExecuteResult CheckSessionStatus4JointSession(); // 0x00440904

    common::CallContext m_CallContext; // 0x40, of the matchmake session check
    bool m_IsChecking;                 // 0x54, the matchmake session checks
    bool m_IsCheckRequested;           // 0x55, the host checks again (joint sessions)
};
ASSERT_OFFSET(SessionStatusCheckJob, m_CallContext, 0x40);
ASSERT_OFFSET(SessionStatusCheckJob, m_IsChecking, 0x54);
ASSERT_SIZE(SessionStatusCheckJob, 0x58);
} // namespace session
} // namespace pia
} // namespace nn
