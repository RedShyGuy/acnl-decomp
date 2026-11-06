#include "Sound/dSoundObjFurniture.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Sound/dSoundFurnitureMgr.h"
#include "Sound/dSoundHistoryMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Other/dRegion.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

namespace {
const u32 KIND_LOOP_WITH_CUES = 39;
const u8 HISTORY_FRAMES = 3;
const s32 VARIABLE_INDEX = 14;
const f32 FRAME_JUMP = 2.0f;

// the kinds that are switches: on and off take effect with the next update
inline bool IsSwitch(u32 kind)
{
    return kind == 201 || kind == 202 || kind == 234;
}

// how the frame moved since the last update
enum Direction
{
    DIRECTION_FORWARD = 0,
    DIRECTION_WRAPPED = 1,   // back to the start
    DIRECTION_BACKWARD = 2,
    DIRECTION_WRAPPED_BACKWARD = 3,
};
} // namespace

// 0x002D3018
SoundObjFurniture::SoundObjFurniture()
    : m_Index(0xFFFFFFFF), m_IsOn(false), m_IsCueDisabled(false), m_Kind(0), m_pInfo(NULL), m_pCues(NULL), m_LastCue(0),
      m_LastFrame(0.0f), m_IsHeld(false), m_IsSwitch(false), m_NextIsOn(false), m_IsAllowed(false)
{
    m_Priority = 2;
}

// 0x002D315C
// 0x002D30B0 (deleting dtor)
SoundObjFurniture::~SoundObjFurniture()
{
    SoundObjFurniture::Stop();
}

// 0x002D250C
void SoundObjFurniture::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative, u32 kind, bool isOn)
{
    if (m_IsActive) {
        return;
    }
    SoundObjBase::Start(world, relative);
    m_Index = SoundFurnitureMgr::s_pInstance->Register(this);
    SetKind(kind);
    if (!m_IsSwitch) {
        m_IsOn = isOn;
    }
    m_LastCue = 0;
    m_LastFrame = 0.0f;
}

// 0x002D25DC
void SoundObjFurniture::TurnOff()
{
    if (m_pInfo == NULL) {
        return;
    }
    if (!m_IsSwitch) {
        m_IsOn = false;
    }
    if (m_pInfo->soundOff != 0) {
        PlaySound(m_pInfo->soundOff, false);
    }
}

// 0x002D265C
void SoundObjFurniture::UpdateCues(bool isLoop, f32 frame)
{
    if (m_pCues != NULL) {
        SoundMgr* mgr = SoundMgr::s_pInstance;
        if (mgr->m_State != 2) {
            return;
        }
        if (isLoop && m_Kind != KIND_LOOP_WITH_CUES) {
            return;
        }
        if (!m_IsAllowed) {
            m_IsAllowed = mgr->_CBC >= m_Index * 2;
            if (!m_IsAllowed) {
                return;
            }
        }
        f32 last = m_LastFrame;
        f32 diff = frame - last;
        s32 direction;
        if (diff < -FRAME_JUMP && frame < FRAME_JUMP) {
            direction = DIRECTION_WRAPPED;
        } else if (diff >= FRAME_JUMP && last <= FRAME_JUMP) {
            direction = DIRECTION_WRAPPED_BACKWARD;
        } else if (diff > 0.0f) {
            direction = DIRECTION_FORWARD;
        } else if (diff < 0.0f) {
            direction = DIRECTION_BACKWARD;
        } else {
            return;
        }

        const Cue* cue = m_pCues;
        for (s32 i = 0; i < CUE_NUM; i++, cue++) {
            if (cue->soundId == 0) {
                break;
            }
            f32 cueFrame = static_cast<f32>(cue->frame);
            bool isHit;
            switch (direction) {
            case DIRECTION_FORWARD:
                isHit = last < cueFrame && frame >= cueFrame;
                break;
            case DIRECTION_WRAPPED:
                isHit = last < cueFrame || cueFrame <= frame;
                break;
            case DIRECTION_BACKWARD:
                isHit = frame <= cueFrame && cueFrame < last;
                break;
            case DIRECTION_WRAPPED_BACKWARD:
                isHit = frame <= cueFrame || last > cueFrame;
                break;
            default:
                isHit = false;
                break;
            }
            if (isHit) {
                u32 soundId = cue->soundId;
                PlaySound(soundId, true);
                m_LastCue = soundId;
                break;
            }
        }
    }
    m_LastFrame = frame;
}

namespace {
// (name is ours)
inline bool IsHandlePlaying(sead::CheckedArray<sead::SoundHandle, 2>& handles, u32 soundId)
{
    for (s32 i = 0; i < 2; i++) {
        if (handles[i].m_pSound != NULL && handles[i].m_pSound->m_Id == soundId) {
            return true;
        }
    }
    return false;
}

// (name is ours)
inline sead::SoundHandle* FindHandle(sead::CheckedArray<sead::SoundHandle, 2>& handles, u32 soundId)
{
    for (s32 i = 0; i < 2; i++) {
        if (handles[i].GetId() == soundId) {
            return &handles[i];
        }
    }
    return NULL;
}
} // namespace

