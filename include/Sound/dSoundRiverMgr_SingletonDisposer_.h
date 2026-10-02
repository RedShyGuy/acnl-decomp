#pragma once

#include "decomp.h"
#include "Sound/dSoundRiverMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N13SoundRiverMgr18SingletonDisposer_E @ 0x008CD9C4
// vtable 0x008FB414 (vptr 0x008FB41C), offset_to_top 0, 2 entries
class SoundRiverMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00247274 (unverified)
    virtual ~SingletonDisposer_(); // 0x00247364 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00247310 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
