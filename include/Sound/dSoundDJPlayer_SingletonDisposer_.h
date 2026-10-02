#pragma once

#include "decomp.h"
#include "Sound/dSoundDJPlayer.h"
#include "sead/seadIDisposer.h"

// RTTI N13SoundDJPlayer18SingletonDisposer_E @ 0x008CD9B8
// vtable 0x008FB404 (vptr 0x008FB40C), offset_to_top 0, 2 entries
class SoundDJPlayer::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00245104 (unverified)
    virtual ~SingletonDisposer_(); // 0x0024565C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00245604 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
