#pragma once

#include "decomp.h"
#include "Sound/dSoundHaniwaMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N14SoundHaniwaMgr18SingletonDisposer_E @ 0x008CDA24
// vtable 0x008FB48C (vptr 0x008FB494), offset_to_top 0, 2 entries
class SoundHaniwaMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00276A28 (unverified)
    virtual ~SingletonDisposer_(); // 0x00276B3C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00276AF8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
