#include "nn/nex/nex_NotificationEventManager.h"
#include "nn/nex/nex_CallProtocolMethodOperation.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_NotificationEventHandler.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// the events of these types have a time stamp in their string parameter
const u32 TIMESTAMPED_TYPE_FIRST = 4000;
const u32 TIMESTAMPED_TYPE_COUNT = 1000;
// the events of this type (divided by 1000) are passed on only with m_Unknown0x80
const u32 TYPE_0x115 = 115;
} // namespace

// 0x003B5244 | fefates:bytes [tier B]
nn::nex::NotificationEventManager::NotificationEventManager(nn::nex::NotificationEventManager* pOriginal) : m_pOriginal(pOriginal), m_PrincipalId(0), m_ConnectionId(0)
{
    m_Flags |= FLAG_UNKNOWN_8;
    SetName(String(L"NotificationEventManager"));
}

// 0x003B5438 slot 0x00 | fefates:bytes
// 0x003B5428 (deleting dtor)
nn::nex::NotificationEventManager::~NotificationEventManager()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

// 0x0072D548 slot 0x4C
nn::nex::Protocol* nn::nex::NotificationEventManager::Clone()
{
    return new NotificationEventManager(this);
}

// 0x003B4EA4 slot 0x5C
void nn::nex::NotificationEventManager::ProcessNotificationEvent(const nn::nex::NotificationEvent& event)
{
    // nothing to do
}

// 0x003B4EA8 slot 0x60
void nn::nex::NotificationEventManager::ProcessNotificationEvent(unsigned int principalId, unsigned int connectionId, const nn::nex::NotificationEvent& event)
{
    if (m_PrincipalId != principalId || m_ConnectionId != connectionId || !IsEventValid(event)) {
        return;
    }
    for (std::set<NotificationEventHandler*>::iterator it = m_Handlers.begin(); it != m_Handlers.end(); ++it) {
        if (event.m_uiType / 1000 != TYPE_0x115 || m_Unknown0x80) {
            (*it)->ProcessNotificationEvent(event);
        }
    }
}

// 0x003B4C5C | fefates:bytes [tier B]
bool nn::nex::NotificationEventManager::IsEventValid(const nn::nex::NotificationEvent& event)
{
    if (event.m_uiType - TIMESTAMPED_TYPE_FIRST >= TIMESTAMPED_TYPE_COUNT) {
        return true;
    }
    if (String::IsEqual(event.m_strParam.m_pString, L"")) {
        return true;
    }
    u64 time = event.m_strParam.ToUInt64();
    if (time == 0) {
        return true;
    }
    // the last event with the same first parameter
    for (qList<NotificationEvent>::Node* pNode = m_RecentEvents.GetFirst(); !m_RecentEvents.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (event.m_uiParam1 == pNode->m_Value.m_uiParam1) {
            if (pNode->m_Value.m_strParam.ToUInt64() >= time) {
                return false;
            }
            m_RecentEvents.erase(pNode);
            break;
        }
    }
    m_RecentEvents.push_back(event);
    if (m_RecentEvents.GetSize() > MAX_RECENT_EVENTS) {
        m_RecentEvents.pop_back();
    }
    return true;
}

// 0x003B4F88
bool nn::nex::NotificationEventManager::RegisterEventHandler(nn::nex::NotificationEventHandler* pHandler)
{
    ScopedCS lock(GetSchedulerLock());
    if (m_Handlers.find(pHandler) != m_Handlers.end()) {
        return false;
    }
    m_Handlers.InsertInline(pHandler);
    return true;
}

// 0x003B5178
bool nn::nex::NotificationEventManager::UnregisterEventHandler(nn::nex::NotificationEventHandler* pHandler)
{
    ScopedCS lock(GetSchedulerLock());
    std::set<NotificationEventHandler*>::iterator it = m_Handlers.find(pHandler);
    if (it == m_Handlers.end()) {
        return false;
    }
    m_Handlers.erase(it);
    return true;
}

// 0x003CB6F8 slot 0x50 | fefates:bytes
void nn::nex::_Proto_NotificationProtocolServer::DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint)
{
    u32 methodId;
    pRequest->ExtractPrimitive(&methodId);
    if (static_cast<u16>(methodId) != 1) {
        SetCallErrorInline(qResult(static_cast<int>(0x80010002)));
        return;
    }
    NotificationEvent event;
    _DDL_NotificationEvent::Extract(pRequest, &event);
    if (!pRequest->m_IsError) {
        ProcessNotificationEvent(event);
        u32 responseId = 0x8001;
        pResponse->AppendRaw(reinterpret_cast<const u8*>(&responseId), sizeof(responseId));
    }
    *pIsHandled = true;
}

} // namespace nex
} // namespace nn

namespace __rw {
// 0x008008AC (name is ours)
template __rb_tree<nn::nex::NotificationEventHandler*, nn::nex::NotificationEventHandler*, __ident<nn::nex::NotificationEventHandler*, nn::nex::NotificationEventHandler*>, std::less<nn::nex::NotificationEventHandler*>, nn::nex::MemAllocator<nn::nex::NotificationEventHandler*> >::iterator __rb_tree<nn::nex::NotificationEventHandler*, nn::nex::NotificationEventHandler*, __ident<nn::nex::NotificationEventHandler*, nn::nex::NotificationEventHandler*>, std::less<nn::nex::NotificationEventHandler*>, nn::nex::MemAllocator<nn::nex::NotificationEventHandler*> >::erase(iterator it);
} // namespace __rw
