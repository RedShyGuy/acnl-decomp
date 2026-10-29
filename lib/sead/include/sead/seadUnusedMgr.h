#pragma once

#include "decomp.h"

namespace sead {
// The object behind the global pointer 0x009762F8 (in the data of sead): no code of the binary sets the
// pointer (its class was not linked), only the game's SystemTask::enter resets two pairs of words through
// it with the two setters (code between ResourceMgr and SoundHandle). Nothing else is known; the class
// and all names are ours.
class UnusedMgr
{
public:
    void setPair0x12C(u32 first, u32 second); // 0x00540B9C (name is ours)
    void setPair0x134(u32 first, u32 second); // 0x00540B90 (name is ours)

    static UnusedMgr* s_pInstance;

    u8 _unknown[0x12C];  // 0x000
    u32 field_0x12C[2];  // 0x12C
    u32 field_0x134[2];  // 0x134
};
} // namespace sead
