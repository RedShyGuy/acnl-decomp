#include "nn/pia/inet/inet_NatPortDetecter.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/pia/common/common_ErrorHandler.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
const u64 TRACE_FLAG = 0x8000ULL;
// the flag of the trace of the replies
const u64 RESULT_TRACE_FLAG = 0x10000ULL;
// the check runs at most this many times more
const u32 RETRY_NUM_MAX = 4;
const u32 DETECTION_TIMEOUT_MSEC = 1000;
// the messages to the primary server (to both of its ports with an endpoint independent mapping)
const u32 MESSAGE_TYPE = nex::NATCheckMessage::TYPE_REPLY_MIN;
const u16 MESSAGE_NUM = 5;
// the mapping of a NAT that maps endpoint dependent
const u8 NAT_MAPPING_EDM = 2;

// the milliseconds since start (inline; name is ours)
inline s64 GetElapsedMSec(const common::Time& start)
{
    common::Time now;
    now.SetNow();
    return (now - start).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
}
} // namespace

// 0x0097FA08
u16 NatPortDetecter::s_LastPerceivedPort = 0;

// 0x003E3694 slot 0x0C | slot vf_0x0C of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPortDetecter::Cleanup()
{
    NatDetecter::Cleanup();
}

// 0x003E3730 slot 0x08 | fefates:bytes
nn::Result nn::pia::inet::NatPortDetecter::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::common::InetAddress& localAddress)
{
    m_StartTime.SetNow();
    return NatDetecter::Startup(pCallContext, localAddress);
}

// 0x003E79E8 | fefates:bytes
bool nn::pia::inet::NatPortDetecter::CheckRetry()
{
    u16 port = GetPerceivedPort();
    if (port == 0 && m_RetryCount < RETRY_NUM_MAX) {
        return true;
    }
    if (port < s_LastPerceivedPort && m_IsInverse && m_RetryCount < RETRY_NUM_MAX) {
        // the NAT wrapped around: the check runs again from another local port
        nn::Result result = RetryBind();
        if (result.IsSuccess()) {
            return true;
        }
        common::ErrorHandler::TraceResult(TRACE_FLAG, result);
        return false;
    }
    s_LastPerceivedPort = port;
    return false;
}

// 0x003E7A7C slot 0x1C | slot vf_0x1C of nn::pia::inet::NatDetecter
bool nn::pia::inet::NatPortDetecter::HandleResult()
{
    TraceReceivedMessageArray(RESULT_TRACE_FLAG);
    common::InetAddress& predicted = m_SelfLocation.m_StationAddress.m_InetAddress;
    GetPredictedAddress(&predicted);
    if (!predicted.IsValid()) {
        return false;
    }
    if (m_IsInverse) {
        m_pProtocol->SendProbeRequest(m_TargetLocation, m_SelfLocation);
        u16 elapsed = GetElapsedMSec(m_StartTime);
        NatTraversalTimeList& times = NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol->m_TraversalTimeList;
        NatTraversalTime* pTime = times.Find(m_TargetLocation);
        if (pTime != nullptr) {
            pTime->m_PortCheckMSec = elapsed;
        } else {
            NatTraversalTime time;
            time.m_Location = m_TargetLocation;
            time.m_PortCheckMSec = elapsed;
            times.Add(time);
        }
    } else {
        nex::NATProperties properties = nex::RootTransport::GetInstance()->GetNATProperties();
        properties.m_PublicPort = GetPerceivedPort();
        properties.m_PrivatePort = m_LocalAddress.m_Port;
        nex::RootTransport::GetInstance()->SetNATProperties(properties);
        common::g_SessionBeginMonitoringContent.m_Unknown0x48 = GetElapsedMSec(m_StartTime);
    }
    return true;
}

// 0x003E7C50 slot 0x18 | slot vf_0x18 of nn::pia::inet::NatDetecter
bool nn::pia::inet::NatPortDetecter::CheckAllMessage()
{
    return m_ReceivedMessages[0].m_Count != 0;
}

// 0x003E7C60 | fefates:bytes
void nn::pia::inet::NatPortDetecter::StartSendingMessage()
{
    sendDummyMessage();
    const common::InetAddress* pSecondary = GetPrimaryServerSecondaryPortAddress();
    if (pSecondary != nullptr && NexFacade::s_pInstance->GetNatPropertyMapping() == NAT_MAPPING_EDM) {
        // the mapping depends on the port of the server: the secondary port sees the next port
        AddSendMessage(SendNatCheckMessage(*pSecondary, MESSAGE_TYPE), MESSAGE_NUM);
    } else {
        AddSendMessage(SendNatCheckMessage(*GetPrimaryServerPrimaryPortAddress(), MESSAGE_TYPE), MESSAGE_NUM);
    }
}

// 0x003E7D68 | fefates:callseq
nn::Result nn::pia::inet::NatPortDetecter::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::common::InetAddress& localAddress, const nn::pia::transport::StationLocation& target, const nn::pia::transport::StationLocation& self, nn::pia::inet::NexNatTraversalProtocol* pProtocol, bool isInverse)
{
    if (m_DetectionJob.IsRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    m_TargetLocation = target;
    m_SelfLocation = self;
    m_pProtocol = pProtocol;
    m_IsInverse = isInverse;
    nn::Result result = Startup(pCallContext, localAddress);
    if (result.IsFailure()) {
        return result;
    }
    result = m_DetectionJob.Startup(pCallContext, this, NexFacade::s_pInstance->m_pNatTraverser);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x003E7E10 | fefates:bytes [tier B]
nn::pia::inet::NatPortDetecter::NatPortDetecter() : m_TargetLocation(), m_SelfLocation(), m_StartTime()
{
}

// 0x003E7E74 | fefates:bytes
// 0x003E7E48 (deleting dtor)
nn::pia::inet::NatPortDetecter::~NatPortDetecter()
{
    // the locations are destroyed
}

// 0x0072F0F8 slot 0x24 | slot vf_0x24 of nn::pia::inet::NatDetecter
u32 nn::pia::inet::NatPortDetecter::GetDetectionTimeout() const
{
    return DETECTION_TIMEOUT_MSEC;
}

} // namespace inet
} // namespace pia
} // namespace nn
