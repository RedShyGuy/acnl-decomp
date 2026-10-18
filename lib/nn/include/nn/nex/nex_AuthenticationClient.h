#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyDataAdapter.h"
#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_TicketGrantingProtocolClient.h"
#include "nn/nex/nex_TicketManager.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
class Data;
class KerberosEncryption;
class ProtocolCallContext;
class RVConnectionData;
class String;
class TicketGrantingProtocolClient;
class TicketManager;
class qResult;
template <typename T0, typename T1>
class AnyObjectHolder;

// The client of the authentication server: the login gets the principal id and the ticket of the
// secure server. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex20AuthenticationClientE @ 0x008CE838
// vtable 0x008FD750 (vptr 0x008FD758), offset_to_top 0, 10 entries
class AuthenticationClient : public ::nn::nex::ServiceClient
{
public:
    // the protocol id of the ticket granting protocol
    static const u16 PROTOCOL_ID = 10;
    // the time of the calls of the jobs (ms)
    static const u32 CALL_TIMEOUT = 30000;

    // (inline in the constructor of SecureStream)
    AuthenticationClient() : ServiceClient(1), m_pProtocol(nullptr), m_pKerberos(nullptr), m_pTicketManager(nullptr), m_CallDeadline(0)
    {
        m_pKerberos = new KerberosEncryption();
        m_pTicketManager = new TicketManager(this);
        m_pProtocol = new TicketGrantingProtocolClient(0);
        m_pProtocol->m_ProtocolId = PROTOCOL_ID;
        m_PrincipalId = 0;
        m_CallDeadline = Time::ConvertTimeoutToDeadline(CALL_TIMEOUT);
        AnyDataAdapter::Register();
        // the responder of the protocol
        ProtocolRegistry* pRegistry = RVClientCore::GetInstance()->m_pProtocolRegistry;
        TicketGrantingProtocolClient* pResponder = new TicketGrantingProtocolClient(0);
        pResponder->m_ProtocolId = PROTOCOL_ID;
        pRegistry->RegisterProtocol(pResponder);
    }
    virtual ~AuthenticationClient(); // 0x003954BC slot 0x00 | fefates:bytes
    // 0x003954AC (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x00395390 slot 0x20
    // the login (in a JobTicketManagerLogin)
    bool Login(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::String& name, const char* pPassword, nn::nex::RVConnectionData* pConnectionData,
               nn::nex::String* pServerName, nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData); // 0x003953A0

    TicketGrantingProtocolClient* m_pProtocol; // 0x10
    KerberosEncryption* m_pKerberos;           // 0x14
    TicketManager* m_pTicketManager;           // 0x18
    u32 m_PrincipalId;                         // 0x1C (of the login)
    Time m_CallDeadline;                       // 0x20 (of the calls of the jobs)
};
ASSERT_SIZE(AuthenticationClient, 0x28);
} // namespace nex
} // namespace nn
