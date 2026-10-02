#pragma once

#include "decomp.h"

namespace nn {
namespace os {
namespace detail {
void InvokeAllTlsDestructors();
} // namespace detail

// One slot of thread local storage: a word at the same place in every thread's local region
// (see os_ThreadLocalRegion.h). There are 16 slots for the whole process.
class ThreadLocalStorage
{
public:
    // number of slots (name is ours)
    static const s32 MAX_SLOTS = 16;

    static void ClearAllSlots(); // 0x00123D18 | nintendogs:bytes [tier A]
    uptr GetValue() const; // 0x00139ABC | nintendogs:callgraph [tier A]
    void SetValue(uptr value); // 0x0013B0D8 | nintendogs:callgraph [tier A]
    ThreadLocalStorage(); // 0x0013B0E8 | nintendogs:callgraph [tier A]
    ~ThreadLocalStorage(); // 0x0034C228 | nintendogs:callgraph [tier A]

private:
    friend void detail::InvokeAllTlsDestructors();

    s32 mIndex; // slot, -1 if none was free

    // names are ours
    static u16 s_UsedSlots;                                 // 0x00975F94, bit i: slot i is taken
    static void (*s_Destructors[MAX_SLOTS])(uptr value);   // 0x00AE1F94, called when a thread ends
};
ASSERT_SIZE(ThreadLocalStorage, 4);

} // namespace os
} // namespace nn
