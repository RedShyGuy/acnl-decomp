#include "nn/pia/session/session_Session.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/session/session_Api.h"
#include "nn/pia/session/session_AutoMatchmakeJob.h"
#include "nn/pia/session/session_BrowseMatchmakeJob.h"
#include "nn/pia/session/session_ClearMatchmakeSystemPasswordJob.h"
#include "nn/pia/session/session_CloseParticipationJob.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_ConfigParticipationJobBase.h"
#include "nn/pia/session/session_CreateSessionJob.h"
#include "nn/pia/session/session_DestroySessionJob.h"
#include "nn/pia/session/session_GenerateMatchmakeSystemPasswordJob.h"
#include "nn/pia/session/session_ISessionInfoList.h"
#include "nn/pia/session/session_JoinSessionJob.h"
#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/session/session_LeaveSessionJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshEventListenerForSession.h"
#include "nn/pia/session/session_MeshLayerController.h"
#include "nn/pia/session/session_ModifyAttributeJob.h"
#include "nn/pia/session/session_OpenParticipationJob.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/session/session_SessionProtocol.h"
#include "nn/pia/session/session_SessionSearchCriteria.h"
#include "nn/pia/session/session_SessionStatusCheckJob.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/session/session_UpdateApplicationDataJob.h"
#include "nn/pia/session/session_UpdateSessionSettingJob.h"
#include "nn/pia/transport/transport_NetworkFactory.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"
#include "pead/peadHeapMgr.h"
#include <string.h>

