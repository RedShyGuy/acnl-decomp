#pragma once

#include "decomp.h"
#include "Sound/dSoundHistoryMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N15SoundHistoryMgr18SingletonDisposer_E @ 0x008CDAB4
// vtable 0x008FBA30 (vptr 0x008FBA38), offset_to_top 0, 2 entries
class SoundHistoryMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00123680 (unverified)
    virtual ~SingletonDisposer_(); // 0x002A3910 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x002A38CC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
