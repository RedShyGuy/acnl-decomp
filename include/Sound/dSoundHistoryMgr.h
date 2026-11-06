#pragma once

#include "decomp.h"
#include "sead/seadCheckedArray.h"

namespace sead { class Heap; }

// The sounds that were played a moment ago: a sound stays for some frames, so that the same sound does not start twice at
// once (the class name is from the RTTI of its disposer, everything else is ours).
class SoundHistoryMgr
{
public:
    class SingletonDisposer_;

    struct Entry
    {
        DECOMP_EMIT_COPY Entry() : soundId(0xFFFFFFFF), frames(0) {} // 0x00128C3C

        u32 soundId; // 0x00 (-1: free)
        u8 frames;   // 0x04
    };
    // with a value (for example the volume of the sound)
    struct ValueEntry
    {
        DECOMP_EMIT_COPY ValueEntry() : soundId(0xFFFFFFFF), frames(0), value(0.0f) {} // 0x00128C1C

        u32 soundId; // 0x00 (-1: free)
        u8 frames;   // 0x04
        f32 value;   // 0x08
    };

    SoundHistoryMgr() : m_EntryCount(0), m_ValueEntryCount(0) {}

    static SoundHistoryMgr* createInstance(sead::Heap* heap); // 0x00123680

    // the sound was played (for some frames); without room it replaces the entry with the fewest frames
    void Add(u32 soundId, u8 frames); // 0x002A3818
    void Add(u32 soundId, u8 frames, f32 value); // 0x002A3944
    void Update(); // 0x002A3A90
    // (copies in the shared area)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool Contains(u32 soundId) const // 0x0071CC58
    {
        if (m_EntryCount != 0) {
            for (s32 i = 0; i < 10; i++) {
                if (m_Entries[i].soundId == soundId) {
                    return true;
                }
            }
        }
        return false;
    }
    // how many entries the sound has
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 Count(u32 soundId) const // 0x0071CC10
    {
        s32 count = 0;
        if (m_EntryCount != 0) {
            for (s32 i = 0; i < 10; i++) {
                if (m_Entries[i].soundId == soundId) {
                    count++;
                }
            }
        }
        return count;
    }
    // with at least this value
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool Contains(u32 soundId, f32 value) const // 0x0071CC9C
    {
        if (m_ValueEntryCount != 0) {
            for (s32 i = 0; i < 5; i++) {
                if (m_ValueEntries[i].soundId == soundId && m_ValueEntries[i].value >= value) {
                    return true;
                }
            }
        }
        return false;
    }

    static SoundHistoryMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4];                // 0x00
    sead::CheckedArray<Entry, 10> m_Entries;         // 0x10
    s32 m_EntryCount;                                // 0x60
    sead::CheckedArray<ValueEntry, 5> m_ValueEntries; // 0x64
    s32 m_ValueEntryCount;                           // 0xA0
};
ASSERT_OFFSET(SoundHistoryMgr, m_EntryCount, 0x60);
ASSERT_SIZE(SoundHistoryMgr, 0xA4);
