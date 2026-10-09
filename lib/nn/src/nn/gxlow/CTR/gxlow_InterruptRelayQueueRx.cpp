#include "nn/gxlow/CTR/gxlow_InterruptRelayQueueRx.h"
#include "nn/os/os_Atomic.h"

namespace nn {
namespace gxlow {
namespace CTR {
namespace {
// byte 3 of the header
const u32 FLAG_SUPPRESS_PDC_EVENTS = 1 << 0;
} // namespace

// 0x00131474 | nintendogs:callgraph [tier A]
void nn::gxlow::CTR::InterruptRelayQueueRx::SuppressPdcEvents(bool isSuppressed)
{
    if (m_pQueue == 0) {
        return;
    }
    s32 header;
    do {
        header = nn::os::detail::LoadExclusive(m_pQueue);
        u32 flags = static_cast<u32>(header) >> 24;
        if (isSuppressed) {
            flags |= FLAG_SUPPRESS_PDC_EVENTS;
        } else {
            flags &= ~FLAG_SUPPRESS_PDC_EVENTS;
        }
        header = (header & 0x00FFFFFF) | (flags << 24);
    } while (nn::os::detail::StoreExclusive(m_pQueue, header));
}

} // namespace CTR
} // namespace gxlow
} // namespace nn
