#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv01Color @ 0x008CBDE4
// vtable 0x008F089C (vptr 0x008F08A4), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv01Color : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv01Color> 0x0082000C)
    SoundTv01Color() {}
    // 0x00278EB0
    // 0x00278EA0 (deleting dtor)
    virtual ~SoundTv01Color();
    virtual void Start() override; // 0x00278E24 slot 0x08
    virtual void OnLoaded() override; // 0x00278E90 slot 0x0C
};
ASSERT_SIZE(SoundTv01Color, 0xE4);
