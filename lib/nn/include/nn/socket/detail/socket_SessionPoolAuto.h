#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_IAllocator.h"
#include "nn/socket/detail/socket_SessionPool.h"

#include <new>
#include <string.h>

namespace nn {
namespace socket {
namespace detail {
// RTTI N2nn6socket6detail15SessionPoolAutoE @ 0x008D0508
// vtable 0x00902338 (vptr 0x00902340), offset_to_top 0, 1 entries
//
// A SessionPool that gets its items from an allocator. The members and the inline functions are
// ours.
class SessionPoolAuto : public ::nn::socket::detail::SessionPool
{
public:
    // inline (in __sti___21_socket_IpcWrapper_cpp)
    SessionPoolAuto() : mItems(0), mAllocator(0) {}
    ~SessionPoolAuto(); // 0x00486C84 | fefates:bytes [tier B]

    // inline (in InitializeSessionPool)
    nn::Result Initialize(nn::fnd::IAllocator& allocator, s32 count, const char* name)
    {
        size_t nameLength = strlen(name);
        mItems = static_cast<SessionItem*>(allocator.Allocate(count * sizeof(SessionItem), 4));
        if (mItems == 0) {
            return nn::Result(RESULT_OUT_OF_MEMORY);
        }
        new (mItems) SessionItem[count];
        mAllocator = &allocator;
        nn::Result result = SessionPool::Initialize(mItems, count, name, nameLength);
        if (result.IsFailure()) {
            allocator.Free(mItems);
            mItems = 0;
        }
        return result;
    }

    // inline (in FinalizeSessionPool)
    void Finalize()
    {
        SessionPool::Finalize();
        mAllocator->Free(mItems);
        mItems = 0;
        mAllocator = 0;
    }

    // the memory the items of count sessions need (inline)
    static size_t GetRequiredMemorySize(s32 count) { return count * sizeof(SessionItem); }

    static const bit32 RESULT_OUT_OF_MEMORY = 0xD86073F3;  // permanent, out of resource, socket, 1011

private:
    SessionItem* mItems;                // 0x44
    nn::fnd::IAllocator* mAllocator;    // 0x48
};
ASSERT_SIZE(SessionPoolAuto, 0x4C);
} // namespace detail
} // namespace socket
} // namespace nn
