#pragma once

#include "decomp.h"

// RTTI 11SoundTvBase @ 0x008CB360
// vtable 0x008ECE04 (vptr 0x008ECE0C), offset_to_top 0, 5 entries
class SoundTvBase
{
public:
    SoundTvBase(); // ctor address unknown
    virtual ~SoundTvBase(); // 0x001DBC0C slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x001DBBF8 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x001DB544 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x001DB7CC slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x001DB5A4 slot 0x10 | virtual slot, introduced by SoundTvBase
};
