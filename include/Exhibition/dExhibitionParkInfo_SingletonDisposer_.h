#pragma once

#include "decomp.h"
#include "Exhibition/dExhibitionParkInfo.h"
#include "sead/seadIDisposer.h"

// RTTI N18ExhibitionParkInfo18SingletonDisposer_E @ 0x008CDB2C
// vtable 0x008FBAC8 (vptr 0x008FBAD0), offset_to_top 0, 2 entries
class ExhibitionParkInfo::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x002E11B8 (unverified)
    virtual ~SingletonDisposer_(); // 0x002E129C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x002E1258 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
