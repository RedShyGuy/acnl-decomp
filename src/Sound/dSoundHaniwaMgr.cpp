#include "Sound/dSoundHaniwaMgr.h"
#include "Sound/dSoundHaniwaMgr_SingletonDisposer_.h"
#include "Sound/dSoundIHaniwaControl.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundObjHaniwa.h"
#include "Sound/dSoundRoomMusicPlayer.h"
#include "sead/seadNew.h"
#include <cstring>

namespace {
const u32 COUNTED_PLAYER_NUM = 18;
// this player counts three times when the kind of the room music is 1
const u32 PLAYER_WEIGHTED = 16;
const u32 HISTORY_NUM = 60;
const u32 HISTORY_CLEAR_SIZE = 150;
const f32 TEMPO_NORMAL = 120.0f;
const s32 STOP_WAIT = 5;
} // namespace

// 0x0094DFF4
SoundHaniwaMgr* SoundHaniwaMgr::s_pInstance = NULL;
// 0x0094DFF8
SoundHaniwaMgr::SingletonDisposer_* SoundHaniwaMgr::s_pSingletonDisposer = NULL;

SoundHaniwaMgr::SoundHaniwaMgr()
    : m_ControlKind(CONTROL_ROOM_MUSIC), m_ObjNum(0), m_pControl(NULL), m_pChords(NULL), m_BeatScale(1.0f), m_Level(0),
      m_HistoryIndex(0), m_Wait(0)
{
    for (u32 i = 0; i < OBJ_NUM; i++) {
        m_pObjs[i] = NULL;
    }
    std::memset(m_History, 0, HISTORY_CLEAR_SIZE);
}

// 0x00276A28
SoundHaniwaMgr* SoundHaniwaMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundHaniwaMgr)];
        SoundHaniwaMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundHaniwaMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x0012FA08
void SoundHaniwaMgr::destroyInstance()
{
    if (s_pSingletonDisposer != NULL) {
        SingletonDisposer_* disposer = s_pSingletonDisposer;
        s_pSingletonDisposer = NULL;
        disposer->~SingletonDisposer_();
        ::operator delete(s_pInstance);
        s_pInstance = NULL;
    }
}

// 0x00276978
void SoundHaniwaMgr::SetControl(u32 kind, SoundIHaniwaControl* control)
{
    if (kind == CONTROL_ROOM_MUSIC) {
        if (SoundRoomMusicPlayer::s_pInstance != NULL) {
            m_pControl = control;
            m_pChords = &SoundRoomMusicPlayer::s_pInstance->m_Chords;
        }
    } else if (kind == CONTROL_DJ) {
        m_pControl = control;
        m_Level = 3;
    }
    m_ControlKind = kind;
}

// 0x002769C4
void SoundHaniwaMgr::StopAll()
{
    for (u32 i = 0; i < OBJ_NUM; i++) {
        if (m_pObjs[i] != NULL) {
            m_pObjs[i]->StopDance();
        }
    }
    m_Wait = STOP_WAIT;
}

// 0x002769F8
void SoundHaniwaMgr::Unregister(u32 index)
{
    if (index < OBJ_NUM && m_pObjs[index] != NULL) {
        m_pObjs[index] = NULL;
        m_ObjNum--;
    }
}

// 0x00276AF4
void SoundHaniwaMgr::UpdateNothing()
{
    // empty (in the original too)
}

// 0x00276B70
void SoundHaniwaMgr::UpdateLevel()
{
    u8 count = 0;
    for (u32 i = 0; i < COUNTED_PLAYER_NUM; i++) {
        if (s_IsPlayerCounted[i] == 0) {
            continue;
        }
        if (i == PLAYER_WEIGHTED) {
            s32 weight = 1;
            if (m_pChords->m_MinValue == 1) {
                weight = 3;
            }
            count += SoundMgr::s_pInstance->GetSoundPlayer(i)->m_SoundList.size() * weight;
        } else {
            count += SoundMgr::s_pInstance->GetSoundPlayer(i)->m_SoundList.size();
        }
    }
    m_History[m_HistoryIndex] = count;
    m_HistoryIndex++;
    if (m_HistoryIndex >= HISTORY_NUM) {
        m_HistoryIndex = 0;
    }

    u32 sum = 0;
    for (u32 i = 0; i < HISTORY_NUM; i++) {
        sum += m_History[i];
    }
    f32 mean = static_cast<f32>(sum) * (1.0f / HISTORY_NUM);
    u8 level = 0;
    if (mean >= 1.0f) {
        level = 1;
    }
    if (mean >= 2.0f) {
        level++;
    }
    if (mean >= 3.0f) {
        level++;
    }
    if (m_Level == level) {
        return;
    }
    m_Level = level;
    u32 notified = 0;
    for (u32 i = 0; i < OBJ_NUM; i++) {
        if (m_pObjs[i] != NULL) {
            m_pObjs[i]->SetLevel(m_Level, notified);
            notified++;
            if (m_ObjNum <= notified) {
                break;
            }
        }
    }
}

// 0x00276CF8
void SoundHaniwaMgr::Update()
{
    if (m_pControl == NULL) {
        return;
    }
    (this->*s_UpdateFuncs[m_ControlKind])();
    m_BeatScale = m_pControl->GetTempo() * (1.0f / TEMPO_NORMAL);
    if (m_Wait > 0) {
        m_Wait--;
    }
}

// 0x00276D78
u32 SoundHaniwaMgr::Register(SoundObjHaniwa* obj)
{
    if (m_ObjNum <= OBJ_NUM) {
        for (u32 i = 0; i < OBJ_NUM; i++) {
            if (m_pObjs[i] == NULL) {
                m_pObjs[i] = obj;
                m_ObjNum++;
                return i;
            }
        }
    }
    return INVALID_INDEX;
}

// 0x00719AD0
s16 SoundHaniwaMgr::GetBeat()
{
    if (m_pControl == NULL) {
        return 0;
    }
    return m_pControl->GetBeat();
}

// 0x00719B40
u32 SoundHaniwaMgr::GetRandom()
{
    if (m_pControl == NULL) {
        return 0;
    }
    return m_pControl->GetRandom();
}

// 0x00719B5C
s32 SoundHaniwaMgr::GetTicksPerBeat()
{
    if (m_pControl == NULL) {
        return 24;
    }
    return m_pControl->GetTicksPerBeat();
}

// 0x00719B7C
s32 SoundHaniwaMgr::GetFramesPerBeat()
{
    if (m_pControl == NULL) {
        return 24;
    }
    return m_pControl->GetFramesPerBeat();
}

// 0x00719B9C
s32 SoundHaniwaMgr::GetFramesToBeat()
{
    if (m_pControl == NULL) {
        return 24;
    }
    return m_pControl->GetFramesToBeat();
}

// 0x00719BBC
s16 SoundHaniwaMgr::GetTickInBeat()
{
    if (m_pControl == NULL) {
        return 0;
    }
    return m_pControl->GetTickInBeat();
}

// 0x00719AEC
u32 SoundHaniwaMgr::GetDancingCount() const
{
    u32 count = 0;
    u32 seen = 0;
    for (s32 i = 0; m_ObjNum > seen && i < OBJ_NUM; i++) {
        if (m_pObjs[i] != NULL) {
            seen++;
            if (m_pObjs[i]->m_DanceState != 0) {
                count++;
            }
        }
    }
    return count;
}
