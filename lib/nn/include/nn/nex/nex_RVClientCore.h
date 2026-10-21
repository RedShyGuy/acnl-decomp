#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocolRequestBroker.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_InstanceTable.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class GlobalNotificationEventManager;
class ProtocolRegistry;
class ServerProtocol;

// The client core of a context (a pseudo singleton): the protocol registry, the global
// notification event manager and the server protocols of the clients. The names of the members and
// of the functions without a reference name are ours.
// RTTI N2nn3nex12RVClientCoreE @ 0x008CE168
// vtable 0x008FC624 (vptr 0x008FC62C), offset_to_top 0, 2 entries
class RVClientCore : public ::nn::nex::RefCountedObject
{
public:
    // the kind of the pseudo singleton
    static const u32 KIND = 12;
    // the protocol id of the global notification event manager
    static const u16 NOTIFICATION_PROTOCOL_ID = 14;

    RVClientCore(); // 0x0035EBD8
    virtual ~RVClientCore(); // 0x0035EDC8 slot 0x00
    // 0x0035ED48 slot 0x04 | fefates:bytes (deleting dtor)
    static RVClientCore* GetInstance(); // 0x0035E8F4 | fefates:callgraph [tier C]
    // (where the original inlines GetInstance)
    static RVClientCore* GetInstanceInline()
    {
        InstanceControl* pInstance = InstanceTable::s_Instance.GetInstance(KIND, PseudoSingleton::GetCurrentContext());
        return pInstance != nullptr ? static_cast<RVClientCore*>(pInstance->m_pOwner) : nullptr;
    }
    // the server protocol with the id (null if none)
    nn::nex::ServerProtocol* FindProtocol(unsigned short id); // 0x0035EA4C | fefates:bytes [tier A]
    // false if there is one with the id already
    bool RegisterServerProtocol(unsigned short id, nn::nex::ServerProtocol* pProtocol); // 0x0035EB0C (name is ours)

    // (inline) creates the instance or takes a reference to it
    static void AcquireInstance()
    {
        if (GetInstance() != nullptr) {
            GetInstance()->AcquireRef();
            return;
        }
        CreateInstance();
    }
    // (inline) the instance is created or referenced under the lock of the instances
    static void CreateInstance()
    {
        Core::AcquireInstance();
        ScopedCS lock(InstanceTable::s_Lock);
        if (GetInstance() != nullptr) {
            GetInstance()->AcquireRef();
        } else {
            new RVClientCore();
        }
    }
    // (inline) the reference of CreateInstance is released
    static void ReleaseInstance()
    {
        {
            ScopedCS lock(InstanceTable::s_Lock);
            GetInstance()->ReleaseRef();
        }
        Core::ReleaseInstance();
    }

    ProtocolRegistry* m_pProtocolRegistry;                  // 0x0C
    PseudoSingleton m_Singleton;                            // 0x10
    GlobalNotificationEventManager* m_pNotificationManager; // 0x24
    qMap<u16, ServerProtocol*> m_ServerProtocols;           // 0x28
    u8 m_Unknown0x44[0xC];                                  // 0x44
    // the local time and the server time (seconds since the epoch) of the last login
    Time m_LoginTime;                                       // 0x50
    u64 m_ServerTime;                                       // 0x58
};
ASSERT_OFFSET(RVClientCore, m_ServerProtocols, 0x28);
ASSERT_SIZE(RVClientCore, 0x60);
} // namespace nex
} // namespace nn
