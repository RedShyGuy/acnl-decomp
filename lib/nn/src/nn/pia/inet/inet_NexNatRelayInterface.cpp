#include "nn/pia/inet/inet_NexNatRelayInterface.h"
#include "nn/nex/nex_NATTraversalRelayClient.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/pia/inet/inet_NatRelayInterface.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// a running call is cancelled before the next one (inline; name is ours)
inline void ResetCallContext(nex::ProtocolCallContext* pContext)
{
    if (pContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        pContext->Cancel(nex::CallContext::STATE_CANCELLED);
    }
    pContext->Reset();
}
} // namespace

// 0x003873B8
// 0x00401560 (deleting dtor)
nn::pia::inet::NexNatRelayInterface::~NexNatRelayInterface()
{
    // empty (in the original too)
}

// 0x004013C8 | fefates:bytes
void nn::pia::inet::NexNatRelayInterface::RequestProbe(const nn::nex::StationURL& url)
{
    if (m_pNatRelay == nullptr) {
        return;
    }
    transport::StationLocation location;
    NexFacade::ConvertNexStationUrlToStationLocation(url, &location);
    // (a trace of the location; armlink removed the call)
    m_pNatRelay->RequestProbe(location);
}

// 0x0040142C (name is ours)
void nn::pia::inet::NexNatRelayInterface::Setup(nn::pia::inet::NatRelayInterface* pNatRelay, nn::nex::ProtocolCallContext* pContext)
{
    m_pNatRelay = pNatRelay;
    m_pCallContext = pContext;
}

// 0x00401438
void nn::pia::inet::NexNatRelayInterface::vf_0x10()
{
    // empty (in the original too)
}

// 0x0040143C | fefates:bytes
void nn::pia::inet::NexNatRelayInterface::RelayProbeRequest(const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source)
{
    ResetCallContext(m_pCallContext);
    m_pRelayClient->CallRequestProbeInitiationExt(m_pCallContext, targets, source);
}

// 0x00401480 (name is ours)
void nn::pia::inet::NexNatRelayInterface::RegisterRelayClient(nn::nex::NATTraversalRelayClient* pClient)
{
    if (pClient != nullptr) {
        m_pRelayClient = pClient;
    }
}

// 0x0040148C (name is ours)
void nn::pia::inet::NexNatRelayInterface::ReportNATProperties(unsigned int mapping, unsigned int filtering, unsigned int rtt)
{
    ResetCallContext(m_pCallContext);
    m_pRelayClient->CallReportNATProperties(m_pCallContext, mapping, filtering, rtt);
}

// 0x004014D8
bool nn::pia::inet::NexNatRelayInterface::vf_0x2C()
{
    return false;
}

// 0x004014E0 | slot vf_0x0C of nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::UnregisterRelayClient()
{
    m_pRelayClient = nullptr;
}

// 0x004014EC | fefates:bytes
void nn::pia::inet::NexNatRelayInterface::UpdateConnectionState(unsigned int cid, const nn::nex::StationURL&)
{
    if (m_pNatRelay != nullptr) {
        m_pNatRelay->SetLocalCID(cid);
    }
}

// 0x00401508 | slot vf_0x18 of nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::CheckCurrentPublicPort(nn::nex::CallContext*, const nn::nex::InetAddress&, bool)
{
    // empty (in the original too)
}

// 0x0040150C (name is ours)
void nn::pia::inet::NexNatRelayInterface::ReportNATTraversalResult(const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt)
{
    ResetCallContext(m_pCallContext);
    m_pRelayClient->CallReportNATTraversalResult(m_pCallContext, cid, isSucceeded, rtt);
}

// 0x00401558
void nn::pia::inet::NexNatRelayInterface::vf_0x1C()
{
    // empty (in the original too)
}

// 0x0040155C | slot vf_0x30 of nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::ReportNATTraversalResultDetail(const unsigned int&, const bool&, const nn::nex::NATTraversalResult&, unsigned int&)
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