namespace nn {
namespace pia {
namespace session {
// 0x00975A80
nn::pia::session::Session::GlobalSetting nn::pia::session::Session::s_GlobalSetting;
// 0x00975A84
nn::pia::session::Session* nn::pia::session::Session::s_pInstance;

namespace {
const u64 TRACE_FLAG = 0x40000000;

// Transport::m_pStationIdCallback (GetStationIdCallback; name is ours)
// 0x00447BB4
s32 IsJointSessionStationCallback(const transport::StationIdTable::Entry* pEntry)
{
    return Session::s_pInstance->m_pStationIdStatusTable->IsJointSessionStation(pEntry);
}
} // namespace

// 0x0044A0F0 (name is ours)
void nn::pia::session::Session::NotifyJoinEvent(StationId stationId)
{
    if (common::IsValidPointer(reinterpret_cast<void*>(m_EventCallback))) {
        m_EventCallback(EVENT_TYPE_JOIN, stationId);
    }
}

// 0x0044A114 (name is ours)
void nn::pia::session::Session::OnParticipantDisconnected(u32 sessionId, u32 principalId)
{
    if (m_DisconnectState == 2 || m_DisconnectState == 3) {
        return;
    }
    if (m_pLeaveSessionJob != nullptr && m_pLeaveSessionJob->IsRunning()) {
        return;
    }
    if (m_pJointSessionJob != nullptr && m_pJointSessionJob->IsRunning()) {
        m_pJointSessionJob->vf_0x58(sessionId, principalId);
    }
    if (m_pStationIdStatusTable == nullptr) {
        return;
    }
    if (m_SessionIds[m_CurrentIndex == 0 ? 1 : 0] != sessionId && m_SessionIds[m_CurrentIndex] != sessionId) {
        return;
    }
    if (Mesh::s_pInstance->m_IsJoined != true) {
        return;
    }
    transport::StationManager* pManager = transport::StationManager::s_pInstance;
    bool isFound = false;
    for (transport::Station** ppStation = pManager->m_ActiveStations.Begin(); ppStation != pManager->m_ActiveStations.End(); ppStation++) {
        transport::Station* pStation = *ppStation;
        if (pStation->m_StationId == StationId(principalId, 0)) {
            isFound = true;
            if (pStation->m_State == transport::Station::STATION_STATE_CONNECTED) {
                pStation->m_Unknown0x68 = true;
            } else {
                pStation->m_State = transport::Station::STATION_STATE_DISCONNECTED;
            }
        }
    }
    if (!isFound) {
        pManager->Trace(TRACE_FLAG);
    }
}

// 0x0044A26C (name is ours)
void nn::pia::session::Session::ClearStatus(bool keepStationIds)
{
    m_Unknown0xB2 = false;
    if (m_pStationIdStatusTable != nullptr) {
        transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
        if (m_LocalStationId == GetStationIdOfIndex253()) {
            pTable->Clear();
            m_pStationIdStatusTable->Clear();
        } else {
            // all stations of the table go, the local one last
            StationId stationIds[transport::StationManager::STATION_NUM_MAX];
            u32 num = 0;
            bool isLocalFound = false;
            typedef common::ObjList<transport::StationIdTable::Entry> EntryList;
            for (EntryList::Node* pNode = pTable->m_List.Begin(); pNode != transport::Transport::s_pInstance->m_pStationIdTable->m_List.End();
                 pNode = EntryList::Advance(pNode)) {
                if (pNode->m_Value.m_StationId == m_LocalStationId) {
                    isLocalFound = true;
                } else if (pNode->m_Value.m_StationId != GetStationIdOfIndex253()) {
                    stationIds[num++] = pNode->m_Value.m_StationId;
                }
            }
            if (isLocalFound) {
                stationIds[num++] = m_LocalStationId;
            }
            for (u32 i = 0; i < num; i++) {
                transport::StationIdTable::Entry entry;
                entry.m_StationId = stationIds[i];
                entry.m_StationIndex = STATION_INDEX_UNIDENTIFIED;
                entry.m_Key = 0;
                RemoveStation(entry, 0);
            }
            // (two calls were removed by the linker here)
            transport::Transport::s_pInstance->m_pStationIdTable->Clear();
            m_pStationIdStatusTable->Clear();
        }
    }
    m_State = 0;
    m_DisconnectState = 0;
    if (!keepStationIds) {
        m_LocalStationId = GetStationIdOfIndex253();
        m_HostStationId = GetStationIdOfIndex253();
        m_JointHostStationId = GetStationIdOfIndex253();
    }
    m_SessionIds[0] = 0;
    m_SessionIds[1] = 0;
    m_StationIdEntryNumMax[0] = 0;
    m_StationIdEntryNumMax[1] = 0;
    memset(m_Unknown0x101, 0, sizeof(m_Unknown0x101));
    ClearString();
}

// 0x0044A4D4 (name is ours)
nn::Result nn::pia::session::Session::CreateInstance(Setting setting)
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
    transport::NetworkFactory* pFactory = setting.m_pNetworkFactory;
    if (!common::IsValidPointer(pFactory) || (setting.m_RelayMode != 0 && !pFactory->IsRelayRouteSupported()) ||
        pFactory->GetSessionInfoNumMax() < setting.m_SessionInfoNumMax) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    Mesh::GlobalSetting globalSetting;
    globalSetting.m_IsRelayRouteNetwork = s_GlobalSetting.m_IsRelayRouteNetwork;
    globalSetting.m_IsTimeoutFree = s_GlobalSetting.m_IsTimeoutFree;
    nn::Result result = Mesh::SetGlobalSetting(globalSetting);
    if (result.IsFailure()) {
        return result;
    }
    Mesh::Setting meshSetting;
    meshSetting.m_pNetworkFactory = pFactory;
    meshSetting.m_RelayMode = setting.m_RelayMode;
    meshSetting.m_IsBandwidthCheckEnabled = setting.m_IsBandwidthCheckEnabled;
    result = Mesh::CreateInstance(meshSetting);
    if (result.IsFailure()) {
        return result;
    }
    if (s_pInstance == nullptr) {
        s_pInstance = new Session(setting);
        s_pInstance->m_CurrentIndex = 0;
        s_pInstance->m_pSessionInfoList = pFactory->CreateSessionInfoList(setting.m_SessionInfoNumMax);
        s_pInstance->m_pMatchmakeSessions[0] = pFactory->CreateMatchmakeSession();
        transport::Transport* pTransport = transport::Transport::s_pInstance;
        u32 stationNum = pTransport->m_StationNum;
        pTransport->m_IsUsingStationIdTable = true;
        if (pFactory->IsJointSessionSupported()) {
            s_pInstance->m_pMatchmakeSessions[1] = pFactory->CreateMatchmakeSession();
            if (s_pInstance->m_pStationIdStatusTable == nullptr) {
                s_pInstance->m_pStationIdStatusTable = new StationIdStatusTable();
            }
            s_pInstance->m_SessionProtocolId =
                transport::Transport::s_pInstance->m_ProtocolManager.CreateProtocol<SessionProtocol>(transport::PROTOCOL_TYPE_SESSION, 0);
            s_pInstance->m_pSessionProtocol = transport::Transport::s_pInstance->m_ProtocolManager.GetProtocol<SessionProtocol>(
                s_pInstance->m_SessionProtocolId, transport::PROTOCOL_TYPE_SESSION);
            s_pInstance->m_pStationIdStatusTable->Initialize(stationNum);
            s_pInstance->m_pSessionProtocol->Initialize(stationNum);
        } else {
            s_pInstance->m_pMatchmakeSessions[1] = nullptr;
            s_pInstance->m_pStationIdStatusTable = nullptr;
            s_pInstance->m_SessionProtocolId =
                transport::Transport::s_pInstance->m_ProtocolManager.CreateProtocol<SessionProtocol>(transport::PROTOCOL_TYPE_SESSION, 0);
            s_pInstance->m_pSessionProtocol = transport::Transport::s_pInstance->m_ProtocolManager.GetProtocol<SessionProtocol>(
                s_pInstance->m_SessionProtocolId, transport::PROTOCOL_TYPE_SESSION);
            s_pInstance->m_pSessionProtocol->Initialize(stationNum);
        }
        if (!pFactory->IsJointSessionSupported()) {
            s_pInstance->m_pJointSessionJob = nullptr;
        } else if (s_pInstance->m_pJointSessionJob == nullptr) {
            s_pInstance->m_pJointSessionJob = pFactory->CreateJointSessionJob();
        }
        if (s_pInstance->m_pSessionStatusCheckJob == nullptr) {
            s_pInstance->m_pSessionStatusCheckJob = new SessionStatusCheckJob();
        }
        if (s_pInstance->m_pCreateSessionJob == nullptr) {
            s_pInstance->m_pCreateSessionJob = pFactory->CreateCreateSessionJob();
        }
        if (s_pInstance->m_pAutoMatchmakeJob == nullptr) {
            s_pInstance->m_pAutoMatchmakeJob = pFactory->CreateAutoMatchmakeJob();
        }
        if (s_pInstance->m_pBrowseMatchmakeJob == nullptr) {
            s_pInstance->m_pBrowseMatchmakeJob = pFactory->CreateBrowseMatchmakeJob();
        }
        if (s_pInstance->m_pJoinSessionJob == nullptr) {
            s_pInstance->m_pJoinSessionJob = pFactory->CreateJoinSessionJob();
        }
        if (s_pInstance->m_pLeaveSessionJob == nullptr) {
            s_pInstance->m_pLeaveSessionJob = pFactory->CreateLeaveSessionJob();
        }
        if (s_pInstance->m_pDestroySessionJob == nullptr) {
            s_pInstance->m_pDestroySessionJob = pFactory->CreateDestroySessionJob();
        }
        if (s_pInstance->m_pOpenParticipationJob == nullptr) {
            s_pInstance->m_pOpenParticipationJob = new OpenParticipationJob();
        }
        if (s_pInstance->m_pCloseParticipationJob == nullptr) {
            s_pInstance->m_pCloseParticipationJob = new CloseParticipationJob();
        }
        if (s_pInstance->m_pConfigParticipationJob == nullptr) {
            s_pInstance->m_pConfigParticipationJob = new ConfigParticipationJobBase();
        }
        if (s_pInstance->m_pGenerateMatchmakeSystemPasswordJob == nullptr) {
            s_pInstance->m_pGenerateMatchmakeSystemPasswordJob = pFactory->CreateGenerateMatchmakeSystemPasswordJob();
        }
        if (s_pInstance->m_pClearMatchmakeSystemPasswordJob == nullptr) {
            s_pInstance->m_pClearMatchmakeSystemPasswordJob = pFactory->CreateClearMatchmakeSystemPasswordJob();
        }
        if (s_pInstance->m_pString == nullptr) {
            s_pInstance->m_StringLength = pFactory->GetStringBufferLength();
            s_pInstance->m_pString = pFactory->CreateStringBuffer(s_pInstance->m_StringLength);
        }
        if (s_pInstance->m_pModifyAttributeJob == nullptr) {
            s_pInstance->m_pModifyAttributeJob = pFactory->CreateModifyAttributeJob();
        }
        if (s_pInstance->m_pUpdateSessionSettingJob == nullptr) {
            s_pInstance->m_pUpdateSessionSettingJob = pFactory->CreateUpdateSessionSettingJob();
        }
        if (s_pInstance->m_pUpdateApplicationDataJob == nullptr) {
            s_pInstance->m_pUpdateApplicationDataJob = pFactory->CreateUpdateApplicationDataJob();
        }
        if (s_pInstance->m_pMeshLayerController == nullptr) {
            s_pInstance->m_pMeshLayerController = pFactory->CreateMeshLayerController();
        }
        s_pInstance->m_Unknown0x5 = pFactory->vf_0xA0();
        if (s_pInstance->m_pMeshEventListener == nullptr) {
            s_pInstance->m_pMeshEventListener = new MeshEventListenerForSession();
        }
        if (s_pInstance->m_pStationIdNodeBuffer == nullptr) {
            typedef common::ObjList<StationId>::Node Node;
            u64* pBuffer = common::NewArray<u64>((stationNum * sizeof(Node) + sizeof(u64) - 1) / sizeof(u64));
            s_pInstance->m_pStationIdNodeBuffer = pBuffer;
            s_pInstance->m_StationIdList.Initialize(reinterpret_cast<Node*>(pBuffer), stationNum);
        }
    }
    s_pInstance->m_IsStarted = false;
    return nn::Result();
}

// 0x0044AC2C (name is ours)
void nn::pia::session::Session::SetJoinable(u32 sessionId, bool isJoinable)
{
    if (isJoinable) {
        for (u32 i = 0; i < 4; i++) {
            if (m_UnjoinableSessionIds[i] == sessionId) {
                m_UnjoinableSessionIds[i] = 0;
            }
        }
        return;
    }
    for (u32 i = 0; i < 4; i++) {
        if (m_UnjoinableSessionIds[i] == sessionId) {
            return;
        }
    }
    for (u32 i = 0; i < 4; i++) {
        if (m_UnjoinableSessionIds[i] == 0) {
            m_UnjoinableSessionIds[i] = sessionId;
            return;
        }
    }
}

// 0x0044ACD0 (name is ours)
bool nn::pia::session::Session::AddStation(transport::StationIdTable::Entry entry, u32 sessionId)
{
    nn::Result result = transport::Transport::s_pInstance->m_pStationIdTable->Add(entry.m_StationId, entry.m_StationIndex, entry.m_Key);
    if (result.IsFailure()) {
        // (a call was removed by the linker here)
        return result.IsSuccess();
    }
    bool isJoint = m_Unknown0xB2;
    if (m_pStationIdStatusTable != nullptr) {
        result = m_pStationIdStatusTable->Add(entry.m_StationId, sessionId, 0, !isJoint);
    }
    if (!isJoint) {
        StationId* pStationId = m_StationIdList.PushBackNew();
        if (pStationId != nullptr) {
            *pStationId = entry.m_StationId;
        }
        if (common::IsValidPointer(reinterpret_cast<void*>(m_EventCallback))) {
            m_EventCallback(EVENT_TYPE_JOIN, entry.m_StationId);
        }
    }
    return result.IsSuccess();
}

// 0x0044AE04 (name is ours)
void nn::pia::session::Session::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
    if (Mesh::s_pInstance != nullptr) {
        Mesh::DestroyInstance();
    }
}

// 0x0044AE4C (name is ours)
u8 nn::pia::session::Session::GetJoinSessionPhase() const
{
    u8 phase = m_pJoinSessionJob->GetPhase();
    if (phase != 2) {
        return phase;
    }
    if (Mesh::s_pInstance->GetJoinMeshJobPhase() == 0) {
        return 2;
    }
    if (Mesh::s_pInstance->GetJoinMeshJobPhase() == 1) {
        return 4;
    }
    if (Mesh::s_pInstance->GetJoinMeshJobPhase() == 2) {
        return 5;
    }
    if (Mesh::s_pInstance->GetJoinMeshJobPhase() == 3) {
        return 6;
    }
    return 7;
}

// 0x0044AEC4 (name is ours)
bool nn::pia::session::Session::RemoveStation(transport::StationIdTable::Entry entry, u32)
{
    StationId stationId = GetStationIdOfIndex253();
    if (entry.m_Key != 0) {
        transport::StationIdTable::Entry found;
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&found, entry.m_Key).IsSuccess()) {
            stationId = found.m_StationId;
        }
    } else if (entry.m_StationId != GetStationIdOfIndex253()) {
        stationId = entry.m_StationId;
    } else if (entry.m_StationIndex != STATION_INDEX_UNIDENTIFIED) {
        transport::StationIdTable::Entry found;
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&found, entry.m_StationIndex).IsSuccess()) {
            stationId = found.m_StationId;
        }
    }
    if (stationId == GetStationIdOfIndex253()) {
        return false;
    }
    if (m_Unknown0xB2) {
        m_pStationIdStatusTable->SetValid(stationId, false);
        return true;
    }
    if (m_pStationIdStatusTable == nullptr || m_pStationIdStatusTable->IsNotified(stationId)) {
        if (common::IsValidPointer(reinterpret_cast<void*>(m_EventCallback))) {
            m_EventCallback(EVENT_TYPE_LEAVE, stationId);
        }
        for (common::ObjList<StationId>::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End();
             pNode = common::ObjList<StationId>::Advance(pNode)) {
            if (pNode->m_Value == stationId) {
                m_StationIdList.Erase(&pNode->m_Value);
                break;
            }
        }
    }
    nn::Result result = transport::Transport::s_pInstance->m_pStationIdTable->Remove(stationId);
    if (m_pStationIdStatusTable != nullptr) {
        result = m_pStationIdStatusTable->Remove(stationId);
    }
    return result.IsSuccess();
}

