#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead16AudioFxMemoryMgrE @ 0x008D1AF4
// vtable 0x00905E04 (vptr 0x00905E0C), offset_to_top 0, 2 entries
class AudioFxMemoryMgr
{
public:
    AudioFxMemoryMgr(); // ctor candidate(s) 0x0013C69C (unverified)
    virtual void vf_0x00(); // 0x00546628 slot 0x00 | virtual slot, introduced by sead::AudioFxMemoryMgr
    virtual void vf_0x04(); // 0x00546620 slot 0x04 | virtual slot, introduced by sead::AudioFxMemoryMgr
    void dispose(); // 0x00546604 | nintendogs:bytes [tier B]
};
} // namespace sead
