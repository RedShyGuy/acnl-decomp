#pragma once

// The enum name is from the binary (nn::applet::CTR::detail::GetTargetPlatform); like all ARMCC
// enums it takes the smallest type that holds its values (one byte). The value names are ours.

#include "types.h"

namespace nn {
namespace ptm {
namespace CTR {

enum TargetPlatform : u8 {
    TARGET_PLATFORM_CTR = 0,    // Nintendo 3DS
    // anything else: New Nintendo 3DS ("SNAKE", nn::os::CTR::IsRunOnSnake)
};

} // namespace CTR
} // namespace ptm
} // namespace nn
