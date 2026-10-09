#include "nn/hid/CTR/hid_HidDevices.h"
#include <string.h>
#include "nn/dbg/dbg_Api.h"
#include "nn/hid/CTR/detail/hid_Ipc.h"
#include "nn/hidlow/CTR/hidlow_AccelerometerLifoRing.h"
#include "nn/hidlow/CTR/hidlow_GyroscopeLowLifoRing.h"
#include "nn/hidlow/CTR/hidlow_PadLifoRing.h"
#include "nn/hidlow/CTR/hidlow_TouchPanelLifoRing.h"
#include "nn/srv/srv_Api.h"

namespace nn {
namespace hid {
namespace CTR {
namespace {
// the shared memory and its rings (3dbrew "HID Shared Memory")
const size_t SHARED_MEMORY_SIZE = 0x2B0;
const uptr OFFSET_PAD = 0x0;
const uptr OFFSET_TOUCH_PANEL = 0xA8;
const uptr OFFSET_ACCELEROMETER = 0x108;
const uptr OFFSET_GYROSCOPE = 0x158;
const uptr OFFSET_DEBUG_PAD = 0x238;

// usage, invalid state, module 19 hid, already initialized
const bit32 RESULT_ALREADY_INITIALIZED = 0xE0A04FF9;
const bit32 DESCRIPTION_ALREADY_INITIALIZED = 1017;
} // namespace

// (name is ours)
// 0x0097FA60
bool s_IsInitialized;

// 0x003525EC | nintendogs:bytes [tier A]
nn::Result nn::hid::CTR::HidDevices::Initialize(const char* serviceName)
{
    nn::Handle sharedMemory;
    nn::Handle padEvent;
    nn::Handle touchPanelEvent;
    nn::Handle accelerometerEvent;
    nn::Handle gyroscopeEvent;
    nn::Handle debugPadEvent;
    if (s_IsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    nn::Result result = nn::srv::Initialize();
    if (result.GetDescription() != DESCRIPTION_ALREADY_INITIALIZED && result.IsFailure()) {
        nndbgPanic();
    }
    if (nn::srv::GetServiceHandle(&detail::s_Session, serviceName, strlen(serviceName), 0).IsFailure()) {
        nndbgPanic();
    }
    if (detail::Ipc::GetIPCHandles(&sharedMemory, &padEvent, &touchPanelEvent, &accelerometerEvent, &gyroscopeEvent, &debugPadEvent).IsFailure()) {
        nndbgPanic();
    }
    m_SharedMemory.AttachAndMap(sharedMemory, SHARED_MEMORY_SIZE, true);
    uptr address = m_SharedMemory.GetAddress();
    m_Pad.m_pRing = reinterpret_cast<nn::hidlow::CTR::PadLifoRing*>(address + OFFSET_PAD);
    m_Gyroscope.m_pRing = reinterpret_cast<nn::hidlow::CTR::GyroscopeLowLifoRing*>(address + OFFSET_GYROSCOPE);
    m_TouchPanel.m_pRing = reinterpret_cast<nn::hidlow::CTR::TouchPanelLifoRing*>(address + OFFSET_TOUCH_PANEL);
    m_Accelerometer.m_pRing = reinterpret_cast<nn::hidlow::CTR::AccelerometerLifoRing*>(address + OFFSET_ACCELEROMETER);
    m_DebugPad.m_pRing = reinterpret_cast<void*>(address + OFFSET_DEBUG_PAD);
    m_Pad.SetEventHandle(padEvent);
    m_TouchPanel.SetEventHandle(touchPanelEvent);
    m_Accelerometer.SetEventHandle(accelerometerEvent);
    m_Gyroscope.SetEventHandle(gyroscopeEvent);
    m_DebugPad.SetEventHandle(debugPadEvent);
    s_IsInitialized = true;
    return nn::Result();
}

// 0x00352714 | nintendogs:bytes [tier A]
nn::hid::CTR::HidDevices::~HidDevices()
{
}

} // namespace CTR
} // namespace hid
} // namespace nn
