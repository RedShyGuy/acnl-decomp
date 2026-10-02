#pragma once

#include "decomp.h"
#include "Sound/dSoundLivePlayer.h"
#include "sead/seadIDisposer.h"

// RTTI N15SoundLivePlayer18SingletonDisposer_E @ 0x008CDAC0
// vtable 0x008FBA40 (vptr 0x008FBA48), offset_to_top 0, 2 entries
class SoundLivePlayer::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x002A3C20 (unverified)
    virtual ~SingletonDisposer_(); // 0x002A45C4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x002A452C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
