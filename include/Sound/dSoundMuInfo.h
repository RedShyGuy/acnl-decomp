#pragma once

#include "decomp.h"
#include "Sound/dSoundLabel.h"

// The K.K. songs (the class name is from the RTTI of its disposer, everything else is ours). (being written)
class SoundMuInfo
{
public:
    class SingletonDisposer_;

    enum
    {
        SONG_NUM = 91,
    };

    // the sequence of a song (-1: none)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetSoundId(u32 song) const // 0x0070F8A0
    {
        if (song >= SONG_NUM) {
            return 0xFFFFFFFF;
        }
        return song + SEQ_BGM_MU01_SANBIKA;
    }

    static SoundMuInfo* s_pInstance;
};
