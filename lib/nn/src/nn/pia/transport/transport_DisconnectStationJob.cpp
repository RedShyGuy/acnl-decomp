#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_ProcessConnectionRequestJob.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_StationProtocol.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00458274 slot 0x1C | fefates:bytes
nn::Result nn::pia::transport::DisconnectStationJob::StartupImpl(nn::pia::transport::Station* pStation)
{
    if (pStation == nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (pStation->m_State != Station::STATION_STATE_CONNECTED) {
        return common::RESULT_INVALID_STATE;
    }
    m_pStation = pStation;
    m_IsWaitingResponse = false;
    common::Time now;
    now.SetNow();
    m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    Reset(true);
    SetStep(&DisconnectStationJob::SendDisconnectionRequest, "DisconnectStationJob::SendDisconnectionRequest");
    // a station behind a relay
    RelayRouteManager* pRelayRouteManager = Transport::s_pInstance->m_pRelayRouteManager;
    if (common::IsValidPointer(pRelayRouteManager)) {
        Station* pLocalStation = StationManager::s_pInstance->m_pLocalStation;
        if (common::IsValidPointer(pLocalStation)) {
            StationIndex relayStationIndex;
            if (pRelayRouteManager->GetRelayRoute(pLocalStation->m_StationIndex, m_pStation->m_StationIndex, &relayStationIndex).IsSuccess() &&
                m_pStation->m_StationIndex != relayStationIndex) {
                SetStep(&DisconnectStationJob::CutRouteOfRelayConnection, "DisconnectStationJob::CutRouteOfRelayConnection");
            }
        }
    }
    return nn::Result();
}

// 0x0045841C slot 0x18 (name is ours)
void nn::pia::transport::DisconnectStationJob::OnDisconnected(nn::pia::transport::Station*)
{
    // empty (in the original too)
}

// 0x00458420 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::DisconnectStationJob::WaitForDisconnection()
{
    if (m_IsWaitingResponse) {
        common::Scheduler* pScheduler = common::Scheduler::s_pInstance;
        if (pScheduler->m_DispatchTime >= m_Deadline ||
            (m_pStation->m_StationIndex > STATION_INDEX_MAX && !m_pStation->m_StationAddress.IsValid())) {
            // no answer: the station goes anyway
            m_IsWaitingResponse = false;
        } else {
            if (pScheduler->m_DispatchTime < m_NextSendTime) {
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            if (m_pStation->m_StationIndex <= STATION_INDEX_MAX) {
                m_pStation->m_pStationProtocol->SendDisconnectionRequest(m_pStation->m_StationIndex, false);
            } else {
                m_pStation->m_pStationProtocol->SendDisconnectionRequest(m_pStation->m_StationAddress);
            }
            m_NextSendTime = pScheduler->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_IntervalMSec);
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
    }
    m_pStation->m_State = Station::STATION_STATE_DISCONNECTED;
    SetStep(&DisconnectStationJob::DisconnectionSucceeded, "DisconnectStationJob::DisconnectionSucceeded");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00458588 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::DisconnectStationJob::DisconnectionSucceeded()
{
    m_pStation->m_pDisconnectStationJob->OnDisconnected(m_pStation);
    StationConnectionInfoTable::s_pInstance->EraseFromTable(m_pStation);
    IdentificationInfoTable::s_pInstance->EraseFromTable(m_pStation);
    ConnectStationJob* pConnectJob = m_pStation->m_pConnectStationJob;
    if (common::IsValidPointer(pConnectJob)) {
        pConnectJob->Cleanup();
        pConnectJob->Reset(false);
    }
    ProcessConnectionRequestJob* pProcessJob = m_pStation->m_pProcessConnectionRequestJob;
    if (common::IsValidPointer(pProcessJob)) {
        pProcessJob->Cleanup();
        pProcessJob->Reset(false);
    }
    m_pStation->Cleanup();
    StationManager::s_pInstance->DestroyStation(m_pStation);
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00458664 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::DisconnectStationJob::SendDisconnectionRequest()
{
    if (m_pStation->m_StationIndex <= STATION_INDEX_MAX) {
        m_pStation->m_pStationProtocol->SendDisconnectionRequest(m_pStation->m_StationIndex, false);
    } else {
        if (!m_pStation->m_StationAddress.IsValid()) {
            SetStep(&DisconnectStationJob::DisconnectionSucceeded, "DisconnectStationJob::DisconnectionSucceeded");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        m_pStation->m_pStationProtocol->SendDisconnectionRequest(m_pStation->m_StationAddress);
    }
    m_NextSendTime = common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_IntervalMSec);
    m_IsWaitingResponse = true;
    SetStep(&DisconnectStationJob::WaitForDisconnection, "DisconnectStationJob::WaitForDisconnection");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004587B8 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::DisconnectStationJob::CutRouteOfRelayConnection()
{
    // twice, in packets of their own
    if (m_pStation->m_StationIndex <= STATION_INDEX_MAX) {
        m_pStation->m_pStationProtocol->SendDisconnectionRequest(m_pStation->m_StationIndex, true);
        m_pStation->m_pStationProtocol->SendDisconnectionRequest(m_pStation->m_StationIndex, true);
    }
    m_pStation->m_State = Station::STATION_STATE_DISCONNECTED;
    SetStep(&DisconnectStationJob::DisconnectionSucceeded, "DisconnectStationJob::DisconnectionSucceeded");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0045885C | fefates:bytes [tier B]
void nn::pia::transport::DisconnectStationJob::Cleanup()
{
    m_pStation = nullptr;
    m_IsWaitingResponse = false;
    m_Deadline = common::Time();
}

// 0x00458878 (name is ours)
nn::Result nn::pia::transport::DisconnectStationJob::Startup(nn::pia::transport::Station* pStation)
{
    return StartupImpl(pStation);
}

// 0x00458884 | fefates:bytes [tier B]
nn::pia::transport::DisconnectStationJob::DisconnectStationJob()
    : m_TimeoutMSec(4000), m_pStation(nullptr), m_IntervalMSec(500), m_IsWaitingResponse(false)
{
}

// 0x004588E8
// 0x004588D4 (deleting dtor)
nn::pia::transport::DisconnectStationJob::~DisconnectStationJob()
{
    // empty (in the original too)
}

// 0x007360A8 slot 0x14
void nn::pia::transport::DisconnectStationJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
