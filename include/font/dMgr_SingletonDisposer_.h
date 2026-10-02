#pragma once

#include "decomp.h"
#include "font/dMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N4font3Mgr18SingletonDisposer_E @ 0x008D10E4
// vtable 0x00904864 (vptr 0x0090486C), offset_to_top 0, 2 entries
class font::Mgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0052EAE4 (unverified)
    virtual ~SingletonDisposer_(); // 0x0052ECFC slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0052ECAC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
