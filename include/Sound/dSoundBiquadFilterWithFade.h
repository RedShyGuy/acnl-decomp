#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"
#include "nn/snd/CTR/snd_Types.h"

// RTTI 25SoundBiquadFilterWithFade @ 0x008CD078
// vtable 0x008F7D7C (vptr 0x008F7D84), offset_to_top 0, 3 entries
// A user biquad filter whose value (0..1) picks one of a table of coefficient sets (member names are ours).
class SoundBiquadFilterWithFade : public ::nw::snd::BiquadFilterCallback
{
public:
    SoundBiquadFilterWithFade(); // 0x0033FC54
    virtual ~SoundBiquadFilterWithFade();
    virtual void GetCoefficients(int, f32 value, nn::snd::CTR::BiquadFilterCoefficients* pCoefficients) const override; // 0x0072677C slot 0x08 (a copy in the shared area)

    const nn::snd::CTR::BiquadFilterCoefficients* m_pTable; // 0x04
    u32 _8;                                                  // 0x08
    s32 m_Count;                                             // 0x0C entries in the table
};
ASSERT_SIZE(SoundBiquadFilterWithFade, 0x10);
