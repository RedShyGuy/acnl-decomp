#include "nn/pia/session/session_MeshEventListenerForSession.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_AutoMatchmakeJob.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_ConfigParticipationJobBase.h"
#include "nn/pia/session/session_JoinSessionJob.h"
#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/session/session_LeaveSessionJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_ProcessDestroyMeshJob.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// the session id in the identification info of a station, 0 without one (name is ours)
inline u32 GetSessionIdOfStation(const transport::Station* pStation)
{
    transport::Station::IdentificationInfo info;
    return transport::IdentificationInfoTable::s_pInstance->GetIdentificationInfo(pStation, &info).IsSuccess() ? info.m_Unknown0x48 : 0;
}
} // namespace

// 0x004463CC
void nn::pia::session::MeshEventListenerForSession::OnEvent(const nn::pia::session::Mesh::Event& event)
{
    Session* pSession = Session::s_pInstance;
    if (pSession->IsUsingStationIdTable()) {
        OnEventWithStationIdTable(event);
    } else {
        OnEventWithoutStationIdTable(event);
    }
    // the matchmake session checks its status after a host change (SessionStatusCheckJob)
    if (Session::s_GlobalSetting.m_Unknown0x1 && event.m_Type == Mesh::EVENT_TYPE_HOST_CHANGED) {
        pSession->m_Unknown0x100 = true;
    }
    // the joining jobs note some events
    if (pSession->m_pJoinSessionJob != nullptr && pSession->m_pJoinSessionJob->IsRunning()) {
        if (event.m_Type == Mesh::EVENT_TYPE_19) {
            pSession->m_pJoinSessionJob->m_IsMeshEvent19 = true;
        }
        if (event.m_Type == Mesh::EVENT_TYPE_20) {
            pSession->m_pJoinSessionJob->m_IsMeshEvent20 = true;
        }
        if (event.m_Type == Mesh::EVENT_TYPE_CONNECTION_FAILED) {
            pSession->m_pJoinSessionJob->m_IsConnectionFailed = true;
        }
    }
    if (pSession->m_pAutoMatchmakeJob != nullptr && pSession->m_pAutoMatchmakeJob->IsRunning()) {
        if (event.m_Type == Mesh::EVENT_TYPE_19) {
            pSession->m_pAutoMatchmakeJob->m_IsMeshEvent19 = true;
        }
        if (event.m_Type == Mesh::EVENT_TYPE_20) {
            pSession->m_pAutoMatchmakeJob->m_IsMeshEvent20 = true;
        }
        if (event.m_Type == Mesh::EVENT_TYPE_CONNECTION_FAILED) {
            pSession->m_pAutoMatchmakeJob->m_IsConnectionFailed = true;
        }
    }
    if (pSession->m_pJointSessionJob != nullptr && pSession->m_pJointSessionJob->IsRunning()) {
        if (event.m_Type == Mesh::EVENT_TYPE_19) {
            pSession->m_pJointSessionJob->m_IsMeshEvent19 = true;
        }
        if (event.m_Type == Mesh::EVENT_TYPE_20) {
            pSession->m_pJointSessionJob->m_IsMeshEvent20 = true;
        }
    }
}

// 0x00446514 (name is ours)
void nn::pia::session::MeshEventListenerForSession::RemoveLeftStation(nn::pia::StationId stationId)
{
    Session* pSession = Session::s_pInstance;
    if (pSession->m_pStationIdStatusTable->IsNotified(stationId)) {
        pSession->NotifyEvent(Session::EVENT_TYPE_LEAVE, stationId);
        pSession->RemoveFromStationIdList(stationId);
    }
    transport::Transport::s_pInstance->m_pStationIdTable->Remove(stationId);
    pSession->m_pStationIdStatusTable->Remove(stationId);
}

