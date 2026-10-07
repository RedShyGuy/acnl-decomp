#include "nn/pia/inet/inet_NatPropertyDetecter.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_String.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"
#include "pead/peadHeapMgr.h"
#include <new>

namespace nn {
namespace pia {
namespace inet {
namespace {
// the flag of the trace of the replies
const u64 RESULT_TRACE_FLAG = 0x10000ULL;
// the check runs at most this many times more
const u32 RETRY_NUM_MAX = 3;
const u32 DETECTION_TIMEOUT_MSEC = 1500;
// the replies: from the primary port of the primary server, from its secondary port, and from
// another server with the same port
const u32 MESSAGE_TYPE_PRIMARY = nex::NATCheckMessage::TYPE_REPLY_MIN;
const u32 MESSAGE_TYPE_SECONDARY_PORT = nex::NATCheckMessage::TYPE_REPLY_MIN + 1;
const u32 MESSAGE_TYPE_OTHER_SERVER = nex::NATCheckMessage::TYPE_REPLY_MIN + 2;
const u16 MESSAGE_NUM = 5;
// the values of the nex NAT properties
const u8 NAT_MAPPING_EIM = 1;
const u8 NAT_MAPPING_EDM = 2;
const u8 NAT_FILTERING_EIF = 1;
const u8 NAT_FILTERING_EDF = 2;
} // namespace

// 0x003F8D10 | fefates:bytes
bool nn::pia::inet::NatPropertyDetecter::CheckRetry()
{
    if (GetReceiveMessage(0)->m_Message.m_Port != 0 && GetReceiveMessage(2)->m_Message.m_Port != 0) {
        return false;
    }
    return m_RetryCount < RETRY_NUM_MAX;
}

// 0x003F8D5C slot 0x1C | slot vf_0x1C of nn::pia::inet::NatDetecter
bool nn::pia::inet::NatPropertyDetecter::HandleResult()
{
    TraceReceivedMessageArray(RESULT_TRACE_FLAG);
    const nex::NATCheckMessage& primary = m_ReceivedMessages[0].m_Message;
    const nex::NATCheckMessage& otherServer = m_ReceivedMessages[2].m_Message;
    if (primary.m_Port == 0 || otherServer.m_Port == 0) {
        return false;
    }
    nex::NATProperties properties = nex::RootTransport::GetInstance()->GetNATProperties();
    common::InetAddress publicAddress;
    // the private address and port
    NexFacade::ConvertInetAddressToNexInetAddress(m_LocalAddress, m_pNexAddress);
    properties.SetPrivateAddress(m_pNexAddress->GetAddressStr());
    properties.m_PrivatePort = m_LocalAddress.m_Port;
    // the public one as the other server saw it
    publicAddress.m_Address = otherServer.m_Address;
    publicAddress.m_Port = otherServer.m_Port;
    NexFacade::ConvertInetAddressToNexInetAddress(publicAddress, m_pNexAddress);
    properties.SetPublicAddress(m_pNexAddress->GetAddressStr());
    properties.m_PublicPort = publicAddress.m_Port;
    // a reply from the other port arrives only if the NAT filters endpoint independent
    if (m_ReceivedMessages[1].m_Message.m_Port != 0) {
        properties.m_NatFiltering = NAT_FILTERING_EIF;
    } else {
        properties.m_NatFiltering = NAT_FILTERING_EDF;
    }
    // the same port for both servers: the mapping is endpoint independent
    if (primary.m_Port == otherServer.m_Port) {
        properties.m_NatMapping = NAT_MAPPING_EIM;
    } else {
        properties.m_NatMapping = NAT_MAPPING_EDM;
        properties.m_PortIncrement = 1;
        if (m_LocalAddress.m_Port == static_cast<u16>(otherServer.m_Port)) {
            properties.m_IsPortPreserved = true;
            properties.m_PortIncrement = 1;
        }
    }
    nex::RootTransport::GetInstance()->SetNATProperties(properties);
    if (NexFacade::s_pInstance != nullptr) {
        NexNatTraversalProtocol* pProtocol = NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol;
        if (pProtocol != nullptr) {
            u32 filtering = properties.GetNATFiltering();
            u32 mapping = properties.GetNATMapping();
            pProtocol->ReportNatProperties(mapping, filtering, m_Rtt);
        }
    }
    return true;
}

// 0x003F8F40 | fefates:bytes
bool nn::pia::inet::NatPropertyDetecter::CheckAllMessage()
{
    return m_ReceivedMessages[0].m_Count != 0 && m_ReceivedMessages[1].m_Count != 0 && m_ReceivedMessages[2].m_Count != 0;
}

// 0x003F8F6C slot 0x14 | slot vf_0x14 of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPropertyDetecter::StartSendingMessage()
{
    sendDummyMessage();
    sendNatPropertyDetectionMessage();
}

// 0x003F8F84 | fefates:bytes [tier B]
void nn::pia::inet::NatPropertyDetecter::sendNatPropertyDetectionMessage()
{
    const common::InetAddress* pPrimary = GetPrimaryServerPrimaryPortAddress();
    AddSendMessage(SendNatCheckMessage(*pPrimary, MESSAGE_TYPE_PRIMARY), MESSAGE_NUM);
    AddSendMessage(SendNatCheckMessage(*pPrimary, MESSAGE_TYPE_SECONDARY_PORT), MESSAGE_NUM);
    // the first other server with the same port
    common::SimpleContainer<common::InetAddress, 4>& servers = NexFacade::s_pInstance->m_pNatTraverser->m_ServerAddresses;
    common::InetAddress* it = servers.Begin();
    if (it == servers.End()) {
        return;
    }
    for (it++; it != servers.End(); it++) {
        if (it->m_Address != pPrimary->m_Address && it->m_Port == pPrimary->m_Port) {
            AddSendMessage(SendNatCheckMessage(*it, MESSAGE_TYPE_OTHER_SERVER), MESSAGE_NUM);
            return;
        }
    }
}

// 0x003F9100 | fefates:bytes
void nn::pia::inet::NatPropertyDetecter::Cleanup()
{
    common::Time now;
    now.SetNow();
    common::g_SessionBeginMonitoringContent.m_Unknown0x48 = (now - m_StartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
    NatDetecter::Cleanup();
}

// 0x003F9164 | fefates:callseq
nn::Result nn::pia::inet::NatPropertyDetecter::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::common::InetAddress& localAddress)
{
    common::Time now;
    now.SetNow();
    m_StartTime = now;
    if (m_DetectionJob.IsRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = NatDetecter::Startup(pCallContext, localAddress);
    if (result.IsFailure()) {
        return result;
    }
    result = m_DetectionJob.Startup(pCallContext, this, NexFacade::s_pInstance->m_pNatTraverser);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x003F91FC | fefates:bytes [tier B]
nn::pia::inet::NatPropertyDetecter::NatPropertyDetecter() : m_StartTime()
{
    void* pBuffer = pead::AllocMemory(sizeof(nex::InetAddress), common::HeapManager::GetHeap());
    m_pNexAddress = ::new (pBuffer) nex::InetAddress();
}

// 0x003F929C | fefates:bytes
// 0x003F9248 (deleting dtor)
nn::pia::inet::NatPropertyDetecter::~NatPropertyDetecter()
{
    if (m_pNexAddress != nullptr) {
        m_pNexAddress->~InetAddress();
        pead::FreeMemory(m_pNexAddress);
        m_pNexAddress = nullptr;
    }
}

// 0x0072F168 slot 0x24 | slot vf_0x24 of nn::pia::inet::NatDetecter
u32 nn::pia::inet::NatPropertyDetecter::GetDetectionTimeout() const
{
    return DETECTION_TIMEOUT_MSEC;
}

} // namespace inet
} // namespace pia
} // namespace nn
