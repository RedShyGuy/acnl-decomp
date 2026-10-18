#pragma once

#include "decomp.h"
#include "nn/nex/nex_NotificationEvent.h"
#include "nn/nex/nex_NotificationEventFilterInterface.h"
#include "nn/nex/nex_NotificationEventHandler.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// The handler of a JobWaitForNotification: the first event of the type (type * 1000 + subtype)
// that the filter accepts goes to the result. Until a filter is set, the last 16 events of the type
// are kept. The names of the members are ours.
// RTTI N2nn3nex26MyNotificationEventHandlerE @ 0x008CEF68
// vtable 0x008FEDA0 (vptr 0x008FEDA8), offset_to_top 0, 5 entries
// vtable 0x008FEDBC (vptr 0x008FEDC4), offset_to_top -4, 4 entries
class MyNotificationEventHandler : public ::nn::nex::NotificationEventHandler, public ::nn::nex::NotificationEventFilterInterface
{
public:
    // the number of the kept events
    static const u32 MAX_KEPT_EVENTS = 16;

    // (inline in JobWaitForNotification::Initialize)
    MyNotificationEventHandler(unsigned int type, unsigned int subType, NotificationEvent* pResult)
        : m_IsFound(false), m_UsesParam1(false), m_UsesParam64(false), m_Type(type), m_SubType(subType), m_Param1(0), m_pResult(pResult)
    {
    }
    virtual ~MyNotificationEventHandler(); // 0x003BA150 slot 0x00
    // 0x003BA0FC slot 0x04 (deleting dtor)
    virtual void ProcessNotificationEvent(const nn::nex::NotificationEvent& event); // 0x003B9C3C slot 0x08
    virtual void FilterByParam1(unsigned int param1); // 0x003B9AD4 slot 0x0C
    virtual void FilterByParam64(unsigned long long param); // 0x003B9B84 slot 0x10

    bool m_IsFound;                     // 0x08
    bool m_UsesParam1;                  // 0x09
    bool m_UsesParam64;                 // 0x0A
    u32 m_Type;                         // 0x0C
    u32 m_SubType;                      // 0x10
    u32 m_Param1;                       // 0x14
    u64 m_Param64;                      // 0x18 (not set by the constructor)
    qVector<NotificationEvent> m_Events; // 0x20 (the events of the type until a filter is set)
    NotificationEvent* m_pResult;       // 0x2C
};
ASSERT_OFFSET(MyNotificationEventHandler, m_Events, 0x20);
ASSERT_SIZE(MyNotificationEventHandler, 0x30);
} // namespace nex
} // namespace nn
