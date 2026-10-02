#include "nn/os/os_Api.h"
#include "nn/os/CTR/MPCore/MPCore_Api.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/os/os_WaitableCounter.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
namespace {

const uptr HEAP_BEGIN = 0x08000000;
const uptr PAGE_MASK = 0xFFF;
const uptr DEVICE_MEMORY_STEP_MASK = 0xFFFFF;

// ControlMemory (3dbrew "MemoryOperation" / "MemoryPermission")
const u32 MEMORY_OPERATION_FREE = 1;
const u32 MEMORY_OPERATION_COMMIT = 3;
const u32 MEMORY_OPERATION_LINEAR = 0x10000;
const u32 MEMORY_PERMISSION_RW = 3;

// resource limit "commit" (3dbrew "ResourceLimitType")
const u32 RESOURCE_LIMIT_COMMIT = 1;
// GetHandleInfo of a process: when it was created
const u32 HANDLE_INFO_CREATION_TIME = 0;

// results (module os); the names are ours
const bit32 RESULT_MISALIGNED_SIZE = 0xE0E01BF2; // usage, wrong argument, 1010

} // namespace

// 0x00975F74 (names are ours)
uptr s_DeviceMemoryAddress;
// 0x00975F78
size_t s_DeviceMemorySize;
// 0x00975F7C
size_t s_HeapSize;

// 0x0011D2B8 | nintendogs:bytes [tier A]
nn::Result SetHeapSize(size_t size)
{
    if (size & PAGE_MASK) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    u32 address;
    nn::Result result;
    if (size > s_HeapSize) {
        result = nn::svc::ControlMemory(&address, HEAP_BEGIN + s_HeapSize, 0, size - s_HeapSize,
                                        MEMORY_OPERATION_COMMIT, MEMORY_PERMISSION_RW);
    } else {
        result = nn::svc::ControlMemory(&address, HEAP_BEGIN + size, 0, s_HeapSize - size, MEMORY_OPERATION_FREE, 0);
    }
    if (result.IsSuccess()) {
        s_HeapSize = size;
    }
    return result;
}

// 0x0011D360 | nintendogs:bytes [tier A]
size_t GetUsingMemorySize()
{
    nn::Handle resourceLimit;
    u32 name = RESOURCE_LIMIT_COMMIT;
    nn::svc::GetResourceLimit(&resourceLimit, nn::Handle(PSEUDO_HANDLE_CURRENT_PROCESS));
    s64 value;
    nn::svc::GetResourceLimitCurrentValues(&value, resourceLimit, &name, 1);
    nn::svc::CloseHandle(resourceLimit);
    return static_cast<size_t>(value);
}

// 0x0011D3B4 | nintendogs:bytes [tier A]
nn::Result SetDeviceMemorySize(size_t size)
{
    if (size & PAGE_MASK) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    u32 address;
    if (size > s_DeviceMemorySize) {
        if (s_DeviceMemorySize != 0 && ((s_DeviceMemorySize | size) & DEVICE_MEMORY_STEP_MASK)) {
            return nn::Result(RESULT_MISALIGNED_SIZE);
        }
        nn::Result result = nn::svc::ControlMemory(
            &address, s_DeviceMemorySize ? s_DeviceMemoryAddress + s_DeviceMemorySize : 0, 0, size - s_DeviceMemorySize,
            MEMORY_OPERATION_COMMIT | MEMORY_OPERATION_LINEAR, MEMORY_PERMISSION_RW);
        if (result.IsFailure()) {
            return result;
        }
        if (s_DeviceMemorySize == 0) {
            s_DeviceMemoryAddress = address;
        }
        s_DeviceMemorySize = size;
        return result;
    }
    nn::Result result = nn::svc::ControlMemory(&address, s_DeviceMemoryAddress + size, 0, s_DeviceMemorySize - size,
                                               MEMORY_OPERATION_FREE, 0);
    if (result.IsFailure()) {
        return result;
    }
    if (size == 0) {
        s_DeviceMemoryAddress = 0;
    }
    s_DeviceMemorySize = size;
    return result;
}

// 0x0011DF04 | fefates:bytes [tier B]
void Initialize()
{
    WaitableCounter::Initialize();
    detail::SaveThreadLocalRegionAddress();
    detail::InitializeSharedMemory();
    detail::InitializeStackMemory();
    detail::InitializeThreadEnvrionment();
    CTR::MPCore::InitializeDeviceAddress();
}

// 0x0013E878 | nintendogs:callgraph [tier A]
size_t GetDeviceMemorySize()
{
    return s_DeviceMemorySize;
}

// 0x00140690 | nintendogs:callgraph [tier A]
uptr GetDeviceMemoryAddress()
{
    return s_DeviceMemoryAddress;
}

// 0x0034C074 | fefates:bytes [tier B]
nn::os::Tick GetCreationTime()
{
    s64 time;
    nn::svc::GetHandleInfo(&time, nn::Handle(PSEUDO_HANDLE_CURRENT_PROCESS), HANDLE_INFO_CREATION_TIME);
    return nn::os::Tick(time);
}

} // namespace os
} // namespace nn
