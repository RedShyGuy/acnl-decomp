#include "Sound/dSoundTvBase.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "nw/snd/snd_SoundPlayer.h"

// the position the object starts at
// 0x009851D4
nn::math::VEC3 SoundTvBase::s_Origin(0.0f, 0.0f, 0.0f);
// 0x0078505C (static initializer)

// 0x001DBB70
SoundTvBase::SoundTvBase()
    : m_Position(0.0f, 0.0f, 0.0f), m_Program(0), m_ItemId(0xFFFFFFFF), m_State(STATE_NONE), m_LoadState(LOAD_NONE),
      m_HeapIndex(HEAP_NONE), m_PlayArg(0), m_IsFirstProgram(true)
{
}

// 0x001DBC0C
// 0x001DBBF8 (deleting dtor)
SoundTvBase::~SoundTvBase()
{
    Stop();
}

// 0x001DB544
void SoundTvBase::Start()
{
    if (m_State == STATE_NONE) {
        m_Obj.m_VolumeScale = 0.0f;
        m_Obj.Start(&s_Origin, &s_Origin);
        m_FilterFader.Reset(0.0f);
        m_VolumeFader.Reset(0.0f);
        m_State = STATE_STARTED;
    }
}

// 0x001DB5A4
void SoundTvBase::UpdateProgram(s32 frame)
{
    // empty (in the original too)
}

// 0x001DB7CC
void SoundTvBase::OnLoaded()
{
    // empty (in the original too)
}

// 0x001DB5B0
void SoundTvBase::Mute()
{
    if (m_State == STATE_PLAYING) {
        m_VolumeFader.SetTarget(0.0f, 0);
        m_Obj.m_VolumeScale = 0.0f;
        m_Obj.SoundObj<2>::UpdateChanged();
        for (s32 i = 0; i < 2; i++) {
            m_Obj.UpdateHandle(i);
        }
        m_State = STATE_READY;
    }
}

// 0x001DB68C
void SoundTvBase::Play(u8 arg)
{
    if (m_State == STATE_READY) {
        m_PlayArg = arg;
        m_VolumeFader.SetTarget(0.45f, 0);
        m_IsFirstProgram = true;
        m_State = STATE_PLAYING;
        m_Obj.m_VolumeScale = m_VolumeFader.GetValue();
    }
}

// 0x001DB6D4
bool SoundTvBase::Load()
{
    if (m_State == STATE_NONE) {
        return false;
    }
    SoundDataLoadMgr* mgr = SoundDataLoadMgr::s_pInstance;
    if (m_LoadState == LOAD_NONE) {
        // a free stage heap
        if (mgr->GetLevel(2) == 0 && !mgr->IsBusy(2)) {
            m_HeapIndex = 2;
        } else if (mgr->GetLevel(3) == 0 && !mgr->IsBusy(3)) {
            m_HeapIndex = 3;
        } else {
            return false;
        }
        mgr->Load(m_ItemId, m_HeapIndex);
        m_LoadState = LOAD_BUSY;
        return false;
    }
    if (m_LoadState != LOAD_BUSY) {
        return true;
    }
    if (mgr->IsBusy(m_HeapIndex)) {
        return false;
    }
    OnLoaded();
    m_LoadState = LOAD_DONE;
    m_State = STATE_READY;
    return true;
}

// 0x001DB7D0
void SoundTvBase::Update(s32 frame, u16 program, const nn::math::VEC3* position)
{
    if (m_Program != program) {
        s32 frames = 5;
        if (m_IsFirstProgram == true) {
            frames = 0;
            m_IsFirstProgram = false;
        }
        if (program == PROGRAM_LOUD_A || program == PROGRAM_LOUD_B) {
            m_FilterFader.SetTarget(0.6f, frames);
            m_VolumeFader.SetTarget(0.675f, frames);
        } else {
            m_FilterFader.SetTarget(0.0f, frames);
            m_VolumeFader.SetTarget(0.45f, frames);
        }
    }
    m_FilterFader.Update();
    m_VolumeFader.Update();
    if (m_State == STATE_PLAYING) {
        UpdateProgram(frame);
        f32 filter = m_FilterFader.GetValue();
        if (filter > 0.0f) {
            SoundMgr::s_pInstance->GetSoundPlayer(15)->SetBiquadFilter(2, filter);
        } else {
            SoundMgr::s_pInstance->GetSoundPlayer(15)->SetBiquadFilter(0, 0.0f);
        }
        m_Obj.m_VolumeScale = m_VolumeFader.GetValue();
        f32 x = position->x;
        if (x > m_Position.x + 10.0f) {
            m_Position.x = m_Position.x + 10.0f;
        } else if (x < m_Position.x - 10.0f) {
            m_Position.x = m_Position.x - 10.0f;
        } else {
            m_Position.x = x;
        }
        f32 y = position->y;
        if (y > m_Position.y + 10.0f) {
            m_Position.y = m_Position.y + 10.0f;
        } else if (y < m_Position.y - 10.0f) {
            m_Position.y = m_Position.y - 10.0f;
        } else {
            m_Position.y = y;
        }
        m_Position.z = position->z;
    }
    m_Obj.SetPosition(&s_Origin, &m_Position);
    m_Program = program;
}

// 0x001DB990
void SoundTvBase::Stop()
{
    if (m_State == STATE_NONE) {
        return;
    }
    if (m_LoadState != LOAD_NONE) {
        m_Obj.m_VolumeScale = 0.0f;
        m_Obj.StopSounds(0);
        m_Obj.Stop();
        SoundDataLoadMgr* mgr = SoundDataLoadMgr::s_pInstance;
        if (mgr != NULL && mgr->HasHeap(m_HeapIndex)) {
            SoundDataLoadMgr::s_pInstance->UnloadOrDefer(0, m_HeapIndex);
        }
        m_HeapIndex = HEAP_NONE;
    }
    m_State = STATE_NONE;
}

// 0x001DBA9C
void SoundTvBase::PlaySound(u32 index, u32 soundId)
{
    sead::SoundHandle* handle = &m_Obj.m_HandleSet.m_Handles[index];
    handle->Stop(0);
    SoundMgr::s_pInstance->StartSound(handle, soundId);
    m_Obj.SoundObj<2>::UpdateChanged();
    m_Obj.UpdateHandle(index);
}

// 0x001DB5A8
void SoundTvHandle::Mute()
{
    m_pTv->Mute();
}

// 0x001DB684
void SoundTvHandle::Play(u8 arg)
{
    m_pTv->Play(arg);
}
