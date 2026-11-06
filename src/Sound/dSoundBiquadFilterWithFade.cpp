#include "Sound/dSoundBiquadFilterWithFade.h"

// 0x0033FC54
SoundBiquadFilterWithFade::SoundBiquadFilterWithFade() : m_pTable(NULL), _8(0), m_Count(0)
{
}

// 0x0033FC78
// 0x0033FC74 (deleting dtor)
SoundBiquadFilterWithFade::~SoundBiquadFilterWithFade()
{
    // empty (in the original too)
}

// 0x0072677C
void SoundBiquadFilterWithFade::GetCoefficients(int, f32 value, nn::snd::CTR::BiquadFilterCoefficients* pCoefficients) const
{
    s32 last = m_Count - 1;
    s32 index = static_cast<s32>(last * value);
    if (index > last) {
        index = last;
    } else if (index < 0) {
        index = 0;
    }
    *pCoefficients = m_pTable[index];
}
