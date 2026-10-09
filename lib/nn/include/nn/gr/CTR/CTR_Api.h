#pragma once

#include "decomp.h"
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/gr/CTR/gr_Types.h"

namespace nn {
namespace math {
class MTX34;
class MTX44;
} // namespace math

namespace gr {
namespace CTR {
// 1.7.16 float (the PICA float24). Inline as in the original (ARMCC expands it everywhere); "used"
// keeps the out-of-line copy that the original also has.
inline u32 Float32ToFloat24(f32 value); // 0x0016D0D8 | fefates:bytes [tier B]

// write a matrix as four float uniform rows (w, z, y, x each) behind a header
void CopyMtx34WithHeader(f32* dst, const nn::math::MTX34* src, u32 header); // 0x00148EA8 | tier C
void CopyMtx44WithHeader(f32* dst, const nn::math::MTX44* src, u32 header); // 0x00148EE4 | tier C

u32* MakeChannelKickCommand(u32* command, CommandBufferChannel channel); // 0x00349D40 | fefates:bytes [tier B]
u32* AddDummyDataForCommandBuffer(u32* command, u32 size); // 0x00349DC0 | fefates:bytes [tier B]
u32* MakeChannel0SubroutineCommand(u32* command, u32* sizeAddress, u32 address, u32 size); // 0x00349E0C | fefates:bytes [tier B]

// resets the shader, the fixed attributes, lighting, textures and the render state (name is ours)
u32* MakeDisableAllCommand(u32* command); // 0x00349C54 (name is ours)

__attribute__((used)) inline u32 Float32ToFloat24(f32 value)
{
    const u32 bits = detail::BitsOf(value);
    const u32 sign = bits >> 31;
    s32 exponent = 0;
    if ((bits & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits << 1) >> 24) - 64;
        if (exponent < 0) {
            return sign << 23;
        }
    }
    return ((bits << 9) >> 16) | (exponent << 16) | (sign << 23);
}
} // namespace CTR
} // namespace gr
} // namespace nn
