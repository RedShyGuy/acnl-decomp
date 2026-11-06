#include "Sound/dSoundSharedObjMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundObjSimple.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "sead/seadNew.h"

// 0x00128CB8
SoundSharedObjMgr::SoundSharedObjMgr() : m_IsInitialized(false), m_IsDisabled(false)
{
}

// 0x002D3200
void SoundSharedObjMgr::initialize(sead::AudioMgr*, sead::Heap* heap)
{
    if (m_IsInitialized) {
        return;
    }
    nn::math::VEC3 world(1536.0f, 110.0f, 1536.0f);
    nn::math::VEC3 relative(0.0f, 0.0f, 0.0f);
    for (s32 i = 0; i < 4; i++) {
        m_Entries[i].obj = new (heap, 4) SoundObjSimple();
        m_Entries[i].obj->Start(&world, &relative);
        m_Entries[i].owner = NULL;
        m_Entries[i].soundId = 0xFFFFFFFF;
    }
    m_IsInitialized = true;
}

// 0x002D332C
void SoundSharedObjMgr::PlaySound(u32 soundId, const nn::math::VEC3* world, const nn::math::VEC3* relative,
                                  const void* owner, s16 value, bool isIgnoreMaster)
{
    if (!m_IsInitialized || m_IsDisabled) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].owner == NULL) {
            m_Entries[i].obj->SetIsIgnoreMaster(isIgnoreMaster);
            m_Entries[i].obj->m_VolumeScale = 1.0f;
            m_Entries[i].obj->SetPosition(world, relative);
            m_Entries[i].obj->StartSound(soundId, value);
            m_Entries[i].owner = owner;
            m_Entries[i].soundId = soundId;
            return;
        }
    }
}

// 0x002D344C
void SoundSharedObjMgr::WriteVariable(u32 soundId, const void* owner, s16 value, s32 variable)
{
    if (variable == -1) {
        variable = 15;
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].owner == owner && m_Entries[i].soundId == soundId) {
            sead::SoundHandle* handle = &m_Entries[i].obj->m_HandleSet.m_Handles[0];
            if (handle != NULL && handle->GetId() == soundId) {
                nw::snd::SequenceSoundHandle sequence(handle);
                if (sequence.m_pSound != NULL) {
                    sequence.m_pSound->WriteVariable(variable, value);
                }
            }
        }
    }
}

// 0x002D3518
void SoundSharedObjMgr::OnEnterStage()
{
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].obj != NULL) {
            m_Entries[i].obj->OnEnterStage();
        }
    }
    m_IsDisabled = false;
}

// 0x002D3570
void SoundSharedObjMgr::calc()
{
    if (SoundMgr::s_pInstance->IsPaused() || !m_IsInitialized) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].owner != NULL &&
            m_Entries[i].obj->m_HandleSet.m_Handles[0].GetId() != m_Entries[i].soundId) {
            m_Entries[i].owner = NULL;
            m_Entries[i].soundId = 0xFFFFFFFF;
        }
    }
}

// 0x002D3634
void SoundSharedObjMgr::HoldSound(u32 soundId, const nn::math::VEC3* world, const nn::math::VEC3* relative,
                                  const void* owner)
{
    if (!m_IsInitialized || m_IsDisabled) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].owner == owner && m_Entries[i].soundId == soundId) {
            m_Entries[i].obj->HoldSound(soundId);
            return;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].owner == NULL) {
            m_Entries[i].obj->SetIsFaded(soundId != SE_OBJ_BALLOON_HIT_FALL);
            m_Entries[i].obj->SetPosition(world, relative);
            m_Entries[i].obj->HoldSound(soundId);
            m_Entries[i].obj->m_VolumeScale = 1.0f;
            m_Entries[i].owner = owner;
            m_Entries[i].soundId = soundId;
            return;
        }
    }
}

// 0x002D37B8
void SoundSharedObjMgr::PlaySound(u32 soundId, const nn::math::VEC3* world, const nn::math::VEC3* relative,
                                  const void* owner, bool isIgnoreMaster)
{
    PlaySound(soundId, world, relative, owner, -1, isIgnoreMaster);
}

// 0x002D37E4
void SoundSharedObjMgr::PlaySoundWithValue(u32 soundId, const nn::math::VEC3* world, const nn::math::VEC3* relative,
                                           const void* owner, s16 value, bool isIgnoreMaster)
{
    PlaySound(soundId, world, relative, owner, value, isIgnoreMaster);
}

// 0x002D3804
void SoundSharedObjMgr::finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        m_Entries[i].obj->Stop();
        delete m_Entries[i].obj;
        m_Entries[i].obj = NULL;
        m_Entries[i].owner = NULL;
        m_Entries[i].soundId = 0xFFFFFFFF;
    }
    m_IsInitialized = false;
}

// 0x002D38B4
void SoundSharedObjMgr::SetVolumeScale(const void* owner, f32 scale)
{
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].owner == owner) {
            m_Entries[i].obj->m_VolumeScale = scale;
        }
    }
}

// 0x002D38FC
void SoundSharedObjMgr::StopSounds(s32 fadeFrames)
{
    if (fadeFrames == -1) {
        fadeFrames = 5;
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].obj != NULL) {
            m_Entries[i].obj->StopSounds(fadeFrames);
        }
    }
}

// 0x002D3964
void SoundSharedObjMgr::StopSounds(const void* owner, s32 fadeFrames)
{
    if (fadeFrames == -1) {
        fadeFrames = 5;
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].owner == owner) {
            m_Entries[i].obj->StopSounds(fadeFrames);
        }
    }
}

// 0x002D39D0
void SoundSharedObjMgr::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative, const void* owner)
{
    for (s32 i = 0; i < 4; i++) {
        if (m_Entries[i].owner == owner) {
            m_Entries[i].obj->SetPosition(world, relative);
            return;
        }
    }
}

// 0x0013E340
// 0x002D3A2C (deleting dtor)
SoundSharedObjMgr::~SoundSharedObjMgr()
{
    SoundSharedObjMgr::finalize();
}
