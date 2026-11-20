#include "Sound/dSoundObjTheremin.h"
#include "Sound/dSoundFurnitureMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

// 0x009851C8
nn::math::VEC3 SoundObjTheremin::s_Position(0.0f, 0.0f, 0.0f);
// 0x0094E5F0
SoundObjTheremin::SoundParam SoundObjTheremin::s_SoundParam = {SE_FUR_THEREMIN, 0x1.ce2ea8p-15f};

// 0x00247850
SoundObjTheremin::SoundObjTheremin() : m_IsPlaying(false)
{
}

// 0x0024786C
SoundObjTheremin::~SoundObjTheremin()
{
    if (m_Obj.m_IsActive) {
        m_Obj.Stop();
    }
}

// 0x00247520
void SoundObjTheremin::Start()
{
    if (m_Obj.m_IsActive) {
        return;
    }
    m_Obj.Start(&s_Position, &s_Position, 0, false);
}

// 0x00247554
void SoundObjTheremin::Update(const nn::math::VEC3* position, f32 distance)
{
    if (!m_Obj.m_IsActive) {
        return;
    }
    SoundFurnitureMgr* mgr = SoundFurnitureMgr::s_pInstance;
    if (!(distance < 200.0f)) {
        return;
    }
    m_IsPlaying = true;
    const u32 index = m_Obj.m_Index;
    u8 state = mgr->m_Entries[index].state;
    if (state == SoundFurnitureMgr::STATE_NONE) {
        state = mgr->Play(index, s_SoundParam.soundId);
    }
    if (state != SoundFurnitureMgr::STATE_PLAYING) {
        return;
    }
    SoundMgr* soundMgr = SoundMgr::s_pInstance;
    soundMgr->HoldSound(&m_Handle, s_SoundParam.soundId);
    nw::snd::SequenceSoundHandle handle(&m_Handle);
    if (handle.m_pSound != NULL) {
        handle.m_pSound->WriteVariable(0, 15);
    }
    m_Obj.Update(&s_Position, position, 0.0f);
    m_Obj.SoundObjBase::Calc();

    // closer: louder (1.0 up to 35, 0.2 at 200), higher (variable 1: 63.5 up to 35, -64 at 200) and more of the
    // filter (0.525 up to 50, 0 from 135)
    f32 volume = 1.0f;
    if (distance >= 35.0f) {
        if (distance > 200.0f) {
            volume = 0.2f;
        } else {
            volume = (distance * 0.8f - 193.0f) * (-1.0f / 165.0f);
        }
    }
    f32 pitch;
    if (distance < 35.0f) {
        pitch = 63.5f;
    } else if (distance > 200.0f) {
        pitch = -64.0f;
    } else {
        pitch = (distance * 255.0f - 29880.0f) * (-1.0f / 165.0f) * 0.5f;
    }
    f32 filter;
    if (distance < 50.0f) {
        filter = 0.525f;
    } else if (distance > 135.0f) {
        filter = 0.0f;
    } else {
        filter = (distance * 0.525f - 70.875f) * (-1.0f / 85.0f);
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->m_Pan = m_Obj.m_Pan;
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->m_SurroundPan = m_Obj.m_SurroundPan;
    }
    f32 masterVolume = soundMgr->m_VolumeMgr.m_MasterFader.GetValue() * volume;
    if (masterVolume < 0.0f) {
        masterVolume = 0.0f;
    } else if (masterVolume > 1.0f) {
        masterVolume = 1.0f;
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetVolume(masterVolume, 0);
    }
    if (pitch < -128.0f) {
        pitch = -128.0f;
    } else if (pitch > 127.0f) {
        pitch = 127.0f;
    }
    if (handle.m_pSound != NULL) {
        handle.m_pSound->WriteVariable(1, static_cast<s16>(pitch));
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetBiquadFilter(1, filter);
    }
}

// 0x002477FC
void SoundObjTheremin::StopSound()
{
    if (!m_Obj.m_IsActive) {
        return;
    }
    // (a frame with sound only clears the flag; the next one without sound stops it)
    if (m_IsPlaying) {
        m_IsPlaying = false;
        return;
    }
    SoundFurnitureMgr::s_pInstance->Stop(m_Obj.m_Index);
}

// 0x00247838
void SoundObjTheremin::Stop()
{
    if (m_Obj.m_IsActive) {
        m_Obj.Stop();
    }
}
