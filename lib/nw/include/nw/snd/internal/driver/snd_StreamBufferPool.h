#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class StreamBufferPool
{
public:
    void Initialize(void*, unsigned, int); // 0x004CE634 | nintendogs:bytes [tier A]
    void Free(void*); // 0x004CE678 | nintendogs:bytes [tier A]
    void Alloc(); // 0x004CE6B8 | nintendogs:bytes [tier A]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
