#pragma once

#include "decomp.h"
#include "Sound/dSoundLabel.h"

namespace sead { class Heap; }

// The songs of the music (the class name is from the RTTI of its disposer, everything else is ours; the meaning of some
// values is open).
class SoundMuInfo
{
public:
    class SingletonDisposer_;

    enum
    {
        SONG_NUM = 91,
    };

    struct Song
    {
        const char* name;  // 0x00
        u32 _4;            // 0x04
        u32 _8;            // 0x08
        f32 _C;            // 0x0C
        u32 loopStart;     // 0x10
        u32 loopEnd;       // 0x14
        u32 _18;           // 0x18
    };
    ASSERT_SIZE(Song, 0x1C);

    static SoundMuInfo* createInstance(sead::Heap* heap); // 0x001234A4

    // (copies in the shared area)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetUpbeatTicks(u32 song) const // 0x0070F854
    {
        return song < SONG_NUM ? s_Songs[song]._8 : 0;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE f32 GetTempoRatio(u32 song) const // 0x0070F878
    {
        return song < SONG_NUM ? s_Songs[song]._C : 1.0f;
    }
    // the sequence of a song (-1: none)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetSoundId(u32 song) const // 0x0070F8A0
    {
        if (song >= SONG_NUM) {
            return 0xFFFFFFFF;
        }
        return song + SEQ_BGM_MU01_SANBIKA;
    }
    // a position past the loop end goes back into the loop
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 WrapPosition(u32 song, u32 position) const // 0x0070F8B8
    {
        if (song < SONG_NUM) {
            const Song& info = s_Songs[song];
            if (position >= info.loopEnd) {
                position = (position - info.loopStart) % (info.loopEnd - info.loopStart) + info.loopStart;
            }
        }
        return position;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetFadeTick(u32 song) const // 0x0070F8FC
    {
        return song < SONG_NUM ? s_Songs[song]._18 : 0x1020;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetGridKind(u32 song) const // 0x0070F924
    {
        return song < SONG_NUM ? s_Songs[song]._4 : 0;
    }
    // the song of a sequence (-1: none)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetSong(u32 soundId) const // 0x0070F948
    {
        u32 song = soundId - SEQ_BGM_MU01_SANBIKA;
        return song < SONG_NUM ? song : 0xFFFFFFFF;
    }
    // (out of range: the first song)
    DECOMP_EMIT_COPY DECOMP_NOINLINE const Song* GetInfo(u32 song) const // 0x0070F960
    {
        return song < SONG_NUM ? &s_Songs[song] : &s_Songs[0];
    }

    static const Song s_Songs[SONG_NUM];
    static SoundMuInfo* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4]; // 0x00
};
ASSERT_SIZE(SoundMuInfo, 0x10);
