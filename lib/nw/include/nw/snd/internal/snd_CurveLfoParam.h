#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// The parameters of an LFO (the class and function names are from the reference symbols, the
// members are ours).
struct CurveLfoParam
{
    // (inline)
    DECOMP_EMIT_COPY CurveLfoParam() { Initialize(); } // 0x004C72EC (name is ours)

    // depth 0, 6.25 Hz, no delay, range 1, sine, phase 0
    void Initialize(); // 0x004C72BC | fefates:bytes [tier B]

    f32 depth;  // 0x0
    f32 speed;  // 0x4 cycles per second
    u32 delay;  // 0x8 milliseconds
    u8 range;   // 0xC
    u8 curve;   // 0xD CurveLfo::CurveType
    u8 phase;   // 0xE the start of the cycle, 0 to 127
};
ASSERT_SIZE(CurveLfoParam, 0x10);
} // namespace internal
} // namespace snd
} // namespace nw
