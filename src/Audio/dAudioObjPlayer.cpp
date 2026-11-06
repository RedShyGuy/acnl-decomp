#include "Audio/dAudioObjPlayer.h"
#include "Sound/sound_Extern.h"

// 0x00250190
void AudioObjPlayer::Start(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative);
    m_IsStarted = true;
}

// 0x002501C8
void AudioObjPlayer::Walk(f32 speed, u32 walkSound, u32 groundValue)
{
    // the speed relative to the fastest step (0..1)
    f32 rate = speed / m_MaxSpeed;
    if (rate < 0.0f) {
        rate = 0.0f;
    } else if (rate > 1.0f) {
        rate = 1.0f;
    }
    m_Obj.Walk(rate, walkSound, groundValue);
}

// 0x00250200
void AudioObjPlayer::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}
