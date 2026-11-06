#include "Sound/dSoundFootstep.h"
#include "Sound/dSoundHistoryMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundObjBase.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "nw/snd/internal/snd_SequenceSound.h"

// 0x0083CEB4
const f32 SoundFootstep::s_SizePitches[SIZE_NUM] = {1.6f, 1.1f, 0.8f};

namespace {
// SoundMgr::m_GroundCover
enum GroundCover
{
    GROUND_COVER_NONE = 0,
    GROUND_COVER_LEAVES = 1,
    GROUND_COVER_SNOW = 2,
};

inline u32 GetCoveredSound()
{
    u8 cover = SoundMgr::s_pInstance->m_GroundCover;
    if (cover == GROUND_COVER_LEAVES) {
        return SE_ACT_WALK_L_LEAF;
    }
    if (cover == GROUND_COVER_SNOW) {
        return SE_ACT_WALK_L_SNOW;
    }
    return SE_ACT_WALK_L_GRASS;
}

inline void ApplyOwner(nw::snd::SoundHandle& handle, const SoundObjBase* owner)
{
    handle.SetPan(owner->m_Pan);
    handle.SetSurroundPan(owner->m_SurroundPan);
}
} // namespace

// 0x002A6B9C
SoundFootstep::SoundFootstep()
    : m_pOwner(NULL), m_Variable(0), m_IsRightFoot(false), m_Size(SIZE_MEDIUM), m_IsBell(false), m_IsHeel(false),
      m_Speed(0.0f)
{
}

// 0x002A6574
void SoundFootstep::Init(SoundObjBase* owner)
{
    if (m_pOwner == NULL) {
        m_pOwner = owner;
    }
}

// 0x002A6B60
void SoundFootstep::Stop()
{
    if (m_pOwner != NULL) {
        m_pOwner = NULL;
    }
}

u32 SoundFootstep::GetGroundSound(u32 walkSound, u32 groundValue)
{
    if (walkSound == SE_ACT_WALK_L_SOIL) {
        if (groundValue > 55) {
            walkSound = GetCoveredSound();
            if (groundValue < 110) {
                walkSound += 2; // the thin variant
            }
        }
    } else if (walkSound == SE_ACT_WALK_L_GRASS) {
        walkSound = GetCoveredSound();
    }
    return walkSound;
}

// 0x002A6584
void SoundFootstep::PlayOnce(u32 baseSound, u32 walkSound, u32 groundValue)
{
    walkSound = GetGroundSound(walkSound, groundValue);
    SoundMgr* mgr = SoundMgr::s_pInstance;
    nw::snd::SoundHandle handle;
    // the jump and land sounds have one sound for each pair of walk sounds
    mgr->StartSound(&handle, baseSound + (walkSound - SE_ACT_WALK_L_GRASS) / 2);
    handle.SetVolume(m_pOwner->m_Volume, 0);
    ApplyOwner(handle, m_pOwner);
    nw::snd::SequenceSoundHandle sequence(&handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(10, m_Variable);
    }
    if (m_IsBell) {
        nw::snd::SoundHandle bell;
        SoundMgr::s_pInstance->StartSound(&bell, SE_ACT_WALK_BELL);
        bell.SetVolume(m_pOwner->m_Volume, 0);
        ApplyOwner(bell, m_pOwner);
    }
    if (m_IsHeel) {
        nw::snd::SoundHandle heel;
        SoundMgr::s_pInstance->StartSound(&heel, SE_ACT_WALK_L_HEEL);
        heel.SetVolume(m_pOwner->m_Volume * (walkSound == SE_ACT_WALK_L_WATER ? 0.1f : 0.5f), 0);
        ApplyOwner(heel, m_pOwner);
    }
}

// 0x002A67F8
void SoundFootstep::Walk(f32 speed, u32 walkSound, u32 groundValue)
{
    if (walkSound == 0xFFFFFFFF) {
        return;
    }
    if (m_pOwner == NULL) {
        return;
    }
    f32 ownerVolume = m_pOwner->m_Volume;
    if (ownerVolume == 0.0f) {
        return;
    }
    // only the bell when the same step already sounds as loud
    bool isBellOnly = false;
    if (SoundHistoryMgr::s_pInstance->Contains(walkSound, ownerVolume)) {
        if (m_IsBell != true) {
            return;
        }
        isBellOnly = true;
    }
    u32 foot = m_IsRightFoot ? 1 : 0;
    m_IsRightFoot = !m_IsRightFoot;
    m_Speed = speed;
    f32 maxVolume;
    f32 maxPitch = 1.0f;
    if (m_pOwner->m_Priority == 1) {
        maxVolume = 0.7f;
    } else {
        // (slow steps)
        maxVolume = speed > 20.0f / 27.0f ? 1.0f : 0.82f;
        maxPitch = speed > 20.0f / 27.0f ? 1.0f : 0.82f;
    }
    f32 volume = (0.2f + speed * (maxVolume - 0.2f)) * ownerVolume;
    f32 pitch = (0.8f + speed * (maxPitch - 0.8f)) * s_SizePitches[m_Size];
    if (!isBellOnly) {
        SoundHistoryMgr::s_pInstance->Add(walkSound, 5, volume);
    }
    walkSound = GetGroundSound(walkSound, groundValue);
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (!isBellOnly) {
        nw::snd::SoundHandle handle;
        mgr->StartSound(&handle, walkSound + foot);
        handle.SetVolume(volume, 0);
        handle.SetPitch(pitch);
        ApplyOwner(handle, m_pOwner);
        nw::snd::SequenceSoundHandle sequence(&handle);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteVariable(10, m_Variable);
        }
    }
    if (m_IsBell) {
        nw::snd::SoundHandle bell;
        mgr->StartSound(&bell, SE_ACT_WALK_BELL);
        bell.SetVolume(volume, 0);
        ApplyOwner(bell, m_pOwner);
    }
    if (m_IsHeel) {
        nw::snd::SoundHandle heel;
        mgr->StartSound(&heel, SE_ACT_WALK_L_HEEL + foot);
        heel.SetVolume(volume * (walkSound == SE_ACT_WALK_L_WATER ? 0.1f : 0.5f), 0);
        ApplyOwner(heel, m_pOwner);
    }
}

// 0x002A6B74
void SoundFootstep::Jump(u32 walkSound, u32 groundValue)
{
    PlayOnce(SE_ACT_JUMP_GRASS, walkSound, groundValue);
}

// 0x002A6B88
void SoundFootstep::Land(u32 walkSound, u32 groundValue)
{
    PlayOnce(SE_ACT_LAND_GRASS, walkSound, groundValue);
}