// 0x0044B0D8 (name is ours)
u8 nn::pia::session::Session::GetJointSessionPhase() const
{
    return m_pJointSessionJob->GetPhase();
}

// 0x0044B0E8 (name is ours)
nn::Result nn::pia::session::Session::JoinSessionAsync(const JoinSessionSetting* pSetting)
{
    if (!common::IsValidPointer(pSetting)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS || m_IsStarted != true || m_State != 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = m_pJoinSessionJob->Startup(&m_CallContext, pSetting);
    if (result.IsSuccess()) {
        m_pJoinSessionJob->Ready(false);
        m_AsyncType = ASYNC_TYPE_JOIN;
    }
    return result;
}

// 0x0044B190 (name is ours)
void nn::pia::session::Session::OnUnknownNotification(u32 value)
{
    if (m_pJoinSessionJob != nullptr && m_pJoinSessionJob->IsRunning()) {
        m_pJoinSessionJob->vf_0x1C(value);
        return;
    }
    if (m_pAutoMatchmakeJob != nullptr && m_pAutoMatchmakeJob->IsRunning()) {
        m_pAutoMatchmakeJob->vf_0x24(value);
        return;
    }
    if (m_pLeaveSessionJob != nullptr && m_pLeaveSessionJob->IsRunning()) {
        return;
    }
    if (m_DisconnectState == 2 || m_DisconnectState == 3) {
        return;
    }
    if (m_pJointSessionJob != nullptr && m_pJointSessionJob->IsRunning()) {
        m_pJointSessionJob->vf_0x5C(value);
    }
}

// 0x0044B264 (name is ours)
void nn::pia::session::Session::OnUnknownNotification2(u32 value1, u32 value2)
{
    if (m_pJointSessionJob != nullptr && m_pJointSessionJob->IsRunning()) {
        m_pJointSessionJob->vf_0x54(value1, value2);
    }
}

// 0x0044B2AC (name is ours)
nn::Result nn::pia::session::Session::LeaveSessionAsync()
{
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    if (m_IsStarted != true) {
        return common::RESULT_INVALID_STATE;
    }
    // the host without host migration destroys the session
    Mesh* pMesh = Mesh::s_pInstance;
    nn::Result result;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex && !pMesh->m_IsHostMigrationEnabled) {
        result = m_pDestroySessionJob->Startup(&m_CallContext);
        if (result.IsFailure()) {
            return result;
        }
        m_pDestroySessionJob->Ready(false);
    } else {
        result = m_pLeaveSessionJob->Startup(&m_CallContext);
        if (result.IsFailure()) {
            return result;
        }
        m_pLeaveSessionJob->Ready(false);
    }
    m_AsyncType = ASYNC_TYPE_LEAVE;
    m_pMeshLayerController->vf_0x38();
    return result;
}

// 0x0044B3A0 (name is ours)
void nn::pia::session::Session::AddToStationIdList(const StationId& stationId)
{
    StationId* pStationId = m_StationIdList.PushBackNew();
    if (pStationId != nullptr) {
        *pStationId = stationId;
    }
}

// 0x0044B41C (name is ours)
nn::Result nn::pia::session::Session::BrowseSessionAsync(const SessionSearchCriteria* pCriteria)
{
    if (!common::IsValidPointer(pCriteria)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS || m_IsStarted != true) {
        return common::RESULT_INVALID_STATE;
    }
    // the criteria ask for as many sessions as the list takes
    u32 capacity = m_pMatchmakeSessions[m_CurrentIndex]->GetSessionInfoList()->GetCapacity();
    if (capacity == 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_pMatchmakeSessions[m_CurrentIndex]->GetSessionInfoList()->GetCapacity() != pCriteria->m_ResultNumMax) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE && m_CallContext.m_State != common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = m_pBrowseMatchmakeJob->Startup(&m_CallContext, m_pMatchmakeSessions[m_CurrentIndex], pCriteria);
    if (result.IsSuccess()) {
        m_pBrowseMatchmakeJob->Ready(false);
        m_AsyncType = ASYNC_TYPE_BROWSE;
    }
    return result;
}

// 0x0044B538 (name is ours)
nn::Result nn::pia::session::Session::CreateSessionAsync(const CreateSessionSetting* pSetting)
{
    if (!common::IsValidPointer(pSetting)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS || m_IsStarted != true || m_State != 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = m_pCreateSessionJob->Startup(&m_CallContext, pSetting);
    if (result.IsSuccess()) {
        m_pCreateSessionJob->Ready(false);
        m_AsyncType = ASYNC_TYPE_CREATE;
    }
    return result;
}

// 0x0044B5E0 (name is ours)
u8 nn::pia::session::Session::GetJoinMeshJobPhase()
{
    return Mesh::s_pInstance->GetJoinMeshJobPhase();
}

// 0x0044B5F0 (name is ours)
void nn::pia::session::Session::NotifyStationEvent(EventType type, StationIndex stationIndex)
{
    Session* pSession = s_pInstance;
    if (pSession == nullptr) {
        return;
    }
    StationId stationId = GetStationIdOfIndex253();
    transport::Transport::s_pInstance->ConvertToStationId(&stationId, stationIndex);
    pSession->NotifyEvent(type, stationId);
}

// 0x0044B650 (name is ours)
void nn::pia::session::Session::RemoveFromStationIdList(const StationId& stationId)
{
    for (common::ObjList<StationId>::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End();
         pNode = common::ObjList<StationId>::Advance(pNode)) {
        if (pNode->m_Value == stationId) {
            m_StationIdList.Erase(&pNode->m_Value);
            return;
        }
    }
}

// 0x0044B6C8 (name is ours)
u32 nn::pia::session::Session::GetCurrentSessionId()
{
    if (s_pInstance == nullptr) {
        return 0;
    }
    return s_pInstance->m_SessionIds[s_pInstance->m_CurrentIndex];
}

// 0x0044B6EC (name is ours)
nn::Result nn::pia::session::Session::ModifyAttributeAsync(u32 index, u32 value)
{
    nn::Result result = ModifyAttributeAsyncCore(index, value, m_SessionIds[m_CurrentIndex], m_pMatchmakeSessions[m_CurrentIndex]);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_MODIFY_ATTRIBUTE;
        m_pModifyAttributeJob->Ready(false);
    }
    return result;
}

