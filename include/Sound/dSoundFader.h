#pragma once

#include "decomp.h"

// RTTI 10SoundFader @ 0x008CB1E8
// vtable 0x008EC49C (vptr 0x008EC4A4), offset_to_top 0, 5 entries
class SoundFader
{
public:
    SoundFader(); // ctor candidate(s) 0x0012674C, 0x0012F8B8 (unverified)
    virtual void vf_0x00(); // 0x0013E1A8 slot 0x00 | virtual slot, introduced by SoundFader
    virtual void vf_0x04(); // 0x001B0B48 slot 0x04 | virtual slot, introduced by SoundFader
    virtual void vf_0x08(); // 0x0012F8A4 slot 0x08 | virtual slot, introduced by SoundFader
    virtual void vf_0x0C(); // 0x001B0AC0 slot 0x0C | virtual slot, introduced by SoundFader
    virtual void vf_0x10(); // 0x0070BDF4 slot 0x10 | virtual slot, introduced by SoundFader
};
