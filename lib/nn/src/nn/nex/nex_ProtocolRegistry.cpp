#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_ServiceClient.h"

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

// 0x003837C0 slot 0x00 | fefates:bytes
// 0x003837B0 (deleting dtor)
nn::nex::ProtocolRegistry::~ProtocolRegistry()
{
    for (qMap<u16, Protocol*>::iterator it = m_ClientProtocols.begin(); it != m_ClientProtocols.end(); ++it) {
        if (it->second != nullptr) {
            delete it->second;
        }
    }
    for (qMap<u16, Protocol*>::iterator it = m_ServerProtocols.begin(); it != m_ServerProtocols.end(); ++it) {
        if (it->second != nullptr) {
            delete it->second;
        }
    }
}

// 0x00383454 | fefates:callgraph [tier C]
nn::nex::Protocol* nn::nex::ProtocolRegistry::GetProtocol(unsigned short id, nn::nex::Protocol::_Type type)
{
    ScopedCS lock(GetSchedulerLock());
    qMap<u16, Protocol*>* pProtocols = type == Protocol::TYPE_SERVER ? &m_ServerProtocols : &m_ClientProtocols;
    qMap<u16, Protocol*>::iterator it = pProtocols->find(id);
    if (it == pProtocols->end()) {
        return nullptr;
    }
    return it->second;
}

// 0x00383520 | fefates:bytes [tier B]
bool nn::nex::ServiceClient::RegisterProtocol(nn::nex::ClientProtocol* pProtocol)
{
    u16 id = pProtocol->m_ProtocolId;
    pProtocol->SetDefaultCredentials(m_pDefaultCredentials);
    pProtocol->m_ProtocolId = id;
    ProtocolRegistry* pRegistry = RVClientCore::GetInstance()->m_pProtocolRegistry;
    ClientProtocol* pResponder = pProtocol->CreateResponder();
    pResponder->m_ProtocolId = id;
    return pRegistry->RegisterProtocol(pResponder);
}

// 0x00383580
bool nn::nex::ProtocolRegistry::RegisterProtocol(nn::nex::Protocol* pProtocol)
{
    ScopedCS lock(GetSchedulerLock());
    qMap<u16, Protocol*>* pProtocols = pProtocol->GetProtocolType() == Protocol::TYPE_SERVER ? &m_ServerProtocols : &m_ClientProtocols;
    if (pProtocols->find(pProtocol->m_ProtocolId) != pProtocols->end()) {
        if (pProtocol != nullptr) {
            delete pProtocol;
        }
        return true;
    }
    pProtocols->IndexInline(pProtocol->m_ProtocolId) = pProtocol;
    return true;
}

// 0x0038378C | fefates:bytes [tier B]
bool nn::nex::ProtocolRegistry::RegisterProtocol(unsigned short id, nn::nex::Protocol* pProtocol)
{
    pProtocol->m_ProtocolId = id;
    return RegisterProtocol(pProtocol);
}

// 0x003CE6A0 (name is ours)
template <>
DECOMP_NOINLINE qMap<u16, Protocol*>::qMap()
{
    // nothing to do: the tree is constructed by the compiler
}

// 0x003CE73C (name is ours)
template <>
DECOMP_NOINLINE qMap<u16, Protocol*>::~qMap()
{
    // nothing to do: the tree is destroyed by the compiler
}

} // namespace nex
} // namespace nn
