#include "nn/nex/nex_GlobalNotificationEventManager.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_ClientStreamManager.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}
} // namespace

// 0x003C28F8 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
// 0x003C28E8 (deleting dtor)
nn::nex::GlobalNotificationEventManager::~GlobalNotificationEventManager()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

// 0x003C247C slot 0x50 | mk7dlp:callseq
void nn::nex::GlobalNotificationEventManager::DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint)
{
    u16 methodId = Protocol::ExtractMethodID(pRequest);
    // the credentials of the stream that received it
    Credentials* pCredentials = static_cast<ClientStreamManager*>(pEndPoint->m_pEventHandler)->m_pCredentials;
    u32 principalId = pCredentials->m_PrincipalId;
    u32 connectionId = pCredentials->m_ConnectionId;
    if (methodId != 1) {
        SetCallError(qResult(static_cast<int>(0x80010002)));
        return;
    }
    NotificationEvent event;
    _DDL_NotificationEvent::Extract(pRequest, &event);
    if (!pRequest->m_IsError) {
        ProcessNotificationEvent(principalId, connectionId, event);
        Protocol::AddMethodID(pResponse, 0x8001);
        *pIsHandled = true;
    }
}

// 0x003C2590 slot 0x60
void nn::nex::GlobalNotificationEventManager::ProcessNotificationEvent(unsigned int principalId, unsigned int connectionId, const nn::nex::NotificationEvent& event)
{
    for (std::set<NotificationEventManager*>::iterator it = m_Managers.begin(); it != m_Managers.end(); ++it) {
        (*it)->ProcessNotificationEvent(principalId, connectionId, event);
    }
}

// 0x003C262C
bool nn::nex::GlobalNotificationEventManager::RegisterGlobalNotificationEventManager(nn::nex::NotificationEventManager* pManager)
{
    ScopedCS lock(GetSchedulerLock());
    if (m_Managers.find(pManager) != m_Managers.end()) {
        return false;
    }
    m_Managers.InsertInline(pManager);
    return true;
}

// 0x003C281C (name is ours)
bool nn::nex::GlobalNotificationEventManager::UnregisterGlobalNotificationEventManager(nn::nex::NotificationEventManager* pManager)
{
    ScopedCS lock(GetSchedulerLock());
    std::set<NotificationEventManager*>::iterator it = m_Managers.find(pManager);
    if (it == m_Managers.end()) {
        return false;
    }
    m_Managers.erase(it);
    return true;
}

} // namespace nex
} // namespace nn
