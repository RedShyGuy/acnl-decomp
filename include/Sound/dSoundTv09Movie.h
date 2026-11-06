#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv09Movie @ 0x008CBDFC
// vtable 0x008F08D4 (vptr 0x008F08DC), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv09Movie : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv09Movie> 0x008200C0)
    SoundTv09Movie() {}
    // 0x00279324
    // 0x00279314 (deleting dtor)
    virtual ~SoundTv09Movie();
    virtual void Start() override; // 0x0027907C slot 0x08
    virtual void OnLoaded() override; // 0x002792E4 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x002790E8 slot 0x10
};
ASSERT_SIZE(SoundTv09Movie, 0xE4);
