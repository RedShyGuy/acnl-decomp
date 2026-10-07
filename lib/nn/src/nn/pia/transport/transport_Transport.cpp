#include "nn/pia/transport/transport_Transport.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/common/common_IPacketOutput.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_SignatureManager.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_Api.h"
#include "nn/pia/transport/transport_AttendanceTable.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_KeepAliveSender.h"
#include "nn/pia/transport/transport_NetworkFactory.h"
#include "nn/pia/transport/transport_NetworkRttManager.h"
#include "nn/pia/transport/transport_ProtocolMessageFilteringManager.h"
#include "nn/pia/transport/transport_ReceiveThreadStream.h"
#include "nn/pia/transport/transport_ResendingMessageManager.h"
#include "nn/pia/transport/transport_SendThreadStream.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_StationPacketHandler.h"
#include "nn/pia/transport/transport_StationProtocolManager.h"
#include "nn/pia/transport/transport_ThreadStreamManager.h"
#include "nn/pia/transport/transport_TransportAnalyzer.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the latency emulation and the packet drops of the stream threads (only set to 0 by the static
// initializer at 0x0079D39C; the names are ours)
struct StreamDebugSetting
{
    StreamDebugSetting() : m_SendLatencyPacketNum(0), m_ReceiveLatencyPacketNum(0), m_IsDropEnabled(false) {}

    u32 m_SendLatencyPacketNum;
    u32 m_ReceiveLatencyPacketNum;
    bool m_IsDropEnabled;
};

// 0x00AE08B0
StreamDebugSetting s_StreamDebugSetting;
} // namespace

// 0x00975A94
nn::pia::transport::Transport* nn::pia::transport::Transport::s_pInstance;

