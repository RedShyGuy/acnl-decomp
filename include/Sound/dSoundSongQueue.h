#pragma once

#include "decomp.h"

namespace sead { class Random; }

class SoundSongQueue;

// The queue in the form that can be stored (names are ours).
struct SoundSongQueueData
{
    enum
    {
        SONG_NUM = 30,
        INVALID_SONG = 255,
    };

    SoundSongQueueData(); // 0x002E7008
    void Store(const SoundSongQueue* queue); // 0x002E6F98
    void Load(SoundSongQueue* queue) const; // 0x007214C8

    u8 songs[SONG_NUM]; // 0x00 INVALID_SONG: empty
    u32 seed;           // 0x20
    u8 index;           // 0x24
    u8 isAlternate;     // 0x25
};
ASSERT_SIZE(SoundSongQueueData, 0x28);

// The songs K.K. plays one after the other (DJ K.K. and the live shows): a ring of song numbers, bit 7 = the song starts with
// an intro and ends the set. New songs are picked at random, with a preference for songs in a key near the one of the
// song before (the class is ours, nothing names it).
class SoundSongQueue
{
public:
    enum
    {
        CAPACITY = SoundSongQueueData::SONG_NUM,
        SONG_FLAG = 0x80,
        SONG_MASK = 0x7F,
    };

    SoundSongQueue() : m_pData(m_Songs), m_Capacity(CAPACITY), m_Head(0), m_Count(0), m_Seed(0), m_Index(0), m_IsAlternate(1) {}

    // a new set of 4 songs
    DECOMP_NOINLINE static void Reset(SoundSongQueue* queue); // 0x002A3140
    // the next song; a higher score means a better fit to the last song
    u8 PickSong(sead::Random* random); // 0x002A3228
    // one more song at the end; the oldest is dropped when the queue is full
    static void Advance(SoundSongQueue* queue); // 0x002A36E4
    DECOMP_NOINLINE static void MakeData(SoundSongQueueData* data); // 0x002A37C0

    const u8* At(u32 i) const
    {
        if (static_cast<u32>(m_Count) <= i) {
            return m_pData;
        }
        s32 pos = i + m_Head;
        if (m_Capacity <= pos) {
            pos -= m_Capacity;
        }
        return m_pData + pos;
    }
    void PushBack(u8 song)
    {
        if (m_Count < m_Capacity) {
            s32 pos = m_Count++ + m_Head;
            if (pos >= m_Capacity) {
                pos -= m_Capacity;
            }
            m_pData[pos] = song;
        }
    }
    void PopFront()
    {
        if (m_Count > 0) {
            m_Head++;
            if (m_Head >= m_Capacity) {
                m_Head = 0;
            }
            m_Count--;
        }
    }

    u8* m_pData;            // 0x00
    s32 m_Capacity;         // 0x04
    s32 m_Head;             // 0x08
    s32 m_Count;            // 0x0C
    u8 m_Songs[CAPACITY];   // 0x10
    u32 m_Seed;             // 0x30
    u8 m_Index;             // 0x34 the position of the current song
    u8 m_IsAlternate;       // 0x35 changes with each song with SONG_FLAG
};
ASSERT_OFFSET(SoundSongQueue, m_Seed, 0x30);
ASSERT_OFFSET(SoundSongQueue, m_Index, 0x34);
ASSERT_SIZE(SoundSongQueue, 0x38);
