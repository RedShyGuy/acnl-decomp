#include "nn/pia/inet/inet_NexConnectStationJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/inet/inet_NatTraversalTimeList.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// the special index of the host station while it is not known (see StationIdTable)
const u8 STATION_INDEX_254 = 254;

inline bool IsMonitoringDataSenderFlagSet()
{
    return session::Mesh::s_pInstance != nullptr && session::Mesh::s_pInstance->IsMonitoringDataSenderFlagSet();
}
} // namespace

// 0x00400140 | fefates:callseq
void nn::pia::inet::NexConnectStationJob::CleanupImpl()
{
    m_NatCallContext.SignalCancel();
    m_NatCallContext.Reset();
    if (m_CurrentLocation.m_StationKey != 0) {
        transport::Transport* pTransport = transport::Transport::s_pInstance;
        bool isConnected = true;
        if (pTransport->m_pStationIdCallback != nullptr && m_CurrentLocation.m_PrincipalId != 0 && pTransport->m_IsUsingStationIdTable) {
            // a station that is not gone for good keeps its traversal
            transport::StationIdTable::Entry entry;
            if (pTransport->m_pStationIdTable->Find(&entry, m_CurrentLocation.m_PrincipalId).IsSuccess() &&
                pTransport->m_pStationIdCallback(&entry) == 1) {
                isConnected = false;
            }
        }
        NexFacade::s_pInstance->m_pNatTraverser->ClearNatTraversal(m_CurrentLocation, isConnected);
    }
}

// 0x004001F0
nn::Result nn::pia::inet::NexConnectStationJob::StartupImpl(nn::pia::common::CallContext* pCallContext, nn::pia::transport::Station* pStation,
                                                            const nn::pia::transport::StationConnectionInfo& info, bool isInverseConnection)
{
    m_pStation = pStation;
    m_IsInverseConnection = isInverseConnection;
    info.Trace(0x8000);
    if (NexFacade::IsPublic(info.m_PublicLocation)) {
        m_ConnectionInfo = info;
    } else {
        m_ConnectionInfo.m_PublicLocation.SetStationLocation(info.m_PrivateLocation);
        m_ConnectionInfo.m_PrivateLocation.SetStationLocation(info.m_PublicLocation);
    }
    transport::StationLocation& publicLocation = m_ConnectionInfo.m_PublicLocation;
    const transport::StationLocation& privateLocation = m_ConnectionInfo.m_PrivateLocation;
    if (publicLocation.m_NatFiltering == 0) {
        publicLocation.m_NatFiltering = privateLocation.m_NatFiltering;
    }
    if (publicLocation.m_NatMapping == 0) {
        publicLocation.m_NatMapping = privateLocation.m_NatMapping;
    }
    if (publicLocation.m_Type == 0) {
        publicLocation.m_Type = privateLocation.m_Type;
    }
    m_pStation->m_StationAddress.Trace(0x8000);
    m_ConnectionInfo.m_PrivateLocation.m_StationKey = publicLocation.m_StationKey;
    if (!NexFacade::s_pInstance->m_IsNatSessionStarted) {
        return common::RESULT_INVALID_STATE;
    }
    // the private address if the station is in the same network
    transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
    pTable->m_LocalInfo.Trace(0x8000);
    if (pTable->m_LocalInfo.m_PublicLocation.m_StationAddress.m_InetAddress.m_Address == publicLocation.m_StationAddress.m_InetAddress.m_Address) {
        if (NexFacade::IsGlobal(pTable->m_LocalInfo.m_PublicLocation)) {
            return common::RESULT_INVALID_STATE;
        }
        m_CurrentLocation = m_ConnectionInfo.m_PrivateLocation;
    } else {
        m_CurrentLocation = publicLocation;
    }
    if (m_IsInverseConnection) {
        m_CurrentLocation.SetStationAddress(m_pStation->m_StationAddress);
    }
    m_CompletedRetryNum = 0;
    m_StartedRetryNum = 0;
    if (m_pStation->m_StationIndex == STATION_INDEX_254) {
        // the host
        pTable->m_HostInfo = m_ConnectionInfo;
        pTable->m_HostInfo.Trace(0x8000);
        pTable->m_HostAddress = m_CurrentLocation.m_StationAddress;
        pTable->m_HostAddress.Trace(0x8000);
        if (NexFacade::IsEdmMapping(m_CurrentLocation)) {
            m_StartedRetryNum = 1;
            if (!NexFacade::IsEdmMapping(pTable->m_LocalInfo.m_PublicLocation)) {
                m_CompletedRetryNum = 1;
            }
        }
        if (!IsMonitoringDataSenderFlagSet()) {
            common::g_SessionBeginMonitoringContent.m_Unknown0x49C = 0;
            common::g_SessionBeginMonitoringContent.m_Unknown0x49D = 0;
        }
    }
    transport::StationConnectionInfo currentInfo;
    currentInfo.m_PublicLocation.SetStationLocation(m_CurrentLocation);
    nn::Result result = ConnectStationJob::StartupImpl(pCallContext, m_pStation, currentInfo, isInverseConnection);
    if (result.IsFailure()) {
        return result;
    }
    SetStep(&NexConnectStationJob::testCurrentAddress, "NexConnectStationJob::testCurrentAddress");
    return nn::Result();
}