// 0x0044B738 (name is ours)
bool nn::pia::session::Session::GetUnknown0x100() const
{
    return m_Unknown0x100;
}

// 0x0044B744 (name is ours)
void nn::pia::session::Session::UpdateSessionOwner(u32 sessionId, u32 ownerPrincipalId)
{
    transport::StationIdTable::Entry entry;
    if (m_pJoinSessionJob != nullptr && m_pJoinSessionJob->IsRunning()) {
        m_pJoinSessionJob->vf_0x18(sessionId, ownerPrincipalId);
        return;
    }
    if (m_pAutoMatchmakeJob != nullptr && m_pAutoMatchmakeJob->IsRunning()) {
        m_pAutoMatchmakeJob->vf_0x20(sessionId, ownerPrincipalId);
        return;
    }
    if (m_pLeaveSessionJob != nullptr && m_pLeaveSessionJob->IsRunning()) {
        return;
    }
    if (m_DisconnectState == 2 || m_DisconnectState == 3) {
        return;
    }
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    if (pTable->Find(&entry, ownerPrincipalId).IsFailure()) {
        // the owner is not a station of the session
        ProcessHostMigrationJob* pMigrationJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
        if (pMigrationJob->m_IsRunning && pMigrationJob->m_IsWaitingGreeting) {
            pMigrationJob->vf_0x20();
        } else if (m_SessionIds[m_CurrentIndex] == sessionId) {
            JointSessionJob* pJointSessionJob = m_pJointSessionJob;
            if (pJointSessionJob == nullptr || pJointSessionJob->m_Phase == 0) {
                SetDisconnectedByError();
                return;
            }
            pJointSessionJob->m_IsFailed = true;
            pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
        }
    }
    if (m_pStationIdStatusTable == nullptr) {
        if (m_SessionIds[m_CurrentIndex] == sessionId && IsHost()) {
            NotifyEvent(EVENT_TYPE_HOST_CHANGED, m_HostStationId);
        }
        return;
    }
    if (m_SessionIds[m_CurrentIndex] == sessionId) {
        if (m_State == 4) {
            if (m_Unknown0xFC == sessionId) {
                if (IsHostOfBothSessions()) {
                    NotifyEvent(EVENT_TYPE_JOINT_HOST_CHANGED, GetJointHostStationId());
                }
                if (IsHost()) {
                    NotifyEvent(EVENT_TYPE_HOST_CHANGED, m_HostStationId);
                }
                return;
            }
            if (m_HostStationId.m_Low == ownerPrincipalId) {
                SetDisconnectedByError();
                return;
            }
            if (pTable->Find(&entry, m_HostStationId).IsSuccess()) {
                if (m_LocalStationId == m_HostStationId) {
                    SetDisconnectedByError();
                }
                return;
            }
            ProcessHostMigrationJob* pMigrationJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
            if (pMigrationJob->m_IsRunning && !pMigrationJob->m_IsWaitingMigrationFinish) {
                return;
            }
            m_HostStationId = StationId(ownerPrincipalId, 0);
            if (m_LocalStationId == StationId(ownerPrincipalId, 0)) {
                m_pMatchmakeSessions[m_CurrentIndex]->vf_0x74(sessionId);
            }
            NotifyEvent(EVENT_TYPE_HOST_CHANGED, m_HostStationId);
        } else if (m_State == 2) {
            if (IsHost()) {
                NotifyEvent(EVENT_TYPE_HOST_CHANGED, m_HostStationId);
                return;
            }
            if (m_HostStationId != m_LocalStationId) {
                return;
            }
            if (m_HostStationId.m_Low != ownerPrincipalId) {
                SetDisconnectedByError();
            }
        } else if (m_State == 3) {
            JointSessionJob* pJointSessionJob = m_pJointSessionJob;
            if (pJointSessionJob == nullptr || !pJointSessionJob->IsRunning()) {
                return;
            }
            if (m_HostStationId.m_Low != ownerPrincipalId && m_LocalStationId == m_HostStationId) {
                // the local station was the host
                pJointSessionJob->m_IsFailed = true;
                pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
            } else if (pJointSessionJob->m_Phase == 9) {
                u32 jointSessionId;
                if (!m_pStationIdStatusTable->GetSessionId(m_JointHostStationId, &jointSessionId)) {
                    jointSessionId = 0;
                }
                if (jointSessionId != sessionId &&
                    (pTable->Find(&entry, m_HostStationId).IsFailure() || !m_pStationIdStatusTable->IsValid(m_HostStationId))) {
                    StationId newHostStationId(ownerPrincipalId, 0);
                    if (m_LocalStationId == newHostStationId) {
                        m_pMatchmakeSessions[m_CurrentIndex]->vf_0x74(sessionId);
                    }
                    m_pJointSessionJob->vf_0x4C(newHostStationId);
                }
            }
            m_pJointSessionJob->vf_0x50(sessionId, ownerPrincipalId);
        }
        return;
    }
    // the other session of the joint session
    if (m_SessionIds[m_CurrentIndex == 0 ? 1 : 0] != sessionId) {
        return;
    }
    if (m_State == 4) {
        if (m_JointHostStationId.m_Low == ownerPrincipalId) {
            if (pTable->Find(&entry, ownerPrincipalId).IsFailure()) {
                return;
            }
            u32 jointSessionId = 0;
            m_Unknown0xFC = m_pStationIdStatusTable->GetSessionId(entry.m_StationId, &jointSessionId) ? jointSessionId : 0;
        } else if (m_JointHostStationId == m_LocalStationId) {
            JointSessionJob* pJointSessionJob = m_pJointSessionJob;
            if (pJointSessionJob == nullptr || pJointSessionJob->m_Phase == 0) {
                SetDisconnectedByError();
                return;
            }
            pJointSessionJob->m_IsFailed = true;
            pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
        }
        if (IsHostOfBothSessions()) {
            NotifyEvent(EVENT_TYPE_JOINT_HOST_CHANGED, GetJointHostStationId());
        }
    } else if (m_State == 3) {
        if (m_JointHostStationId.m_Low != ownerPrincipalId && m_JointHostStationId == m_LocalStationId) {
            JointSessionJob* pJointSessionJob = m_pJointSessionJob;
            if (pJointSessionJob == nullptr || pJointSessionJob->m_Phase == 0) {
                SetDisconnectedByError();
                return;
            }
            pJointSessionJob->m_IsFailed = true;
            pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
        }
        m_pJointSessionJob->vf_0x50(sessionId, ownerPrincipalId);
    }
}

// 0x0044BCF0 (name is ours)
nn::Result nn::pia::session::Session::AutoMatchmakeAsync(const CreateSessionSetting* pCreateSetting, const SessionSearchCriteria* pCriteria, u32 criteriaNum)
{
    if (m_pAutoMatchmakeJob == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pCreateSetting) || !common::IsValidPointer(pCriteria) || criteriaNum == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS || m_IsStarted != true || m_State != 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = m_pAutoMatchmakeJob->Startup(&m_CallContext, pCreateSetting, pCriteria, criteriaNum);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_AUTO_MATCHMAKE;
        m_pAutoMatchmakeJob->Ready(false);
    }
    return result;
}

// 0x0044BDC8 (name is ours)
nn::Result nn::pia::session::Session::OpenParticipationAsync()
{
    CommonMatchmakeSession* pSession = m_pMatchmakeSessions[m_CurrentIndex];
    u32 sessionId = m_SessionIds[m_CurrentIndex];
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS || m_IsStarted != true) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = m_pOpenParticipationJob->Startup(&m_CallContext, sessionId, pSession);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_OPEN_PARTICIPATION;
        m_pOpenParticipationJob->Ready(false);
    }
    return result;
}

// 0x0044BE58 (name is ours)
nn::Result nn::pia::session::Session::CloseParticipationAsync()
{
    CommonMatchmakeSession* pSession = m_pMatchmakeSessions[m_CurrentIndex];
    u32 sessionId = m_SessionIds[m_CurrentIndex];
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS || m_IsStarted != true) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = m_pCloseParticipationJob->Startup(&m_CallContext, sessionId, pSession);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_CLOSE_PARTICIPATION;
        m_pCloseParticipationJob->Ready(false);
    }
    return result;
}

