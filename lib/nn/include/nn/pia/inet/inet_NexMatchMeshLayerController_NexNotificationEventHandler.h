#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/nex/nex_NotificationEventHandler.h"
#include "nn/pia/common/common_FixedObjList.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include "nn/pia/session/session_Session.h"

// RTTI N2nn3pia4inet27NexMatchMeshLayerController27NexNotificationEventHandlerE @ 0x008CFA38
// vtable 0x009006A8 (vptr 0x009006B0), offset_to_top 0, 3 entries
//
// The notifications of the server for up to four matchmake sessions: per session the
// participation events of the principals (a principal whose events cancel out is removed), the
// sessions that were left or deleted, and the changes of the owner, the host and the string. The
// type names of the nex notifications are not in the binary; all names are ours.
//
// ARMCC unrolls all loops over the four entries (also in FindEntry and over the left sessions)
// and destroys the entries with __aeabi_vec_dtor, so the functions with them stay close / far.
class nn::pia::inet::NexMatchMeshLayerController::NexNotificationEventHandler : public ::nn::nex::NotificationEventHandler
{
public:
    // the nex notification types (type / 1000; the subtype is type % 1000)
    enum NotificationType
    {
        NOTIFICATION_TYPE_PARTICIPATION = 3,     // subtype 1: joined, others: left
        NOTIFICATION_TYPE_OWNER_CHANGED = 4,     // param2: the new owner
        NOTIFICATION_TYPE_SESSION_DELETED = 109,
        NOTIFICATION_TYPE_110 = 110,
        NOTIFICATION_TYPE_STRING_SET = 120,
        NOTIFICATION_TYPE_STRING_CLEARED = 121,
        NOTIFICATION_TYPE_122 = 122,             // param2: the local principal (joint session)
    };
    static const u32 SESSION_ENTRY_NUM = 4;
    // (Participant, SessionEntry and their constants are in NexMatchMeshLayerController)

    NexNotificationEventHandler() : m_IsSessionDeleted(false)
    {
        for (u32 i = 0; i < SESSION_ENTRY_NUM; i++) {
            m_Entries[i].Clear();
        }
    }
    virtual ~NexNotificationEventHandler(); // 0x00410CB4 slot 0x00
    // 0x00410C80 slot 0x04 (deleting dtor)
    virtual void ProcessNotificationEvent(const nn::nex::NotificationEvent& event); // 0x00410144 slot 0x08

    // an entry for the session; RESULT_ALREADY_EXISTS / RESULT_BUFFER_IS_FULL
    nn::Result AddSession(u32 sessionId); // 0x0040FF5C
    // its entry is cleared and the session is left
    void RemoveSession(u32 sessionId); // 0x00410058

    SessionEntry* FindEntry(u32 sessionId)
    {
        for (u32 i = 0; i < SESSION_ENTRY_NUM; i++) {
            if (m_Entries[i].m_SessionId == sessionId) {
                return &m_Entries[i];
            }
        }
        return nullptr;
    }
    bool IsLeftSession(u32 sessionId) const
    {
        for (u32 i = 0; i < SESSION_ENTRY_NUM; i++) {
            if (m_LeftSessionIds[i] != 0 && m_LeftSessionIds[i] == sessionId) {
                return true;
            }
        }
        return false;
    }
    void AddLeftSession(u32 sessionId)
    {
        for (u32 i = 0; i < SESSION_ENTRY_NUM; i++) {
            if (m_LeftSessionIds[i] == sessionId) {
                return;
            }
            if (m_LeftSessionIds[i] == 0) {
                m_LeftSessionIds[i] = sessionId;
                return;
            }
        }
    }
    void ClearLeftSessions()
    {
        for (u32 i = 0; i < SESSION_ENTRY_NUM; i++) {
            m_LeftSessionIds[i] = 0;
        }
    }
    void Reset()
    {
        m_IsSessionDeleted = false;
        // (the result is not used)
        session::Session::s_pInstance->GetStatus();
        for (u32 i = 0; i < SESSION_ENTRY_NUM; i++) {
            m_Entries[i].Clear();
        }
        ClearLeftSessions();
    }

    bool m_IsSessionDeleted;                       // 0x004
    SessionEntry m_Entries[SESSION_ENTRY_NUM];     // 0x008
    u32 m_LeftSessionIds[SESSION_ENTRY_NUM];       // 0x3E8
};
ASSERT_SIZE(nn::pia::inet::NexMatchMeshLayerController::SessionEntry, 0xF8);
ASSERT_SIZE(nn::pia::inet::NexMatchMeshLayerController::NexNotificationEventHandler, 0x3F8);