// 0x004004FC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NexConnectStationJob::tryCurrentAddress()
{
    m_pStation->m_StationAddress = m_CurrentLocation.m_StationAddress;
    m_pStation->m_StationAddress.Trace(0x10000);
    NexFacade::s_pInstance->m_pNatTraverser->ReportNatTraversalResult(m_CurrentLocation, nn::Result(), !m_IsInverseConnection);
    NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol->ResetProbeStatus(m_CurrentLocation, m_IsInverseConnection);
    SetStep(&ConnectStationJob::SendConnectionRequest, "ConnectStationJob::SendConnectionRequest");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004005CC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NexConnectStationJob::testCurrentAddress()
{
    if (m_pStation->m_StationIndex == STATION_INDEX_254) {
        SetStep(&NexConnectStationJob::prepareNatTraversal, "NexConnectStationJob::prepareNatTraversal");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // an inverse connection to an address that is not behind a NAT or not public is resolved
    if ((!NexFacade::IsBehindNat(m_CurrentLocation) && m_IsInverseConnection) || (!NexFacade::IsPublic(m_CurrentLocation) && m_IsInverseConnection)) {
        SetStep(&NexConnectStationJob::resolveCurrentAddress, "NexConnectStationJob::resolveCurrentAddress");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexConnectStationJob::prepareNatTraversal, "NexConnectStationJob::prepareNatTraversal");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040072C
nn::pia::common::ExecuteResult nn::pia::inet::NexConnectStationJob::prepareNatTraversal()
{
    NexFacade* pFacade = NexFacade::s_pInstance;
    s32 timeoutMSec = pFacade->m_Unknown0x34;
    if (m_IsInverseConnection) {
        // a mapping that depends on the destination takes longer
        if (NexFacade::IsEdmMapping(transport::StationConnectionInfoTable::s_pInstance->m_LocalInfo.m_PublicLocation)) {
            timeoutMSec += NexFacade::s_pInstance->m_Unknown0x38;
        }
        if (NexFacade::IsEdmMapping(m_ConnectionInfo.m_PublicLocation)) {
            timeoutMSec += NexFacade::s_pInstance->m_Unknown0x38;
        }
    }
    if (NexFacade::s_pInstance->m_pNatTraverser->RequestNatTraversal(m_CurrentLocation, m_ConnectionInfo, &m_NatCallContext, m_IsInverseConnection)) {
        if (m_IsInverseConnection) {
            common::Time now;
            now.SetNow();
            now += common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * timeoutMSec);
            m_Deadline = now;
            SetStep(&NexConnectStationJob::waitForNatTraversalCompleted, "NexConnectStationJob::waitForNatTraversalCompleted");
        } else {
            common::Time now;
            now.SetNow();
            now += common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * NAT_TRAVERSAL_START_TIMEOUT_MSEC);
            m_Deadline = now;
            SetStep(&NexConnectStationJob::waitForNatTraversalStarted, "NexConnectStationJob::waitForNatTraversalStarted");
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // no traversal: the time of the location starts now
    NatTraversalTimeList& times = NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol->m_TraversalTimeList;
    NatTraversalTime* pTime = times.Find(m_CurrentLocation);
    if (pTime != nullptr) {
        pTime->m_Unknown0x28.SetNow();
        pTime->m_Unknown0x30.SetNow();
        pTime->m_Unknown0x38 = 0;
        pTime->m_PortCheckMSec = 0;
    } else {
        NatTraversalTime time;
        time.m_Location = m_CurrentLocation;
        time.m_Unknown0x28.SetNow();
        time.m_Unknown0x30.SetNow();
        time.m_Unknown0x38 = 0;
        time.m_PortCheckMSec = 0;
        times.Add(time);
    }
    SetStep(&NexConnectStationJob::resolveCurrentAddress, "NexConnectStationJob::resolveCurrentAddress");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00400A24 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NexConnectStationJob::resolveCurrentAddress()
{
    if (NexFacade::s_pInstance->m_pNatTraverser->CheckLatestStationLocation(m_CurrentLocation) && m_pStation->m_StationIndex == STATION_INDEX_254) {
        transport::StationConnectionInfoTable::s_pInstance->m_HostAddress = m_CurrentLocation.m_StationAddress;
        transport::StationConnectionInfoTable::s_pInstance->m_HostAddress.Trace(0x8000);
    }
    SetStep(&NexConnectStationJob::tryCurrentAddress, "NexConnectStationJob::tryCurrentAddress");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// (inline; name is ours)
DECOMP_ALWAYS_INLINE void nn::pia::inet::NexConnectStationJob::SetNatTraversalTime()
{
    if (!m_CurrentLocation.m_ProbeRequestInitiation) {
        NatTraversalTime* pTime = NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol->m_TraversalTimeList.Find(m_CurrentLocation);
        if (pTime != nullptr) {
            common::Time now;
            now.SetNow();
            pTime->m_Unknown0x38 = (now - pTime->m_Unknown0x30).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
        }
    }
}

// 0x00400AE8
nn::pia::common::ExecuteResult nn::pia::inet::NexConnectStationJob::waitForNatTraversalStarted()
{
    switch (m_NatCallContext.m_State) {
    case common::CallContext::STATE_CALL_IN_PROGRESS: {
        // started: the wait for the end
        s32 timeoutMSec = NexFacade::s_pInstance->m_Unknown0x34;
        if (NexFacade::IsEdmMapping(transport::StationConnectionInfoTable::s_pInstance->m_LocalInfo.m_PublicLocation)) {
            timeoutMSec += NexFacade::s_pInstance->m_Unknown0x38;
        }
        if (m_pStation->m_StationIndex == STATION_INDEX_254 || NexFacade::IsEdmMapping(m_ConnectionInfo.m_PublicLocation)) {
            timeoutMSec += NexFacade::s_pInstance->m_Unknown0x38;
        }
        common::Time now;
        now.SetNow();
        m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * timeoutMSec);
        SetStep(&NexConnectStationJob::waitForNatTraversalCompleted, "NexConnectStationJob::waitForNatTraversalCompleted");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    case common::CallContext::STATE_CALL_SUCCESS:
    case common::CallContext::STATE_CALL_FAILURE:
    case common::CallContext::STATE_CALL_CANCEL:
        SetStep(&NexConnectStationJob::waitForNatTraversalCompleted, "NexConnectStationJob::waitForNatTraversalCompleted");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    default:
        break;
    }
    if (m_pStation->m_StationIndex != STATION_INDEX_254) {
        return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_MSEC);
    }
    common::Time now;
    now.SetNow();
    if (m_Deadline >= now) {
        return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_MSEC);
    }
    if (m_StartedRetryNum > 0) {
        // the host is waited for again
        m_StartedRetryNum--;
        u8 retryCount = 1 - m_StartedRetryNum;
        if (!IsMonitoringDataSenderFlagSet()) {
            common::g_SessionBeginMonitoringContent.m_Unknown0x49C = retryCount;
        }
        common::Time now2;
        now2.SetNow();
        m_Deadline = now2 + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * NAT_TRAVERSAL_START_TIMEOUT_MSEC);
        return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_MSEC);
    }
    m_pCallContext->SignalFailure(common::RESULT_STATION_CONNECTION_FAILED_F2);
    SetNatTraversalTime();
    NexFacade::s_pInstance->m_pNatTraverser->ReportNatTraversalResult(m_CurrentLocation, common::RESULT_STATION_CONNECTION_FAILED_F2, false);
    NexFacade::s_pInstance->m_pNatTraverser->ClearNatTraversal(m_CurrentLocation, true);
    SetStep(&ConnectStationJob::ConnectionFailed, "ConnectStationJob::ConnectionFailed");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00400E80
nn::pia::common::ExecuteResult nn::pia::inet::NexConnectStationJob::waitForNatTraversalCompleted()
{
    if (NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol->IsTraversed(m_CurrentLocation.m_StationKey)) {
        SetStep(&NexConnectStationJob::resolveCurrentAddress, "NexConnectStationJob::resolveCurrentAddress");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    common::Time now;
    now.SetNow();
    if (m_Deadline >= now && m_NatCallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_MSEC);
    }
    if (m_CompletedRetryNum > 0) {
        // the traversal starts again
        m_CompletedRetryNum--;
        u8 retryCount = 1 - m_CompletedRetryNum;
        if (!IsMonitoringDataSenderFlagSet()) {
            common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
            content.m_Unknown0x49D = retryCount;
            content.m_Unknown0x24E = content.m_Unknown0x24E == 0xFF ? 1 : content.m_Unknown0x24E + 1;
        }
        m_NatCallContext.SignalCancel();
        m_NatCallContext.Reset();
        NexFacade::s_pInstance->m_pNatTraverser->ClearNatTraversal(m_CurrentLocation, true);
        SetStep(&NexConnectStationJob::prepareNatTraversal, "NexConnectStationJob::prepareNatTraversal");
        return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_MSEC);
    }
    // the reason of the failure from the NAT properties
    nn::Result result = m_NatCallContext.m_Result;
    if (m_NatCallContext.m_State == common::CallContext::STATE_CALL_FAILURE && result == common::RESULT_STATION_CONNECTION_FAILED_F2) {
        m_pCallContext->SignalFailure(common::RESULT_STATION_CONNECTION_FAILED_F2);
        SetNatTraversalTime();
        NexFacade::s_pInstance->m_pNatTraverser->ReportNatTraversalResult(m_CurrentLocation, common::RESULT_STATION_CONNECTION_FAILED_F2, false);
    } else if (m_NatCallContext.m_State == common::CallContext::STATE_CALL_FAILURE && result == common::RESULT_NAT_CHECK_FAILED) {
        m_pCallContext->SignalFailure(common::RESULT_NAT_CHECK_FAILED);
    } else {
        if (NexFacade::s_pInstance->GetNatPropertyMapping() == 2 && NexFacade::IsEdmMapping(m_ConnectionInfo.m_PublicLocation)) {
            result = common::RESULT_STATION_CONNECTION_FAILED_ED;
        } else if (NexFacade::s_pInstance->GetNatPropertyMapping() == 2 && NexFacade::IsEimMapping(m_ConnectionInfo.m_PublicLocation)) {
            result = common::RESULT_STATION_CONNECTION_FAILED_EC;
        } else if (NexFacade::s_pInstance->GetNatPropertyMapping() == 1 && NexFacade::IsEdmMapping(m_ConnectionInfo.m_PublicLocation)) {
            result = common::RESULT_STATION_CONNECTION_FAILED_EB;
        } else if (NexFacade::s_pInstance->GetNatPropertyMapping() == 1 && NexFacade::IsEimMapping(m_ConnectionInfo.m_PublicLocation)) {
            result = common::RESULT_STATION_CONNECTION_FAILED_EA;
        } else {
            result = common::RESULT_STATION_CONNECTION_FAILED_E7;
        }
        m_pCallContext->SignalFailure(result);
    }
    NexFacade::s_pInstance->m_pNatTraverser->ReportNatTraversalResult(m_CurrentLocation, result, !m_IsInverseConnection);
    NexFacade::s_pInstance->m_pNatTraverser->ClearNatTraversal(m_CurrentLocation, true);
    SetStep(&ConnectStationJob::ConnectionFailed, "ConnectStationJob::ConnectionFailed");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004012E4 | fefates:bytes [tier B]
nn::pia::inet::NexConnectStationJob::NexConnectStationJob() : m_CurrentLocation(), m_ConnectionInfo(), m_Deadline(), m_NatCallContext()
{
    m_Unknown0x38 = 0x8000;
}

// 0x00401380
// 0x00401338 (deleting dtor)
nn::pia::inet::NexConnectStationJob::~NexConnectStationJob()
{
    m_NatCallContext.SignalCancel();
    m_NatCallContext.Reset();
}

// 0x0072F174
void nn::pia::inet::NexConnectStationJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
