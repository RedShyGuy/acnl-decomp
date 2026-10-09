#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
// The memory of the regular expressions of the profanity filter: units of 16 bytes in a buffer
// given by the filter, with a map of the units in use (one byte each). Allocate searches from
// the last allocation on, then from the start. Member names are ours.
class ProfanityFilterTemporaryPool
{
public:
    static const u32 UNIT_SIZE = 16;
    static const u32 UNIT_COUNT = 2624;
    static const u32 USED_MAP_SIZE = 3072;
    // what the filter gives (the map, then the units)
    static const u32 BUFFER_SIZE = USED_MAP_SIZE + UNIT_COUNT * UNIT_SIZE;

    ProfanityFilterTemporaryPool(); // 0x003DFEE4 | fefates:callgraph [tier C]
    void Initialize(uptr buffer); // 0x003DFD50 | fefates:bytes [tier B]
    void Free(void* buffer, u32 unitCount); // 0x003DFD70 | fefates:bytes [tier B]
    void* Allocate(u32 unitCount); // 0x003DFDA4 | fefates:bytes [tier B]

    u8* m_UsedMap;      // 0x0, a byte per unit
    u8* m_Units;        // 0x4
    u32 m_NextIndex;    // 0x8, behind the last allocation
};
ASSERT_SIZE(ProfanityFilterTemporaryPool, 0xC);
} // namespace ngc
} // namespace nn