// 0x002D2830
void SoundObjFurniture::PlaySound(u32 soundId, bool isCue)
{
    (void)isCue;
    // not while it appears
    if (IsHandlePlaying(m_HandleSet.m_Handles, SE_OBJ_FUR_CMN_APPEAR)) {
        return;
    }
    if (IsHandlePlaying(m_HandleSet.m_Handles, SE_OBJ_FUR_WALL_APPEAR)) {
        return;
    }
    if (FindHandle(m_HandleSet.m_Handles, soundId) != NULL && !(m_pInfo->flags & FLAG_RESTART)) {
        return;
    }
    if (SoundHistoryMgr::s_pInstance->Contains(soundId)) {
        return;
    }
    if (StartSound(soundId) != NULL) {
        SoundHistoryMgr::s_pInstance->Add(soundId, HISTORY_FRAMES);
    }
}

// 0x002D29A0
sead::SoundHandle* SoundObjFurniture::HoldSound(u32 soundId)
{
    m_IsHeld = true;
    SoundFurnitureMgr* mgr = SoundFurnitureMgr::s_pInstance;
    u8 state = mgr->m_Entries[m_Index].state;
    if (state == SoundFurnitureMgr::STATE_NONE) {
        if (!SoundDataLoadMgr::s_pInstance->IsLoaded(soundId, 1)) {
            return NULL;
        }
        if (mgr->Play(m_Index, soundId) != SoundFurnitureMgr::STATE_PLAYING) {
            return NULL;
        }
    } else if (state != SoundFurnitureMgr::STATE_PLAYING) {
        return NULL;
    }
    if (!m_IsAllowed) {
        m_IsAllowed = SoundMgr::s_pInstance->_CBC >= m_Index * 2;
        if (!m_IsAllowed) {
            return NULL;
        }
    }
    return SoundObj<2>::HoldSound(soundId);
}

// 0x002D2AAC
void SoundObjFurniture::Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame)
{
    if (m_pInfo != NULL) {
        if (!m_IsCueDisabled) {
            UpdateCues(HoldLoop(), frame);
        }
        if (m_IsHeld) {
            m_IsHeld = false;
        } else {
            SoundFurnitureMgr::s_pInstance->Stop(m_Index);
        }
    }
    SoundObj<2>::SetPosition(world, relative);
    if (m_IsSwitch) {
        m_IsOn = m_NextIsOn;
        m_NextIsOn = false;
    }
}

// 0x002D2C50
sead::SoundHandle* SoundObjFurniture::StartSound(u32 soundId)
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (soundId == STRM_SE_FUR_MNH_MEAT_JP) {
        // the voice of the language of the region
        switch (GetRegion()) {
        case 1:
        case 4:
            soundId = STRM_SE_FUR_MNH_MEAT_EN;
            break;
        case 2:
        case 5:
            soundId = STRM_SE_FUR_MNH_MEAT_ES;
            break;
        case 3:
        case 6:
            soundId = STRM_SE_FUR_MNH_MEAT_FR;
            break;
        case 7:
            soundId = STRM_SE_FUR_MNH_MEAT_IT;
            break;
        case 8:
            soundId = STRM_SE_FUR_MNH_MEAT_DE;
            break;
        }
    } else if (soundId != STRM_SE_FUR_DISKWRITER) {
        if (soundId - SE_CUR_CLOSE <= 15) {
            return SoundObj<2>::StartSound(soundId);
        }
        if (!SoundDataLoadMgr::s_pInstance->IsLoaded(soundId, 1)) {
            return NULL;
        }
        sead::SoundHandle* handle = SoundObj<2>::StartSound(soundId);
        if (handle != NULL) {
            nw::snd::SequenceSoundHandle sequence(handle);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->WriteVariable(VARIABLE_INDEX, m_Index);
            }
        }
        return handle;
    }
    // the streams of the furniture are read from the files of the game
    mgr->SetExternalFileRoot(false);
    sead::SoundHandle* handle = SoundObj<2>::StartSound(soundId);
    mgr->SetExternalFileRoot(true);
    return handle;
}

// 0x002D2DAC
void SoundObjFurniture::Stop()
{
    if (!m_IsActive) {
        return;
    }
    if (m_pInfo != NULL) {
        sead::SoundHandle* handle = FindHandle(m_HandleSet.m_Handles, m_pInfo->soundOn);
        if (handle != NULL && handle->m_pSound != NULL) {
            handle->m_pSound->Stop(0);
        }
        handle = FindHandle(m_HandleSet.m_Handles, m_pInfo->soundOff);
        if (handle != NULL && handle->m_pSound != NULL) {
            handle->m_pSound->Stop(0);
        }
        handle = FindHandle(m_HandleSet.m_Handles, m_LastCue);
        if (handle != NULL && handle->m_pSound != NULL) {
            handle->m_pSound->Stop(0);
        }
    }
    m_IsSwitch = false;
    m_IsOn = false;
    m_pCues = NULL;
    m_pInfo = NULL;
    SoundFurnitureMgr::s_pInstance->Unregister(m_Index);
    m_Index = 0xFFFFFFFF;
    SoundObj<2>::Stop();
}

// 0x002D2F6C
void SoundObjFurniture::SetKind(u32 kind)
{
    if (kind >= KIND_NUM) {
        m_pInfo = &s_Infos[0];
        m_Kind = 0;
    } else {
        m_Kind = kind;
        m_pInfo = &s_Infos[kind];
        if (m_pInfo->cueSet != 0) {
            m_pCues = s_Cues[m_pInfo->cueSet];
        }
    }
    m_IsSwitch = IsSwitch(kind);
}

// 0x002D2FE0
void SoundObjFurniture::TurnOn()
{
    if (m_pInfo == NULL) {
        return;
    }
    if (!m_IsSwitch) {
        m_IsOn = true;
    }
    if (m_pInfo->soundOn != 0) {
        PlaySound(m_pInfo->soundOn, false);
    }
}
