#include "nn/hid/CTR/detail/hid_Ipc.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace hid {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "HID Services")
const bit32 COMMAND_GET_IPC_HANDLES = 0x000A0000;
const bit32 COMMAND_ENABLE_ACCELEROMETER = 0x00110000;
const bit32 COMMAND_DISABLE_ACCELEROMETER = 0x00120000;
const bit32 COMMAND_DISABLE_GYROSCOPE_LOW = 0x00140000;

inline nn::Result SendCommand(bit32 header)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}
} // namespace

// 0x00982ECC
nn::Handle s_Session;

// 0x00354578 | nintendogs:bytes [tier A]
nn::Result nn::hid::CTR::detail::Ipc::GetIPCHandles(nn::Handle* pSharedMemory, nn::Handle* pPadEvent, nn::Handle* pTouchPanelEvent, nn::Handle* pAccelerometerEvent, nn::Handle* pGyroscopeEvent, nn::Handle* pDebugPadEvent)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_IPC_HANDLES;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pSharedMemory = nn::Handle(command[3]);
    *pPadEvent = nn::Handle(command[4]);
    *pTouchPanelEvent = nn::Handle(command[5]);
    *pAccelerometerEvent = nn::Handle(command[6]);
    *pGyroscopeEvent = nn::Handle(command[7]);
    *pDebugPadEvent = nn::Handle(command[8]);
    return nn::Result(command[1]);
}

// 0x003545F0 | tier C
nn::Result nn::hid::CTR::detail::Ipc::DisableGyroscopeLow()
{
    return SendCommand(COMMAND_DISABLE_GYROSCOPE_LOW);
}

// 0x00354620 | nintendogs:bytes [tier B]
nn::Result nn::hid::CTR::detail::Ipc::EnableAccelerometer()
{
    return SendCommand(COMMAND_ENABLE_ACCELEROMETER);
}

// 0x00354650 (name after 3dbrew)
nn::Result nn::hid::CTR::detail::Ipc::DisableAccelerometer()
{
    return SendCommand(COMMAND_DISABLE_ACCELEROMETER);
}

} // namespace detail
} // namespace CTR
} // namespace hid
} // namespace nn
