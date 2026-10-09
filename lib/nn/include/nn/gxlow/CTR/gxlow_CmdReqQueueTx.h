#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace gxlow {
namespace CTR {
namespace detail {
struct CmdReq;
}

// The sending side of the command queue in the shared memory of the GPU module (3dbrew "GSP
// Shared Memory"): a header word (byte 0 the first request, byte 1 the count, bytes 2/3 flags)
// and 15 requests from offset 0x20. The member name is ours.
class CmdReqQueueTx
{
public:
    static const u32 CAPACITY = 15;

    CmdReqQueueTx() : m_pQueue(0) {}

    void Initialize(void* pQueue); // 0x00136E1C | fefates:bytes [tier B]
    // copies the request into the queue and wakes the module if the queue was empty
    nn::Result TryEnqueue(const nn::gxlow::CTR::detail::CmdReq* pRequest); // 0x0013B5C0 | nintendogs:callgraph [tier A]

    volatile s32* m_pQueue; // 0x0
};
ASSERT_SIZE(CmdReqQueueTx, 4);
} // namespace CTR
} // namespace gxlow
} // namespace nn
