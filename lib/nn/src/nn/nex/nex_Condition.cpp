#include "nn/nex/nex_Condition.h"
#include "nn/os/os_Api.h"

namespace nn {
namespace nex {
namespace {
// the time between the wakings of the waiters
const s64 WAIT_INTERVAL_NS = 1000000;
} // namespace

// 0x003D6950 slot 0x00
// 0x003D6920 (deleting dtor)
nn::nex::Condition::~Condition()
{
    s32 count = m_WaiterCount;
    while (count > 0) {
        for (s32 i = 0; i < count; i++) {
            Signal();
        }
        if (m_WaiterCount > 0) {
            OsSleepThread(WAIT_INTERVAL_NS);
        }
        count = m_WaiterCount;
    }
    MemoryManager::FreeInline(m_pEvent);
}

} // namespace nex
} // namespace nn
