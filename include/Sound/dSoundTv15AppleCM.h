#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 16SoundTv15AppleCM @ 0x008CC400
// vtable 0x008F2EAC (vptr 0x008F2EB4), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv15AppleCM : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv15AppleCM> 0x00820434)
    SoundTv15AppleCM() {}
    // 0x002BE894
    // 0x002BE884 (deleting dtor)
    virtual ~SoundTv15AppleCM();
    virtual void Start() override; // 0x002BE808 slot 0x08
    virtual void OnLoaded() override; // 0x002BE874 slot 0x0C
};
ASSERT_SIZE(SoundTv15AppleCM, 0xE4);
