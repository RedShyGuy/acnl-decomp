#pragma once

#include "decomp.h"
#include "Sound/dSoundMelodyPlayerBase.h"

// RTTI 20SoundMelodyPlayerPos @ 0x008CCC18
// vtable 0x008F5B3C (vptr 0x008F5B44), offset_to_top 0, 3 entries
class SoundMelodyPlayerPos : public ::SoundMelodyPlayerBase
{
public:
    SoundMelodyPlayerPos(); // ctor candidate(s) 0x00322270 (unverified)
    virtual void vf_0x00(); // 0x0013E56C slot 0x00 | virtual slot, introduced by SoundMelodyPlayerBase
    virtual void vf_0x04(); // 0x003222B0 slot 0x04 | virtual slot, introduced by SoundMelodyPlayerBase
    virtual void vf_0x08(); // 0x003221F8 slot 0x08 | virtual slot, introduced by SoundMelodyPlayerBase
};