// 0x0044659C (name is ours)
void nn::pia::session::MeshEventListenerForSession::OnEventWithoutStationIdTable(const nn::pia::session::Mesh::Event& event)
{
    Session* pSession = Session::s_pInstance;
    // the station id is the index
    StationId stationId;
    switch (event.m_StationIndex) {
    case STATION_INDEX_UNIDENTIFIED:
        stationId = GetStationIdOfIndex253();
        break;
    case 254:
        stationId = GetStationIdOfIndex254();
        break;
    case 255:
        stationId = GetStationIdOfIndex255();
        break;
    default:
        stationId.m_Low = event.m_StationIndex;
        break;
    }
    switch (event.m_Type) {
    case Mesh::EVENT_TYPE_JOIN: {
        transport::Transport::s_pInstance->m_pStationIdTable->Add(stationId, event.m_StationIndex, event.m_StationIndex);
        pSession->AddToStationIdList(stationId);
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(event.m_StationIndex);
        if (common::IsValidPointer(pStation)) {
            pStation->SetStationId(stationId);
        }
        if (event.m_StationIndex == Mesh::s_pInstance->m_LocalStationIndex) {
            pSession->m_LocalStationId = stationId;
            pSession->m_HostStationId = StationId(Mesh::s_pInstance->m_HostStationIndex, 0);
        }
        if (event.m_StationIndex == Mesh::s_pInstance->m_HostStationIndex) {
            pSession->m_HostStationId = stationId;
        }
        pSession->NotifyEvent(Session::EVENT_TYPE_JOIN, stationId);
        break;
    }
    case Mesh::EVENT_TYPE_LEAVE:
        pSession->NotifyEvent(Session::EVENT_TYPE_LEAVE, stationId);
        transport::Transport::s_pInstance->m_pStationIdTable->Remove(event.m_StationIndex);
        pSession->RemoveFromStationIdList(stationId);
        break;
    case Mesh::EVENT_TYPE_HOST_CHANGED:
        pSession->m_HostStationId = StationId(Mesh::s_pInstance->m_HostStationIndex, 0);
        if (pSession->m_LocalStationId == stationId) {
            // the local station is the new host
            if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
                pSession->NotifyEvent(Session::EVENT_TYPE_HOST_CHANGED, stationId);
            }
        } else {
            pSession->NotifyEvent(Session::EVENT_TYPE_HOST_CHANGED, stationId);
        }
        break;
    case Mesh::EVENT_TYPE_HOST_MIGRATION_FAILED:
        pSession->NotifyEvent(Session::EVENT_TYPE_3, stationId);
        break;
    case Mesh::EVENT_TYPE_MESH_JOINED:
        break;
    case Mesh::EVENT_TYPE_MESH_LEFT:
        pSession->StopSessionStatusCheck();
        break;
    case Mesh::EVENT_TYPE_JOIN_RESPONSE: {
        // the join fails if the host has another station number
        u8 stationNum = static_cast<u8>(event.m_Unknown0x4 >> 16);
        if (stationNum != 0 && transport::Transport::s_pInstance->m_StationNum != stationNum && pSession->m_pJoinSessionJob->IsRunning()) {
            pSession->m_pJoinSessionJob->m_IsHostLeft = true;
        }
        break;
    }
    default:
        break;
    }
}