// 0x0044BEE8 (name is ours)
void nn::pia::session::Session::NotifyEvent(EventType type, StationId stationId)
{
    if (type == EVENT_TYPE_3 || type == EVENT_TYPE_HOST_CHANGED) {
        // not while a session is joined or left
        if (IsJoining() || m_pLeaveSessionJob->IsRunning() || m_DisconnectState == 2 || m_DisconnectState == 3) {
            return;
        }
    }
    if (common::IsValidPointer(reinterpret_cast<void*>(m_EventCallback))) {
        m_EventCallback(type, stationId);
    }
}

// 0x0044BF94 (name is ours)
nn::Result nn::pia::session::Session::ModifyAttributeAsyncCore(u32 index, u32 value, u32 sessionId, CommonMatchmakeSession* pSession)
{
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS || m_IsStarted != true || m_pModifyAttributeJob->IsRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE && m_CallContext.m_State != common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    if (m_pModifyAttributeJob == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    return m_pModifyAttributeJob->Startup(&m_CallContext, sessionId, index, value, pSession);
}

// 0x0044C04C (name is ours)
void nn::pia::session::Session::SetString(const u16* pString, u32 length)
{
    u16* pDst = m_pString;
    for (u32 i = 0; i < length; i++) {
        *pDst++ = *pString++;
    }
    *pDst = 0;
}

// 0x0044C0A8 (name is ours)
nn::pia::session::ISessionInfoList* nn::pia::session::Session::GetSessionInfoList() const
{
    return m_pMatchmakeSessions[m_CurrentIndex]->GetSessionInfoList();
}

// 0x0044C0C0 (name is ours)
u8* nn::pia::session::Session::GetUnknown0x101()
{
    return m_Unknown0x101;
}

// 0x0044C0CC (name is ours)
void nn::pia::session::Session::SetSyncClockRequestInterval(s32 intervalMSec)
{
    if (common::IsValidPointer(Mesh::s_pInstance)) {
        Mesh::s_pInstance->SetSyncClockRequestInterval(intervalMSec);
    }
}

// 0x0044C0F4 (name is ours)
void nn::pia::session::Session::ClearString()
{
    for (u32 i = 0; i < m_StringLength; i++) {
        m_pString[i] = 0;
    }
}

// 0x0044C134 (name is ours)
void nn::pia::session::Session::CleanupConfigParticipationJob()
{
    m_pConfigParticipationJob->Cleanup();
}

// 0x0044C144 (name is ours)
void nn::pia::session::Session::SetupStationIdsAsHost()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (s_pInstance->IsUsingStationIdTable()) {
        u32 principalId =
            transport::StationConnectionInfoTable::s_pInstance->GetPrincipalIdByStation(transport::StationManager::s_pInstance->m_pLocalStation);
        m_LocalStationId = StationId(principalId, 0);
    } else {
        m_LocalStationId = StationId(pMesh->m_LocalStationIndex, 0);
    }
    m_HostStationId = m_LocalStationId;
}

// 0x0044C1CC (name is ours)
void nn::pia::session::Session::StopSessionStatusCheck()
{
    common::ExecuteResult result = m_pStationIdStatusTable != nullptr ? m_pSessionStatusCheckJob->CheckSessionStatus4JointSession()
                                                                     : m_pSessionStatusCheckJob->CheckSessionStatus();
    if (result.m_State != common::ExecuteResult::STATE_NEXT_DISPATCH) {
        m_pSessionStatusCheckJob->Reset(false);
    }
}

// 0x0044C21C (name is ours)
u8 nn::pia::session::Session::GetAutoMatchmakePhase() const
{
    u8 phase = m_pAutoMatchmakeJob->GetPhase();
    if (phase != 2) {
        return phase;
    }
    if (Mesh::s_pInstance->GetJoinMeshJobPhase() == 0) {
        return 2;
    }
    if (Mesh::s_pInstance->GetJoinMeshJobPhase() == 1) {
        return 4;
    }
    if (Mesh::s_pInstance->GetJoinMeshJobPhase() == 2) {
        return 5;
    }
    if (Mesh::s_pInstance->GetJoinMeshJobPhase() == 3) {
        return 6;
    }
    return 7;
}

// 0x0044C294 (name is ours)
nn::Result nn::pia::session::Session::SetEventCallback(EventCallback callback)
{
    if (m_EventCallback != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    m_EventCallback = callback;
    return nn::Result();
}

// 0x0044C2B0 (name is ours)
void nn::pia::session::Session::SetUnknownFlagOfJoiningJobs()
{
    if (m_pAutoMatchmakeJob != nullptr && m_pAutoMatchmakeJob->IsRunning()) {
        m_pAutoMatchmakeJob->m_IsHostLeft = true;
    }
    if (m_pJoinSessionJob != nullptr && m_pJoinSessionJob->IsRunning()) {
        m_pJoinSessionJob->m_IsHostLeft = true;
    }
}

// 0x0044C2F8 (name is ours)
nn::pia::session::Session::StationIdCallback nn::pia::session::Session::GetStationIdCallback() const
{
    return IsJointSessionStationCallback;
}

// 0x0044C304 (name is ours)
bool nn::pia::session::Session::SetupStationIdsAsClient()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (m_pStationIdStatusTable == nullptr) {
        m_LocalStationId = StationId(pMesh->m_LocalStationIndex, 0);
        m_HostStationId = StationId(Mesh::s_pInstance->m_HostStationIndex, 0);
        return true;
    }
    transport::StationConnectionInfoTable* pInfoTable = transport::StationConnectionInfoTable::s_pInstance;
    m_LocalStationId = StationId(pInfoTable->GetPrincipalIdByStation(transport::StationManager::s_pInstance->m_pLocalStation), 0);
    u32 hostPrincipalId = transport::StationConnectionInfoTable::s_pInstance->GetPrincipalIdByStation(
        transport::StationManager::s_pInstance->GetStation(Mesh::s_pInstance->m_HostStationIndex));
    m_Unknown0xFC = 0;
    if (m_pStationIdStatusTable == nullptr || m_State != 4) {
        m_HostStationId = StationId(hostPrincipalId, 0);
        return true;
    }
    // a joint session: the host of the mesh hosts the other session, the owner of this one is
    // the host
    m_JointHostStationId = StationId(hostPrincipalId, 0);
    m_HostStationId = StationId(m_pMatchmakeSessions[m_CurrentIndex]->vf_0x90(), 0);
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, m_HostStationId).IsFailure()) {
        return false;
    }
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, m_JointHostStationId).IsFailure()) {
        return false;
    }
    u32 sessionId;
    m_Unknown0xFC = s_pInstance->m_pStationIdStatusTable->GetSessionId(entry.m_StationId, &sessionId) ? sessionId : 0;
    return true;
}

// 0x0044C478 (name is ours)
bool nn::pia::session::Session::CheckJoinApproval(const transport::Station::IdentificationInfo* pInfo)
{
    JoinApprovalCallback callback = s_pInstance->m_JoinApprovalCallback;
    if (!common::IsValidPointer(reinterpret_cast<void*>(callback))) {
        return true;
    }
    return callback(pInfo);
}

// 0x0044C4A8 (name is ours)
void nn::pia::session::Session::ClearEventCallback()
{
    m_EventCallback = nullptr;
}

// 0x0044C4B4 (name is ours)
void nn::pia::session::Session::SetTransportState(bool isAvailable)
{
    if (isAvailable) {
        transport::Transport::s_pInstance->SetState(nn::Result());
    } else {
        transport::Transport::s_pInstance->SetState(common::RESULT_INVALID_STATE_TEMPORARY);
    }
}

// 0x0044C4DC (name is ours)
bool nn::pia::session::Session::ClearStationBit(u32 stationIndex)
{
    if (stationIndex > STATION_INDEX_MAX) {
        return false;
    }
    u32 bit = 1;
    for (u32 i = 0; i < stationIndex; i++) {
        bit *= 2;
    }
    m_StationBitmap &= ~bit;
    return true;
}

