#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 16SoundObjFieldObj @ 0x008CC3DC
// vtable 0x008F2E34 (vptr 0x008F2E3C), offset_to_top 0, 14 entries
class SoundObjFieldObj : public ::SoundObj<1>
{
public:
    SoundObjFieldObj(); // ctor candidate(s) 0x002BD5A4 (unverified)
    virtual ~SoundObjFieldObj(); // 0x002BD670 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002BD618 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x002BD3AC slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x002BD3C8 slot 0x10 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x1C(); // 0x002BD520 slot 0x1C | virtual slot, introduced by SoundObj<1>
    virtual void vf_0x20(); // 0x002BD558 slot 0x20 | virtual slot, introduced by SoundObj<1>
};
