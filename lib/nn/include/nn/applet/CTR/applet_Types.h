#pragma once

// The enum name is from the binary (nn::applet::CTR::detail::APPLET::GetApplicationRunningMode);
// one byte like all ARMCC enums. Values are not named yet: nn::os::CTR::IsRunningAsExtApplication
// checks for 2 and 4.

#include "types.h"

namespace nn {
namespace applet {
namespace CTR {

enum ApplicationRunningMode : u8 {
};

} // namespace CTR
} // namespace applet
} // namespace nn
