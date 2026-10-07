#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_ResendingMessageManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationProtocol.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
const u64 TRACE_FLAG = 0x80000;
const u64 TRACE_FLAG_STATION = 0x80000000;
// the size of the connection request buffer
const u32 REQUEST_BUFFER_SIZE = 116;
} // namespace

inline void nn::pia::transport::ConnectStationJob::GiveUpStation()
{
    if (m_pStation != nullptr && m_pStation->m_State == Station::STATION_STATE_CONNECTING) {
        m_pStation->m_State = Station::STATION_STATE_DISCONNECTED;
    }
}

// 0x004534A4 slot 0x1C
void nn::pia::transport::ConnectStationJob::CleanupImpl()
{
    // empty (in the original too)
}

// 0x004534A8 slot 0x18 | fefates:bytes
nn::Result nn::pia::transport::ConnectStationJob::StartupImpl(nn::pia::common::CallContext* pCallContext, nn::pia::transport::Station* pStation, const nn::pia::transport::StationConnectionInfo& info, bool isInverseConnection)
{
    if (!common::IsValidPointer(pStation)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    pStation->Trace(TRACE_FLAG_STATION);
    if (common::IsValidPointer(m_pStation)) {
        m_pStation->Trace(TRACE_FLAG_STATION);
    }
    if (GetState() != EXECUTE_STATE_IDLE && GetState() != EXECUTE_STATE_FINISHED) {
        Trace(TRACE_FLAG);
        return common::RESULT_INVALID_STATE;
    }
    if (pCallContext == nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    m_pStation = pStation;
    pStation->m_StationAddress = info.m_PublicLocation.m_StationAddress;
    m_IsInverseConnection = isInverseConnection;
    Reset(true);
    m_pStation->m_ConnectionRoute = Station::CONNECTION_ROUTE_DIRECT;
    SetStep(&ConnectStationJob::SendConnectionRequest, "ConnectStationJob::SendConnectionRequest");
    return nn::Result();
}

// 0x00453604 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ConnectStationJob::WaitRequestAck()
{
    ResendingMessageManager* pManager = ResendingMessageManager::s_pInstance;
    if (!pManager->CheckNowResending(m_AckId)) {
        // the request is acked
        m_AckId = 0;
        SetStep(&ConnectStationJob::WaitForConnection, "ConnectStationJob::WaitForConnection");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    common::CallContext* pCallContext = m_pCallContext;
    if (pCallContext != nullptr && pCallContext->IsCancelRequested()) {
        if (!pCallContext->IsFinished()) {
            pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        pManager->StopResending(m_AckId);
        m_AckId = 0;
        GiveUpStation();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (common::Scheduler::s_pInstance->m_DispatchTime >= m_Deadline) {
        if (pCallContext != nullptr) {
            if (!pCallContext->IsFinished()) {
                pCallContext->SignalFailure(common::RESULT_TIMEOUT);
            }
            m_pCallContext = nullptr;
        }
        pManager->StopResending(m_AckId);
        m_AckId = 0;
        GiveUpStation();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pStation->m_State == Station::STATION_STATE_CONNECTED) {
        pManager->StopResending(m_AckId);
        m_AckId = 0;
        SetStep(&ConnectStationJob::ConnectionSucceeded, "ConnectStationJob::ConnectionSucceeded");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_DenyReason == 0) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // denied
    if (pCallContext != nullptr) {
        if (!pCallContext->IsFinished()) {
            nn::Result result;
            if (m_DenyReason == StationProtocol::DENY_REASON_REFUSED) {
                result = common::RESULT_CONNECTION_REFUSED;
            } else if (m_DenyReason == StationProtocol::DENY_REASON_INCOMPATIBLE_VERSION) {
                result = common::RESULT_INCOMPATIBLE_VERSION;
            } else {
                result = common::RESULT_CONNECTION_FAILED;
            }
            pCallContext->SignalFailure(result);
        }
        m_pCallContext = nullptr;
    }
    pManager->StopResending(m_AckId);
    m_AckId = 0;
    GiveUpStation();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00453898 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ConnectStationJob::ConnectionFailed()
{
    GiveUpStation();
    if (m_pCallContext != nullptr && !m_pCallContext->IsFinished()) {
        m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004538FC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ConnectStationJob::WaitForConnection()
{
    if (m_pStation->m_State == Station::STATION_STATE_CONNECTED) {
        SetStep(&ConnectStationJob::ConnectionSucceeded, "ConnectStationJob::ConnectionSucceeded");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    common::CallContext* pCallContext = m_pCallContext;
    if (pCallContext != nullptr && pCallContext->IsCancelRequested()) {
        if (!pCallContext->IsFinished()) {
            pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        GiveUpStation();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (common::Scheduler::s_pInstance->m_DispatchTime >= m_Deadline) {
        if (pCallContext != nullptr) {
            if (!pCallContext->IsFinished()) {
                pCallContext->SignalFailure(common::RESULT_TIMEOUT);
            }
            m_pCallContext = nullptr;
        }
        GiveUpStation();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00453A50 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ConnectStationJob::ConnectionSucceeded()
{
    common::CallContext* pCallContext = m_pCallContext;
    if (pCallContext != nullptr && pCallContext->IsCancelRequested()) {
        if (!pCallContext->IsFinished()) {
            pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pStation->m_StationIndex <= STATION_INDEX_MAX) {
        Transport::s_pInstance->OutputStreamUpdateEvent();
    }
    if (m_pCallContext != nullptr && !m_pCallContext->IsFinished()) {
        m_pCallContext->SignalSuccess(nn::Result());
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00453AF4 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ConnectStationJob::SendConnectionRequest()
{
    common::CallContext* pCallContext = m_pCallContext;
    if (pCallContext != nullptr && pCallContext->IsCancelRequested()) {
        if (!pCallContext->IsFinished()) {
            pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        GiveUpStation();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    // the connection id of the packets to the station (2..255)
    if (m_pStation->m_LocalConnectionId == 0) {
        common::Time now;
        now.SetNow();
        m_pStation->m_LocalConnectionId = static_cast<u8>(static_cast<u64>(now.m_Tick) % 254 + 2);
    }
    if (!SendConnectionRequestMessage()) {
        SetStep(&ConnectStationJob::ConnectionFailed, "ConnectStationJob::ConnectionFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_pStation->m_StationIndex > STATION_INDEX_MAX) {
        m_pStation->m_LastSendTime = Transport::s_pInstance->m_DispatchTime;
    }
    m_DenyReason = 0;
    m_Deadline = common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    SetStep(&ConnectStationJob::WaitRequestAck, "ConnectStationJob::WaitRequestAck");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00453CD0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ConnectStationJob::StartupRelayConnection(nn::pia::common::CallContext* pCallContext, nn::pia::transport::Station* pStation, const nn::pia::transport::StationConnectionInfo& info, bool isInverseConnection, int timeoutMSec)
{
    m_TimeoutMSec = timeoutMSec;
    nn::Result result = StartupImpl(pCallContext, pStation, info, isInverseConnection);
    if (result.IsFailure()) {
        return result;
    }
    m_pStation->m_ConnectionRoute = Station::CONNECTION_ROUTE_RELAY;
    SetStep(&ConnectStationJob::SendRelayConnectionRequest, "ConnectStationJob::SendRelayConnectionRequest");
    return nn::Result();
}

// 0x00453D60 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::transport::ConnectStationJob::SendRelayConnectionRequest()
{
    common::CallContext* pCallContext = m_pCallContext;
    if (pCallContext != nullptr && pCallContext->IsCancelRequested()) {
        if (!pCallContext->IsFinished()) {
            pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        GiveUpStation();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Station* pStation = m_pStation;
    StationProtocol* pProtocol = pStation->m_pStationProtocol;
    if (pProtocol == nullptr || pStation->m_StationIndex > STATION_INDEX_MAX) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    u8 buffer[REQUEST_BUFFER_SIZE] = {};
    pProtocol->MakeConnectionRequestData(buffer, pStation->m_LocalConnectionId, true, m_IsInverseConnection);
    u32 size = pProtocol->GetConnectionRequestDataSize();
    if (ResendingMessageManager::s_pInstance
            ->SetSendMessage(&m_AckId, buffer, size, m_pStation->m_StationIndex, m_pStation->m_StationAddress, pProtocol->m_ProtocolId, 0)
            .IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_DenyReason = 0;
    m_Deadline = common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    SetStep(&ConnectStationJob::WaitRequestAck, "ConnectStationJob::WaitRequestAck");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00453F18 | fefates:bytes [tier B]
bool nn::pia::transport::ConnectStationJob::SendConnectionRequestMessage()
{
    Station* pStation = m_pStation;
    StationProtocol* pProtocol = pStation->m_pStationProtocol;
    if (pProtocol == nullptr) {
        return false;
    }
    u8 buffer[REQUEST_BUFFER_SIZE] = {};
    pProtocol->MakeConnectionRequestData(buffer, pStation->m_LocalConnectionId, false, m_IsInverseConnection);
    // with a relay route manager to an indexed station by its index, else by its address
    StationIndex stationIndex = STATION_INDEX_UNIDENTIFIED;
    common::StationAddress address;
    if (common::IsValidPointer(Transport::s_pInstance->m_pRelayRouteManager) && m_pStation->m_StationIndex != 254 &&
        m_pStation->m_StationIndex <= STATION_INDEX_MAX) {
        stationIndex = m_pStation->m_StationIndex;
    } else {
        address = m_pStation->m_StationAddress;
    }
    return ResendingMessageManager::s_pInstance
        ->SetSendMessage(&m_AckId, buffer, pProtocol->GetConnectionRequestDataSize(), stationIndex, address, pProtocol->m_ProtocolId, 0)
        .IsSuccess();
}

// 0x00454024 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::Cleanup()
{
    CleanupImpl();
    m_pStation = nullptr;
    if (m_AckId != 0) {
        if (ResendingMessageManager::s_pInstance != nullptr) {
            ResendingMessageManager::s_pInstance->StopResending(m_AckId);
        }
        m_AckId = 0;
    }
    m_Deadline = common::Time();
    if (m_pCallContext != nullptr) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
    if (IsRunning()) {
        Reset(false);
    }
}

// 0x004540D0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ConnectStationJob::Startup(nn::pia::common::CallContext* pCallContext, nn::pia::transport::Station* pStation, const nn::pia::transport::StationConnectionInfo& info, bool isInverseConnection, int timeoutMSec)
{
    m_TimeoutMSec = timeoutMSec;
    return StartupImpl(pCallContext, pStation, info, isInverseConnection);
}

// 0x004540F4 | fefates:bytes [tier B]
nn::pia::transport::ConnectStationJob::ConnectStationJob()
    : m_TimeoutMSec(15000), m_AckId(0), m_pCallContext(nullptr), m_pStation(nullptr), m_DenyReason(0)
{
}

// 0x0045414C
// 0x0045413C (deleting dtor)
nn::pia::transport::ConnectStationJob::~ConnectStationJob()
{
    // empty (in the original too)
}

// 0x007356D4 slot 0x14
void nn::pia::transport::ConnectStationJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
