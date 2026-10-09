#include "nn/hid/CTR/hid_Api.h"
#include "nn/hid/CTR/hid_HidDevices.h"

namespace nn {
namespace hid {
namespace CTR {
namespace {
// the services of hid (the program uses hid:USER)
// 0x008C23F0
const char* const SERVICE_NAMES[] = { NULL, "hid:SPVR", "hid:USER", "hid:QTM" };
const int SERVICE_USER = 2;
} // namespace

// 0x00AF61B8
HidDevices s_HidDevices;

// 0x003527BC | tier C
nn::Result Initialize()
{
    return s_HidDevices.Initialize(SERVICE_NAMES[SERVICE_USER]);
}

// 0x003527D4 | tier C
TouchPanel& GetTouchPanel()
{
    return s_HidDevices.m_TouchPanel;
}

// 0x00353828 | tier C
Accelerometer& GetAccelerometer()
{
    return s_HidDevices.m_Accelerometer;
}

// 0x00354518 | tier C
Pad& GetPad()
{
    return s_HidDevices.m_Pad;
}

} // namespace CTR
} // namespace hid
} // namespace nn
