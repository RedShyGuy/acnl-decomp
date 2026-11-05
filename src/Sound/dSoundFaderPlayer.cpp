#include "Sound/dSoundFaderPlayer.h"
#include "nw/snd/snd_SoundPlayer.h"

// 0x0013E33C
// 0x002BD3A8 (deleting dtor)
SoundFaderPlayer::~SoundFaderPlayer()
{
    // empty (in the original too)
}

// 0x002BD1CC
f32 SoundFaderPlayer::Update()
{
    if (m_pPlayer == NULL)
    {
        return 0.0f;
    }
    f32 value = SoundFader::Update();
    ApplyVolume();
    return value;
}

// 0x002BD30C
void SoundFaderPlayer::SetTarget(f32 target, s32 frames)
{
    if (m_pPlayer == NULL)
    {
        return;
    }
    // reversing a fade that is under way: only the part of the frames for the way back
    if (m_Frames > 0 && m_Target < m_Value && m_Value < target)
    {
        frames = static_cast<s32>((target - m_Value) * frames / (target - m_Target));
    }
    SoundFader::SetTarget(target, frames);
    ApplyVolume();
}