// 0x0045FA5C | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::transport::Transport::initialize(const nn::pia::transport::Transport::Setting& setting)
{
    NetworkFactory* pFactory = setting.m_pNetworkFactory;
    u32 stationNum = setting.m_StationNumMax;
    if (!common::IsValidPointer(pFactory) || stationNum - 1 >= STATION_INDEX_MAX + 1 || stationNum > 24) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    common::SignatureManager::s_pInstance->SetNecessity(pFactory->IsSignatureNecessary());
    NetworkRttManager::s_pInstance->InitializeImpl(stationNum + 8, stationNum);
    nn::Result result = StationManager::s_pInstance->Initialize(pFactory, stationNum);
    if (result.IsFailure()) {
        return result;
    }
    m_StationNum = stationNum;
    m_ProtocolManager.Initialize();
    StationProtocolManager::s_pInstance->Initialize(stationNum);
    StationConnectionInfoTable::s_pInstance->Initialize(stationNum);
    ResendingMessageManager::s_pInstance->Initialize(stationNum * 2);
    IdentificationInfoTable::s_pInstance->Initialize(stationNum);
    ThreadStreamManager::CreateInstance(pFactory, setting.m_SendPacketNum, setting.m_ReceivePacketNum, s_StreamDebugSetting.m_SendLatencyPacketNum,
                                        s_StreamDebugSetting.m_ReceiveLatencyPacketNum, s_StreamDebugSetting.m_IsDropEnabled);
    ThreadStreamManager* pStreamManager = ThreadStreamManager::s_pInstance;

    PacketHandler* pPacketHandler = pFactory->CreatePacketHandler();
    if (pPacketHandler == nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!pPacketHandler->checkDerivedRuntimeTypeInfo(StationPacketHandler::getRuntimeTypeInfoStatic())) {
        delete pPacketHandler;
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pPacketHandler = pPacketHandler->checkDerivedRuntimeTypeInfo(StationPacketHandler::getRuntimeTypeInfoStatic())
                           ? static_cast<StationPacketHandler*>(pPacketHandler)
                           : nullptr;
    u32 headerSize = pFactory->GetPacketHeaderSize();
    bool isBroadcast = pStreamManager->m_pOutput->IsBroadcast();
    u32 destinationNumMax = pStreamManager->m_pOutput->GetDestinationNumMax();
    result = m_pPacketHandler->Initialize(destinationNumMax, isBroadcast, headerSize);
    if (result.IsFailure()) {
        return result;
    }
    result = pFactory->CreateProtocols();
    if (result.IsFailure()) {
        return result;
    }
    result = TransportAnalyzer::s_pInstance->Initialize();
    if (result.IsFailure()) {
        return result;
    }
    m_AnalysisIntervalSec = setting.m_AnalysisIntervalSec;
    result = m_pStationIdTable->Initialize(stationNum);
    if (result.IsFailure()) {
        return result;
    }
    result = AttendanceTable::s_pInstance->Initialize();
    if (result.IsFailure()) {
        return result;
    }
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_StationNumMax = setting.m_StationNumMax;
    content.m_SendPacketNum = setting.m_SendPacketNum;
    content.m_ReceivePacketNum = setting.m_ReceivePacketNum;
    return nn::Result();
}

// 0x0045FD6C | fefates:bytes
nn::pia::common::ExecuteResult nn::pia::transport::Transport::DispatchJob::ExecuteCore()
{
    Transport::s_pInstance->dispatch();
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00428BA4
// 0x0045FD94 (deleting dtor)
nn::pia::transport::Transport::DispatchJob::~DispatchJob()
{
    // empty (in the original too)
}

// 0x0045FDA4 (name is ours)
nn::Result nn::pia::transport::Transport::CreateInstance(const Setting& setting)
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
    s_pInstance = new Transport;
    nn::Result result = s_pInstance->initialize(setting);
    if (result.IsSuccess()) {
        return nn::Result();
    }
    delete s_pInstance;
    s_pInstance = nullptr;
    return result;
}

// 0x0045FEE0 | fefates:bytes [tier B]
nn::pia::common::IPacketInput* nn::pia::transport::Transport::GetInputStream()
{
    ThreadStreamManager* pManager = ThreadStreamManager::s_pInstance;
    return common::IsValidPointer(pManager) ? pManager->m_pInput : nullptr;
}

// 0x0045FF08 | fefates:bytes [tier B]
void nn::pia::transport::Transport::DestroyInstance()
{
    if (common::IsValidPointer(s_pInstance)) {
        s_pInstance->finalize();
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00450048 (name is ours)
nn::Result nn::pia::transport::Transport::SetKeepAliveInterval(int intervalMSec)
{
    return m_pPacketHandler->m_KeepAliveSender.SetInterval(intervalMSec);
}

// 0x0045FFEC (name is ours)
void nn::pia::transport::Transport::EnableKeepAlive()
{
    m_pPacketHandler->m_KeepAliveSender.m_IsEnabled = true;
    m_pPacketHandler->m_KeepAliveReceiver.m_IsEnabled = true;
}

// 0x00460000 | fefates:bytes [tier B]
nn::pia::common::IPacketOutput* nn::pia::transport::Transport::GetOutputStream()
{
    ThreadStreamManager* pManager = ThreadStreamManager::s_pInstance;
    return common::IsValidPointer(pManager) ? pManager->m_pOutput : nullptr;
}

// 0x00460060 | fefates:bytes [tier B]
void nn::pia::transport::Transport::OutputStreamUpdateEvent()
{
    ThreadStreamManager* pManager = ThreadStreamManager::s_pInstance;
    if (common::IsValidPointer(pManager) && pManager->m_pOutput != nullptr) {
        pManager->m_pOutput->OnStationConnectionEvent();
    }
}

// 0x0046009C | fefates:callseq [tier C]
void nn::pia::transport::Transport::SetMonitoringNetworkRtt(bool isSessionBegin)
{
    NetworkRttManager* pRttManager = NetworkRttManager::s_pInstance;
    if (!common::IsValidPointer(pRttManager)) {
        return;
    }
    StationManager* pManager = StationManager::s_pInstance;
    if (!common::IsValidPointer(pManager)) {
        return;
    }
    // the stations with the least and the most round trip time
    u32 minRtt = 0xFFFF;
    u32 maxRtt = 0;
    Station* pMinStation = nullptr;
    Station* pMaxStation = nullptr;
    for (Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        if ((*it)->m_State != Station::STATION_STATE_CONNECTED || !(*it)->m_StationAddress.IsValid()) {
            continue;
        }
        u32 rtt = pRttManager->GetAverage((*it)->m_StationAddress);
        if (rtt == 0) {
            continue;
        }
        if (rtt < minRtt) {
            minRtt = rtt;
            pMinStation = *it;
        }
        if (rtt > maxRtt) {
            maxRtt = rtt;
            pMaxStation = *it;
        }
    }
    StationConnectionInfoTable* pTable = StationConnectionInfoTable::s_pInstance;
    u32 principalId = pTable->GetPrincipalIdByStation(pMinStation);
    u32 hash = (minRtt < 0xFFFF && principalId != 0) ? common::hashWithMd5(principalId) : 0xFFFFFFFF;
    if (isSessionBegin) {
        common::g_SessionBeginMonitoringContent.m_MinRtt = minRtt < 0xFFFF ? minRtt : 0xFFFF;
        common::g_SessionBeginMonitoringContent.m_MinRttPrincipalIdHash = hash;
    } else {
        common::g_SessionStateMonitoringContent.m_MinRtt = minRtt < 0xFFFF ? minRtt : 0xFFFF;
        common::g_SessionStateMonitoringContent.m_MinRttPrincipalIdHash = hash;
    }
    principalId = pTable->GetPrincipalIdByStation(pMaxStation);
    hash = (maxRtt != 0 && principalId != 0) ? common::hashWithMd5(principalId) : 0xFFFFFFFF;
    if (isSessionBegin) {
        common::g_SessionBeginMonitoringContent.m_MaxRtt = maxRtt != 0 ? maxRtt : 0xFFFF;
        common::g_SessionBeginMonitoringContent.m_MaxRttPrincipalIdHash = hash;
    } else {
        common::g_SessionStateMonitoringContent.m_MaxRtt = maxRtt != 0 ? maxRtt : 0xFFFF;
        common::g_SessionStateMonitoringContent.m_MaxRttPrincipalIdHash = hash;
    }
}

// 0x00460254 | fefates:bytes [tier B]
void nn::pia::transport::Transport::Cleanup()
{
    if (!m_IsStarted) {
        return;
    }
    m_StreamResult = nn::Result();
    m_IsStarted = false;
    m_DispatchJob.Reset(false);
    if (common::IsValidPointer(AttendanceTable::s_pInstance)) {
        AttendanceTable::s_pInstance->Cleanup();
    }
    if (common::IsValidPointer(TransportAnalyzer::s_pInstance)) {
        TransportAnalyzer::s_pInstance->Cleanup();
    }
    if (common::IsValidPointer(ResendingMessageManager::s_pInstance)) {
        ResendingMessageManager::s_pInstance->Cleanup();
    }
    m_ProtocolManager.Cleanup();
    if (common::IsValidPointer(m_pPacketHandler)) {
        m_pPacketHandler->Cleanup();
    }
    if (common::IsValidPointer(ThreadStreamManager::s_pInstance)) {
        ThreadStreamManager::s_pInstance->Cleanup();
    }
}

// 0x00460314 (name after C++)
nn::pia::transport::Transport::Setting::Setting()
    : m_pNetworkFactory(nullptr), m_StationNumMax(0), m_SendPacketNum(0), m_ReceivePacketNum(0), m_AnalysisIntervalSec(0)
{
}

// 0x00460330 | fefates:bytes [tier B]
nn::Result nn::pia::transport::Transport::Startup(const nn::pia::common::StationAddress* pRelayNodeAddress, const nn::pia::common::CryptoSetting* pCryptoSetting)
{
    if (IsInSetupMode() || m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    ThreadStreamManager* pStreamManager = ThreadStreamManager::s_pInstance;
    pStreamManager->Startup();
    m_pPacketHandler->Startup(&pStreamManager->m_pSendStream->m_PacketStream.m_Writer, &pStreamManager->m_pReceiveStream->m_PacketStream.m_Reader,
                              m_pRelayRouteManager, pRelayNodeAddress, pCryptoSetting);
    m_ProtocolManager.Startup(m_pPacketHandler);
    ResendingMessageManager::s_pInstance->Startup(m_pPacketHandler);
    TransportAnalyzer::s_pInstance->Startup();
    AttendanceTable::s_pInstance->Startup();
    m_DispatchJob.Ready(false);
    m_AnalysisTime = m_DispatchTime;
    m_IsStarted = true;
    m_StreamResult = nn::Result();
    return nn::Result();
}

// 0x00460408 | fefates:callseq [tier C]
nn::Result nn::pia::transport::Transport::dispatch()
{
    common::Time now;
    now.SetNow();
    m_DispatchTime = now;
    ThreadStreamManager* pStreamManager = ThreadStreamManager::s_pInstance;
    if (!common::IsValidPointer(pStreamManager)) {
        return common::RESULT_INVALID_STATE;
    }
    // an error of a stream thread stops the dispatch
    nn::Result result = pStreamManager->m_pSendStream->GetLastResult();
    if (result.IsSuccess()) {
        result = ThreadStreamManager::s_pInstance->m_pReceiveStream->GetLastResult();
    }
    if (result.IsFailure()) {
        m_StreamResult = result;
        return nn::Result();
    }
    m_pPacketHandler->BeginDispatch(m_DispatchTime);
    result = m_ProtocolManager.Dispatch();
    if (result.IsSuccess()) {
        result = ResendingMessageManager::s_pInstance->Dispatch();
    }
    if (result.IsFailure()) {
        m_pPacketHandler->EndDispatch(m_DispatchTime);
        return result;
    }
    m_pPacketHandler->EndDispatch(m_DispatchTime);
    if (m_IsStarted && m_AnalysisIntervalSec != 0) {
        TransportAnalyzer* pAnalyzer = TransportAnalyzer::s_pInstance;
        if (common::IsValidPointer(pAnalyzer) &&
            (m_DispatchTime - m_AnalysisTime).GetTick() / common::TimeSpan::GetTicksPerSec().GetTick() >= m_AnalysisIntervalSec &&
            pAnalyzer->Update().IsSuccess()) {
            m_AnalysisTime = m_DispatchTime;
            TransportAnalysisData data = pAnalyzer->m_Data;
            data.Print(false);
        }
    }
    return nn::Result();
}

// 0x004605AC | fefates:bytes [tier B]
void nn::pia::transport::Transport::finalize()
{
    if (common::IsValidPointer(AttendanceTable::s_pInstance)) {
        AttendanceTable::s_pInstance->Finalize();
    }
    if (common::IsValidPointer(m_pStationIdTable)) {
        m_pStationIdTable->Finalize();
    }
    if (common::IsValidPointer(TransportAnalyzer::s_pInstance)) {
        TransportAnalyzer::s_pInstance->Finalize();
    }
    if (common::IsValidPointer(m_pPacketHandler)) {
        m_pPacketHandler->Finalize();
        delete m_pPacketHandler;
        m_pPacketHandler = nullptr;
    }
    ThreadStreamManager::DestroyInstance();
    StationProtocolManager::s_pInstance->Finalize();
    m_ProtocolManager.Finalize();
    if (common::IsValidPointer(m_pPacketHandler)) {
        delete m_pPacketHandler;
        m_pPacketHandler = nullptr;
    }
    StationManager::s_pInstance->Finalize();
    NetworkRttManager::s_pInstance->Finalize();
    common::SignatureManager::s_pInstance->ResetNecessity();
}

// 0x004606B0 | fefates:bytes [tier B]
nn::pia::transport::Transport::Transport()
    : m_pPacketHandler(nullptr), m_StationNum(0), m_IsStarted(false), m_StreamResult(), m_Unknown0x60(0), m_pRelayRouteManager(nullptr),
      m_AnalysisIntervalSec(0), m_pStationIdCallback(nullptr), m_pStationIdTable(nullptr), m_IsUsingStationIdTable(false), m_State(common::RESULT_INVALID_STATE)
{
    StationManager::CreateInstance();
    StationProtocolManager::CreateInstance();
    StationConnectionInfoTable::CreateInstance();
    ResendingMessageManager::CreateInstance();
    IdentificationInfoTable::CreateInstance();
    NetworkRttManager::CreateInstance();
    ProtocolMessageFilteringManager::CreateInstance();
    TransportAnalyzer::CreateInstance();
    m_pStationIdTable = new StationIdTable;
    AttendanceTable::CreateInstance();
}

// (inline in CreateInstance and DestroyInstance)
inline nn::pia::transport::Transport::~Transport()
{
    AttendanceTable::DestroyInstance();
    delete m_pStationIdTable;
    TransportAnalyzer::DestroyInstance();
    ProtocolMessageFilteringManager::DestroyInstance();
    NetworkRttManager::DestroyInstance();
    IdentificationInfoTable::DestroyInstance();
    ResendingMessageManager::DestroyInstance();
    StationConnectionInfoTable::DestroyInstance();
    StationProtocolManager::DestroyInstance();
    StationManager::DestroyInstance();
}

// 0x00736FAC (name is ours)
s32 nn::pia::transport::Transport::GetKeepAliveIntervalMSec() const
{
    return m_pPacketHandler->m_KeepAliveSender.m_IntervalMSec;
}

// 0x007370E8
void nn::pia::transport::Transport::Trace(u64) const
{
    // empty (in the original too)
}

// 0x00460028 (name is ours)
nn::Result nn::pia::transport::Transport::SetState(nn::Result state)
{
    if (state.IsSuccess() || state == common::RESULT_INVALID_STATE || state == common::RESULT_INVALID_STATE_TEMPORARY) {
        m_State = state;
        return nn::Result();
    }
    return common::RESULT_INVALID_ARGUMENT;
}

// 0x00736E84 | fefates:callseq [tier C]
nn::Result nn::pia::transport::Transport::ConvertToStationId(nn::pia::StationId* pStationId, nn::pia::StationIndex stationIndex) const
{
    if (!common::IsValidPointer(pStationId)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_State == common::RESULT_INVALID_STATE || m_State == common::RESULT_INVALID_STATE_TEMPORARY) {
        return m_State;
    }
    switch (stationIndex) {
    case 253:
        *pStationId = GetStationIdOfIndex253();
        return nn::Result();
    case 254:
        *pStationId = GetStationIdOfIndex254();
        return nn::Result();
    case 255:
        *pStationId = GetStationIdOfIndex255();
        return nn::Result();
    }
    if (!m_IsUsingStationIdTable) {
        *pStationId = StationId(stationIndex, 0);
        return nn::Result();
    }
    StationIdTable::Entry entry;
    nn::Result result = m_pStationIdTable->Find(&entry, stationIndex);
    if (result.IsFailure()) {
        return result;
    }
    *pStationId = entry.m_StationId;
    return nn::Result();
}

// 0x00736FB8 | fefates:callseq [tier C]
nn::Result nn::pia::transport::Transport::ConvertToStationIndex(nn::pia::StationIndex* pStationIndex, nn::pia::StationId stationId) const
{
    if (!common::IsValidPointer(pStationIndex)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_State == common::RESULT_INVALID_STATE || m_State == common::RESULT_INVALID_STATE_TEMPORARY) {
        return m_State;
    }
    if (stationId == GetStationIdOfIndex253()) {
        *pStationIndex = static_cast<StationIndex>(253);
        return nn::Result();
    }
    if (stationId == GetStationIdOfIndex254()) {
        *pStationIndex = static_cast<StationIndex>(254);
        return nn::Result();
    }
    if (stationId == GetStationIdOfIndex255()) {
        *pStationIndex = static_cast<StationIndex>(255);
        return nn::Result();
    }
    if (!m_IsUsingStationIdTable) {
        *pStationIndex = static_cast<StationIndex>(stationId.m_Low);
        return nn::Result();
    }
    StationIdTable::Entry entry;
    nn::Result result = m_pStationIdTable->Find(&entry, stationId);
    if (result.IsFailure()) {
        return result;
    }
    *pStationIndex = entry.m_StationIndex;
    return nn::Result();
}

} // namespace transport
} // namespace pia
} // namespace nn
