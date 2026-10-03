#include "nn/fslow/fslow_Api.h"

namespace nn {
namespace fslow {

namespace {
// how many numbers after count are tried
const u32 MAX_TRIES = 100;
} // namespace

// 0x0047F5F4 | nintendogs:bytes [tier A]
// a bucket count for a hash table of count entries: the first odd number from count on that no
// prime up to 17 divides; count | 1 if there is none within the tries
u32 QueryOptimalBucketCount(u32 count)
{
    if (count <= 3) {
        return 3;
    }
    if (count <= 19) {
        return count | 1;
    }
    for (u32 i = 0; i < MAX_TRIES; i++) {
        u32 n = count + i;
        if (n % 2 == 0) {
            continue;
        }
        if (n % 3 != 0 && n % 5 != 0 && n % 7 != 0 && n % 11 != 0 && n % 13 != 0 && n % 17 != 0) {
            return n;
        }
    }
    return count | 1;
}

} // namespace fslow
} // namespace nn
