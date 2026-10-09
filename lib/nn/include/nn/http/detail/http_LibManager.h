#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/http/http_ConnectionIpc.h"
#include "nn/os/os_IpcSession.h"
#include "nn/os/os_TransferMemoryBlock.h"

namespace nn {
namespace http {
namespace detail {
// RTTI N2nn4http6detail10LibManagerE @ 0x008D03BC
// vtable 0x009021FC (vptr 0x00902204), offset_to_top 0, 2 entries
// The session of the library and the memory it shares with http (one object; member names are
// ours).
class LibManager
{
public:
    LibManager() : m_IsInitialized(false) {}
    // 0x0046FE70 slot 0x00 (deleting dtor 0x0046FE14)
    virtual ~LibManager();
    // connects to http:C and shares size bytes at buffer (none for size 0)
    nn::Result Initialize(uptr buffer, size_t size); // 0x0046FD10 | fefates:bytes [tier B]

    bool m_IsInitialized;                        // 0x04
    nn::os::TransferMemoryBlock m_TransferMemory; // 0x08
    u32 m_Reserved24;                            // 0x24, never used
    nn::os::ipc::Session m_Session;              // 0x28
    ConnectionIpc m_Ipc;                         // 0x2C
};
ASSERT_SIZE(LibManager, 0x30);

// (name is ours)
extern LibManager s_LibManager;
} // namespace detail
} // namespace http
} // namespace nn
