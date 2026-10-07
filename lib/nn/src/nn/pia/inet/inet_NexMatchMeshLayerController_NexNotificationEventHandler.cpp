#include "nn/pia/inet/inet_NexMatchMeshLayerController_NexNotificationEventHandler.h"
#include "nn/nex/nex_NotificationEvent.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/session/session_MeshLayerController.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// whether the notification is about the current or the other matchmake session of Session
inline bool IsSessionOfSession(session::Session* pSession, session::CommonMatchmakeSession* pOther, u32 sessionId)
{
    return pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId ||
           (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == sessionId && pOther != nullptr);
}
} // namespace

typedef NexMatchMeshLayerController::NexNotificationEventHandler Handler;

// 0x003E434C
nn::pia::inet::NexMatchMeshLayerController::SessionEntry::SessionEntry()
{
    Clear();
}

// 0x003E4448
nn::pia::inet::NexMatchMeshLayerController::SessionEntry::~SessionEntry()
{
    Clear();
}

// 0x0040FF5C
nn::Result nn::pia::inet::NexMatchMeshLayerController::NexNotificationEventHandler::AddSession(u32 sessionId)
{
    u32 freeIndex = SESSION_ENTRY_NUM;
    for (u32 i = 0; i < SESSION_ENTRY_NUM; i++) {
        if (m_Entries[i].m_SessionId == sessionId) {
            return common::RESULT_ALREADY_EXISTS;
        }
        if (freeIndex == SESSION_ENTRY_NUM && m_Entries[i].m_SessionId == 0) {
            freeIndex = i;
        }
    }
    if (freeIndex == SESSION_ENTRY_NUM) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    SessionEntry& entry = m_Entries[freeIndex];
    entry.m_SessionId = sessionId;
    entry.m_List.ClearNodes();
    entry.m_Time.SetNow();
    return nn::Result();
}

// 0x00410058
void nn::pia::inet::NexMatchMeshLayerController::NexNotificationEventHandler::RemoveSession(u32 sessionId)
{
    for (u32 i = 0; i < SESSION_ENTRY_NUM; i++) {
        if (m_Entries[i].m_SessionId == sessionId) {
            m_Entries[i].Clear();
        }
    }
    AddLeftSession(sessionId);
}

