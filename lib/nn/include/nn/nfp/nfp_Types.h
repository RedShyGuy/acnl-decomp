#pragma once

#include "decomp.h"

namespace nn {
namespace nfp {

// the model information of a tag (the reply of NFC:GetModelInfo, 0x36 bytes; 3dbrew "NFC
// Services"). The struct name is from the symbols; the fields are not worked out yet.
struct RomInfo
{
    u8 data[0x36];
};
ASSERT_SIZE(RomInfo, 0x36);

} // namespace nfp
} // namespace nn
