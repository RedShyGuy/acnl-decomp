#include "Sound/dSoundFader.h"

// 0x0013E1A8
// 0x001B0B48 (deleting dtor)
SoundFader::~SoundFader()
{
    // empty (in the original too)
}

// 0x0012F8A4
void SoundFader::Reset(f32 value)
{
    m_Value = value;
    m_Target = value;
    m_Frames = -1;
}

// 0x001B0AC0
f32 SoundFader::Update()
{
    s32 frames = m_Frames;
    if (frames == 0)
    {
        m_Frames = -1;
    }
    else if (frames > 0)
    {
        m_Frames = --frames;
        if (frames == 0)
        {
            m_Value = m_Target;
        }
        else
        {
            m_Value += m_Step;
        }
    }
    return m_Value;
}

// 0x001B0B00
void SoundFader::SetTarget(f32 target, s32 frames)
{
    if (frames < 0)
    {
        frames = 0;
    }
    m_Target = target;
    if (frames != 0)
    {
        m_Frames = frames;
        m_Step = (target - m_Value) / frames;
    }
    else
    {
        m_Value = target;
        m_Frames = -1;
    }
}