// 0x00410144
void nn::pia::inet::NexMatchMeshLayerController::NexNotificationEventHandler::ProcessNotificationEvent(const nn::nex::NotificationEvent& event)
{
    u32 type = event.m_Type / 1000;
    if (type != NOTIFICATION_TYPE_PARTICIPATION && type != NOTIFICATION_TYPE_OWNER_CHANGED && type != NOTIFICATION_TYPE_SESSION_DELETED &&
        type != NOTIFICATION_TYPE_110 && type != NOTIFICATION_TYPE_STRING_SET && type != NOTIFICATION_TYPE_STRING_CLEARED &&
        type != NOTIFICATION_TYPE_122) {
        return;
    }
    session::Session* pSession = session::Session::s_pInstance;
    u8 index = pSession->m_CurrentIndex;
    session::CommonMatchmakeSession* pCurrent = pSession->m_pMatchmakeSessions[index];
    session::CommonMatchmakeSession* pOther = pSession->m_pMatchmakeSessions[index == 0 ? 1 : 0];
    if (!IsLeftSession(event.m_Param1)) {
        AddSession(event.m_Param1);
    }

    switch (event.m_Type / 1000) {
    case NOTIFICATION_TYPE_PARTICIPATION: {
        u32 subtype = event.m_Type % 1000;
        if (subtype == PARTICIPATION_JOINED) {
            if (pSession->m_Unknown0x8C == event.m_Param2) {
                ClearLeftSessions();
            }
            u32 sessionId = event.m_Param1;
            u32 principalId = event.m_Param2;
            SessionEntry* pEntry = FindEntry(sessionId);
            if (pEntry == nullptr) {
                if (AddSession(sessionId).IsFailure()) {
                    return;
                }
                pEntry = FindEntry(sessionId);
                if (pEntry == nullptr) {
                    return;
                }
            }
            pEntry->DropOldEvents();
            for (SessionEntry::List::Node* node = pEntry->m_List.Begin(); node != pEntry->m_List.End(); node = SessionEntry::List::Advance(node)) {
                Participant& participant = node->m_Value;
                if (participant.m_PrincipalId == principalId) {
                    if (participant.m_Type == PARTICIPATION_7) {
                        participant.m_Count = 1;
                        participant.m_Type = PARTICIPATION_JOINED;
                        return;
                    }
                    participant.m_Count++;
                    participant.m_Type = PARTICIPATION_JOINED;
                    if (participant.m_Count == 0) {
                        pEntry->m_List.Erase(&participant);
                    }
                    return;
                }
            }
            Participant* pParticipant = pEntry->m_List.PushBackNew();
            if (pParticipant != nullptr) {
                pParticipant->m_PrincipalId = principalId;
                pParticipant->m_Count = 1;
                pParticipant->m_Type = PARTICIPATION_JOINED;
            }
            return;
        }

        // a principal left
        u32 sessionId = event.m_Param1;
        u32 principalId = event.m_Param2;
        u16 leaveType = static_cast<s16>(subtype);
        SessionEntry* pEntry = FindEntry(sessionId);
        if (pEntry == nullptr && !IsLeftSession(sessionId) && AddSession(sessionId).IsSuccess()) {
            pEntry = FindEntry(sessionId);
        }
        if (pEntry != nullptr) {
            pEntry->DropOldEvents();
            SessionEntry::List::Node* node = pEntry->m_List.Begin();
            for (; node != pEntry->m_List.End(); node = SessionEntry::List::Advance(node)) {
                if (node->m_Value.m_PrincipalId == principalId) {
                    break;
                }
            }
            if (node != pEntry->m_List.End()) {
                Participant& participant = node->m_Value;
                if (participant.m_Type == PARTICIPATION_7 && leaveType != PARTICIPATION_7) {
                    participant.m_Type = leaveType;
                } else {
                    participant.m_Count--;
                    participant.m_Type = leaveType;
                    if (participant.m_Count == 0) {
                        pEntry->m_List.Erase(&participant);
                    }
                }
            } else {
                Participant* pParticipant = pEntry->m_List.PushBackNew();
                if (pParticipant != nullptr) {
                    pParticipant->m_PrincipalId = principalId;
                    pParticipant->m_Count = -1;
                    pParticipant->m_Type = leaveType;
                }
            }
        }
        pSession->OnParticipantDisconnected(event.m_Param1, event.m_Param2);
        if (pSession->m_Unknown0x8C == event.m_Param2) {
            // the local principal left the session
            RemoveSession(event.m_Param1);
            AddLeftSession(event.m_Param1);
        }
        break;
    }
    case NOTIFICATION_TYPE_OWNER_CHANGED:
        if (pSession->m_SessionIds[pSession->m_CurrentIndex] == event.m_Param1) {
            pCurrent->vf_0x8C(event.m_Param2 == pSession->m_Unknown0x8C);
            pCurrent->vf_0x94(event.m_Param2);
        } else if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == event.m_Param1 && pOther != nullptr) {
            pOther->vf_0x8C(event.m_Param2 == pSession->m_Unknown0x8C);
            pOther->vf_0x94(event.m_Param2);
        }
        pSession->UpdateSessionOwner(event.m_Param1, event.m_Param2);
        break;
    case NOTIFICATION_TYPE_SESSION_DELETED:
        if (pSession->m_SessionIds[pSession->m_CurrentIndex] == event.m_Param1) {
            m_IsSessionDeleted = true;
        }
        if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == event.m_Param1 && pOther != nullptr) {
            m_IsSessionDeleted = true;
        }
        RemoveSession(event.m_Param1);
        AddLeftSession(event.m_Param1);
        pSession->OnUnknownNotification(event.m_Param1);
        pSession->SetJoinable(event.m_Param1, true);
        break;
    case NOTIFICATION_TYPE_110:
        if (IsSessionOfSession(pSession, pOther, event.m_Param1)) {
            pSession->OnUnknownNotification2(event.m_Param1, event.m_Param2);
        }
        break;
    case NOTIFICATION_TYPE_STRING_SET:
        if (IsSessionOfSession(pSession, pOther, event.m_Param1)) {
            pSession->SetString(reinterpret_cast<const u16*>(event.m_StringParam.m_pString), event.m_StringParam.GetLength());
            session::Session::NotifyStationEvent(session::Session::EVENT_TYPE_STRING_SET, STATION_INDEX_UNIDENTIFIED);
        }
        break;
    case NOTIFICATION_TYPE_STRING_CLEARED:
        if (IsSessionOfSession(pSession, pOther, event.m_Param1)) {
            pSession->ClearString();
            session::Session::NotifyStationEvent(session::Session::EVENT_TYPE_STRING_CLEARED, STATION_INDEX_UNIDENTIFIED);
        }
        break;
    case NOTIFICATION_TYPE_122:
        if (event.m_Param2 == pSession->m_Unknown0x8C && session::Session::s_pInstance->IsUsingStationIdTable()) {
            ClearLeftSessions();
            AddSession(event.m_Param1);
            if (event.m_Param2 == pSession->m_Unknown0x8C && pSession->m_SessionIds[pSession->m_CurrentIndex] != event.m_Param1 &&
                pOther != nullptr && pSession->m_State == 3) {
                u32 otherSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
                if (otherSessionId == 0 || otherSessionId == event.m_Param1) {
                    pSession->m_pMeshLayerController->SetOtherSessionId(event.m_Param1);
                } else if (pSession->m_pMeshLayerController->GetJointSessionJob() != nullptr &&
                           pSession->m_pMeshLayerController->GetJointSessionJob()->IsRunning()) {
                    // the joint session fails
                    session::JointSessionJob* pJob = pSession->m_pMeshLayerController->GetJointSessionJob();
                    pJob->m_IsFailed = true;
                    pJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
                }
            }
        }
        break;
    }
}

// 0x00410CB4
// 0x00410C80 (deleting dtor)
nn::pia::inet::NexMatchMeshLayerController::NexNotificationEventHandler::~NexNotificationEventHandler()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
