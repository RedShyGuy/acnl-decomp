#include "Sound/dSoundObjPlayer.h"
#include "Sound/dSoundMgr.h"

// 0x002785A0
SoundObjPlayer::SoundObjPlayer() : m_IsListener(false)
{
    m_Priority = 0;
}

// 0x00278700
// 0x00278608 (deleting dtor)
SoundObjPlayer::~SoundObjPlayer()
{
    Stop();
}

// 0x00278354
void SoundObjPlayer::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (!m_IsActive) {
        m_Footstep.Init(this);
        SoundObjBase::Start(world, relative);
    }
}

// 0x00278394
void SoundObjPlayer::Walk(f32 speed, u32 walkSound, u32 groundValue)
{
    UpdateChanged();
    m_Footstep.Walk(speed, walkSound, groundValue);
}

// 0x002783DC
void SoundObjPlayer::Jump(u32 walkSound, u32 groundValue)
{
    UpdateChanged();
    m_Footstep.Jump(walkSound, groundValue);
}

// 0x00278414
void SoundObjPlayer::Land(u32 walkSound, u32 groundValue)
{
    UpdateChanged();
    m_Footstep.Land(walkSound, groundValue);
}

// 0x00278448
void SoundObjPlayer::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    SoundObj<4>::SetPosition(world, relative);
    if (m_IsListener) {
        SoundMgr::s_pInstance->SetListenerPos(world);
    }
}

// 0x0027853C
void SoundObjPlayer::Stop()
{
    if (m_IsActive) {
        m_Footstep.Stop();
        SoundObj<4>::Stop();
    }
}
