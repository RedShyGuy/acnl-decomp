#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundArchive.h"
#include "nw/snd/snd_SoundHandle.h"
#include "sead/seadCheckedArray.h"

// The parameters of a sound the game keeps next to its handle: the archive's player and priority, the kind from the user
// parameter and which 3D parameters (Sound3DInfo flags) the game applies itself (name and members are ours).
struct SoundParam
{
    DECOMP_EMIT_COPY SoundParam() // 0x00128EBC
        : soundId(0xFFFFFFFF), playerId(0xFFFFFFFF), priority(64), kind(0), _D(0), isPan(true), isSurroundPan(true),
          isPriority(false)
    {
    }

    u32 soundId;        // 0x00
    u32 playerId;       // 0x04 the index of the player (without the item type)
    s32 priority;       // 0x08 the player priority of the archive
    u8 kind;            // 0x0C the user parameter (bits 0-1)
    u8 _D;              // 0x0D
    bool isPan;         // 0x0E Sound3DInfo flag 4
    bool isSurroundPan; // 0x0F Sound3DInfo flag 8
    bool isPriority;    // 0x10 Sound3DInfo flag 2
};
ASSERT_SIZE(SoundParam, 0x14);

// The helpers of all handle sets (name is ours)
class SoundHandleSetBase
{
public:
    u32 ReadUserParam(u32 soundId); // 0x003220D4
    bool ReadSoundInfo(nw::snd::SoundArchive::SoundInfo* info, u32 soundId); // 0x003220E4
    // the pan, surround pan and priority (lowered by priorityOffset) of the parameters to the sound of the handle
    static void ApplyParam(nw::snd::SoundHandle* handle, s32 priorityOffset, const SoundParam* param, f32 pan,
                           f32 surroundPan); // 0x003220F4

    // the sound info read for a sound without parameters
    static nw::snd::SoundArchive::SoundInfo s_SoundInfo;
};

// N sound handles with the parameters of their sounds (SoundMgr has one with 8, the sound objects SoundObj<N> one with N)
// (name is ours)
template <s32 N>
class SoundHandleSet : public SoundHandleSetBase
{
public:
    // a free handle, else the one with the lowest priority if the new sound's priority is not lower (-1: none)
    DECOMP_NOINLINE s32 FindFreeIndex(u32 soundId);
    // the same as a handle (NULL: none)
    DECOMP_NOINLINE nw::snd::SoundHandle* FindFreeHandle(u32 soundId);

    // the priority of a sound: its parameters if a handle has it, else the archive
    s32 GetPriority(u32 soundId)
    {
        for (s32 i = 0; i < N; i++) {
            if (m_Params[i].soundId == soundId) {
                const SoundParam* param = &m_Params[i];
                if (param != NULL) {
                    return param->priority;
                }
                break;
            }
        }
        ReadSoundInfo(&s_SoundInfo, soundId);
        return s_SoundInfo.playerPriority;
    }

    sead::CheckedArray<nw::snd::SoundHandle, N> m_Handles;
    sead::CheckedArray<SoundParam, N> m_Params;
};

template <s32 N>
s32 SoundHandleSet<N>::FindFreeIndex(u32 soundId)
{
    s32 index = -1;
    s32 lowest = 128;
    for (s32 i = 0; i < N; i++) {
        if (m_Handles[i].m_pSound == NULL) {
            return i;
        }
        if (m_Params[i].priority < lowest) {
            index = i;
            lowest = m_Params[i].priority;
        }
    }
    if (index < 0) {
        return -1;
    }
    if (soundId != 0xFFFFFFFF && GetPriority(soundId) < lowest) {
        return -1;
    }
    return index;
}

template <s32 N>
nw::snd::SoundHandle* SoundHandleSet<N>::FindFreeHandle(u32 soundId)
{
    s32 index = -1;
    s32 lowest = 128;
    s32 i;
    for (i = 0; i < N; i++) {
        if (m_Handles[i].m_pSound == NULL) {
            break;
        }
        if (m_Params[i].priority < lowest) {
            index = i;
            lowest = m_Params[i].priority;
        }
    }
    if (i == N) {
        if (index < 0) {
            return NULL;
        }
        if (soundId != 0xFFFFFFFF && GetPriority(soundId) < lowest) {
            return NULL;
        }
        i = index;
    }
    if (i == -1) {
        return NULL;
    }
    return &m_Handles[i];
}
