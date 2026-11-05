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
