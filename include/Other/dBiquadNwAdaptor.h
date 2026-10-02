#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

// RTTI 15BiquadNwAdaptor @ 0x008CBECC
// vtable 0x008F121C (vptr 0x008F1224), offset_to_top 0, 3 entries
class BiquadNwAdaptor : public ::nw::snd::BiquadFilterCallback
{
public:
    BiquadNwAdaptor(); // ctor candidate(s) 0x00283D8C (unverified)
    virtual void vf_0x00(); // 0x00283DB8 slot 0x00 | virtual slot, introduced by BiquadNwAdaptor
    virtual void vf_0x04(); // 0x00283DB4 slot 0x04 | virtual slot, introduced by BiquadNwAdaptor
    virtual void vf_0x08(); // 0x0071B6E0 slot 0x08 | virtual slot, introduced by BiquadNwAdaptor
};
