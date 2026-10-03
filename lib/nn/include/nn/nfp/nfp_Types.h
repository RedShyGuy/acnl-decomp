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

// the target connection status of NFC (the enum name is from the symbols; values not named)
enum TargetConnectionStatus : u8 {};

namespace CTR {

// The parameter of the amiibo Settings applet: sent to it by StartAmiiboSettings and written back
// by the applet (0x2C4 bytes). The struct name is from the symbols, the field names are ours.
struct Parameter
{
    s32 mode;                   // 0x0 (0, 1, 2, 3 or 100; InitializeParameterForUpdate sets 3)
    u8 unknown04[0x54];         // 0x4
    u8 unknown58;               // 0x58
    u8 unknown59;               // 0x59
    u8 unknown5A;               // 0x5A
    u8 unknown5B;               // 0x5B
    u8 unknown5C[0xA8];         // 0x5C
    u32 unknown104[24];         // 0x104
    s32 result;                 // 0x164 (StartAmiiboSettings: -1 failed, -2 NFC is in use)
    u8 unknown168[0x15C];       // 0x168
};
ASSERT_OFFSET(Parameter, unknown58, 0x58);
ASSERT_OFFSET(Parameter, unknown5C, 0x5C);
ASSERT_OFFSET(Parameter, unknown104, 0x104);
ASSERT_OFFSET(Parameter, result, 0x164);
ASSERT_SIZE(Parameter, 0x2C4);

} // namespace CTR
} // namespace nfp
} // namespace nn
