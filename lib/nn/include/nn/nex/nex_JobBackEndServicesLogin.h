#pragma once

#include "decomp.h"
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
class StreamManager;
template <typename T0, typename T1>
class AnyObjectHolder;

// The login of BackEndServices: the connection to the authentication server, the login there, the
// connection to the secure server and the registration of the station URLs. The names of the
// members and of the steps without a reference name are ours.
// RTTI N2nn3nex23JobBackEndServicesLoginE @ 0x008CECE8
// vtable 0x008FE534 (vptr 0x008FE53C), offset_to_top 0, 13 entries
class JobBackEndServicesLogin : public ::nn::nex::StepSequenceJob
{
public:
    // the stream type and the principal id of the secure server
    static const u32 SECURE_STREAM_TYPE = 10;
    // the principal id of a guest (no secure server)
    static const u32 GUEST_PRINCIPAL_ID = 2;

    JobBackEndServicesLogin(unsigned int callContextId, nn::nex::BackEndServices* pBackEndServices, nn::nex::qResult* pResult, const nn::nex::String& name, const char* pPassword,
                            const wchar_t* pAddress, unsigned short port, nn::nex::Credentials** ppCredentials, nn::nex::String* pServerName,
                            nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData, int timeout, nn::nex::StreamManager* (*pCreateStreamManager)()); // 0x003B0A04 | fefates:bytes-fuzzy [tier B]
    virtual ~JobBackEndServicesLogin(); // 0x003B0C9C slot 0x00
    // 0x003B0BB8 (deleting dtor) | fefates:bytes
    virtual void CancelJob(); // 0x003B0940 slot 0x24 | fefates:bytes
    // after a failure the back end logs out first
    void CompleteJob(nn::nex::qResult result); // 0x003AF97C | fefates:bytes [tier B]
    // the login to the authentication server
    void LoginToAuthenticationService(); // 0x003AFACC (name is ours)
    void RegisterURLs(); // 0x003AFC10 | fefates:bytes [tier B]
    // the public URL of the registration is stored (and in the station URLs of the Network)
    void ProcessRegistrationResult(); // 0x003AFD58 (name is ours)
    void CompleteLogout(); // 0x003AFFA0 | fefates:bytes [tier B]
    void ValidateArguments(); // 0x003AFFC4 | fefates:bytes [tier B]
    void ProcessAuthConnectionResult(); // 0x003B0050 | fefates:bytes [tier B]
    void ProcessAuthenticationResult(); // 0x003B0150 | fefates:bytes [tier B]
    void ConnectToAuthenticationService(); // 0x003B0298 | fefates:bytes [tier B]
    void CompleteLogin(); // 0x003B04F8 (name is ours)
    void ProcessSecConnConnectionResult(); // 0x003B0530 | fefates:bytes [tier B]
    void ConnectToSecureConnectionService(); // 0x003B0638 | fefates:bytes [tier B]
    void DisconnectFromAuthenticationService(); // 0x003B083C | fefates:bytes [tier B]
    // (inline in the original, with a copy out of line)
    nn::nex::StreamManager* CreateStreamManager() const; // 0x0072D108 | fefates:bytes-fuzzy [tier B]

    u32 m_CallContextId;                          // 0x068
    String m_Name;                                // 0x06C
    char* m_pPassword;                            // 0x074 (NewArray)
    String m_Address;                             // 0x078 (of the authentication server)
    u16 m_Port;                                   // 0x080
    Credentials** m_ppCredentials;                // 0x084
    CallContext m_CallContext;                    // 0x088 (of the connections)
    ProtocolCallContext m_ProtocolCallContext;    // 0x0E8 (of the login and the registration)
    qResult m_LoginResult;                        // 0x160 (of the server)
    qResult m_Result;                             // 0x16C (of the job)
    Buffer m_Unknown0x178;                        // 0x178
    BackEndServices* m_pBackEndServices;          // 0x198
    StationURL m_URL;                             // 0x19C (the public URL of the registration)
    RVConnectionData m_ConnectionData;            // 0x208 (of the secure server)
    qResult* m_pResult;                           // 0x300
    String* m_pServerName;                        // 0x304
    AnyObjectHolder<Data, String>* m_pData;       // 0x308 (null: a plain login)
    JobConnectEndPoint* m_pConnectJob;            // 0x30C
    EndPoint* m_pAuthenticationEndPoint;          // 0x310
    EndPoint* m_pSecureEndPoint;                  // 0x314
    Time m_Deadline;                              // 0x318
    bool m_IsCancelled;                           // 0x320
    StreamManager* (*m_pCreateStreamManager)();   // 0x324
};
ASSERT_OFFSET(JobBackEndServicesLogin, m_ProtocolCallContext, 0xE8);
ASSERT_OFFSET(JobBackEndServicesLogin, m_pBackEndServices, 0x198);
ASSERT_OFFSET(JobBackEndServicesLogin, m_ConnectionData, 0x208);
ASSERT_OFFSET(JobBackEndServicesLogin, m_Deadline, 0x318);
ASSERT_SIZE(JobBackEndServicesLogin, 0x328);
} // namespace nex
} // namespace nn