// 0x0044C524 (name is ours)
void nn::pia::session::Session::SetDisconnected()
{
    m_DisconnectState = 3;
    m_HostStationId = GetStationIdOfIndex253();
    m_JointHostStationId = GetStationIdOfIndex253();
    StopSessionStatusCheck();
}

// 0x0044C5AC (name is ours)
void nn::pia::session::Session::SetDisconnectedByError()
{
    m_DisconnectState = 2;
    m_HostStationId = GetStationIdOfIndex253();
    m_JointHostStationId = GetStationIdOfIndex253();
    StopSessionStatusCheck();
}

// 0x0044C634 (name is ours)
u32 nn::pia::session::Session::GetHostCandidatePriority(StationIndex stationIndex, bool isFromConnectionInfo)
{
    Session* pSession = s_pInstance;
    if (pSession->m_pStationIdStatusTable == nullptr || !pSession->m_IsHostMigrationEnabled) {
        return 0;
    }
    if (pSession->m_State == 4) {
    } else if (pSession->m_State == 3) {
        if (!pSession->m_pJointSessionJob->m_Unknown0x9F) {
            return 0;
        }
    } else {
        return 0;
    }
    // (two calls were removed by the linker here)
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure()) {
        return 253;
    }
    u32 sessionId = 0;
    if (!s_pInstance->m_pStationIdStatusTable->GetSessionId(entry.m_StationId, &sessionId)) {
        return 252;
    }
    if (s_pInstance->m_Unknown0xFC == sessionId) {
        return 0;
    }
    // the stations of the host's session first
    u8 isHost = false;
    u8 rank = s_pInstance->m_pStationIdStatusTable->GetSessionRank(sessionId);
    if (isFromConnectionInfo) {
        s_pInstance->m_pStationIdStatusTable->GetUnknown0x10(entry.m_StationId, &isHost);
    }
    if (entry.m_StationId == s_pInstance->m_HostStationId) {
        isHost = true;
    }
    return static_cast<u8>(isHost ? rank * 4 : 1 + rank * 4);
}

// 0x0044C79C (name is ours)
void nn::pia::session::Session::ClearJoinApprovalCallback()
{
    m_JoinApprovalCallback = nullptr;
}

// 0x0044C7A8 (name is ours)
nn::pia::session::CommonMatchmakeSession* nn::pia::session::Session::GetJoinedMatchmakeSession() const
{
    if (m_State == 4) {
        return m_pMatchmakeSessions[m_CurrentIndex == 0 ? 1 : 0];
    }
    return m_pMatchmakeSessions[m_CurrentIndex];
}

// 0x0044C7DC (name is ours)
void nn::pia::session::Session::Cleanup()
{
    m_pMatchmakeSessions[0]->Cleanup();
    if (m_pMatchmakeSessions[1] != nullptr) {
        m_pMatchmakeSessions[1]->Cleanup();
    }
    if (m_pCreateSessionJob != nullptr) {
        m_pCreateSessionJob->Reset(false);
        m_pCreateSessionJob->Cleanup();
    }
    if (m_pAutoMatchmakeJob != nullptr) {
        m_pAutoMatchmakeJob->Reset(false);
        m_pAutoMatchmakeJob->Cleanup();
    }
    if (m_pBrowseMatchmakeJob != nullptr) {
        m_pBrowseMatchmakeJob->Reset(false);
        m_pBrowseMatchmakeJob->Cleanup();
    }
    if (m_pJoinSessionJob != nullptr) {
        m_pJoinSessionJob->Reset(false);
        m_pJoinSessionJob->Cleanup();
    }
    if (m_pLeaveSessionJob != nullptr) {
        m_pLeaveSessionJob->Reset(false);
        m_pLeaveSessionJob->Cleanup();
    }
    if (m_pDestroySessionJob != nullptr) {
        m_pDestroySessionJob->Reset(false);
        m_pDestroySessionJob->Cleanup();
    }
    if (m_pOpenParticipationJob != nullptr) {
        m_pOpenParticipationJob->Reset(false);
        m_pOpenParticipationJob->Cleanup();
    }
    if (m_pCloseParticipationJob != nullptr) {
        m_pCloseParticipationJob->Reset(false);
        m_pCloseParticipationJob->Cleanup();
    }
    if (m_pConfigParticipationJob != nullptr) {
        m_pConfigParticipationJob->Reset(false);
        m_pConfigParticipationJob->Cleanup();
    }
    if (m_pGenerateMatchmakeSystemPasswordJob != nullptr) {
        m_pGenerateMatchmakeSystemPasswordJob->Reset(false);
        m_pGenerateMatchmakeSystemPasswordJob->Cleanup();
    }
    if (m_pClearMatchmakeSystemPasswordJob != nullptr) {
        m_pClearMatchmakeSystemPasswordJob->Reset(false);
        m_pClearMatchmakeSystemPasswordJob->Cleanup();
    }
    if (m_pModifyAttributeJob != nullptr) {
        m_pModifyAttributeJob->Reset(false);
        m_pModifyAttributeJob->Cleanup();
    }
    if (m_pUpdateSessionSettingJob != nullptr) {
        m_pUpdateSessionSettingJob->Reset(false);
        m_pUpdateSessionSettingJob->Cleanup();
    }
    if (m_pUpdateApplicationDataJob != nullptr) {
        m_pUpdateApplicationDataJob->Reset(false);
        m_pUpdateApplicationDataJob->Cleanup();
    }
    if (m_pJointSessionJob != nullptr) {
        m_pJointSessionJob->Reset(false);
        m_pJointSessionJob->Cleanup();
    }
    m_pMeshLayerController->Cleanup();
    ClearStatus(true);
    transport::Transport::s_pInstance->SetState(nn::Result());
    if (m_pSessionStatusCheckJob != nullptr) {
        m_pSessionStatusCheckJob->Reset(false);
        m_pSessionStatusCheckJob->Cleanup();
    }
    m_LocalStationId = GetStationIdOfIndex253();
    m_HostStationId = GetStationIdOfIndex253();
    m_JointHostStationId = GetStationIdOfIndex253();
    m_IsStarted = false;
    m_CallContext.Reset();
    m_AsyncType = ASYNC_TYPE_NONE;
    m_StationBitmap = 0;
    m_Unknown0xB0 = false;
    for (u32 i = 0; i < 4; i++) {
        m_UnjoinableSessionIds[i] = 0;
    }
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_Unknown0x16F = 0xFF;
    content.m_Unknown0x170 = 0xFFFFFFFF;
    content.m_Unknown0x174 = 0xFFFFFFFF;
    content.m_Unknown0x178 = 0xFFFFFFFF;
    content.m_Unknown0x17C = 0xFF;
    content.m_Unknown0x17D = 0xFF;
    content.m_Unknown0x25C = 0xFF;
    content.m_Unknown0x25D = 0xFF;
    content.m_Unknown0x25E = 0xFF;
    content.m_Unknown0x25F = 0xFF;
    content.m_JoinPhase = 0xFF;
    content.m_Unknown0x22B = 0xFF;
    content.m_Unknown0x24F = 0xFF;
    content.m_Unknown0x250 = 0xFF;
    content.m_Unknown0x251 = 0xFF;
    content.m_Unknown0x252 = 0xFF;
    content.m_Unknown0x253 = 0xFF;
    content.m_Unknown0x270 = 0xFFFF;
    content.m_Unknown0x272 = 0xFFFF;
    common::g_SessionBeginMonitoringContent.Cleanup();
    common::g_SessionStateMonitoringContent.Cleanup();
    Mesh::s_pInstance->SetUnknown0xA5(false);
}

