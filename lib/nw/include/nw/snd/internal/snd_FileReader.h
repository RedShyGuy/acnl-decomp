#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class FileReader
{
public:
    struct Priority { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void Initialize(const char*, bool); // 0x004C5B98 | fefates:bytes [tier B]
    void Read(void*, int, int*); // 0x004C5CF8 | fefates:bytes [tier B]
    void Finalize(); // 0x004C5DD8 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
