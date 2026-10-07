#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_NetworkFactory.h"
#include "nn/pia/transport/transport_ProcessConnectionRequestJob.h"
#include "nn/pia/transport/transport_ReliableSlidingWindow.h"
#include "nn/pia/transport/transport_RttProtocol.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationProtocolManager.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the window of the reliable data of a station: messages to send and to receive (name is ours)
const u32 RELIABLE_WINDOW_SIZE = 4;
} // namespace

// 0x0045F338 | fefates:bytes [tier B]
nn::Result nn::pia::transport::Station::Initialize(nn::pia::transport::NetworkFactory* pFactory)
{
    if (!common::IsValidPointer(pFactory)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pConnectStationJob = pFactory->CreateConnectStationJob();
    m_pDisconnectStationJob = pFactory->CreateDisconnectStationJob();
    m_pReliableSlidingWindow->Initialize(RELIABLE_WINDOW_SIZE, RELIABLE_WINDOW_SIZE);
    return nn::Result();
}

// 0x0045F3A0 | fefates:bytes [tier B]
void nn::pia::transport::Station::CleanupJobs()
{
    if (common::IsValidPointer(m_pConnectStationJob)) {
        m_pConnectStationJob->Cleanup();
        m_pConnectStationJob->Reset(false);
    }
    if (common::IsValidPointer(m_pDisconnectStationJob)) {
        m_pDisconnectStationJob->Cleanup();
        m_pDisconnectStationJob->Reset(false);
    }
    if (common::IsValidPointer(m_pProcessConnectionRequestJob)) {
        m_pProcessConnectionRequestJob->Cleanup();
        m_pProcessConnectionRequestJob->Reset(false);
    }
}

// 0x0045F430 (name is ours)
void nn::pia::transport::Station::SetStationId(StationId stationId)
{
    m_StationId = stationId;
}

// 0x0045F43C | fefates:bytes [tier B]
nn::Result nn::pia::transport::Station::GetPrincipalId(unsigned int* pPrincipalId)
{
    if (!common::IsValidPointer(StationConnectionInfoTable::s_pInstance)) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pPrincipalId)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    StationConnectionInfo info;
    nn::Result result = StationConnectionInfoTable::s_pInstance->GetStationConnectionInfo(this, &info);
    if (result.IsFailure()) {
        StationConnectionInfoTable::s_pInstance->Trace(0x80000);
        Trace(0x80000);
        return result;
    }
    *pPrincipalId = info.m_PublicLocation.m_PrincipalId;
    return nn::Result();
}

// 0x0045F510 | fefates:bytes [tier B]
void nn::pia::transport::Station::helperStartupCleanup(bool isStartup, nn::pia::transport::StationProtocol* pProtocol, nn::pia::StationIndex stationIndex, const nn::pia::common::StationAddress& address, nn::pia::transport::Station::StationState state)
{
    if (isStartup) {
        m_SequenceIdController.Startup();
        common::Time now;
        now.SetNow();
        m_LastSendTime = now;
        m_LastReceiveTime = now;
    } else {
        m_pReliableSlidingWindow->Cleanup();
    }
    m_pStationProtocol = pProtocol;
    m_StationIndex = stationIndex;
    m_StationAddress = address;
    m_State = state;
    m_StationId = GetStationIdOfIndex253();
    m_LocalConnectionId = false;
    m_RemoteConnectionId = false;
    m_Unknown0x68 = false;
    m_Unknown0x69 = false;
}

// 0x0045F5C0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::Station::GetPlayerName(nn::pia::transport::Station::PlayerName* pName)
{
    if (!common::IsValidPointer(IdentificationInfoTable::s_pInstance)) {
        return common::RESULT_INVALID_STATE;
    }
    return IdentificationInfoTable::s_pInstance->GetPlayerName(this, pName);
}

// 0x0045F5FC | fefates:bytes [tier B]
void nn::pia::transport::Station::Cleanup()
{
    common::StationAddress address;
    address.Clear();
    helperStartupCleanup(false, nullptr, STATION_INDEX_UNIDENTIFIED, address, STATION_STATE_NONE);
}

// 0x0045F658 | fefates:bytes [tier B]
bool nn::pia::transport::Station::Startup(nn::pia::transport::StationProtocol* pProtocol)
{
    if (!common::IsValidPointer(pProtocol)) {
        return false;
    }
    common::StationAddress address;
    address.Clear();
    helperStartupCleanup(true, pProtocol, STATION_INDEX_UNIDENTIFIED, address, STATION_STATE_STARTED);
    return true;
}

