#pragma once

#include "decomp.h"
#include "nn/gx/gx_Api.h"

namespace sead {
// The pattern of the CTR graphics classes that write commands into the current command buffer: the
// current address (nngx parameter 0x208), then the buffer pointer moved by the written bytes
// (names are ours).
inline u32* getCurrentCommand()
{
    u32* command;
    nngxGetCmdlistParameteri(0x208, reinterpret_cast<GLint*>(&command));
    return command;
}

inline void moveCommandBufferPointer(const u32* start, const u32* end)
{
    nngxMoveCommandbufferPointer((reinterpret_cast<uptr>(end) - reinterpret_cast<uptr>(start)) & ~3u);
}
} // namespace sead
