#pragma once

// Types of nn::cfg. The type names are from the binary; members are ours.

#include "decomp.h"

namespace nn {
namespace cfg {
namespace CTR {

// the user name of the system settings (first 24 bytes of config block 0xA0000)
struct UserName
{
    u16 name[11];       // 0x00 UTF-16, 10 characters and the terminator
    u8 unknown16[2];    // 0x16
};
ASSERT_SIZE(UserName, 0x18);

} // namespace CTR
} // namespace cfg
} // namespace nn
