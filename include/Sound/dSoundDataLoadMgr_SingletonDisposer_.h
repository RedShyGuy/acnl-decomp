#pragma once

#include "decomp.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N16SoundDataLoadMgr18SingletonDisposer_E @ 0x008CDAD8
// vtable 0x008FBA5C (vptr 0x008FBA64), offset_to_top 0, 2 entries
class SoundDataLoadMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0012392C (unverified)
    virtual ~SingletonDisposer_(); // 0x002BD010 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x002BCFAC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
