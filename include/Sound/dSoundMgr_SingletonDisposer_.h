#pragma once

#include "decomp.h"
#include "Sound/dSoundMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N8SoundMgr18SingletonDisposer_E @ 0x008D403C
// vtable 0x0090C0F4 (vptr 0x0090C0FC), offset_to_top 0, 2 entries
class SoundMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0012155C (unverified)
    virtual ~SingletonDisposer_(); // 0x006B3C0C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006B3BBC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
