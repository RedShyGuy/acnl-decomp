#include "Other/dDuckingFader.h"

// 0x0014049C
// 0x00200028 (deleting dtor)
DuckingFader::~DuckingFader()
{
    // empty (in the original too)
}

// 0x001FFFC8
f32 DuckingFader::Update()
{
    f32 value = SoundFaderEx::Update();
    if (m_Value == 1.0f)
    {
        m_State = STATE_NONE;
    }
    else if (m_Target == 1.0f)
    {
        m_State = m_Delay > 0 ? STATE_UP_WAIT : STATE_UP;
    }
    else if (m_Delay > 0)
    {
        m_State = STATE_DOWN_WAIT;
    }
    else
    {
        m_State = m_Frames > 0 ? STATE_DOWN : STATE_DUCKED;
    }
    return value;
}
