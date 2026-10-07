#include "nn/pia/transport/transport_TransportAnalyzer.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/transport/transport_Api.h"
#include "nn/pia/transport/transport_ConnectionAnalyzer.h"
#include "nn/pia/transport/transport_StationPacketHandler.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_ThreadStreamManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the protocols of a direction in the monitoring data (after the sum)
const u32 MONITORING_PROTOCOL_NUM = 15;
} // namespace

// 0x0097E488
nn::pia::transport::TransportAnalyzer* nn::pia::transport::TransportAnalyzer::s_pInstance;

// 0x0045739C (name is ours)
nn::Result nn::pia::transport::TransportAnalyzer::Initialize()
{
    if (common::IsValidPointer(m_pConnectionAnalyzer)) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    m_pConnectionAnalyzer = new ConnectionAnalyzer;
    if (!common::IsValidPointer(m_pConnectionAnalyzer)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    return nn::Result();
}

// 0x004573F0 | fefates:callseq [tier C]
nn::Result nn::pia::transport::TransportAnalyzer::CreateInstance()
{
    if (!IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!IsInSetupMode()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new TransportAnalyzer;
    if (!common::IsValidPointer(s_pInstance)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    return nn::Result();
}

// 0x004574BC | fefates:bytes [tier B]
void nn::pia::transport::TransportAnalyzer::DestroyInstance()
{
    if (common::IsValidPointer(s_pInstance)) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00457514 | fefates:callseq [tier C]
void nn::pia::transport::TransportAnalyzer::SetMonitoringData()
{
    if (!common::IsValidPointer(m_pConnectionAnalyzer)) {
        return;
    }
    common::SessionStateMonitoringContent& content = common::g_SessionStateMonitoringContent;
    PacketHandler* pPacketHandler = Transport::s_pInstance->m_pPacketHandler;
    PacketAnalysisData data;

    if (pPacketHandler->GetPacketAnalysisData(&data, nullptr).IsFailure()) {
        return;
    }
    content.m_SendProtocolIds[0] = 0;
    content.m_SendPacketNums[0] = data.m_TotalPacketNum;
    content.m_SendSizes[0] = data.m_TotalSize;
    u32 num = data.m_ProtocolNum < MONITORING_PROTOCOL_NUM ? data.m_ProtocolNum : MONITORING_PROTOCOL_NUM;
    for (u32 i = 0; i < num; i++) {
        content.m_SendProtocolIds[1 + i] = data.m_ProtocolData[i].m_ProtocolId.m_Id;
        content.m_SendPacketNums[1 + i] = data.m_ProtocolData[i].m_TotalPacketNum;
        content.m_SendSizes[1 + i] = data.m_ProtocolData[i].m_TotalSize;
    }

    if (pPacketHandler->GetPacketAnalysisData(nullptr, &data).IsFailure()) {
        return;
    }
    content.m_ReceiveProtocolIds[0] = 0;
    content.m_ReceivePacketNums[0] = data.m_TotalPacketNum;
    content.m_ReceiveSizes[0] = data.m_TotalSize;
    num = data.m_ProtocolNum < MONITORING_PROTOCOL_NUM ? data.m_ProtocolNum : MONITORING_PROTOCOL_NUM;
    for (u32 i = 0; i < num; i++) {
        content.m_ReceiveProtocolIds[1 + i] = data.m_ProtocolData[i].m_ProtocolId.m_Id;
        content.m_ReceivePacketNums[1 + i] = data.m_ProtocolData[i].m_TotalPacketNum;
        content.m_ReceiveSizes[1 + i] = data.m_ProtocolData[i].m_TotalSize;
    }

    StationManager* pManager = StationManager::s_pInstance;
    if (!common::IsValidPointer(pManager)) {
        return;
    }
    Station* pLocalStation = pManager->m_pLocalStation;
    if (!common::IsValidPointer(pLocalStation)) {
        return;
    }
    u32 stationNum = 0;
    for (Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        Station* pStation = *it;
        if (pStation == pLocalStation) {
            continue;
        }
        const SequenceIdController* pController = pStation->GetSequenceIdController();
        if (!common::IsValidPointer(pController)) {
            continue;
        }
        StationIndex stationIndex = pStation->m_StationIndex;
        s32 rtt = pStation->GetRtt();
        // the lost packets in 0.01 %
        u16 packetLoss;
        if (pController->m_TotalReceivedNum == 0) {
            packetLoss = 0xFFFF;
        } else {
            packetLoss = static_cast<u16>(static_cast<u64>(pController->m_TotalLostNum) * 10000 / pController->m_TotalReceivedNum);
        }
        u32 principalId;
        if (pStation->GetPrincipalId(&principalId).IsFailure()) {
            principalId = 0xFFFFFFFF;
        }
        content.m_StationPrincipalIdHashes[stationNum] = common::hashWithMd5(principalId);
        content.m_StationIndices[stationNum] = stationIndex;
        content.m_StationRtts[stationNum] = rtt;
        content.m_StationPacketLosses[stationNum] = packetLoss;
        stationNum++;
    }
}

// 0x004577A8 (name is ours)
nn::Result nn::pia::transport::TransportAnalyzer::Update()
{
    if (!common::IsValidPointer(m_pConnectionAnalyzer)) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(ThreadStreamManager::s_pInstance)) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pConnectionAnalyzer->Update();
    if (result.IsFailure()) {
        return result;
    }
    result = m_pConnectionAnalyzer->GetConnectionAnalysisData(&m_Data.m_ConnectionData);
    if (result.IsFailure()) {
        return result;
    }
    PacketHandler* pPacketHandler = Transport::s_pInstance->m_pPacketHandler;
    result = pPacketHandler->GetPacketAnalysisData(&m_Data.m_SendData, &m_Data.m_ReceiveData);
    if (result.IsFailure()) {
        return result;
    }
    pPacketHandler->ClearPacketAnalysisData();
    return nn::Result();
}

// 0x0045785C (name is ours)
void nn::pia::transport::TransportAnalyzer::Cleanup()
{
    if (m_IsStarted) {
        if (common::IsValidPointer(m_pConnectionAnalyzer)) {
            m_pConnectionAnalyzer->Cleanup();
        }
        m_IsStarted = false;
    }
}

// 0x00457894 | fefates:callseq [tier C]
nn::Result nn::pia::transport::TransportAnalyzer::Startup()
{
    if (!common::IsValidPointer(m_pConnectionAnalyzer)) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(ThreadStreamManager::s_pInstance)) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pConnectionAnalyzer->Startup();
    if (result.IsFailure()) {
        return result;
    }
    m_StartTime = Transport::s_pInstance->m_DispatchTime;
    m_IsStarted = true;
    return nn::Result();
}

// 0x00457924 | fefates:bytes [tier B]
void nn::pia::transport::TransportAnalyzer::Finalize()
{
    if (m_pConnectionAnalyzer != nullptr) {
        delete m_pConnectionAnalyzer;
        m_pConnectionAnalyzer = nullptr;
    }
}

} // namespace transport
} // namespace pia
} // namespace nn
