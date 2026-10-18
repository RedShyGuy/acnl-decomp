#include "nn/nex/nex_MyNotificationEventHandler.h"

namespace nn {
namespace nex {
namespace {
// the factor of the type in the type of an event
const u32 TYPE_FACTOR = 1000;
} // namespace

// 0x003B9AD4 slot 0x0C
// 0x007780D4 (thunk)
void nn::nex::MyNotificationEventHandler::FilterByParam1(unsigned int param1)
{
    m_Param1 = param1;
    m_UsesParam1 = true;
    NotificationEvent* pEvent = m_Events.begin();
    for (; pEvent != m_Events.end(); pEvent++) {
        if (pEvent->m_uiParam1 == param1) {
            break;
        }
    }
    if (pEvent != m_Events.end()) {
        *m_pResult = *pEvent;
        m_IsFound = true;
    }
    m_Events.clear();
}

// 0x003B9B84 slot 0x10
// 0x00778180 (thunk)
void nn::nex::MyNotificationEventHandler::FilterByParam64(unsigned long long param)
{
    m_UsesParam64 = true;
    m_Param64 = param;
    NotificationEvent* pEvent = m_Events.begin();
    for (; pEvent != m_Events.end(); pEvent++) {
        if ((static_cast<u64>(pEvent->m_uiParam2) << 32 | pEvent->m_uiParam1) == param) {
            break;
        }
    }
    if (pEvent != m_Events.end()) {
        *m_pResult = *pEvent;
        m_IsFound = true;
    }
    m_Events.clear();
}

// 0x003B9C3C slot 0x08
void nn::nex::MyNotificationEventHandler::ProcessNotificationEvent(const nn::nex::NotificationEvent& event)
{
    u32 type = event.m_uiType / TYPE_FACTOR;
    if (m_UsesParam1) {
        if (type != m_Type || event.m_uiType - type * TYPE_FACTOR != m_SubType || event.m_uiParam1 != m_Param1) {
            return;
        }
    } else if (m_UsesParam64) {
        if (type != m_Type || event.m_uiType - type * TYPE_FACTOR != m_SubType || (static_cast<u64>(event.m_uiParam2) << 32 | event.m_uiParam1) != m_Param64) {
            return;
        }
    } else {
        // no filter yet: the event is kept
        if (type == m_Type && event.m_uiType - type * TYPE_FACTOR == m_SubType) {
            m_Events.push_back(event);
            if (m_Events.size() > MAX_KEPT_EVENTS) {
                m_Events.erase(m_Events.begin());
            }
        }
        return;
    }
    *m_pResult = event;
    m_IsFound = true;
}

// 0x003BA150 slot 0x00
// 0x003BA0FC (deleting dtor)
// 0x00778294 (thunk)
// 0x0077823C (deleting thunk)
nn::nex::MyNotificationEventHandler::~MyNotificationEventHandler()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
