#include "nn/os/os_ThreadLocalStorage.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/os/CTR/detail/detail_Api.h"

namespace nn {
namespace os {
namespace {

// all 16 slots are taken (level permanent, summary out of resource, module os, description 56;
// the name is ours)
const bit32 RESULT_OUT_OF_TLS_SLOTS = 0xD8601838;

// first free slot, or -1
s32 FindFreeSlot(u16 used)
{
    for (s32 i = 0; i < ThreadLocalStorage::MAX_SLOTS; i++) {
        if (!((used >> i) & 1)) {
            return i;
        }
    }
    return -1;
}

} // namespace

// 0x00975F94
u16 ThreadLocalStorage::s_UsedSlots;
// 0x00AE1F94
void (*ThreadLocalStorage::s_Destructors[ThreadLocalStorage::MAX_SLOTS])(uptr value);

// 0x00123D18 | nintendogs:bytes [tier A]
// a new thread starts with all slots 0
void nn::os::ThreadLocalStorage::ClearAllSlots()
{
    uptr* slots = detail::GetThreadLocalRegion();
#pragma GCC unroll 16   // ARMCC unrolls this loop completely
    for (s32 i = 0; i < MAX_SLOTS; i++) {
        slots[i] = 0;
    }
}

// 0x00139ABC | nintendogs:callgraph [tier A]
uptr nn::os::ThreadLocalStorage::GetValue() const
{
    return detail::GetThreadLocalRegion()[mIndex];
}

// 0x0013B0D8 | nintendogs:callgraph [tier A]
void nn::os::ThreadLocalStorage::SetValue(uptr value)
{
    detail::GetThreadLocalRegion()[mIndex] = value;
}

// 0x0013B0E8 | nintendogs:callgraph [tier A]
nn::os::ThreadLocalStorage::ThreadLocalStorage()
{
    s32 index = FindFreeSlot(s_UsedSlots);
    if (static_cast<u32>(index) < MAX_SLOTS) {
        s_UsedSlots |= 1 << index;
    }
    mIndex = index;
    if (index < 0) {
        CTR::detail::HandleInternalError(nn::Result(RESULT_OUT_OF_TLS_SLOTS));
    }
    s_Destructors[mIndex] = 0;
}

// 0x0034C228 | nintendogs:callgraph [tier A]
nn::os::ThreadLocalStorage::~ThreadLocalStorage()
{
    if (static_cast<u32>(mIndex) < MAX_SLOTS && ((s_UsedSlots >> mIndex) & 1)) {
        s_UsedSlots &= ~(1 << mIndex);
    }
}

} // namespace os
} // namespace nn
