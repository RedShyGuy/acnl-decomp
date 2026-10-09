#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace gxlow {
namespace CTR {
// The receiving side of the interrupt queue in the shared memory of the GPU module (3dbrew "GSP
// Shared Memory"): a header word (byte 0 the first entry, byte 1 the count, byte 2 set when
// interrupts were lost, byte 3 flags) and the interrupt ids from offset 0x0C. The members and the
// inline function are ours.
class InterruptRelayQueueRx
{
public:
    static const u32 CAPACITY = 52;

    InterruptRelayQueueRx() : m_pQueue(0) {}

    // the next interrupt id; RESULT_QUEUE_EMPTY when there is none (inline in ReceiverThreadFunc)
    nn::Result TryDequeue(u8* pId);

    // bit 0 of byte 3: the module does not relay the interrupts of the display (PDC)
    void SuppressPdcEvents(bool isSuppressed); // 0x00131474 | nintendogs:callgraph [tier A]

    nn::Handle m_Event;      // 0x0, signalled by the module
    volatile s32* m_pQueue;  // 0x4
};
ASSERT_SIZE(InterruptRelayQueueRx, 8);
} // namespace CTR
} // namespace gxlow
} // namespace nn
