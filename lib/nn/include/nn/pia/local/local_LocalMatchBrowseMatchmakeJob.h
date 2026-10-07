#pragma once

#include "decomp.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/session/session_BrowseMatchmakeJob.h"

namespace nn {
namespace pia {
namespace local {
class LocalMatchmakeSession;

// RTTI N2nn3pia5local28LocalMatchBrowseMatchmakeJobE @ 0x008CFD30
// vtable 0x00901284 (vptr 0x0090128C), offset_to_top 0, 8 entries
//
// Searches the sessions of the local network: the matchmake session scans for networks and
// fills the session info list with the matching ones. The step names are from the strings; the
// member names are ours.
class LocalMatchBrowseMatchmakeJob : public ::nn::pia::session::BrowseMatchmakeJob
{
public:
    LocalMatchBrowseMatchmakeJob(); // 0x00423290
    virtual ~LocalMatchBrowseMatchmakeJob(); // 0x004232D4 slot 0x00
    // 0x004232B0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316CC slot 0x14
    virtual void Cleanup(); // 0x0042328C slot 0x18
    virtual nn::Result vf_0x1C(nn::pia::session::CommonMatchmakeSession* pSession, const nn::pia::session::SessionSearchCriteria* pCriteria); // 0x0042304C slot 0x1C

    // the steps
    common::ExecuteResult BrowseMatchmake(); // 0x004230B8
    common::ExecuteResult WaitBrowseMatchmake(); // 0x00423194

    common::CallContext m_CallContext; // 0x44, of the matchmake session (in the tail padding of the base)
    LocalMatchmakeSession* m_pSession; // 0x58
};
ASSERT_OFFSET(LocalMatchBrowseMatchmakeJob, m_CallContext, 0x44);
ASSERT_OFFSET(LocalMatchBrowseMatchmakeJob, m_pSession, 0x58);
} // namespace local
} // namespace pia
} // namespace nn
