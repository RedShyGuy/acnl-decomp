#include "Sound/dSoundFurnitureMgr.h"
#include "Sound/dSoundFurnitureMgr_SingletonDisposer_.h"
#include "Sound/dSoundMgr.h"
#include "sead/seadNew.h"

namespace {
const s32 MAX_PRIORITY = 127;

// one sound less in its group (name is ours)
inline void RemoveFromGroup(sead::CheckedArray<SoundFurnitureMgr::Group, SoundFurnitureMgr::GROUP_NUM>& groups, u32 soundId)
{
    for (s32 i = 0; i < SoundFurnitureMgr::GROUP_NUM; i++) {
        if (groups[i].soundId == soundId) {
            groups[i].count--;
            if (groups[i].count == 0) {
                groups[i].soundId = 0xFFFFFFFF;
            }
            break;
        }
    }
}
} // namespace

// 0x0094DFEC
SoundFurnitureMgr* SoundFurnitureMgr::s_pInstance = NULL;
// 0x0094DFF0
SoundFurnitureMgr::SingletonDisposer_* SoundFurnitureMgr::s_pSingletonDisposer = NULL;

// 0x001239D4
SoundFurnitureMgr* SoundFurnitureMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundFurnitureMgr)];
        SoundFurnitureMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundFurnitureMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x002D19AC
u8 SoundFurnitureMgr::Play(u32 index, u32 soundId)
{
    Entry& entry = m_Entries[index];
    if (entry.obj == NULL) {
        return STATE_NONE;
    }
    entry.soundId = soundId;
    nw::snd::SoundArchive::SoundInfo info;
    SoundMgr::s_pInstance->ReadSoundInfo(&info, soundId);
    entry.priority = info.playerPriority;

    if (++m_SoundNum > PLAYING_NUM) {
        // the sound of the largest group, or a group of the same sound
        u32 maxCount = 0;
        u32 shared = 0xFFFFFFFF;
        for (s32 i = 0; i < GROUP_NUM; i++) {
            if (m_Groups[i].count > maxCount) {
                maxCount = m_Groups[i].count;
                shared = m_Groups[i].soundId;
            }
        }
        if (maxCount < 2) {
            for (s32 i = 0; i < GROUP_NUM; i++) {
                if (m_Groups[i].soundId == soundId) {
                    shared = soundId;
                    break;
                }
            }
        }

        if (shared != 0xFFFFFFFF) {
            for (s32 i = 0; i < OBJ_NUM; i++) {
                if (m_Entries[i].state == STATE_PLAYING && m_Entries[i].soundId == shared) {
                    m_Entries[i].state = STATE_MUTED;
                    RemoveFromGroup(m_Groups, shared);
                    entry.state = STATE_PLAYING;
                    AddToGroup(soundId);
                    return STATE_PLAYING;
                }
            }
            entry.state = STATE_MUTED;
            return STATE_MUTED;
        }

        // the playing sound with the lowest priority
        s32 lowest = 0;
        s32 priority = MAX_PRIORITY;
        for (s32 i = 0; i < OBJ_NUM; i++) {
            if (m_Entries[i].state == STATE_PLAYING && m_Entries[i].priority < priority) {
                lowest = i;
                priority = m_Entries[i].priority;
            }
        }
        if (entry.priority < priority) {
            entry.state = STATE_MUTED;
            return STATE_MUTED;
        }
        m_Entries[lowest].state = STATE_MUTED;
        RemoveFromGroup(m_Groups, m_Entries[lowest].soundId);
    }
    entry.state = STATE_PLAYING;
    AddToGroup(soundId);
    return STATE_PLAYING;
}

// 0x002D1D8C
u32 SoundFurnitureMgr::Register(SoundObjBase* obj)
{
    if (m_ObjNum < OBJ_NUM) {
        for (u32 i = 0; i < OBJ_NUM; i++) {
            if (m_Entries[i].obj == NULL) {
                m_Entries[i].obj = obj;
                m_Entries[i].soundId = 0xFFFFFFFF;
                m_Entries[i].priority = 0;
                m_Entries[i].state = STATE_NONE;
                m_ObjNum++;
                return i;
            }
        }
    }
    return INVALID_INDEX;
}

// 0x002D1E24
void SoundFurnitureMgr::Stop(u32 index)
{
    Entry& entry = m_Entries[index];
    if (entry.soundId == 0xFFFFFFFF) {
        return;
    }
    if (entry.state == STATE_PLAYING) {
        RemoveFromGroup(m_Groups, entry.soundId);
        if (m_SoundNum > PLAYING_NUM) {
            // a muted sound plays again: preferably one without a group, else the one with the highest priority
            s32 priority = 0;
            s32 best = -1;
            s32 freePriority = 0;
            s32 bestFree = -1;
            for (s32 i = 0; i < OBJ_NUM; i++) {
                if (m_Entries[i].state != STATE_MUTED) {
                    continue;
                }
                if (m_Entries[i].priority >= priority) {
                    best = i;
                    priority = m_Entries[i].priority;
                }
                bool isInGroup = false;
                for (s32 j = 0; j < GROUP_NUM; j++) {
                    if (m_Groups[j].soundId == m_Entries[i].soundId) {
                        isInGroup = true;
                        break;
                    }
                }
                if (!isInGroup && m_Entries[i].priority >= freePriority) {
                    bestFree = i;
                    freePriority = m_Entries[i].priority;
                }
            }
            s32 pick = bestFree != -1 ? bestFree : best;
            m_Entries[pick].state = STATE_PLAYING;
            AddToGroup(m_Entries[pick].soundId);
        }
    }
    entry.soundId = 0xFFFFFFFF;
    entry.priority = 0;
    entry.state = STATE_NONE;
    m_SoundNum--;
}

// 0x002D203C
void SoundFurnitureMgr::Unregister(u32 index)
{
    if (m_Entries[index].obj != NULL) {
        Stop(index);
        m_Entries[index].obj = NULL;
        m_ObjNum--;
    }
}

// 0x002D2108
void SoundFurnitureMgr::AddToGroup(u32 soundId)
{
    s32 free = 0x7FFFFFFF;
    for (s32 i = 0; i < GROUP_NUM; i++) {
        if (m_Groups[i].soundId == soundId) {
            m_Groups[i].count++;
            return;
        }
        if (free == 0x7FFFFFFF && m_Groups[i].soundId == 0xFFFFFFFF) {
            free = i;
        }
    }
    m_Groups[free].soundId = soundId;
    m_Groups[free].count = 1;
}
