#pragma once

#include "decomp.h"
#include "nn/pia/session/session_JoinSessionJob.h"

namespace nn {
namespace pia {
namespace local {
class LocalMatchmakeSession;

// RTTI N2nn3pia5local24LocalMatchJoinSessionJobE @ 0x008CFC88
// vtable 0x0090106C (vptr 0x00901074), offset_to_top 0, 17 entries
//
// Joins a session of the local network: the matchmake session connects to the network of the
// found session, then LocalFacade starts and gives the host info for JoinSessionJob::MeshStartup.
// The application data of the setting is the key of the signature. On a failure after the
// mesh the network is left again. The step names are from the strings; the member name is ours.
class LocalMatchJoinSessionJob : public ::nn::pia::session::JoinSessionJob
{
public:
    static const u32 SIGNATURE_KEY_SIZE = 32;

    LocalMatchJoinSessionJob(); // 0x00420028
    virtual ~LocalMatchJoinSessionJob(); // 0x00420058 slot 0x00
    // 0x00420048 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00731698 slot 0x14
    virtual nn::Result vf_0x30(const nn::pia::session::JoinSessionSetting* pSetting); // 0x0041F7E0 slot 0x30
    virtual nn::pia::common::ExecuteResult vf_0x38(nn::Result result); // 0x0041FFC0 slot 0x38
    virtual nn::pia::common::ExecuteResult vf_0x3C(); // 0x0041FF54 slot 0x3C
    virtual nn::pia::common::ExecuteResult vf_0x40(); // 0x0041FEEC slot 0x40

    // the steps
    common::ExecuteResult WaitForCancel(); // 0x0041F908
    common::ExecuteResult WaitJoinMatchmake(); // 0x0041F988
    common::ExecuteResult WaitLeaveMatchmake(); // 0x0041FBF0
    common::ExecuteResult JoinMatchmakeSession(); // 0x0041FCE0
    common::ExecuteResult LeaveMatchmakeSession(); // 0x0041FDF0

    LocalMatchmakeSession* m_pSession; // 0xE0, the current matchmake session of Session
};
ASSERT_OFFSET(LocalMatchJoinSessionJob, m_pSession, 0xE0);
} // namespace local
} // namespace pia
} // namespace nn
