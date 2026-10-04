#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
// The data size limit of the chunks in a PacketOld (set by PayloadSizeManager). Only these static
// functions are in common; the member names are ours.
class ChunkOld
{
public:
    static const unsigned int DATA_SIZE_LIMIT_MAX = 0x594;

    static nn::Result SetDataSizeLimit(unsigned int size); // 0x004293F0 | fefates:bytes [tier B]
    static bool IsValidDataSizeLimit(unsigned int size); // 0x00429418 (name is ours)

    static unsigned int s_DataSizeLimit; // 0x0097F9E8
};
} // namespace common
} // namespace pia
} // namespace nn
