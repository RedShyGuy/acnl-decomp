#pragma once

// nn::y2r - the YUV to RGB conversion hardware (service "y2r:u"). Enum names are from the binary
// (signatures like nn::y2r::CTR::detail::SetRotation(nn::y2r::CTR::Rotation)), one byte each like
// all ARMCC enums; the values are 3dbrew's ("Y2R Services"), the value names are ours.

#include "types.h"

namespace nn {
namespace y2r {
namespace CTR {

enum InputFormat : u8 {
    INPUT_YUV422_INDIV_8 = 0,
    INPUT_YUV420_INDIV_8 = 1,
    INPUT_YUV422_INDIV_16 = 2,
    INPUT_YUV420_INDIV_16 = 3,
    INPUT_YUV422_BATCH = 4,
};

enum OutputFormat : u8 {
    OUTPUT_RGB_32 = 0,
    OUTPUT_RGB_24 = 1,
    OUTPUT_RGB_16_555 = 2,
    OUTPUT_RGB_16_565 = 3,
};

enum Rotation : u8 {
    ROTATION_NONE = 0,
    ROTATION_CLOCKWISE_90 = 1,
    ROTATION_CLOCKWISE_180 = 2,
    ROTATION_CLOCKWISE_270 = 3,
};

enum BlockAlignment : u8 {
    BLOCK_LINE = 0,
    BLOCK_8_BY_8 = 1,
};

enum StandardCoefficient : u8 {
    COEFFICIENT_ITU_R_BT_601 = 0,
    COEFFICIENT_ITU_R_BT_709 = 1,
    COEFFICIENT_ITU_R_BT_601_SCALING = 2,
    COEFFICIENT_ITU_R_BT_709_SCALING = 3,
};

// all conversion settings at once (3dbrew "SetConversionParams"; the member names are ours)
struct PackageParameter {
    InputFormat inputFormat;                    // 0x0
    OutputFormat outputFormat;                  // 0x1
    Rotation rotation;                          // 0x2
    BlockAlignment blockAlignment;              // 0x3
    s16 inputLineWidth;                         // 0x4
    s16 inputLines;                             // 0x6
    StandardCoefficient standardCoefficient;    // 0x8
    u8 padding9;                                // 0x9
    u16 alpha;                                  // 0xA
};
ASSERT_SIZE(PackageParameter, 0xC);

} // namespace CTR
} // namespace y2r
} // namespace nn
