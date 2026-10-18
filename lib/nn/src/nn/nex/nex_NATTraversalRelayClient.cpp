#include "nn/nex/nex_NATTraversalRelayClient.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_NATRelayInterface.h"
#include "nn/nex/nex_NATTraversalRelayProtocolImpl.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StreamManager.h"

namespace nn {
namespace nex {
// 0x003B12F4 | fefates:callgraph [tier C]
bool nn::nex::NATTraversalRelayClient::CallReportNATProperties(nn::nex::ProtocolCallContext* pContext, const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt)
{
    return m_pRelayProtocol->m_pClient->CallReportNATProperties(pContext, mapping, filtering, rtt);
}

// 0x003B1310 slot 0x1C | mk7dlp:callseq
void nn::nex::NATTraversalRelayClient::ConnectionStateHasChanged()
{
    EndPoint* pConnection = GetConnection(PROTOCOL_ID);
    NATTraversalProtocolClient* pClient = m_pRelayProtocol->m_pClient;
    if (pClient != nullptr) {
        pClient->m_pConnection = pConnection;
    }
    if (m_pRelayInterface == nullptr) {
        return;
    }
    if (pConnection != nullptr) {
        m_pRelayInterface->UpdateConnectionState(m_pDefaultCredentials->m_ConnectionId, pConnection->m_URL);
    } else {
        m_pRelayInterface->UpdateConnectionState(0, StationURL());
    }
}

// 0x003B13A4 | fefates:callgraph [tier C]
bool nn::nex::NATTraversalRelayClient::CallReportNATTraversalResult(nn::nex::ProtocolCallContext* pContext, const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt)
{
    return m_pRelayProtocol->m_pClient->CallReportNATTraversalResult(pContext, cid, isSucceeded, rtt);
}

// 0x003B13C0 slot 0x28 | fefates:bytes
nn::nex::NATTraversalRelayProtocol* nn::nex::NATTraversalRelayClient::CreateNATTraversalRelayProtocol()
{
    return new NATTraversalRelayProtocolImpl();
}

// 0x003B1430 slot 0x20 | fefates:bytes
void nn::nex::NATTraversalRelayClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    m_pRelayProtocol->SetDefaultCredentials(pCredentials);
    // the server moves from the broker of the old credentials to the one of the new ones
    if (m_pDefaultCredentials != nullptr && m_pDefaultCredentials->m_pStreamManager != nullptr) {
        m_pDefaultCredentials->m_pStreamManager->m_pBroker->UnregisterServerProtocol(PROTOCOL_ID);
    }
    if (pCredentials != nullptr && pCredentials->m_pStreamManager != nullptr) {
        pCredentials->m_pStreamManager->m_pBroker->RegisterServerProtocol(PROTOCOL_ID, m_pRelayProtocol);
    }
}

// 0x003B14A8 | fefates:bytes-fuzzy [tier B]
void nn::nex::NATTraversalRelayClient::Init()
{
    GetProtocolRegistry()->RegisterProtocol(PROTOCOL_ID, new NATTraversalRelayProtocol());
    GetProtocolRegistry()->RegisterProtocol(PROTOCOL_ID, new NATTraversalProtocolClient(0));
    m_pRelayProtocol = CreateNATTraversalRelayProtocol();
    m_pRelayProtocol->SetFlag(Protocol::FLAG_UNKNOWN_8);
    m_pRelayProtocol->m_pOwner = this;
}

// 0x003B1580 | fefates:bytes [tier B]
nn::nex::NATTraversalRelayClient::NATTraversalRelayClient() : ServiceClient(PROTOCOL_ID), m_pRelayInterface(nullptr), m_pRelayProtocol(nullptr)
{
    // nothing to do
}

// 0x003B160C slot 0x00 | fefates:bytes
// 0x003B15A8 (deleting dtor)
nn::nex::NATTraversalRelayClient::~NATTraversalRelayClient()
{
    if (m_pRelayInterface != nullptr) {
        m_pRelayInterface->UnregisterRelayClient();
        m_pRelayInterface = nullptr;
    }
    if (m_pRelayProtocol != nullptr) {
        delete m_pRelayProtocol;
    }
}

// 0x003BA4F0 | fefates:callgraph [tier C]
bool nn::nex::NATTraversalRelayClient::CallRequestProbeInitiationExt(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source)
{
    return m_pRelayProtocol->m_pClient->CallRequestProbeInitiationExt(pContext, targets, source);
}

// 0x0072D280 slot 0x14 | fefates:bytes
bool nn::nex::NATTraversalRelayClient::IsConnected() const
{
    EndPoint* pConnection = GetConnection(PROTOCOL_ID);
    return pConnection != nullptr && pConnection->IsConnected();
}

} // namespace nex
} // namespace nn