// 0x0044CBD8 (name is ours)
nn::Result nn::pia::session::Session::Startup(const StartupSetting& setting)
{
    if (m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    m_IsHostMigrationEnabled = setting.m_IsHostMigrationEnabled;
    if (!s_GlobalSetting.m_IsTimeoutFree && setting.m_TimeoutMSec - 1000 > 29000) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::Result result = m_pMeshLayerController->Startup(
        setting.m_IsHostMigrationEnabled, setting.m_pIdentificationData, s_pInstance->m_Unknown0x5 ? setting.m_CryptoMode : common::Crypto::MODE_NONE,
        setting.m_TimeoutMSec, setting.m_KeepAliveIntervalMSec, setting.m_BandwidthCheckBandwidth, setting.m_BandwidthCheckPacketSize,
        setting.m_IsBandwidthCheckOneWay, setting.m_BandwidthCheckDurationMSec, setting.m_pPlayerName, m_Unknown0x134);
    if (result.IsFailure()) {
        return result;
    }
    m_IsStarted = true;
    result = m_pSessionStatusCheckJob->Startup();
    if (result.IsFailure()) {
        return result;
    }
    m_pSessionStatusCheckJob->Ready(false);
    m_DisconnectState = 0;
    m_StationIdList.ClearNodes();
    Mesh::s_pInstance->m_Unknown0xA5 = true;
    return nn::Result();
}

// 0x0044CD34 (name is ours)
nn::pia::session::Session::Session(Setting)
    : m_IsHostMigrationEnabled(false), m_Unknown0x5(false), m_pMeshLayerController(nullptr), m_pSessionStatusCheckJob(nullptr),
      m_pCreateSessionJob(nullptr), m_pAutoMatchmakeJob(nullptr), m_pBrowseMatchmakeJob(nullptr), m_pJoinSessionJob(nullptr),
      m_pLeaveSessionJob(nullptr), m_pDestroySessionJob(nullptr), m_pOpenParticipationJob(nullptr), m_pCloseParticipationJob(nullptr),
      m_pConfigParticipationJob(nullptr), m_pGenerateMatchmakeSystemPasswordJob(nullptr), m_pClearMatchmakeSystemPasswordJob(nullptr),
      m_StringLength(0), m_pString(nullptr), m_pJointSessionJob(nullptr), m_pModifyAttributeJob(nullptr), m_pUpdateSessionSettingJob(nullptr),
      m_pUpdateApplicationDataJob(nullptr), m_EventCallback(nullptr), m_JoinApprovalCallback(nullptr), m_pStationIdStatusTable(nullptr),
      m_SessionProtocolId(0, 0), m_pSessionProtocol(nullptr), m_IsStarted(false), m_State(0), m_DisconnectState(0),
      m_LocalStationId(GetStationIdOfIndex253()), m_HostStationId(GetStationIdOfIndex253()), m_JointHostStationId(GetStationIdOfIndex253()),
      m_Unknown0x8C(0), m_AsyncType(ASYNC_TYPE_NONE), m_pMeshEventListener(nullptr), m_StationBitmap(0), m_Unknown0xB0(false),
      m_Unknown0xB1(0), m_Unknown0xB2(false), m_CurrentIndex(0), m_pSessionInfoList(nullptr), m_pStationIdNodeBuffer(nullptr),
      m_Unknown0x100(false)
{
    m_SessionIds[0] = 0;
    m_SessionIds[1] = 0;
    m_StationIdEntryNumMax[0] = 0;
    m_StationIdEntryNumMax[1] = 0;
    memset(m_Unknown0x101, 0, sizeof(m_Unknown0x101));
    m_Unknown0x134 = false;
    for (u32 i = 0; i < 4; i++) {
        m_UnjoinableSessionIds[i] = 0;
    }
}

// 0x0044CE90 (name is ours)
nn::pia::session::Session::~Session()
{
    if (m_pSessionStatusCheckJob != nullptr) {
        delete m_pSessionStatusCheckJob;
        m_pSessionStatusCheckJob = nullptr;
    }
    if (m_pCreateSessionJob != nullptr) {
        delete m_pCreateSessionJob;
        m_pCreateSessionJob = nullptr;
    }
    if (m_pAutoMatchmakeJob != nullptr) {
        delete m_pAutoMatchmakeJob;
        m_pAutoMatchmakeJob = nullptr;
    }
    if (m_pBrowseMatchmakeJob != nullptr) {
        delete m_pBrowseMatchmakeJob;
        m_pBrowseMatchmakeJob = nullptr;
    }
    if (m_pJoinSessionJob != nullptr) {
        delete m_pJoinSessionJob;
        m_pJoinSessionJob = nullptr;
    }
    if (m_pLeaveSessionJob != nullptr) {
        delete m_pLeaveSessionJob;
        m_pLeaveSessionJob = nullptr;
    }
    if (m_pDestroySessionJob != nullptr) {
        delete m_pDestroySessionJob;
        m_pDestroySessionJob = nullptr;
    }
    if (m_pOpenParticipationJob != nullptr) {
        delete m_pOpenParticipationJob;
        m_pOpenParticipationJob = nullptr;
    }
    if (m_pCloseParticipationJob != nullptr) {
        delete m_pCloseParticipationJob;
        m_pCloseParticipationJob = nullptr;
    }
    if (m_pConfigParticipationJob != nullptr) {
        delete m_pConfigParticipationJob;
        m_pConfigParticipationJob = nullptr;
    }
    if (m_pGenerateMatchmakeSystemPasswordJob != nullptr) {
        delete m_pGenerateMatchmakeSystemPasswordJob;
        m_pGenerateMatchmakeSystemPasswordJob = nullptr;
    }
    if (m_pClearMatchmakeSystemPasswordJob != nullptr) {
        delete m_pClearMatchmakeSystemPasswordJob;
        m_pClearMatchmakeSystemPasswordJob = nullptr;
    }
    if (m_pString != nullptr) {
        common::DeleteArray(m_pString);
    }
    m_pString = nullptr;
    if (m_pMatchmakeSessions[0] != nullptr) {
        delete m_pMatchmakeSessions[0];
        m_pMatchmakeSessions[0] = nullptr;
    }
    if (m_pMatchmakeSessions[1] != nullptr) {
        delete m_pMatchmakeSessions[1];
        m_pMatchmakeSessions[1] = nullptr;
    }
    if (m_pSessionInfoList != nullptr) {
        delete m_pSessionInfoList;
        m_pSessionInfoList = nullptr;
    }
    if (m_pJointSessionJob != nullptr) {
        delete m_pJointSessionJob;
        m_pJointSessionJob = nullptr;
    }
    if (m_pModifyAttributeJob != nullptr) {
        delete m_pModifyAttributeJob;
        m_pModifyAttributeJob = nullptr;
    }
    if (m_pUpdateSessionSettingJob != nullptr) {
        delete m_pUpdateSessionSettingJob;
        m_pUpdateSessionSettingJob = nullptr;
    }
    if (m_pUpdateApplicationDataJob != nullptr) {
        delete m_pUpdateApplicationDataJob;
        m_pUpdateApplicationDataJob = nullptr;
    }
    if (m_pMeshLayerController != nullptr) {
        delete m_pMeshLayerController;
        m_pMeshLayerController = nullptr;
    }
    if (m_pStationIdStatusTable != nullptr) {
        delete m_pStationIdStatusTable;
        m_pStationIdStatusTable = nullptr;
    }
    if (m_pMeshEventListener != nullptr) {
        delete m_pMeshEventListener;
        m_pMeshEventListener = nullptr;
    }
    if (m_pStationIdNodeBuffer != nullptr) {
        m_StationIdList.Clear();
        if (m_pStationIdNodeBuffer != nullptr) {
            pead::FreeMemory(m_pStationIdNodeBuffer);
        }
        m_pStationIdNodeBuffer = nullptr;
    }
    if (m_pSessionProtocol != nullptr) {
        m_pSessionProtocol->Finalize();
        m_pSessionProtocol = nullptr;
    }
    if (m_SessionProtocolId.m_Id != 0) {
        if (transport::Transport::s_pInstance != nullptr) {
            transport::Transport::s_pInstance->m_ProtocolManager.DestroyProtocol(m_SessionProtocolId.m_Id);
        }
        m_SessionProtocolId = transport::ProtocolId(0, 0);
    }
}

// 0x007343DC (name is ours)
u16 nn::pia::session::Session::GetStationNum() const
{
    if (m_pStationIdStatusTable != nullptr) {
        return m_StationIdList.GetCount();
    }
    if (common::IsValidPointer(Mesh::s_pInstance)) {
        return Mesh::s_pInstance->m_StationNum;
    }
    return 0;
}

// 0x00734418 (name is ours)
bool nn::pia::session::Session::IsJoinable(u32 sessionId) const
{
    if (sessionId != 0) {
        for (u32 i = 0; i < 4; i++) {
            if (m_UnjoinableSessionIds[i] == sessionId) {
                return false;
            }
        }
        return true;
    }
    if (m_State == 3) {
        return true;
    }
    u32 otherSessionId = m_SessionIds[m_CurrentIndex == 0 ? 1 : 0];
    if (otherSessionId != 0) {
        return IsJoinable(otherSessionId);
    }
    u32 currentSessionId = m_SessionIds[m_CurrentIndex];
    if (currentSessionId != 0) {
        return IsJoinable(currentSessionId);
    }
    return true;
}

// 0x007344B0 (name is ours)
bool nn::pia::session::Session::IsJoining() const
{
    bool isJoining = false;
    if (m_pAutoMatchmakeJob != nullptr) {
        isJoining = m_pAutoMatchmakeJob->IsRunning();
    }
    if (m_pJoinSessionJob != nullptr) {
        isJoining |= m_pJoinSessionJob->IsRunning();
    }
    return isJoining;
}

// 0x007344EC (name is ours)
u32 nn::pia::session::Session::GetJointSessionId() const
{
    if (m_pStationIdStatusTable == nullptr || m_State != 4) {
        return 0;
    }
    return m_SessionIds[m_CurrentIndex == 0 ? 1 : 0];
}

// 0x00734528 (name is ours)
bool nn::pia::session::Session::IsHostOfBothSessions() const
{
    if (m_pStationIdStatusTable == nullptr) {
        return false;
    }
    if (m_LocalStationId == GetStationIdOfIndex253() || m_LocalStationId != m_HostStationId) {
        return false;
    }
    if (!m_pMatchmakeSessions[m_CurrentIndex]->vf_0x88()) {
        return false;
    }
    if (m_LocalStationId != m_JointHostStationId) {
        return false;
    }
    return m_pMatchmakeSessions[m_CurrentIndex == 0 ? 1 : 0]->vf_0x88();
}

// 0x00734604 (name is ours)
nn::Result nn::pia::session::Session::CheckStatus() const
{
    switch (GetStatus()) {
    case STATUS_NONE:
    case STATUS_1:
    case STATUS_2:
    case STATUS_JOINT:
        return nn::Result();
    case STATUS_DISCONNECTED_4:
        return common::RESULT_SESSION_DISCONNECTED;
    default:
        return common::RESULT_NOT_IN_SESSION;
    }
}

// 0x00734684 (name is ours)
bool nn::pia::session::Session::IsValidStation(StationId stationId) const
{
    if (m_pStationIdStatusTable != nullptr) {
        return m_pStationIdStatusTable->IsValid(stationId);
    }
    if (!common::IsValidPointer(Mesh::s_pInstance)) {
        return false;
    }
    StationIndex stationIndex;
    if (transport::Transport::s_pInstance->ConvertToStationIndex(&stationIndex, stationId).IsFailure()) {
        return false;
    }
    return Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex);
}

