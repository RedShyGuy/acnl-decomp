#pragma once

#include "decomp.h"
#include "Sound/dSoundIHaniwaControl.h"

// RTTI 13SoundDJPlayer @ 0x008CBABC
// vtable 0x008EF794 (vptr 0x008EF79C), offset_to_top 0, 9 entries
class SoundDJPlayer : public ::SoundIHaniwaControl
{
public:
    class SingletonDisposer_;
    SoundDJPlayer(); // ctor address unknown
    virtual void vf_0x00(); // 0x00246E94 slot 0x00 | virtual slot, introduced by SoundDJPlayer
    virtual void vf_0x04(); // 0x00246E34 slot 0x04 | virtual slot, introduced by SoundDJPlayer
    virtual void vf_0x08(); // 0x00714954 slot 0x08 | virtual slot, introduced by SoundDJPlayer
    virtual void vf_0x0C(); // 0x0071494C slot 0x0C | virtual slot, introduced by SoundDJPlayer
    virtual void vf_0x10(); // 0x007147F4 slot 0x10 | virtual slot, introduced by SoundDJPlayer
    virtual void vf_0x14(); // 0x007147FC slot 0x14 | virtual slot, introduced by SoundDJPlayer
    virtual void vf_0x18(); // 0x00245AAC slot 0x18 | virtual slot, introduced by SoundDJPlayer
    virtual void vf_0x1C(); // 0x007148C0 slot 0x1C | virtual slot, introduced by SoundDJPlayer
    virtual void vf_0x20(); // 0x007147A0 slot 0x20 | virtual slot, introduced by SoundDJPlayer
    static SoundDJPlayer* s_pInstance; // 0x0094DD08
};
