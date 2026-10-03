#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_IpcSession.h"
#include "nn/ssl/ssl_ConnectionIpc.h"

namespace nn {
namespace ssl {
namespace detail {
// RTTI N2nn3ssl6detail10LibManagerE @ 0x008D030C
// vtable 0x009020CC (vptr 0x009020D4), offset_to_top 0, 2 entries
//
// The state of the library (one global object, made by __sti___18_ssl_CommonImpl_cpp):
// Initialize / Finalize count, the first one connects to ssl:C. The members are ours and public
// (the functions of nn::ssl use them).
class LibManager
{
public:
    // inline (in __sti___18_ssl_CommonImpl_cpp)
    LibManager() : mLock(nn::os::CriticalSection::InitializeTag()), mInitializeCount(0) {}
    virtual ~LibManager(); // 0x00467384 slot 0x00, 0x00467348 slot 0x04 (deleting) | fefates:bytes

    nn::os::CriticalSection mLock;      // 0x04
    s32 mInitializeCount;               // 0x10
    nn::os::ipc::Session mSession;      // 0x14 ssl:C
    nn::ssl::ConnectionIpc mConnection; // 0x18 on the same handle
};
ASSERT_SIZE(LibManager, 0x1C);

extern LibManager s_LibManager; // 0x00AEEA58 (name is ours)

// "not initialized" (permanent, invalid state, module 46, 1016; name is ours)
const bit32 RESULT_NOT_INITIALIZED = 0xD8A0BBF8;
} // namespace detail
} // namespace ssl
} // namespace nn
