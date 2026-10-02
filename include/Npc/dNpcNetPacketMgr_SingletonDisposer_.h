#pragma once

#include "decomp.h"
#include "Npc/dNpcNetPacketMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N15NpcNetPacketMgr18SingletonDisposer_E @ 0x008CDA90
// vtable 0x008FBA00 (vptr 0x008FBA08), offset_to_top 0, 2 entries
class NpcNetPacketMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0029D130 (unverified)
    virtual ~SingletonDisposer_(); // 0x0029D204 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0029D1C0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
