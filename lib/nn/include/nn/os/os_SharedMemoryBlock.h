#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/os/os_HandleObject.h"
#include "nn/os/os_MemoryBlockBase.h"

namespace nn {
namespace os {

// A shared memory block that another process (a system module) created: mapped by handle into
// the shared memory space. Same layout as TransferMemoryBlock; member names are ours.
class SharedMemoryBlock : public MemoryBlockBase, public HandleObject
{
public:
    // size is rounded up to whole pages
    nn::Result AttachAndMap(nn::Handle handle, size_t size, bool readOnly); // 0x0013B02C | nintendogs:bytes [tier A]
    nn::Result Map(size_t size, bool readOnly); // 0x0013B054 | nintendogs:bytes [tier A]
    void Finalize(); // 0x0034C190 | nintendogs:bytes [tier A]
    ~SharedMemoryBlock(); // 0x0034C1FC

private:
    // mHandle (HandleObject)  // 0x14
    bool mIsMapped;            // 0x18

    // layout checks (inside the class because the members are private; generates no code)
    static void CheckLayout()
    {
        ASSERT_OFFSET(SharedMemoryBlock, mHandle, 0x14);
        ASSERT_OFFSET(SharedMemoryBlock, mIsMapped, 0x18);
        ASSERT_SIZE(SharedMemoryBlock, 0x1C);
    }
};

} // namespace os
} // namespace nn
