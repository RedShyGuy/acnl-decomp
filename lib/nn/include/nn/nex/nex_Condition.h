#pragma once

#include "decomp.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/os/os_LightEvent.h"

#include <new>

namespace nn {
namespace nex {
// A condition that threads wait for (a light event of the OS). The names of the members and of the
// inline functions are ours.
// RTTI N2nn3nex9ConditionE @ 0x008CF744
// vtable 0x008FFD04 (vptr 0x008FFD0C), offset_to_top 0, 2 entries
class Condition : public ::nn::nex::RootObject
{
public:
    static const u32 LOCK_LEVEL = 0x10000;

    // (inline)
    Condition() : m_Lock(LOCK_LEVEL), m_IsManualReset(false)
    {
        m_pEvent = new (MemoryManager::Allocate(sizeof(nn::os::LightEvent))) nn::os::LightEvent();
        m_WaiterCount = 0;
        m_pEvent->Initialize(m_IsManualReset);
    }
    // the waiters are woken until none is left
    virtual ~Condition(); // 0x003D6950 slot 0x00
    // 0x003D6920 slot 0x04 (deleting dtor)

    // (inline) wakes a waiter (all waiters of a manual reset event, which is not left signalled)
    void Signal()
    {
        m_pEvent->Signal();
        if (m_IsManualReset) {
            m_pEvent->ClearSignal();
        }
    }

    nn::os::LightEvent* m_pEvent;  // 0x04
    ForcedCriticalSection m_Lock;  // 0x08
    s32 m_WaiterCount;             // 0x30
    bool m_IsManualReset;          // 0x34
};
ASSERT_SIZE(Condition, 0x38);
} // namespace nex
} // namespace nn
