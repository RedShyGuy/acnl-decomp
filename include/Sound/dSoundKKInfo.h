#pragma once

#include "decomp.h"
#include "Sound/dSoundLabel.h"

namespace sead { class Heap; }

// The songs of K.K. Slider (the class name is from the RTTI of its disposer, everything else is ours; the meaning of most
// values of a song is open).
class SoundKKInfo
{
public:
    class SingletonDisposer_;

    enum
    {
        SONG_NUM = 94,
    };

    struct Song
    {
        const char* name;  // 0x00
        u8 _4;             // 0x04
        u8 _5;             // 0x05
        u8 _6;             // 0x06
        u32 _8;            // 0x08
        u32 _C;            // 0x0C
        u8 _10;            // 0x10
        u8 _11;            // 0x11
        u8 _12;            // 0x12
    };
    ASSERT_SIZE(Song, 0x14);

    enum
    {
        // the first songs can be picked at random
        RANDOM_SONG_NUM = 90,
    };

    static SoundKKInfo* createInstance(sead::Heap* heap); // 0x001232F0

    // (copies in the shared area; a song out of range gets a default)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetSoundId(u32 song) const // 0x0070F64C
    {
        if (song < SONG_NUM) {
            return song + SEQ_BGM_KK01_SANBIKA;
        }
        return SEQ_BGM_KK01_SANBIKA;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetEndTick(u32 song) const // 0x0070F664
    {
        return song < SONG_NUM ? s_Songs[song]._8 : 0x1E5F;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 Get6(u32 song) const // 0x0070F68C
    {
        return song < SONG_NUM ? s_Songs[song]._6 : 2;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetMainBeats(u32 song) const // 0x0070F6B0
    {
        if (song > SONG_NUM) {
            return 40;
        }
        if (song == SONG_NUM) {
            return 36;
        }
        return s_Songs[song]._11;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetIntroBeats(u32 song) const // 0x0070F6DC
    {
        if (song > SONG_NUM) {
            return 32;
        }
        if (song == SONG_NUM) {
            return 16;
        }
        return s_Songs[song]._10;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetFinBeats(u32 song) const // 0x0070F708
    {
        return song < SONG_NUM ? s_Songs[song]._12 : 16;
    }
    // 0..23 to 0..11
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 ConvertKey(u32 key) const; // 0x0070F72C
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetKey(u32 song) const // 0x0070F7F8
    {
        if (song > SONG_NUM) {
            return 3;
        }
        if (song == SONG_NUM) {
            return 2;
        }
        return s_Songs[song]._5;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetBeatsPerBar(u32 song) const // 0x0070F824
    {
        if (song == 90 || song >= SONG_NUM) {
            return 4;
        }
        return s_Songs[song]._4;
    }

    static const Song s_Songs[SONG_NUM];
    static SoundKKInfo* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4]; // 0x00
};
ASSERT_SIZE(SoundKKInfo, 0x10);
