#pragma once

#include "decomp.h"
#include "Sound/dSoundBarcarollePlayer.h"
#include "sead/seadIDisposer.h"

// RTTI N21SoundBarcarollePlayer18SingletonDisposer_E @ 0x008CDB80
// vtable 0x008FBB94 (vptr 0x008FBB9C), offset_to_top 0, 2 entries
class SoundBarcarollePlayer::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0032B410 (unverified)
    virtual ~SingletonDisposer_(); // 0x0032B704 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0032B698 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
