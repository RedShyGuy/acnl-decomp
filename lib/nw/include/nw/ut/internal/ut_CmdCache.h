#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
namespace internal {
class CmdCache
{
public:
    void Add(const unsigned*, unsigned); // 0x0048BAA0 | nintendogs:bytes [tier A]
    void Init(void*, unsigned, bool); // 0x0048BACC | nintendogs:bytes [tier A]
    void RoundUp(unsigned char); // 0x0048BAF8 | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace ut
} // namespace nw
