#pragma once

#include "decomp.h"
#include "Npc/dNpcPermitBitMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N15NpcPermitBitMgr18SingletonDisposer_E @ 0x008CDAA8
// vtable 0x008FBA20 (vptr 0x008FBA28), offset_to_top 0, 2 entries
class NpcPermitBitMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0029D61C (unverified)
    virtual ~SingletonDisposer_(); // 0x0029D6D8 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0029D694 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
