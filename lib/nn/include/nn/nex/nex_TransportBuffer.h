#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qReservedQueue.h"

namespace nn {
namespace nex {
class Buffer;

// A pool of buffers of the same size that the transport buffer sets of the sockets share. The
// class name is from the reference symbols; the names of the members and of the functions without
// a reference name are ours.
class TransportBuffer : public ::nn::nex::RootObject
{
public:
    static const u32 LOCK_LEVEL = 0x200000;

    TransportBuffer(unsigned int count, unsigned int bufferSize); // 0x0037E580 | fefates:callgraph [tier C]
    ~TransportBuffer(); // 0x0037E6BC

    qReservedQueue<Buffer*>* m_pBuffers; // 0x00 (the free buffers)
    ForcedCriticalSection m_Lock;        // 0x04
    u32 m_FailureCount;                  // 0x2C (no free buffer)
};
ASSERT_SIZE(TransportBuffer, 0x30);
} // namespace nex
} // namespace nn
