#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_GlobalNotificationEventManager.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_ServerProtocol.h"

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

// 0x0035E8F4 | fefates:callgraph [tier C]
nn::nex::RVClientCore* nn::nex::RVClientCore::GetInstance()
{
    InstanceControl* pInstance = InstanceTable::s_Instance.GetInstance(KIND, PseudoSingleton::GetCurrentContext());
    if (pInstance == nullptr) {
        return nullptr;
    }
    return static_cast<RVClientCore*>(pInstance->m_pOwner);
}

// 0x0035EA4C | fefates:bytes [tier A]
nn::nex::ServerProtocol* nn::nex::RVClientCore::FindProtocol(unsigned short id)
{
    ScopedCS lock(GetSchedulerLock());
    qMap<u16, ServerProtocol*>::iterator it = m_ServerProtocols.find(id);
    if (it == m_ServerProtocols.end()) {
        return nullptr;
    }
    return it->second;
}

// 0x0035EB0C (name is ours)
bool nn::nex::RVClientCore::RegisterServerProtocol(unsigned short id, nn::nex::ServerProtocol* pProtocol)
{
    ScopedCS lock(GetSchedulerLock());
    if (m_ServerProtocols.find(id) != m_ServerProtocols.end()) {
        return false;
    }
    m_ServerProtocols[id] = pProtocol;
    return true;
}

// 0x0035EBD8
nn::nex::RVClientCore::RVClientCore() : m_Singleton(KIND), m_LoginTime(0), m_ServerTime(0)
{
    m_Singleton.SetOwner(this);
    Core::AcquireInstance();
    m_pProtocolRegistry = new ProtocolRegistry();
    m_pNotificationManager = new GlobalNotificationEventManager();
    m_pNotificationManager->SetFlag(Protocol::FLAG_UNKNOWN_8);
    RegisterServerProtocol(NOTIFICATION_PROTOCOL_ID, m_pNotificationManager);
}

// 0x0035EDC8 slot 0x00
// 0x0035ED48 (deleting dtor)
nn::nex::RVClientCore::~RVClientCore()
{
    delete m_pProtocolRegistry;
    delete m_pNotificationManager;
    Core::ReleaseInstance();
}

} // namespace nex
} // namespace nn
