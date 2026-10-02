#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv11Talk @ 0x008CBB04
// vtable 0x008EF854 (vptr 0x008EF85C), offset_to_top 0, 5 entries
class SoundTv11Talk : public ::SoundTvBase
{
public:
    SoundTv11Talk(); // ctor candidate(s) 0x0081FF48 (unverified)
    virtual ~SoundTv11Talk(); // 0x0024928C slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x0024927C slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x00248748 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002487C8 slot 0x10 | virtual slot, introduced by SoundTvBase
};
