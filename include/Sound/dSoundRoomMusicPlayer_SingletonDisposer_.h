#pragma once

#include "decomp.h"
#include "Sound/dSoundRoomMusicPlayer.h"
#include "sead/seadIDisposer.h"

// RTTI N20SoundRoomMusicPlayer18SingletonDisposer_E @ 0x008CDB5C
// vtable 0x008FBB6C (vptr 0x008FBB74), offset_to_top 0, 2 entries
class SoundRoomMusicPlayer::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x00322BB8 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00322AE0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
