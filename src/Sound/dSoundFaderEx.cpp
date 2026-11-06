#include "Sound/dSoundFaderEx.h"

// 0x001404A0
// 0x0020AA84 (deleting dtor)
SoundFaderEx::~SoundFaderEx()
{
    // empty (in the original too)
}

// 0x0012F988
void SoundFaderEx::Reset(f32 value)
{
    m_Value = value;
    m_Target = value;
    m_Frames = -1;
    m_Delay = 0;
}

// 0x0020A9D4
f32 SoundFaderEx::Update()
{
    if (m_Delay > 0)
    {
        m_Delay--;
        return m_Value;
    }
    return SoundFader::Update();
}

// 0x0020AA30
void SoundFaderEx::SetTarget(f32 target, s32 frames, s32 delay)
{
    SoundFader::SetTarget(target, frames);
    if (delay < 0)
    {
        delay = 0;
    }
    m_Delay = delay;
}

// 0x007132C8
s32 SoundFaderEx::GetRemainingFrames() const
{
    return m_Delay + m_Frames;
}
