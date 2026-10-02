#include "nn/os/os_TransferMemoryBlock.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
namespace {

// results (module os); the names are ours, the descriptions follow 3dbrew's result code table
const bit32 RESULT_ALREADY_INITIALIZED = 0x08A01BF9;   // info, invalid state, 1017
const bit32 RESULT_MISALIGNED_ADDRESS = 0xE0E01BF1;    // usage, wrong argument, 1009
const bit32 RESULT_MISALIGNED_SIZE = 0xE0E01BF2;       // usage, wrong argument, 1010
const bit32 RESULT_OUT_OF_ADDRESS_SPACE = 0xD8601837;  // permanent, out of resource, 55

const uptr PAGE_MASK = 0xFFF;
const u32 PERMISSION_WRITE = 2; // 3dbrew "MemoryPermission": R 1, W 2, X 4

} // namespace

// 0x0013085C | nintendogs:bytes [tier A]
void nn::os::TransferMemoryBlock::Initialize(void* address, size_t size, u32 myPermission, u32 otherPermission)
{
    nn::Result result = TryInitialize(address, size, myPermission, otherPermission);
    if (result.IsFailure()) {
        CTR::detail::HandleInternalError(result);
    }
}

// 0x00140634 | nintendogs:bytes [tier A]
void nn::os::TransferMemoryBlock::Finalize()
{
    if (!mHandle.IsValid()) {
        return;
    }
    if (mAddress != 0) {
        if (mIsMapped) {
            nn::svc::UnmapMemoryBlock(mHandle, mAddress);
            detail::FreeToSharedMemorySpace(this);
        } else {
            // our own memory: only forget it
            mAddress = 0;
            mSize = 0;
        }
    }
    Close();
}

// 0x0034C364 | nintendogs:callgraph [tier A]
// maps a block that another process created (handle) into the shared memory space
nn::Result nn::os::TransferMemoryBlock::AttachAndMap(nn::Handle handle, size_t size, u32 otherPermission, u32 myPermission)
{
    if (size & PAGE_MASK) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    mHandle = handle;
    if (mAddress != 0) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    uptr address = detail::AllocateFromSharedMemorySpace(this, size);
    if (address == 0) {
        return nn::Result(RESULT_OUT_OF_ADDRESS_SPACE);
    }
    mIsReadOnly = !(myPermission & PERMISSION_WRITE);
    nn::Result result = nn::svc::MapMemoryBlock(mHandle, address, myPermission, otherPermission);
    if (result.IsSuccess()) {
        mIsMapped = true;
        return result;
    }
    detail::FreeToSharedMemorySpace(this);
    return result;
}

// 0x0034C400 | fefates:bytes [tier B]
// creates a block over our own (page aligned) memory
nn::Result nn::os::TransferMemoryBlock::TryInitialize(void* address, size_t size, u32 myPermission, u32 otherPermission)
{
    if (mAddress != 0) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    if (reinterpret_cast<uptr>(address) & PAGE_MASK) {
        return nn::Result(RESULT_MISALIGNED_ADDRESS);
    }
    if (size & PAGE_MASK) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    nn::Handle handle;
    nn::Result result = nn::svc::CreateMemoryBlock(&handle, reinterpret_cast<uptr>(address), size, myPermission,
                                                   otherPermission);
    if (result.IsFailure()) {
        return result;
    }
    mHandle = handle;
    mAddress = reinterpret_cast<uptr>(address);
    mSize = size;
    return result;
}

// 0x0034C480
nn::os::TransferMemoryBlock::~TransferMemoryBlock()
{
    Finalize();
    // ~HandleObject closes the handle
}

} // namespace os
} // namespace nn
