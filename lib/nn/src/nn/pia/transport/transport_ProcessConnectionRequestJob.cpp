#include "nn/pia/transport/transport_ProcessConnectionRequestJob.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_ResendingMessageManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_StationProtocol.h"
#include "nn/pia/transport/transport_Transport.h"
#include "pead/peadHeapMgr.h"
#include <new>

namespace nn {
namespace pia {
namespace transport {
namespace {
const u64 TRACE_FLAG_STATION = 0x80000000;
// the size of the connection response
const u32 RESPONSE_SIZE = 76;
} // namespace

// 0x0045E904 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ProcessConnectionRequestJob::WaitResponseAck()
{
    ResendingMessageManager* pManager = ResendingMessageManager::s_pInstance;
    if (pManager->CheckNowResending(m_AckId)) {
        if (common::Scheduler::s_pInstance->m_DispatchTime < m_Deadline) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_pCallContext->Cancel();
        pManager->StopResending(m_AckId);
    } else if (!m_IsRelay) {
        // the station is reached directly
        RelayRouteManager* pRelayRouteManager = Transport::s_pInstance->m_pRelayRouteManager;
        if (common::IsValidPointer(pRelayRouteManager)) {
            StationIndex stationIndex = m_pStation->m_StationIndex;
            if (stationIndex <= STATION_INDEX_MAX && StationManager::s_pInstance->m_pLocalStation->m_StationIndex <= STATION_INDEX_MAX) {
                pRelayRouteManager->SetRelayRoute(StationManager::s_pInstance->m_pLocalStation->m_StationIndex, stationIndex, stationIndex);
            }
        }
    }
    m_AckId = 0;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0045E9EC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ProcessConnectionRequestJob::WaitInverseConnection()
{
    common::CallContext* pCallContext = m_pCallContext;
    switch (pCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
    case common::CallContext::STATE_CALL_FAILURE:
    case common::CallContext::STATE_CALL_CANCEL:
        if (pCallContext->m_Result.IsFailure()) {
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        break;
    default:
        break;
    }
    // the response is resent until the deadline of the inverse connection at least
    ConnectStationJob* pConnectJob = m_pStation->m_pConnectStationJob;
    if ((pConnectJob->m_Deadline - m_Deadline).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > 0) {
        m_Deadline = m_pStation->m_pConnectStationJob->m_Deadline;
    }
    SetStep(&ProcessConnectionRequestJob::SendConnectionResponse, "ProcessConnectionRequestJob::SendConnectionResponse");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0045EAF8 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ProcessConnectionRequestJob::SendConnectionResponse()
{
    StationProtocol* pProtocol = m_pStation->m_pStationProtocol;
    if (pProtocol == nullptr) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    u8 buffer[RESPONSE_SIZE] = {};
    pProtocol->MakeConnectionResponseData(buffer, m_IsRelay);
    StationIndex stationIndex = STATION_INDEX_UNIDENTIFIED;
    if (m_IsRelay) {
        stationIndex = m_pStation->m_StationIndex;
    }
    if (ResendingMessageManager::s_pInstance
            ->SetSendMessage(&m_AckId, buffer, RESPONSE_SIZE, stationIndex, m_pStation->m_StationAddress,
                             m_pStation->m_pStationProtocol->m_ProtocolId, m_Deadline.m_Tick)
            .IsFailure()) {
        if (common::Scheduler::s_pInstance->m_DispatchTime >= m_Deadline) {
            m_pCallContext->Cancel();
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_pStation->m_StationIndex > STATION_INDEX_MAX) {
        m_pStation->m_LastSendTime = Transport::s_pInstance->m_DispatchTime;
    }
    SetStep(&ProcessConnectionRequestJob::WaitResponseAck, "ProcessConnectionRequestJob::WaitResponseAck");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0045EC7C | fefates:bytes [tier B]
bool nn::pia::transport::ProcessConnectionRequestJob::StartupRelayConnection(nn::pia::transport::Station* pStation, int timeoutMSec, bool isInverseConnection)
{
    if (pStation == nullptr) {
        return false;
    }
    m_pStation = pStation;
    pStation->Trace(TRACE_FLAG_STATION);
    m_pCallContext->SignalSuccess(nn::Result());
    if (m_pStation->m_State == Station::STATION_STATE_REQUESTED && !isInverseConnection) {
        // connect back to the station
        ConnectStationJob* pConnectJob = m_pStation->m_pConnectStationJob;
        StationConnectionInfo info;
        if (StationConnectionInfoTable::s_pInstance->GetStationConnectionInfo(m_pStation, &info).IsSuccess()) {
            if (pConnectJob->StartupRelayConnection(m_pCallContext, m_pStation, info, true, timeoutMSec).IsSuccess()) {
                pConnectJob->Ready(false);
            }
        }
    } else if (m_pStation->m_State != Station::STATION_STATE_REQUESTED && !isInverseConnection) {
        return false;
    }
    m_TimeoutMSec = timeoutMSec;
    m_pStation->m_State = Station::STATION_STATE_CONNECTING;
    m_Deadline = common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    m_AckId = 0;
    m_IsRelay = true;
    Reset(true);
    SetStep(&ProcessConnectionRequestJob::WaitInverseConnection, "ProcessConnectionRequestJob::WaitInverseConnection");
    return true;
}

// 0x0045EE2C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ProcessConnectionRequestJob::ConnectToRequesterStation()
{
    SetStep(&ProcessConnectionRequestJob::WaitInverseConnection, "ProcessConnectionRequestJob::WaitInverseConnection");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0045EE98 | fefates:bytes [tier B]
void nn::pia::transport::ProcessConnectionRequestJob::Cleanup()
{
    m_pStation = nullptr;
    if (m_AckId != 0) {
        if (ResendingMessageManager::s_pInstance != nullptr) {
            ResendingMessageManager::s_pInstance->StopResending(m_AckId);
        }
        m_AckId = 0;
    }
    m_Deadline = common::Time();
    if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pCallContext->SignalCancel();
    }
}

// 0x0045EEF4 | fefates:bytes [tier B]
bool nn::pia::transport::ProcessConnectionRequestJob::Startup(nn::pia::transport::Station* pStation, int timeoutMSec, bool isInverseConnection)
{
    if (pStation == nullptr) {
        return false;
    }
    m_pStation = pStation;
    pStation->Trace(TRACE_FLAG_STATION);
    m_pCallContext->SignalSuccess(nn::Result());
    if (m_pStation->m_State == Station::STATION_STATE_REQUESTED && !isInverseConnection) {
        // connect back to the station
        ConnectStationJob* pConnectJob = m_pStation->m_pConnectStationJob;
        StationConnectionInfo info;
        if (StationConnectionInfoTable::s_pInstance->GetStationConnectionInfo(m_pStation, &info).IsSuccess()) {
            if (pConnectJob->Startup(m_pCallContext, m_pStation, info, true, timeoutMSec).IsSuccess()) {
                pConnectJob->Ready(false);
            }
        }
    } else if (m_pStation->m_State != Station::STATION_STATE_REQUESTED && !isInverseConnection) {
        return false;
    }
    m_TimeoutMSec = timeoutMSec;
    m_pStation->m_State = Station::STATION_STATE_CONNECTING;
    m_Deadline = common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    m_AckId = 0;
    m_IsRelay = false;
    Reset(true);
    SetStep(&ProcessConnectionRequestJob::ConnectToRequesterStation, "ProcessConnectionRequestJob::ConnectToRequesterStation");
    return true;
}

// 0x0045F0A4 | fefates:bytes [tier B]
nn::pia::transport::ProcessConnectionRequestJob::ProcessConnectionRequestJob() : m_TimeoutMSec(15000), m_AckId(0)
{
    void* pBuffer = pead::AllocMemory(sizeof(common::CallContext), common::HeapManager::GetHeap());
    m_pCallContext = ::new (pBuffer) common::CallContext();
}

// 0x0045F164
// 0x0045F0FC (deleting dtor)
nn::pia::transport::ProcessConnectionRequestJob::~ProcessConnectionRequestJob()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalCancel();
        }
        if (m_pCallContext != nullptr) {
            m_pCallContext->~CallContext();
            pead::FreeMemory(m_pCallContext);
        }
        m_pCallContext = nullptr;
    }
}

// 0x00736D0C slot 0x14
void nn::pia::transport::ProcessConnectionRequestJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
