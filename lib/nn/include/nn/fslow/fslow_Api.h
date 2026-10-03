#pragma once

#include "decomp.h"

namespace nn {
namespace fslow {
// a bucket count for a hash table of count entries (odd, no prime factor up to 17)
u32 QueryOptimalBucketCount(u32 count); // 0x0047F5F4 | nintendogs:bytes [tier A]
} // namespace fslow
} // namespace nn