// 0x0045F6D4 | fefates:bytes [tier B]
bool nn::pia::transport::Station::Startup(nn::pia::transport::StationProtocol* pProtocol, nn::pia::StationIndex stationIndex, const nn::pia::common::StationAddress& address)
{
    if (!common::IsValidPointer(pProtocol)) {
        return false;
    }
    if (!common::isValidSourceStationIndex(stationIndex)) {
        return false;
    }
    helperStartupCleanup(true, pProtocol, stationIndex, address, STATION_STATE_STARTED);
    return true;
}

// 0x0045F740 | fefates:bytes [tier B]
bool nn::pia::transport::Station::Startup(nn::pia::transport::StationProtocol* pProtocol, const nn::pia::common::StationAddress& address)
{
    if (!common::IsValidPointer(pProtocol)) {
        return false;
    }
    helperStartupCleanup(true, pProtocol, STATION_INDEX_UNIDENTIFIED, address, STATION_STATE_STARTED);
    return true;
}

// 0x0045F784 | fefates:bytes [tier B]
void nn::pia::transport::Station::Finalize()
{
    if (common::IsValidPointer(m_pReliableSlidingWindow)) {
        m_pReliableSlidingWindow->Finalize();
    }
    if (m_pDisconnectStationJob != nullptr) {
        delete m_pDisconnectStationJob;
        m_pDisconnectStationJob = nullptr;
    }
    if (m_pConnectStationJob != nullptr) {
        delete m_pConnectStationJob;
        m_pConnectStationJob = nullptr;
    }
}

// 0x0045F7E4 | fefates:callgraph [tier C]
nn::pia::transport::Station::Station()
    : m_StationIndex(STATION_INDEX_UNIDENTIFIED), m_StationId(GetStationIdOfIndex253()), m_State(STATION_STATE_NONE),
      m_pReliableSlidingWindow(new ReliableSlidingWindow()), m_pStationProtocol(nullptr), m_pConnectStationJob(nullptr),
      m_pDisconnectStationJob(nullptr), m_pProcessConnectionRequestJob(new ProcessConnectionRequestJob()), m_LocalConnectionId(false),
      m_RemoteConnectionId(false), m_ConnectionRoute(CONNECTION_ROUTE_DIRECT), m_Unknown0x68(false), m_Unknown0x69(false)
{
}

// 0x0045F894 | fefates:bytes [tier B]
nn::pia::transport::Station::~Station()
{
    if (m_pProcessConnectionRequestJob != nullptr) {
        delete m_pProcessConnectionRequestJob;
    }
    if (m_pReliableSlidingWindow != nullptr) {
        delete m_pReliableSlidingWindow;
    }
}

// 0x00736D90 | fefates:bytes [tier B]
bool nn::pia::transport::Station::IsConnectionRouteRelay() const
{
    return m_State == STATION_STATE_CONNECTED && m_ConnectionRoute == CONNECTION_ROUTE_RELAY;
}

// 0x00736DAC | fefates:bytes [tier B]
bool nn::pia::transport::Station::IsConnectionRouteDirect() const
{
    return m_State == STATION_STATE_CONNECTED && m_ConnectionRoute == CONNECTION_ROUTE_DIRECT;
}

// 0x00736DC4
void nn::pia::transport::Station::Trace(u64) const
{
    // empty (in the original too)
}

// 0x00736DC8 | fefates:bytes [tier B]
s32 nn::pia::transport::Station::GetRtt(unsigned int num) const
{
    if (common::IsValidPointer(StationProtocolManager::s_pInstance)) {
        RttProtocol* pRttProtocol = StationProtocolManager::s_pInstance->GetRttProtocol();
        if (common::IsValidPointer(pRttProtocol) && m_StationIndex != STATION_INDEX_UNIDENTIFIED) {
            return pRttProtocol->GetRtt(m_StationIndex, num);
        }
    }
    return -1;
}

// 0x00736E24 | fefates:bytes [tier B]
s32 nn::pia::transport::Station::GetRtt() const
{
    if (common::IsValidPointer(StationProtocolManager::s_pInstance)) {
        RttProtocol* pRttProtocol = StationProtocolManager::s_pInstance->GetRttProtocol();
        if (common::IsValidPointer(pRttProtocol) && m_StationIndex != STATION_INDEX_UNIDENTIFIED) {
            return pRttProtocol->GetRtt(m_StationIndex);
        }
    }
    return -1;
}

} // namespace transport
} // namespace pia
} // namespace nn
