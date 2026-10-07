#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace common {
class CallContext;
}
namespace session {
class CommonMatchmakeSession;
class SessionSearchCriteria;
// RTTI N2nn3pia7session18BrowseMatchmakeJobE @ 0x008D0020
// vtable 0x00901988 (vptr 0x00901990), offset_to_top 0, 8 entries
//
// Session::BrowseAsync: the network (inet / local) searches the sessions in its steps.
class BrowseMatchmakeJob : public ::nn::pia::common::StepSequenceJob
{
public:
    BrowseMatchmakeJob(); // 0x00439A58
    virtual ~BrowseMatchmakeJob(); // 0x00439A88 slot 0x00
    // 0x00439A78 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0073390C slot 0x14
    virtual void Cleanup(); // 0x00439964 slot 0x18
    // the network takes the search (and sets its first step)
    virtual nn::Result vf_0x1C(nn::pia::session::CommonMatchmakeSession* pSession, const nn::pia::session::SessionSearchCriteria* pCriteria) = 0; // slot 0x1C

    // (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, nn::pia::session::CommonMatchmakeSession* pSession,
                       const nn::pia::session::SessionSearchCriteria* pCriteria); // 0x00439990

    // the last step (armlink placed it behind SyncClockProtocol)
    common::ExecuteResult CompleteProcess(); // 0x004398E8

    common::CallContext* m_pCallContext; // 0x40, of Session
};
ASSERT_OFFSET(BrowseMatchmakeJob, m_pCallContext, 0x40);
ASSERT_SIZE(BrowseMatchmakeJob, 0x48);
} // namespace session
} // namespace pia
} // namespace nn
