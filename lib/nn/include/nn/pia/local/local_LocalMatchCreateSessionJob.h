#pragma once

#include "decomp.h"
#include "nn/pia/session/session_CreateSessionJob.h"

namespace nn {
namespace pia {
namespace local {
class LocalMatchmakeSession;

// RTTI N2nn3pia5local26LocalMatchCreateSessionJobE @ 0x008CFCDC
// vtable 0x009011A0 (vptr 0x009011A8), offset_to_top 0, 10 entries
//
// Creates the session of the local network: the matchmake session creates the network, then
// LocalFacade starts before the mesh (CreateSessionJob::MeshStartup). The application data of
// the setting is the key of the signature. On failure the facade stops and the network is
// destroyed. The step names are from the strings; the member names are ours.
class LocalMatchCreateSessionJob : public ::nn::pia::session::CreateSessionJob
{
public:
    static const u32 SIGNATURE_KEY_SIZE = 32;

    LocalMatchCreateSessionJob(); // 0x00421314
    virtual ~LocalMatchCreateSessionJob(); // 0x00421348 slot 0x00
    // 0x00421338 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316AC slot 0x14
    // (armlink placed it in front of the one of the base)
    virtual void Cleanup(); // 0x00438144 slot 0x18
    virtual nn::Result vf_0x1C(const nn::pia::session::CreateSessionSetting* pSetting); // 0x00420C24 slot 0x1C
    virtual void vf_0x20(); // 0x00421258 slot 0x20
    virtual nn::pia::common::ExecuteResult vf_0x24(nn::Result result); // 0x004212A4 slot 0x24

    // the steps
    common::ExecuteResult CompleteFailure(); // 0x00420D14
    common::ExecuteResult StopLocalSession(); // 0x00420D40
    common::ExecuteResult StartLocalSession(); // 0x00420DC8
    common::ExecuteResult CreateLocalNetwork(); // 0x00420EB8
    common::ExecuteResult DestroyLocalNetwork(); // 0x00420F98
    common::ExecuteResult WaitCreateLocalNetwork(); // 0x00421084
    common::ExecuteResult WaitDestroyLocalNetwork(); // 0x004211B0

    LocalMatchmakeSession* m_pSession; // 0x5C, the current matchmake session of Session
    nn::Result m_FailureResult;        // 0x60, of the mesh (vf_0x24)
};
ASSERT_OFFSET(LocalMatchCreateSessionJob, m_FailureResult, 0x60);
} // namespace local
} // namespace pia
} // namespace nn
