#pragma once

#include "decomp.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class BasicCache;

// The caches by their names. The member names are ours.
class CacheManager
{
public:
    CacheManager(); // 0x0035C43C | fefates:bytes [tier B]
    ~CacheManager(); // 0x0035C4D4 | fefates:bytes [tier B]

    u32 m_Unknown0x00;                     // 0x00
    qMap<String, BasicCache*> m_Caches;    // 0x04
};
ASSERT_SIZE(CacheManager, 0x20);
} // namespace nex
} // namespace nn
