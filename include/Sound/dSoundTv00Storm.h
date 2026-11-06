#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv00Storm @ 0x008CBDD8
// vtable 0x008F0880 (vptr 0x008F0888), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv00Storm : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv00Storm> 0x0081FFB4)
    SoundTv00Storm() {}
    // 0x00278E20
    // 0x00278E10 (deleting dtor)
    virtual ~SoundTv00Storm();
    virtual void Start() override; // 0x00278D94 slot 0x08
    virtual void OnLoaded() override; // 0x00278E00 slot 0x0C
};
ASSERT_SIZE(SoundTv00Storm, 0xE4);
