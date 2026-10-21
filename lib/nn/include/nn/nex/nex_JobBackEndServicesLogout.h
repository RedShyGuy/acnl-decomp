#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
class BackEndServices;
class StreamManager;

// The logout of BackEndServices: the disconnection from the secure server, the station URLs and the
// termination of the stream manager. The names of the members and of the steps without a reference
// name are ours.
// RTTI N2nn3nex24JobBackEndServicesLogoutE @ 0x008CED98
// vtable 0x008FE778 (vptr 0x008FE780), offset_to_top 0, 13 entries
class JobBackEndServicesLogout : public ::nn::nex::StepSequenceJob
{
public:
    // (inline in BackEndServices::LogoutImpl)
    JobBackEndServicesLogout(unsigned int callContextId, BackEndServices* pBackEndServices, Credentials* pCredentials)
        : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(callContextId)
    {
        m_TraceFlags = 4096;
        m_pBackEndServices = pBackEndServices;
        m_pCredentials = pCredentials;
        m_pStreamManager = nullptr;
        // the calls of the secure connection are cancelled
        if (pCredentials != nullptr && pCredentials->m_pConnection != nullptr) {
            Core::GetInstance()->m_pCallContextRegister->CancelCallContexts(pCredentials->m_pConnection, 0);
        }
        SetStep(Step(&JobBackEndServicesLogout::DisconnectFromSecureConnectionService, ""));
    }
    virtual ~JobBackEndServicesLogout(); // 0x003B2A6C slot 0x00
    // 0x003B2A48 (deleting dtor)
    void CompleteLogout(); // 0x003B24C0 | fefates:bytes [tier B]
    void CompleteLogoutFailure(); // 0x003B2530 | fefates:bytes [tier B]
    // the station URLs of the connection are removed (the public ones) or lose their connection id
    void RemoveStationURLs(); // 0x003B25A0 (name is ours)
    void TerminateStreamManager(); // 0x003B2720 | fefates:bytes [tier B]
    void ProcessAuthDisconnectionResult(); // 0x003B2778 | fefates:bytes [tier B]
    void WaitForStreamManagerTermination(); // 0x003B282C | fefates:bytes [tier B]
    void ProcessSecConnDisconnectionResult(); // 0x003B28B8 | fefates:bytes [tier B]
    void DisconnectFromSecureConnectionService(); // 0x003B2954 | fefates:bytes [tier B]

    u32 m_CallContextId;               // 0x68
    BackEndServices* m_pBackEndServices; // 0x6C
    Credentials* m_pCredentials;       // 0x70
    StreamManager* m_pStreamManager;   // 0x74 (taken from the credentials)
    CallContext m_CallContext;         // 0x78
};
ASSERT_SIZE(JobBackEndServicesLogout, 0xD8);
} // namespace nex
} // namespace nn
