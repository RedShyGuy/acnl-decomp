#pragma once

#include "decomp.h"
#include "Other/dIslandData.h"
#include "sead/seadIDisposer.h"

// RTTI N10IslandData18SingletonDisposer_E @ 0x008CD834
// vtable 0x008FA974 (vptr 0x008FA97C), offset_to_top 0, 2 entries
class IslandData::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x001AD148 (unverified)
    virtual ~SingletonDisposer_(); // 0x001AD230 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001AD1CC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
