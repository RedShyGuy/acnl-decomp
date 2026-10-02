#pragma once

#include "decomp.h"
#include "sead/seadAudioFxMemoryMgr.h"

namespace sead {
// RTTI N4sead19AudioFxMemoryMgrCtrE @ 0x008D1C4C
// vtable 0x0090613C (vptr 0x00906144), offset_to_top 0, 2 entries
class AudioFxMemoryMgrCtr : public ::sead::AudioFxMemoryMgr
{
public:
    AudioFxMemoryMgrCtr(); // ctor candidate(s) 0x00138D5C (unverified)
    virtual void vf_0x00(); // 0x00546624 slot 0x00 | virtual slot, introduced by sead::AudioFxMemoryMgr
    virtual void vf_0x04(); // 0x0054A140 slot 0x04 | virtual slot, introduced by sead::AudioFxMemoryMgr
};
} // namespace sead
