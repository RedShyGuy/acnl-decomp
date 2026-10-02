#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class SequenceSoundLoader
{
public:
    class DataLoadTask;
    class LoadInfo;
    struct Arg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void Finalize(); // 0x004D267C | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
