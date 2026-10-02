#pragma once

#include "decomp.h"
#include "Npc/dNpcNetShareInfo.h"
#include "sead/seadIDisposer.h"

// RTTI N15NpcNetShareInfo18SingletonDisposer_E @ 0x008CDA9C
// vtable 0x008FBA10 (vptr 0x008FBA18), offset_to_top 0, 2 entries
class NpcNetShareInfo::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0029D538 (unverified)
    virtual ~SingletonDisposer_(); // 0x0029D5E8 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0029D5A4 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
