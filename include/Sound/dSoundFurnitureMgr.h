#pragma once

#include "decomp.h"
#include "sead/seadCheckedArray.h"

namespace sead { class Heap; }
class SoundObjBase;

// The sounds of the furniture: at most three play at the same time, the others are muted. Furniture that plays the same
// sound share a group; when too many play, one of the largest group or the one with the lowest priority is muted (the
// class name is from the RTTI of its disposer, everything else is ours).
class SoundFurnitureMgr
{
public:
    class SingletonDisposer_;

    enum
    {
        OBJ_NUM = 52,
        INVALID_INDEX = 53,
        GROUP_NUM = 3,
        PLAYING_NUM = 3,
    };
    // Entry::state, the result of Play
    enum State
    {
        STATE_NONE = 0,
        STATE_PLAYING = 1,
        STATE_MUTED = 2,
    };

    struct Entry
    {
        Entry() : obj(NULL), soundId(0xFFFFFFFF), priority(0), state(STATE_NONE) {} // 0x00128CA0

        SoundObjBase* obj; // 0x0
        u32 soundId;       // 0x4
        s32 priority;      // 0x8 the player priority of the sound
        u8 state;          // 0xC State
    };
    struct Group
    {
        Group() : soundId(0xFFFFFFFF), count(0) {} // 0x00128C90

        u32 soundId; // 0x0
        u32 count;   // 0x4
    };

    SoundFurnitureMgr() : m_ObjNum(0), m_SoundNum(0) {}
    static SoundFurnitureMgr* createInstance(sead::Heap* heap); // 0x001239D4

    // the State the sound gets (STATE_NONE: no object)
    u8 Play(u32 index, u32 soundId); // 0x002D19AC
    u32 Register(SoundObjBase* obj); // 0x002D1D8C
    void Stop(u32 index); // 0x002D1E24
    void Unregister(u32 index); // 0x002D203C
    void AddToGroup(u32 soundId); // 0x002D2108

    static SoundFurnitureMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4];          // 0x000
    sead::CheckedArray<Entry, OBJ_NUM> m_Entries;  // 0x010
    sead::CheckedArray<Group, GROUP_NUM> m_Groups; // 0x350
    u32 m_ObjNum;                              // 0x368
    u32 m_SoundNum;                            // 0x36C
};
ASSERT_OFFSET(SoundFurnitureMgr, m_Groups, 0x350);
ASSERT_OFFSET(SoundFurnitureMgr, m_ObjNum, 0x368);
ASSERT_SIZE(SoundFurnitureMgr, 0x370);
