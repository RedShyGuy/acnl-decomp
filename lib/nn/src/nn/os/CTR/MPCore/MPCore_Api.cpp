#include "nn/os/CTR/MPCore/MPCore_Api.h"
#include "nn/os/CTR/CTR_Api.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
namespace CTR {
namespace MPCore {
namespace {

// GetProcessInfo: the difference between the virtual and the physical address of linear memory
const u32 PROCESS_INFO_LINEAR_OFFSET = 20;

} // namespace

// 0x00982628 (names are ours)
uptr s_WramAddressOffset;
// 0x0098262C
uptr s_DeviceAddressOffset;

// 0x0011F6B8 | fefates:bytes [tier B]
void InitializeDeviceAddress()
{
    s64 offset;
    nn::Result result = nn::svc::GetProcessInfo(&offset, nn::Handle(PSEUDO_HANDLE_CURRENT_PROCESS),
                                                PROCESS_INFO_LINEAR_OFFSET);
    if (result.IsFailure()) {
        CTR::detail::HandleInternalError(result);
    }
    s_DeviceAddressOffset = static_cast<uptr>(offset);
}

// 0x00143184 | fefates:bytes [tier B]
uptr ConvertAddressForWram(uptr address, size_t size)
{
    uptr end = address + size;
    uptr wram = CTR::GetWramAddress();
    if (wram <= address && address < end && end <= wram + CTR::GetWramSize()) {
        return s_WramAddressOffset + address;
    }
    return 0;
}

// 0x001431D0 | fefates:bytes [tier B]
uptr ConvertAddressForDevice(uptr address, size_t size)
{
    uptr end = address + size;
    uptr device = GetDeviceMemoryAddress();
    uptr deviceEnd = device + GetDeviceMemorySize();
    if (device <= address && address <= deviceEnd && device <= end && end <= deviceEnd && address < end) {
        return s_DeviceAddressOffset + address;
    }
    return 0;
}

} // namespace MPCore
} // namespace CTR
} // namespace os
} // namespace nn
