#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"
#include "nn/os/os_Event.h"

namespace nn {
namespace nex {
class IndependentServer;

// The job of IndependentServer::RequestServiceToken: the login to the friends server (if needed),
// the game authentication, then the service locator request with the access key. The names of the
// members and of the steps are ours.
// RTTI N2nn3nex20JobIndependentServerE
// vtable 0x008FD848 (vptr 0x008FD850), offset_to_top 0, 13 entries
class JobIndependentServer : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    // (inline in IndependentServer::RequestServiceToken)
    JobIndependentServer()
        : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_pServer(nullptr), m_GameServerId(0), m_pAccessKey(nullptr), m_pToken(nullptr),
          m_ChecksRestriction(true)
    {
    }
    virtual ~JobIndependentServer(); // 0x00397440 slot 0x00
    // 0x003973FC slot 0x04 (deleting dtor)

    void Start(); // 0x003968E4
    void WaitForLogin(); // 0x00396A60
    void RequestGameAuthentication(); // 0x00396BDC
    void RequestServiceLocator(); // 0x00396D58
    void WaitForGameAuthentication(); // 0x00396EE4
    void WaitForServiceLocator(); // 0x00397108
    // the call completes (with the network error code of a failure in the server)
    void CompleteJob(const nn::nex::qResult& result); // 0x00397320

    u32 m_CallContextId;          // 0x68
    IndependentServer* m_pServer; // 0x6C
    u32 m_GameServerId;           // 0x70
    const char* m_pAccessKey;     // 0x74
    char* m_pToken;               // 0x78 (256 characters)
    s32 m_ResponseResult;         // 0x7C (of the last friends request)
    s32 m_HttpStatusCode;         // 0x80 (of the last friends request)
    bool m_ChecksRestriction;     // 0x84
    nn::os::Event m_Event;        // 0x88 (of the friends requests)
};
ASSERT_OFFSET(JobIndependentServer, m_Event, 0x88);
ASSERT_SIZE(JobIndependentServer, 0x90);
} // namespace nex
} // namespace nn
