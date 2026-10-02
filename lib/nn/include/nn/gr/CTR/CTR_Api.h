#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
void Float32ToFloat24(float); // 0x0016D0D8 | fefates:bytes [tier B]
void MakeChannelKickCommand(unsigned int*, nn::gr::CTR::CommandBufferChannel); // 0x00349D40 | fefates:bytes [tier B]
void AddDummyDataForCommandBuffer(unsigned int*, unsigned int); // 0x00349DC0 | fefates:bytes [tier B]
void MakeChannel0SubroutineCommand(unsigned int*, unsigned int*, unsigned int, unsigned int); // 0x00349E0C | fefates:bytes [tier B]
} // namespace CTR
} // namespace gr
} // namespace nn
