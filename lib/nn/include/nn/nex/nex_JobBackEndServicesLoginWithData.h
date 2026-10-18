#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_RVConnectionData.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class BackEndServices;
class Credentials;
class Data;
class EndPoint;
class JobConnectEndPoint;

// The login of BackEndServices with authentication data (TicketManager::LoginWithData) instead of
// a name. This build has no constructor, destructor or vtable of it (only the steps); the names of
// the members and of the steps without a reference name are ours.
class JobBackEndServicesLoginWithData : public ::nn::nex::StepSequenceJob
{
public:
    // the stream type of the secure server
    static const u32 SECURE_STREAM_TYPE = 10;
    // the principal id of a guest (no secure server)
    static const u32 GUEST_PRINCIPAL_ID = 2;

    // (not in this build)
    JobBackEndServicesLoginWithData();
    virtual ~JobBackEndServicesLoginWithData();

    void CompleteJob(nn::nex::qResult result); // 0x003C5C18 | fefates:bytes [tier B]
    void Authenticate(); // 0x003C5D68 | fefates:bytes [tier B]
    void RegisterURLs(); // 0x003C5EAC | fefates:bytes [tier B]
    void ProcessRegistrationResult(); // 0x003C5FF4 (name is ours)
    void CompleteLogout(); // 0x003C6194 | fefates:bytes [tier B]
    void ValidateArguments(); // 0x003C61B8 | fefates:bytes [tier B]
    void ProcessAuthConnectionResult(); // 0x003C6248 | fefates:bytes [tier B]
    void ProcessAuthenticationResult(); // 0x003C6348 | fefates:bytes [tier B]
    void ConnectToAuthenticationService(); // 0x003C6490 | fefates:bytes [tier B]
    void CompleteLogin(); // 0x003C667C (name is ours)
    void ProcessSecConnConnectionResult(); // 0x003C66B4 | fefates:bytes [tier B]
    void ConnectToSecureConnectionService(); // 0x003C67BC | fefates:bytes [tier B]
    void DisconnectFromAuthenticationService(); // 0x003C6A04 | fefates:bytes [tier B]

    u32 m_CallContextId;                          // 0x068
    char* m_pPassword;                            // 0x06C
    String m_Address;                             // 0x070 (of the authentication server)
    u16 m_Port;                                   // 0x078
    Credentials** m_ppCredentials;                // 0x07C
    CallContext m_CallContext;                    // 0x080 (of the connections)
    ProtocolCallContext m_ProtocolCallContext;    // 0x0E0 (of the login and the registration)
    qResult m_LoginResult;                        // 0x158 (of the server)
    qResult m_Result;                             // 0x164 (of the job)
    Buffer m_Unknown0x170;                        // 0x170
    BackEndServices* m_pBackEndServices;          // 0x190
    StationURL m_URL;                             // 0x194 (the public URL of the registration)
    RVConnectionData m_ConnectionData;            // 0x200 (of the secure server)
    qResult* m_pResult;                           // 0x2F8
    u32 m_Unknown0x2FC;                           // 0x2FC
    AnyObjectHolder<Data, String> m_Data;         // 0x300 (the authentication info)
    u32 m_Unknown0x308;                           // 0x308
    Data* m_pCustomData;                          // 0x30C (goes to g_LoginCustomData)
    JobConnectEndPoint* m_pConnectJob;            // 0x310
    EndPoint* m_pAuthenticationEndPoint;          // 0x314
    EndPoint* m_pSecureEndPoint;                  // 0x318
    Time m_Deadline;                              // 0x320
    bool m_IsCancelled;                           // 0x328
};
ASSERT_OFFSET(JobBackEndServicesLoginWithData, m_ProtocolCallContext, 0xE0);
ASSERT_OFFSET(JobBackEndServicesLoginWithData, m_pBackEndServices, 0x190);
ASSERT_OFFSET(JobBackEndServicesLoginWithData, m_ConnectionData, 0x200);
ASSERT_OFFSET(JobBackEndServicesLoginWithData, m_Data, 0x300);
ASSERT_OFFSET(JobBackEndServicesLoginWithData, m_Deadline, 0x320);
} // namespace nex
} // namespace nn