// 0x00446868 (name is ours)
void nn::pia::session::MeshEventListenerForSession::OnEventWithStationIdTable(const nn::pia::session::Mesh::Event& event)
{
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    Session* pSession = Session::s_pInstance;
    transport::StationIdTable::Entry entry;
    u32 principalId = 0;
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(event.m_StationIndex);
    if (pStation != nullptr) {
        principalId = transport::StationConnectionInfoTable::s_pInstance->GetPrincipalIdByStation(pStation);
    }
    switch (event.m_Type) {
    case Mesh::EVENT_TYPE_JOIN: {
        transport::StationIdTable::Entry joinedEntry;
        transport::StationIdTable::Entry oldEntry;
        nn::Result result = pTable->Find(&joinedEntry, event.m_StationIndex);
        // the principal had another index that nobody knows of yet
        if (pTable->Find(&oldEntry, principalId).IsSuccess() && event.m_StationIndex != oldEntry.m_StationIndex &&
            !pSession->m_pStationIdStatusTable->IsValid(oldEntry.m_StationId) &&
            !pSession->m_pStationIdStatusTable->IsNotified(oldEntry.m_StationId)) {
            pTable->Remove(oldEntry.m_StationId);
            pSession->m_pStationIdStatusTable->Remove(oldEntry.m_StationId);
        }
        // the index gets the principal id of the station
        if (result.IsSuccess() && joinedEntry.m_Key != principalId && principalId != 0) {
            transport::StationIdTable::Entry* pEntry = pTable->FindCore(event.m_StationIndex);
            pEntry->m_StationIndex = event.m_StationIndex;
            pEntry->m_StationId = StationId(principalId, 0);
            pEntry->m_Key = principalId;
            StationIdStatusTable::Entry* pStatus = pSession->m_pStationIdStatusTable->Find(joinedEntry.m_StationId);
            u32 sessionId = GetSessionIdOfStation(pStation);
            if (pStatus != nullptr) {
                pStatus->m_Status = 0;
                pStatus->m_Unknown0x1 = 0;
                pStatus->m_IsValid = true;
                pStatus->m_IsNotified = !Session::s_pInstance->m_Unknown0xB2;
                pStatus->m_StationId = StationId(principalId, 0);
                pStatus->m_SessionId = sessionId;
                pStatus->m_Unknown0x10 = 0;
                pStatus->m_StationIndex = event.m_StationIndex;
                pStatus->m_Unknown0x12 = false;
                pStatus->m_Unknown0x13 = false;
            } else {
                pSession->m_DisconnectState = 2;
                pSession->StopSessionStatusCheck();
            }
        }
        if (event.m_StationIndex == Mesh::s_pInstance->m_LocalStationIndex) {
            // the local station joined
            transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
            u32 localPrincipalId = transport::StationConnectionInfoTable::s_pInstance->GetPrincipalIdByStation(pLocalStation);
            if (localPrincipalId != 0) {
                transport::StationManager::s_pInstance->m_pLocalStation->SetStationId(StationId(localPrincipalId, 0));
                if (pTable->Find(&entry, localPrincipalId).IsSuccess()) {
                    if (entry.m_StationIndex != event.m_StationIndex) {
                        pTable->FindCore(entry.m_StationId)->m_StationIndex = event.m_StationIndex;
                    }
                    if (pSession->m_State == 3) {
                        pSession->m_pStationIdStatusTable->SetValid(entry.m_StationId, true);
                    }
                } else {
                    transport::Station::IdentificationInfo info;
                    u32 sessionId = transport::IdentificationInfoTable::s_pInstance->GetLocalIdentificationInfo(&info).IsSuccess() ? info.m_Unknown0x48 : 0;
                    transport::StationIdTable::Entry newEntry;
                    newEntry.m_StationId = StationId(localPrincipalId, 0);
                    newEntry.m_StationIndex = event.m_StationIndex;
                    newEntry.m_Key = localPrincipalId;
                    pSession->AddStation(newEntry, sessionId);
                }
            }
        } else {
            transport::Station* pJoinedStation = transport::StationManager::s_pInstance->GetStation(event.m_StationIndex);
            if (pJoinedStation != nullptr) {
                u32 joinedPrincipalId = transport::StationConnectionInfoTable::s_pInstance->GetPrincipalIdByStation(pJoinedStation);
                if (joinedPrincipalId != 0) {
                    bool isNew = true;
                    if (pTable->Find(&entry, joinedPrincipalId).IsSuccess()) {
                        if (pSession->m_State == 3) {
                            // the station is known if it is in the same session as before
                            transport::Station::IdentificationInfo info;
                            transport::IdentificationInfoTable::s_pInstance->GetIdentificationInfo(pJoinedStation, &info);
                            u32 sessionId;
                            pSession->m_pStationIdStatusTable->GetSessionId(entry.m_StationId, &sessionId);
                            if (info.m_Unknown0x48 == sessionId) {
                                if (entry.m_StationIndex != event.m_StationIndex) {
                                    pTable->FindCore(entry.m_StationId)->m_StationIndex = event.m_StationIndex;
                                }
                                pSession->m_pStationIdStatusTable->SetValid(entry.m_StationId, true);
                                isNew = false;
                            } else {
                                RemoveLeftStation(entry.m_StationId);
                            }
                        } else if (entry.m_StationIndex == event.m_StationIndex) {
                            isNew = false;
                        } else if (entry.m_StationIndex != STATION_INDEX_UNIDENTIFIED) {
                            // the station had another index
                            if (pSession->m_pStationIdStatusTable->IsValid(entry.m_StationId)) {
                                OnLeave(entry.m_StationIndex);
                            }
                            if (pSession->IsJoining()) {
                                pSession->ClearStationBit(entry.m_StationIndex);
                                RemoveLeftStation(entry.m_StationId);
                            } else {
                                pSession->RemoveStation(entry, 0);
                            }
                        }
                    }
                    pJoinedStation->SetStationId(StationId(joinedPrincipalId, 0));
                    if (isNew) {
                        u32 sessionId = GetSessionIdOfStation(pJoinedStation);
                        transport::StationIdTable::Entry newEntry;
                        newEntry.m_StationId = StationId(joinedPrincipalId, 0);
                        newEntry.m_StationIndex = event.m_StationIndex;
                        newEntry.m_Key = joinedPrincipalId;
                        if (!pSession->AddStation(newEntry, sessionId) && pSession->m_State == 3) {
                            JointSessionJob* pJointSessionJob = pSession->m_pJointSessionJob;
                            if (pJointSessionJob->m_Phase != 0) {
                                pJointSessionJob->m_IsFailed = true;
                                pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
                            }
                        }
                    }
                }
            }
        }
        if (pSession->m_pConfigParticipationJob->IsRunning()) {
            pSession->m_pConfigParticipationJob->RequestRestart();
        }
        break;
    }
    case Mesh::EVENT_TYPE_LEAVE:
        if (pSession->m_State == 3 && pTable->Find(&entry, event.m_StationIndex).IsSuccess()) {
            // a joint session: the station stays in the table without an index
            pSession->m_pStationIdStatusTable->SetValid(entry.m_StationId, false);
            u32 sessionId;
            pSession->m_pStationIdStatusTable->GetSessionId(entry.m_StationId, &sessionId);
            pTable->FindCore(event.m_StationIndex)->m_StationIndex = STATION_INDEX_UNIDENTIFIED;
            if (pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
                if (pSession->m_Unknown0xB1 != 0) {
                    if (pSession->m_pJointSessionJob->IsRunning()) {
                        pSession->m_pJointSessionJob->MarkStationLeft(entry.m_StationId);
                    }
                    break;
                }
            } else {
                RemoveLeftStation(entry.m_StationId);
            }
            if (entry.m_StationId == pSession->m_HostStationId) {
                if (!pSession->m_IsHostMigrationEnabled) {
                    pSession->m_DisconnectState = 2;
                    pSession->StopSessionStatusCheck();
                } else if (pSession->m_DisconnectState == 1 && !Mesh::s_pInstance->m_pProcessDestroyMeshJob->m_IsRunning &&
                           pSession->m_pJointSessionJob->m_Phase == 9) {
                    // the owner of the current matchmake session continues the joint session
                    u32 jointSessionId;
                    if (!pSession->m_pStationIdStatusTable->GetSessionId(pSession->m_JointHostStationId, &jointSessionId)) {
                        jointSessionId = 0;
                    }
                    if (pSession->m_SessionIds[pSession->m_CurrentIndex] != jointSessionId &&
                        pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() != pSession->m_HostStationId.m_Low) {
                        StationId ownerStationId;
                        ownerStationId.m_Low = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90();
                        if (ownerStationId == pSession->m_LocalStationId) {
                            pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
                        }
                        pSession->m_pJointSessionJob->vf_0x4C(ownerStationId);
                    }
                }
            } else if (entry.m_StationId == pSession->m_JointHostStationId && !pSession->m_IsHostMigrationEnabled &&
                       pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
                pSession->m_DisconnectState = 2;
                pSession->StopSessionStatusCheck();
            }
        } else {
            entry.m_StationId = GetStationIdOfIndex253();
            entry.m_StationIndex = STATION_INDEX_UNIDENTIFIED;
            transport::Station* pLeftStation = transport::StationManager::s_pInstance->GetStation(event.m_StationIndex);
            if (pLeftStation != nullptr) {
                u32 leftPrincipalId = transport::StationConnectionInfoTable::s_pInstance->GetPrincipalIdByStation(pLeftStation);
                if (leftPrincipalId != 0) {
                    pTable->Find(&entry, leftPrincipalId);
                }
            }
            if (entry.m_StationIndex != event.m_StationIndex) {
                pTable->Find(&entry, event.m_StationIndex);
            }
            if (entry.m_StationIndex != event.m_StationIndex) {
                break;
            }
            if (pSession->IsJoining()) {
                pSession->ClearStationBit(event.m_StationIndex);
                RemoveLeftStation(entry.m_StationId);
            } else {
                pSession->RemoveStation(entry, 0);
            }
            if (entry.m_StationId == pSession->m_HostStationId || entry.m_StationId == pSession->m_JointHostStationId) {
                if (!pSession->m_IsHostMigrationEnabled) {
                    pSession->m_DisconnectState = 2;
                } else if (pSession->m_DisconnectState == 1 && !Mesh::s_pInstance->m_pProcessDestroyMeshJob->m_IsRunning) {
                    // the host of the other session left: the owner of the current one is the host
                    u32 jointSessionId;
                    if (!pSession->m_pStationIdStatusTable->GetSessionId(pSession->m_JointHostStationId, &jointSessionId)) {
                        jointSessionId = 0;
                    }
                    u32 hostPrincipalId = pSession->m_HostStationId.m_Low;
                    bool isJointHost = entry.m_StationId == pSession->m_JointHostStationId;
                    if (jointSessionId != 0 && jointSessionId != pSession->m_SessionIds[pSession->m_CurrentIndex] && !isJointHost) {
                        ProcessHostMigrationJob* pJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
                        if (!pJob->m_IsRunning || pJob->m_IsWaitingMigrationFinish) {
                            u32 ownerPrincipalId = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90();
                            if (hostPrincipalId != ownerPrincipalId) {
                                pSession->m_HostStationId = StationId(ownerPrincipalId, 0);
                                pSession->NotifyEvent(Session::EVENT_TYPE_HOST_CHANGED, StationId(ownerPrincipalId, 0));
                            }
                        }
                    }
                }
            }
        }
        if (entry.m_StationId == pSession->m_LocalStationId) {
            pSession->m_DisconnectState = 2;
            pSession->StopSessionStatusCheck();
        }
        break;
    case Mesh::EVENT_TYPE_HOST_CHANGED: {
        if (pSession->IsJoining() || pSession->m_pLeaveSessionJob->IsRunning()) {
            break;
        }
        if (pSession->m_DisconnectState == 2 || pSession->m_DisconnectState == 3) {
            break;
        }
        if (pSession->m_Unknown0xB2) {
            if (pTable->Find(&entry, event.m_StationIndex).IsFailure()) {
                break;
            }
            JointSessionJob* pJointSessionJob = pSession->m_pJointSessionJob;
            if (pJointSessionJob->m_Phase == 0) {
                break;
            }
            pJointSessionJob->vf_0x4C(entry.m_StationId);
            if (pSession->m_LocalStationId != entry.m_StationId ||
                pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
                break;
            }
            pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
            break;
        }
        transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(event.m_StationIndex);
        if (pHostStation == nullptr) {
            break;
        }
        u32 hostPrincipalId = transport::StationConnectionInfoTable::s_pInstance->GetPrincipalIdByStation(pHostStation);
        if (hostPrincipalId == 0 || pTable->Find(&entry, hostPrincipalId).IsFailure()) {
            break;
        }
        if (pSession->m_State == 2) {
            pSession->m_HostStationId = entry.m_StationId;
            if (pSession->m_LocalStationId == pSession->m_HostStationId) {
                if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
                    pSession->NotifyEvent(Session::EVENT_TYPE_HOST_CHANGED, pSession->m_HostStationId);
                }
            } else {
                pSession->NotifyEvent(Session::EVENT_TYPE_HOST_CHANGED, entry.m_StationId);
            }
        } else if (pSession->m_State == 4) {
            // the host of the joint session changed
            bool isJointHostChanged = false;
            bool isHostChanged = false;
            pSession->m_JointHostStationId = entry.m_StationId;
            u32 jointSessionId;
            pSession->m_pStationIdStatusTable->GetSessionId(pSession->m_JointHostStationId, &jointSessionId);
            pSession->m_Unknown0xFC = jointSessionId;
            if (pSession->m_JointHostStationId != pSession->m_LocalStationId) {
                isJointHostChanged = true;
            } else if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x90() == pSession->m_JointHostStationId.m_Low &&
                       pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() == pSession->m_JointHostStationId.m_Low) {
                isJointHostChanged = true;
            }
            if (pSession->m_SessionIds[pSession->m_CurrentIndex] == pSession->m_Unknown0xFC) {
                if (pSession->m_HostStationId == entry.m_StationId) {
                    // the same host
                } else if (pSession->m_HostStationId == pSession->m_LocalStationId) {
                    pSession->m_DisconnectState = 2;
                    pSession->StopSessionStatusCheck();
                } else {
                    // the old host leaves the mesh
                    StationIndex hostStationIndex;
                    if (transport::Transport::s_pInstance->ConvertToStationIndex(&hostStationIndex, pSession->m_HostStationId).IsSuccess() &&
                        Mesh::s_pInstance->CheckStationIndexIsValid(hostStationIndex)) {
                        OnLeave(hostStationIndex);
                    }
                    pSession->m_HostStationId = entry.m_StationId;
                    if (pSession->m_HostStationId == pSession->m_LocalStationId) {
                        pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
                        if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() == pSession->m_HostStationId.m_Low) {
                            isHostChanged = true;
                        }
                    } else {
                        isHostChanged = true;
                    }
                }
            } else if (pTable->Find(&entry, pSession->m_HostStationId).IsFailure() &&
                       pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() != pSession->m_HostStationId.m_Low) {
                // the host is not in the table: the owner of the matchmake session is
                pSession->m_HostStationId.m_Low = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90();
                if (pSession->m_HostStationId == pSession->m_LocalStationId) {
                    pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
                }
                isHostChanged = true;
            }
            if (isJointHostChanged) {
                pSession->NotifyEvent(Session::EVENT_TYPE_JOINT_HOST_CHANGED, pSession->m_JointHostStationId);
            }
            if (isHostChanged) {
                pSession->NotifyEvent(Session::EVENT_TYPE_HOST_CHANGED, pSession->m_HostStationId);
            }
        } else {
            JointSessionJob* pJointSessionJob = pSession->m_pJointSessionJob;
            if (pJointSessionJob->m_Phase != 0) {
                pJointSessionJob->vf_0x4C(entry.m_StationId);
            }
        }
        break;
    }
    case Mesh::EVENT_TYPE_HOST_MIGRATION_FAILED:
        if (pSession->m_State == 3) {
            JointSessionJob* pJointSessionJob = pSession->m_pJointSessionJob;
            pJointSessionJob->m_IsFailed = true;
            pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
        } else {
            pSession->m_State = 0;
            pSession->m_DisconnectState = 2;
            pSession->m_HostStationId = GetStationIdOfIndex253();
            pSession->m_JointHostStationId = GetStationIdOfIndex253();
            pSession->NotifyEvent(Session::EVENT_TYPE_3, GetStationIdOfIndex253());
        }
        break;
    case Mesh::EVENT_TYPE_MESH_LEFT:
        pSession->StopSessionStatusCheck();
        break;
    case Mesh::EVENT_TYPE_GREETING: {
        // the owner of the matchmake session greets the new host: the old host is gone
        StationIndex hostStationIndex = Mesh::s_pInstance->m_HostStationIndex;
        if (principalId == pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() && event.m_StationIndex != hostStationIndex) {
            transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(hostStationIndex);
            if (pHostStation != nullptr) {
                if (pHostStation->m_State == transport::Station::STATION_STATE_CONNECTED) {
                    pHostStation->m_Unknown0x68 = true;
                } else {
                    pHostStation->m_State = transport::Station::STATION_STATE_DISCONNECTED;
                }
            }
        }
        break;
    }
    case Mesh::EVENT_TYPE_JOIN_RESPONSE: {
        // the entry limits of the station id table (bytes 8 and 9 of the join response)
        if (pSession->IsUsingStationIdTable()) {
            // the table takes the limit of the other session while the joint session job
            // moves to it
            bool isOtherSession = false;
            if (pSession->m_pJointSessionJob->IsRunning()) {
                if (pSession->m_StationIdEntryNumMax[pSession->m_CurrentIndex] == 0) {
                    pSession->m_StationIdEntryNumMax[pSession->m_CurrentIndex] = static_cast<u8>(event.m_Unknown0x4 >> 8);
                }
                pSession->m_StationIdEntryNumMax[pSession->m_CurrentIndex == 0 ? 1 : 0] = static_cast<u8>(event.m_Unknown0x4);
                u8 phase = pSession->m_pJointSessionJob->m_Phase;
                isOtherSession = phase == 6 || phase == 7 || phase == 8;
            } else {
                pSession->m_StationIdEntryNumMax[pSession->m_CurrentIndex] = static_cast<u8>(event.m_Unknown0x4 >> 8);
            }
            Session* pInstance = Session::s_pInstance;
            u8 index = isOtherSession ? (pInstance->m_CurrentIndex == 0 ? 1 : 0) : pInstance->m_CurrentIndex;
            transport::Transport::s_pInstance->m_pStationIdTable->SetEntryNumMax(pInstance->m_StationIdEntryNumMax[index]);
        }
        // the join fails if the host has another station number
        u8 stationNum = static_cast<u8>(event.m_Unknown0x4 >> 16);
        if (stationNum != 0 && transport::Transport::s_pInstance->m_StationNum != stationNum && pSession->m_pJoinSessionJob->IsRunning()) {
            pSession->m_pJoinSessionJob->m_IsHostLeft = true;
        }
        break;
    }
    default:
        break;
    }
}

// 0x00447900
nn::pia::session::MeshEventListenerForSession::MeshEventListenerForSession()
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
