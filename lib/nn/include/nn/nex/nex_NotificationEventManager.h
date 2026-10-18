#pragma once

#include "decomp.h"
#include "nn/nex/nex_NotificationEvent.h"
#include "nn/nex/nex__Proto_NotificationProtocolServer.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class NotificationEventHandler;

// The notification events of a station: they go to the registered handlers (an event with the
// same first parameter and a lower or equal time stamp than a recent one is dropped). The names of
// the members and of the functions without a reference name are ours.
// RTTI N2nn3nex24NotificationEventManagerE @ 0x008CEE08
// vtable 0x008FE898 (vptr 0x008FE8A0), offset_to_top 0, 25 entries
class NotificationEventManager : public ::nn::nex::_Proto_NotificationProtocolServer
{
public:
    // the recent events (for the duplicates)
    static const u32 MAX_RECENT_EVENTS = 5;

    // pOriginal: the manager it is a copy of (Clone)
    explicit NotificationEventManager(nn::nex::NotificationEventManager* pOriginal = nullptr); // 0x003B5244 | fefates:bytes [tier B]
    virtual ~NotificationEventManager(); // 0x003B5438 slot 0x00 | fefates:bytes
    // 0x003B5428 slot 0x04 (deleting dtor)
    virtual nn::nex::Protocol* Clone(); // 0x0072D548 slot 0x4C
    virtual void ProcessNotificationEvent(const nn::nex::NotificationEvent& event); // 0x003B4EA4 slot 0x5C
    // the event for the principal and connection goes to the handlers
    virtual void ProcessNotificationEvent(unsigned int principalId, unsigned int connectionId, const nn::nex::NotificationEvent& event); // 0x003B4EA8 slot 0x60

    // false for a duplicate of a recent event of the types 4000 to 4999 (their string parameter is a
    // time stamp)
    bool IsEventValid(const nn::nex::NotificationEvent& event); // 0x003B4C5C | fefates:bytes [tier B]
    bool RegisterEventHandler(nn::nex::NotificationEventHandler* pHandler); // 0x003B4F88
    bool UnregisterEventHandler(nn::nex::NotificationEventHandler* pHandler); // 0x003B5178

    std::set<NotificationEventHandler*> m_Handlers; // 0x40
    NotificationEventManager* m_pOriginal;          // 0x5C
    qList<NotificationEvent> m_RecentEvents;        // 0x60
    u32 m_PrincipalId;                              // 0x78
    u32 m_ConnectionId;                             // 0x7C
    bool m_Unknown0x80;                             // 0x80 (the events of the types 115000 to 115999 are passed on)
};
ASSERT_OFFSET(NotificationEventManager, m_Handlers, 0x40);
ASSERT_OFFSET(NotificationEventManager, m_RecentEvents, 0x60);
ASSERT_SIZE(NotificationEventManager, 0x84);
} // namespace nex
} // namespace nn
