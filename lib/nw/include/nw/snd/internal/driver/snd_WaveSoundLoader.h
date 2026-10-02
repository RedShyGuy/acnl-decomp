#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class WaveSoundLoader
{
public:
    class DataLoadTask;
    struct Arg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct LoadInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void Finalize(); // 0x004CD9D4 | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
