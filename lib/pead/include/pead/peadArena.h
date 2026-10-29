#pragma once

#include "decomp.h"

namespace pead {
// The memory the root heap is made in (pia's copy of sead::Arena; pia's common::HeapManager::Initialize fills
// one and passes it to HeapMgr::initialize; the names are ours).
class Arena
{
public:
    Arena(); // 0x0053B89C (name after sead)
    ~Arena(); // 0x0053B8B0 (name after sead)
    // forgets the memory (HeapMgr::destroy)
    void destroy(); // 0x0053B87C (name is ours)

    // sead sets it when the device memory is used; pead only clears it (name after sead)
    static bool s_IsDeviceMemoryUsed;

    void* mStart;       // 0x0
    u32 mSize;          // 0x4
    bool mInitialized;  // 0x8
};
ASSERT_SIZE(Arena, 0xC);
} // namespace pead
