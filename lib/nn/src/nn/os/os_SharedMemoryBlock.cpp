#include "nn/os/os_SharedMemoryBlock.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
namespace {

// results (module os), as in os_TransferMemoryBlock.cpp; the names are ours
const bit32 RESULT_ALREADY_INITIALIZED = 0x08A01BF9;   // info, invalid state, 1017
const bit32 RESULT_MISALIGNED_SIZE = 0xE0E01BF2;       // usage, wrong argument, 1010
const bit32 RESULT_OUT_OF_ADDRESS_SPACE = 0xD8601837;  // permanent, out of resource, 55

const size_t PAGE_SIZE = 0x1000;
// 3dbrew "MemoryPermission"
const u32 PERMISSION_R = 1;
const u32 PERMISSION_RW = 3;
const u32 PERMISSION_DONT_CARE = 0x10000000;

} // namespace

// 0x0013B02C | nintendogs:bytes [tier A]
nn::Result nn::os::SharedMemoryBlock::AttachAndMap(nn::Handle handle, size_t size, bool readOnly)
{
    mHandle = handle;
    return Map((size + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE, readOnly);
}

// 0x0013B054 | nintendogs:bytes [tier A]
nn::Result nn::os::SharedMemoryBlock::Map(size_t size, bool readOnly)
{
    if (mAddress != 0) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    if (size & (PAGE_SIZE - 1)) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    uptr address = detail::AllocateFromSharedMemorySpace(this, size);
    if (address == 0) {
        return nn::Result(RESULT_OUT_OF_ADDRESS_SPACE);
    }
    mIsReadOnly = readOnly;
    nn::Result result = nn::svc::MapMemoryBlock(mHandle, address, readOnly ? PERMISSION_R : PERMISSION_RW,
                                                PERMISSION_DONT_CARE);
    if (result.IsSuccess()) {
        mIsMapped = true;
    }
    return result;
}

// 0x0034C190 | nintendogs:bytes [tier A]
void nn::os::SharedMemoryBlock::Finalize()
{
    if (!mHandle.IsValid()) {
        return;
    }
    if (mAddress != 0) {
        if (mIsMapped) {
            nn::Result result = nn::svc::UnmapMemoryBlock(mHandle, mAddress);
            if (result.IsFailure()) {
                CTR::detail::HandleInternalError(result);
            }
            detail::FreeToSharedMemorySpace(this);
        } else {
            mAddress = 0;
            mSize = 0;
        }
    }
    Close();
}

// 0x0034C1FC
nn::os::SharedMemoryBlock::~SharedMemoryBlock()
{
    Finalize();
    // ~HandleObject closes the handle
}

} // namespace os
} // namespace nn
