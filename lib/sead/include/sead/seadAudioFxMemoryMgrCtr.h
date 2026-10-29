#pragma once

#include "decomp.h"
#include "sead/seadAudioFxMemoryMgr.h"

namespace sead {
// RTTI N4sead19AudioFxMemoryMgrCtrE @ 0x008D1C4C
// vtable 0x0090613C (vptr 0x00906144), offset_to_top 0, 2 entries
class AudioFxMemoryMgrCtr : public ::sead::AudioFxMemoryMgr
{
public:
    AudioFxMemoryMgrCtr(); // 0x00138D5C
    // (the original falls into the destructor of AudioFxMemoryMgr with a nop)
    virtual ~AudioFxMemoryMgrCtr(); // 0x00546624 slot 0x00
    // 0x0054A140 slot 0x04 (deleting dtor)
};
} // namespace sead
