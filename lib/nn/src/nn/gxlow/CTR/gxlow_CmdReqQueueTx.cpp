#include "nn/gxlow/CTR/gxlow_CmdReqQueueTx.h"
#include "nn/gxlow/CTR/detail/detail_Api.h"
#include "nn/gxlow/CTR/gxlow_Gpu.h"
#include "nn/os/os_Atomic.h"

namespace nn {
namespace gxlow {
namespace CTR {
namespace {
// results (module 10; the names are ours)
const bit32 RESULT_QUEUE_NOT_INITIALIZED = 0xD8A02BF8; // permanent, invalid state, 1016
const bit32 RESULT_QUEUE_FULL = 0xD8A02A02;            // permanent, invalid state, 514

// the requests follow the header
const u32 REQUESTS_OFFSET = 0x20;

// the parts of the header word
inline u32 GetFirst(s32 header)
{
    return header & 0xFF;
}

inline u32 GetCount(s32 header)
{
    return (header >> 8) & 0xFF;
}

inline s32 SetCount(s32 header, u32 count)
{
    return (header & ~0xFF00) | ((count << 8) & 0xFF00);
}
} // namespace

// 0x00136E1C | fefates:bytes [tier B]
void nn::gxlow::CTR::CmdReqQueueTx::Initialize(void* pQueue)
{
    m_pQueue = static_cast<volatile s32*>(pQueue);
    // empty; the first request stays where it is if that is valid
    s32 header;
    do {
        header = nn::os::detail::LoadExclusive(m_pQueue);
        u32 first = GetFirst(header);
        if (first >= CAPACITY) {
            first = 0;
        }
        header = first;
    } while (nn::os::detail::StoreExclusive(m_pQueue, header));
}

// 0x0013B5C0 | nintendogs:callgraph [tier A]
nn::Result nn::gxlow::CTR::CmdReqQueueTx::TryEnqueue(const nn::gxlow::CTR::detail::CmdReq* pRequest)
{
    if (m_pQueue == 0) {
        return RESULT_QUEUE_NOT_INITIALIZED;
    }
    if (reinterpret_cast<volatile u8*>(m_pQueue)[1] >= CAPACITY) {
        return RESULT_QUEUE_FULL;
    }
    s32 header = nn::os::detail::LoadExclusive(m_pQueue);
    u32 count = GetCount(header);
    u32 slot = (GetFirst(header) + count) % CAPACITY;
    detail::CmdReq* entry = reinterpret_cast<detail::CmdReq*>(reinterpret_cast<uptr>(m_pQueue) + REQUESTS_OFFSET + slot * sizeof(detail::CmdReq));
    *entry = *pRequest;
    nn::os::detail::DataSynchronizationBarrier();
    header = SetCount(header, count + 1);
    while (nn::os::detail::StoreExclusive(m_pQueue, header)) {
        header = nn::os::detail::LoadExclusive(m_pQueue);
        header = SetCount(header, GetCount(header) + 1);
    }
    // the module waits for a trigger when the queue was empty
    if (GetCount(header) == 1) {
        detail::GetGpuIpc()->TriggerCmdReqQueue();
    }
    return nn::Result();
}

} // namespace CTR
} // namespace gxlow
} // namespace nn
