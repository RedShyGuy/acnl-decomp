#pragma once

// The thread local region: a per thread memory block whose address the kernel keeps in the
// CP15 register TPIDRURO (c13, c0, 3). It starts with the slots of ThreadLocalStorage; the IPC
// command buffer is at +0x80. The names in this header are ours.

#include "types.h"

namespace nn {
namespace os {
namespace detail {

inline uptr* GetThreadLocalRegion()
{
    uptr tlr;
    __asm__ __volatile__("mrc p15, 0, %0, c13, c0, 3" : "=r"(tlr));
    return reinterpret_cast<uptr*>(tlr);
}

// the IPC command buffer of the thread (thread local region + 0x80): request and reply of
// svc SendSyncRequest; word 0 is the header (command id << 16 | parameter counts)
inline bit32* GetIpcCommandBuffer()
{
    return reinterpret_cast<bit32*>(GetThreadLocalRegion() + 0x80 / sizeof(uptr));
}

// the receive buffers of the thread for IPC replies (thread local region + 0x180): pairs of
// static buffer descriptor and address; a command that receives a static buffer sets pair 0
inline bit32* GetIpcStaticBuffers()
{
    return reinterpret_cast<bit32*>(GetThreadLocalRegion() + 0x180 / sizeof(uptr));
}

} // namespace detail
} // namespace os
} // namespace nn
