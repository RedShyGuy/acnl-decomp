#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

// RTTI 25SoundBiquadFilterWithFade @ 0x008CD078
// vtable 0x008F7D7C (vptr 0x008F7D84), offset_to_top 0, 3 entries
class SoundBiquadFilterWithFade : public ::nw::snd::BiquadFilterCallback
{
public:
    SoundBiquadFilterWithFade(); // ctor candidate(s) 0x0033FC54 (unverified)
    virtual void vf_0x00(); // 0x0033FC78 slot 0x00 | virtual slot, introduced by SoundBiquadFilterWithFade
    virtual void vf_0x04(); // 0x0033FC74 slot 0x04 | virtual slot, introduced by SoundBiquadFilterWithFade
    virtual void vf_0x08(); // 0x0072677C slot 0x08 | virtual slot, introduced by SoundBiquadFilterWithFade
};