// 0x0073470C (name is ours)
nn::Result nn::pia::session::Session::GetJoinSessionAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_JOIN);
}

// 0x00734714 (name is ours)
nn::Result nn::pia::session::Session::GetAsyncResult(AsyncType type) const
{
    if (m_AsyncType != type || !m_CallContext.IsFinished()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_CallContext.m_Result;
}

// 0x00734744 (name is ours)
nn::Result nn::pia::session::Session::GetLeaveSessionAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_LEAVE);
}

// 0x0073474C | fefates:bytes
bool nn::pia::session::Session::IsUsingStationIdTable() const
{
    return m_pStationIdStatusTable != nullptr;
}

// 0x0073475C (name is ours)
nn::Result nn::pia::session::Session::GetBrowseSessionAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_BROWSE);
}

// 0x00734764 (name is ours)
nn::Result nn::pia::session::Session::GetCreateSessionAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_CREATE);
}

// 0x0073476C (name is ours)
bool nn::pia::session::Session::IsJoinSessionAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_JOIN && m_CallContext.IsFinished();
}

// 0x0073479C (name is ours)
bool nn::pia::session::Session::IsLeaveSessionAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_LEAVE && m_CallContext.IsFinished();
}

// 0x007347CC (name is ours)
nn::Result nn::pia::session::Session::GetModifyAttributeAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_MODIFY_ATTRIBUTE);
}

// 0x007347D4 (name is ours)
bool nn::pia::session::Session::IsBrowseSessionAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_BROWSE && m_CallContext.IsFinished();
}

// 0x00734804 (name is ours)
bool nn::pia::session::Session::IsCreateSessionAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_CREATE && m_CallContext.IsFinished();
}

// 0x00734834 (name is ours)
nn::Result nn::pia::session::Session::GetAutoMatchmakeAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_AUTO_MATCHMAKE);
}

// 0x0073483C (name is ours)
nn::Result nn::pia::session::Session::GetOpenParticipationAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_OPEN_PARTICIPATION);
}

// 0x00734844 (name is ours)
bool nn::pia::session::Session::IsModifyAttributeAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_MODIFY_ATTRIBUTE && m_CallContext.IsFinished();
}

// 0x00734874 (name is ours)
nn::Result nn::pia::session::Session::GetCloseParticipationAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_CLOSE_PARTICIPATION);
}

// 0x0073487C (name is ours)
nn::pia::StationId nn::pia::session::Session::GetJointHostStationId() const
{
    if (m_pStationIdStatusTable != nullptr) {
        return m_JointHostStationId;
    }
    return GetStationIdOfIndex253();
}

// 0x007348A8 (name is ours)
bool nn::pia::session::Session::IsAutoMatchmakeAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_AUTO_MATCHMAKE && m_CallContext.IsFinished();
}

// 0x007348D8 (name is ours)
bool nn::pia::session::Session::IsOpenParticipationAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_OPEN_PARTICIPATION && m_CallContext.IsFinished();
}

// 0x00734908 (name is ours)
bool nn::pia::session::Session::IsCloseParticipationAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_CLOSE_PARTICIPATION && m_CallContext.IsFinished();
}

// 0x00734938 (name is ours)
u32 nn::pia::session::Session::GetMeshLayerControllerValue() const
{
    if (!common::IsValidPointer(m_pMeshLayerController)) {
        return 0;
    }
    return m_pMeshLayerController->vf_0x28();
}

// 0x00734964 (name is ours)
u32 nn::pia::session::Session::GetJoinedSessionId() const
{
    u32 sessionId = m_SessionIds[m_CurrentIndex];
    if (m_State == 4 && m_pStationIdStatusTable != nullptr) {
        sessionId = m_SessionIds[m_CurrentIndex == 0 ? 1 : 0];
    }
    return sessionId;
}

// 0x007349A4
void nn::pia::session::Session::Trace(u64) const
{
    // empty (in the original too)
}

// 0x007349A8 (name is ours)
bool nn::pia::session::Session::IsHost() const
{
    if (m_pStationIdStatusTable != nullptr) {
        if (m_LocalStationId == GetStationIdOfIndex253() || m_LocalStationId != m_HostStationId) {
            return false;
        }
    } else {
        Mesh* pMesh = Mesh::s_pInstance;
        if (!common::IsValidPointer(pMesh) || pMesh->m_LocalStationIndex > STATION_INDEX_MAX || pMesh->m_LocalStationIndex != pMesh->m_HostStationIndex) {
            return false;
        }
    }
    return m_pMatchmakeSessions[m_CurrentIndex]->vf_0x88();
}

// 0x00734AB8 (name is ours)
nn::pia::session::Session::Status nn::pia::session::Session::GetStatus() const
{
    if (m_DisconnectState == 2) {
        return STATUS_DISCONNECTED_4;
    }
    if (m_DisconnectState == 3) {
        return STATUS_DISCONNECTED_5;
    }
    if (m_State == 2) {
        return STATUS_1;
    }
    if (m_State == 3) {
        return STATUS_2;
    }
    if (m_State == 4) {
        return STATUS_JOINT;
    }
    return STATUS_NONE;
}

} // namespace session
} // namespace pia
} // namespace nn
