#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 15SoundTv16SingCM @ 0x008CC160
// vtable 0x008F1F74 (vptr 0x008F1F7C), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv16SingCM : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv16SingCM> 0x008202B8)
    SoundTv16SingCM() {}
    // 0x002A64E0
    // 0x002A64D0 (deleting dtor)
    virtual ~SoundTv16SingCM();
    virtual void Start() override; // 0x002A6454 slot 0x08
    virtual void OnLoaded() override; // 0x002A64C0 slot 0x0C
};
ASSERT_SIZE(SoundTv16SingCM, 0xE4);
