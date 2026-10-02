#pragma once

#include "decomp.h"
#include "Sound/dSoundIHaniwaControl.h"

// RTTI 14SoundConductor @ 0x008CBD9C
// vtable 0x008F073C (vptr 0x008F0744), offset_to_top 0, 9 entries
class SoundConductor : public ::SoundIHaniwaControl
{
public:
    SoundConductor(); // ctor address unknown
    virtual void vf_0x00(); // 0x0027693C slot 0x00 | virtual slot, introduced by SoundConductor
    virtual void vf_0x04(); // 0x002768FC slot 0x04 | virtual slot, introduced by SoundConductor
    virtual void vf_0x08(); // 0x00719AC8 slot 0x08 | virtual slot, introduced by SoundConductor
    virtual void vf_0x0C(); // 0x00719AC0 slot 0x0C | virtual slot, introduced by SoundConductor
    virtual void vf_0x10(); // 0x00719A60 slot 0x10 | virtual slot, introduced by SoundConductor
    virtual void vf_0x14(); // 0x00719A68 slot 0x14 | virtual slot, introduced by SoundConductor
    virtual void vf_0x18(); // 0x0027645C slot 0x18 | virtual slot, introduced by SoundConductor
    virtual void vf_0x1C(); // 0x00719AB8 slot 0x1C | virtual slot, introduced by SoundConductor
    virtual void vf_0x20(); // 0x007247D0 slot 0x20 | virtual slot, introduced by SoundConductor
};
