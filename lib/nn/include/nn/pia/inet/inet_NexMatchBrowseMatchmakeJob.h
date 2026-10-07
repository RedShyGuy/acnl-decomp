#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/session/session_BrowseMatchmakeJob.h"

namespace nn {
namespace pia {
namespace inet {
class NexMatchmakeSession;
// RTTI N2nn3pia4inet26NexMatchBrowseMatchmakeJobE @ 0x008CFA14
// vtable 0x00900600 (vptr 0x00900608), offset_to_top 0, 8 entries
//
// Searches the sessions through NexMatchmakeSession: by the search criteria or by the owner
// (NexSessionSearchCriteriaOwner, type 1). The step names are from the strings; the member names
// are ours.
class NexMatchBrowseMatchmakeJob : public ::nn::pia::session::BrowseMatchmakeJob
{
public:
    NexMatchBrowseMatchmakeJob(); // 0x0040CBD8
    virtual ~NexMatchBrowseMatchmakeJob(); // 0x0040CC30 slot 0x00
    // 0x0040CC0C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F854 slot 0x14
    // (armlink placed it in front of the one of the base)
    virtual void Cleanup(); // 0x00439914 slot 0x18
    virtual nn::Result vf_0x1C(nn::pia::session::CommonMatchmakeSession* pSession, const nn::pia::session::SessionSearchCriteria* pCriteria); // 0x0040C64C slot 0x1C

    // the steps
    common::ExecuteResult BrowseMatchmake(); // 0x0040C738
    common::ExecuteResult FindSessionByOwner(); // 0x0040C84C
    common::ExecuteResult WaitBrowseMatchmake(); // 0x0040C96C
    common::ExecuteResult WaitFindSessionByOwner(); // 0x0040CAA8

    common::CallContext m_CallContext; // 0x44, of the matchmake session
    NexMatchmakeSession* m_pSession;   // 0x58
    u32 m_OwnerPrincipalId;            // 0x5C
    u32 m_Offset;                      // 0x60, of the results
    u32 m_ResultNumMax;                // 0x64
};
ASSERT_OFFSET(NexMatchBrowseMatchmakeJob, m_CallContext, 0x44);
ASSERT_OFFSET(NexMatchBrowseMatchmakeJob, m_pSession, 0x58);
ASSERT_SIZE(NexMatchBrowseMatchmakeJob, 0x68);
} // namespace inet
} // namespace pia
} // namespace nn
