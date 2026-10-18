#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyDataAdapter.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_SecureConnectionProtocolClient.h"
#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class ProtocolCallContext;
class StationURL;

// The client of the secure server: the registration of the station URLs and the reports. The names
// of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex22SecureConnectionClientE @ 0x008CEC50
// vtable 0x008FE180 (vptr 0x008FE188), offset_to_top 0, 10 entries
class SecureConnectionClient : public ::nn::nex::ServiceClient
{
public:
    // the protocol id of the secure connection protocol
    static const u16 PROTOCOL_ID = 11;

    // (inline in the constructor of SecureStream)
    SecureConnectionClient() : ServiceClient(2), m_Protocol(0)
    {
        AnyDataAdapter::Register();
        m_Protocol.m_ProtocolId = PROTOCOL_ID;
        // the responder of the protocol
        ProtocolRegistry* pRegistry = RVClientCore::GetInstance()->m_pProtocolRegistry;
        SecureConnectionProtocolClient* pResponder = new SecureConnectionProtocolClient(0);
        pResponder->m_ProtocolId = PROTOCOL_ID;
        pRegistry->RegisterProtocol(pResponder);
    }
    virtual ~SecureConnectionClient(); // 0x0039DDF4 slot 0x00
    // 0x0039DDC4 (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x0039DDBC slot 0x20
    // the local URL target is replaced by url, also on the server
    bool ReplaceURL(nn::nex::ProtocolCallContext* pContext, const nn::nex::StationURL& target, const nn::nex::StationURL& url); // 0x0039DB10 | fefates:bytes [tier B]
    void SendReport(unsigned int reportId, const void* pData, unsigned int size); // 0x0039DB7C | fefates:bytes [tier B]
    // the station URLs of the registration (for the PRUDP stream of the secure server)
    void PrepareURLs(nn::nex::qList<nn::nex::StationURL>& urls); // 0x0039DC74 | fefates:callgraph [tier C]

    SecureConnectionProtocolClient m_Protocol; // 0x10
};
ASSERT_SIZE(SecureConnectionClient, 0x54);
} // namespace nex
} // namespace nn
