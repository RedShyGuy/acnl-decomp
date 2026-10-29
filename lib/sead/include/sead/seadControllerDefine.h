#pragma once

#include "decomp.h"

namespace sead {
// The ids of the controller classes (the namespace and DeviceId are from the reference symbols, the
// values and the other names are ours; only the ones the binary uses)
namespace ControllerDefine {
// the input devices of the controller manager
enum DeviceId : u8
{
    DEVICE_ID_CTR_HID = 2,
};
// the controllers
enum ControllerId : u8
{
    CONTROLLER_ID_NULL = 0,
    CONTROLLER_ID_CTR = 2,
};
// the addons of a controller
enum AddonId : u8
{
    ADDON_ID_ACCELEROMETER = 1,
};
} // namespace ControllerDefine
} // namespace sead
