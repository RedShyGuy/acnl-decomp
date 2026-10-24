#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// The dispose callbacks (the class and function names are from the reference symbols, the member is
// ours).
class DisposeCallbackManager
{
public:
    typedef ut::LinkList<DisposeCallback, 4> CallbackList;

    static DisposeCallbackManager& GetInstance(); // 0x004D3A60 | fefates:bytes [tier B]

    void RegisterDisposeCallback(DisposeCallback* pCallback); // 0x004D3AB4 | fefates:bytes [tier B]
    void UnregisterDisposeCallback(DisposeCallback* pCallback); // 0x004D3AC4 | fefates:bytes [tier B]
    // the callbacks are told that the memory goes away
    void Dispose(const void* pMemory, u32 size); // 0x004D3ACC | fefates:bytes [tier B]

    CallbackList m_CallbackList; // 0x0
};
ASSERT_SIZE(DisposeCallbackManager, 0xC);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
