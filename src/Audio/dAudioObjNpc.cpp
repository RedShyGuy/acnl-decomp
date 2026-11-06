#include "Audio/dAudioObjNpc.h"
#include "Sound/sound_Extern.h"

// 0x001C0DCC
void AudioObjNpc::Start(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative);
    m_IsStarted = true;
}

// 0x001C0E04
void AudioObjNpc::SetHumAlwaysMadeUp(bool isAlwaysMadeUp)
{
    m_Obj.m_Hum.m_IsAlwaysMadeUp = isAlwaysMadeUp;
}

// 0x001C0E0C
void AudioObjNpc::Walk(f32 speed, u32 walkSound, u32 groundValue)
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

// 0x001C0E44
void AudioObjNpc::SetFootstepVariable(s32 value)
{
    m_Obj.m_Footstep.m_Variable = value;
}
