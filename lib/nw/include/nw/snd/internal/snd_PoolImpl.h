#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class PoolImpl
{
public:
    void CreateImpl(void*, unsigned, unsigned); // 0x004D4A80 | nintendogs:bytes [tier A]
    void DestroyImpl(void*, unsigned long); // 0x004D4AE0 | nintendogs:bytes [tier A]
    void AllocImpl(); // 0x004D4B24 | nintendogs:bytes [tier A]
    void CountImpl() const; // 0x007430A4 | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
