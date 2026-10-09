#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/os/os_HandleObject.h"
#include "nn/os/os_MemoryBlockBase.h"

namespace nn {
namespace os {

// Memory shared with another process through a kernel memory block.
// The owner creates it over its own memory (Initialize / TryInitialize); the other side maps the
// handle it got into its shared memory space (AttachAndMap). Member names are ours.
class TransferMemoryBlock : public MemoryBlockBase, public HandleObject
{
public:
    TransferMemoryBlock() : mIsMapped(false) {}
    void Initialize(void* address, size_t size, u32 myPermission, u32 otherPermission); // 0x0013085C | nintendogs:bytes [tier A]
    void Finalize(); // 0x00140634 | nintendogs:bytes [tier A]
    nn::Result AttachAndMap(nn::Handle handle, size_t size, u32 otherPermission, u32 myPermission); // 0x0034C364 | nintendogs:callgraph [tier A]
    nn::Result TryInitialize(void* address, size_t size, u32 myPermission, u32 otherPermission); // 0x0034C400 | fefates:bytes [tier B]
    ~TransferMemoryBlock(); // 0x0034C480

private:
    // mHandle (HandleObject)  // 0x14
    bool mIsMapped;            // 0x18, attached and mapped (AttachAndMap), not created here

    // layout checks (inside the class because the members are private; generates no code)
    static void CheckLayout()
    {
        ASSERT_OFFSET(TransferMemoryBlock, mHandle, 0x14);
        ASSERT_OFFSET(TransferMemoryBlock, mIsMapped, 0x18);
        ASSERT_SIZE(TransferMemoryBlock, 0x1C);
    }
};

} // namespace os
} // namespace nn
