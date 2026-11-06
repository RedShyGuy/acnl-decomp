#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 15SoundTv17SnowCM @ 0x008CC16C
// vtable 0x008F1F90 (vptr 0x008F1F98), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv17SnowCM : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv17SnowCM> 0x00820310)
    SoundTv17SnowCM() {}
    // 0x002A6570
    // 0x002A6560 (deleting dtor)
    virtual ~SoundTv17SnowCM();
    virtual void Start() override; // 0x002A64E4 slot 0x08
    virtual void OnLoaded() override; // 0x002A6550 slot 0x0C
};
ASSERT_SIZE(SoundTv17SnowCM, 0xE4);
