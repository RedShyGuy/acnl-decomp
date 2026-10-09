#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/gr/CTR/gr_FragmentLight.h"
#include "nn/gr/CTR/gr_RenderState.h"
#include "nn/gr/CTR/gr_Shader.h"
#include "nn/gr/CTR/gr_Texture.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;

namespace {
const s32 FIXED_ATTRIBUTE_COUNT = 12;
} // namespace

// 0x00148EA8 | tier C
void CopyMtx34WithHeader(f32* dst, const nn::math::MTX34* src, u32 header)
{
    *reinterpret_cast<u32*>(&dst[1]) = header;
    dst[4] = src->m[0][0];
    dst[3] = src->m[0][1];
    dst[2] = src->m[0][2];
    dst[0] = src->m[0][3];
    dst[8] = src->m[1][0];
    dst[7] = src->m[1][1];
    dst[6] = src->m[1][2];
    dst[5] = src->m[1][3];
    dst[12] = src->m[2][0];
    dst[11] = src->m[2][1];
    dst[10] = src->m[2][2];
    dst[9] = src->m[2][3];
}

// 0x00148EE4 | tier C
void CopyMtx44WithHeader(f32* dst, const nn::math::MTX44* src, u32 header)
{
    *reinterpret_cast<u32*>(&dst[1]) = header;
    dst[4] = src->m[0][0];
    dst[3] = src->m[0][1];
    dst[2] = src->m[0][2];
    dst[0] = src->m[0][3];
    dst[8] = src->m[1][0];
    dst[7] = src->m[1][1];
    dst[6] = src->m[1][2];
    dst[5] = src->m[1][3];
    dst[12] = src->m[2][0];
    dst[11] = src->m[2][1];
    dst[10] = src->m[2][2];
    dst[9] = src->m[2][3];
    dst[16] = src->m[3][0];
    dst[15] = src->m[3][1];
    dst[14] = src->m[3][2];
    dst[13] = src->m[3][3];
}

// 0x00349C54 (name is ours)
u32* MakeDisableAllCommand(u32* command)
{
    command = Shader::MakeDisableCommand(command);

    command[0] = 0;
    command[1] = CommandHeader(0x201);
    command[2] = 0;
    command[3] = CommandHeader(0x202);
    command += 4;

    // the vertex shader input mapping and the attribute arrays
    *command++ = 0;
    *command++ = CommandHeader(0x203, 0xF, 35, true);
    for (s32 i = 0; i < 35; ++i) {
        *command++ = 0;
    }
    *command++ = 0;

    // the fixed vertex attributes
    for (s32 i = 0; i < FIXED_ATTRIBUTE_COUNT; ++i) {
        *command++ = i;
        *command++ = CommandHeader(0x232, 0xF, 3, true);
        *command++ = 0;
        *command++ = 0;
        *command++ = 0;
        *command++ = 0;
    }

    *command++ = 0;
    *command++ = CommandHeader(0xE0, 0x1);
    command = FragmentLight::MakeDisableCommand(command, true);
    command = Texture::MakeDisableCommand(command, true);
    *command++ = 0;
    *command++ = CommandHeader(0x62, 0x1);
    *command++ = 0;
    *command++ = CommandHeader(0x118, 0x1);
    *command++ = 0;
    *command++ = CommandHeader(0x47, 0x1);
    command = RenderState::MakeDisableCommand(command, true);
    *command++ = 0;
    *command++ = CommandHeader(0x11B);
    return command;
}

// 0x00349D40 | fefates:bytes [tier B]
u32* MakeChannelKickCommand(u32* command, CommandBufferChannel channel)
{
    *command++ = 1;
    *command++ = CommandHeader(channel != 0 ? 0x23D : 0x23C);
    return command;
}

// 0x00349DC0 | fefates:bytes [tier B]
u32* AddDummyDataForCommandBuffer(u32* command, u32 size)
{
    // pad the buffer to a multiple of 16 bytes
    if ((size & 0xF) != 0) {
        const s32 count = 4 - ((size & 0xF) >> 2);
        for (s32 i = 0; i < count; ++i) {
            *command++ = 0;
        }
    }
    return command;
}

// 0x00349E0C | fefates:bytes [tier B]
u32* MakeChannel0SubroutineCommand(u32* command, u32* sizeAddress, u32 address, u32 size)
{
    *command++ = size >> 3;
    *command++ = CommandHeader(0x238, 0xF, 4, true);
    // the size of the way back, filled in by the caller once the subroutine is written
    *sizeAddress = reinterpret_cast<uptr>(command);
    *command++ = 0;
    *command++ = nngxGetPhysicalAddr(address) >> 3;
    *command++ = nngxGetPhysicalAddr(reinterpret_cast<uptr>(command + 2)) >> 3;
    *command++ = 1;
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
