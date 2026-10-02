#pragma once

#include "decomp.h"
#include "sead/seadFileDeviceMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N4sead13FileDeviceMgr18SingletonDisposer_E @ 0x008D165C
// vtable 0x0090556C (vptr 0x00905574), offset_to_top 0, 2 entries
class sead::FileDeviceMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0012C540 (unverified)
    virtual ~SingletonDisposer_(); // 0x00542DA8 slot 0x00 | nintendogs:bytes
    // 0x00542D28 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
