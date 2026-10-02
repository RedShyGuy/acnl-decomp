#pragma once

#include "decomp.h"
#include "Npc/dNpcDJCtrl.h"
#include "sead/seadIDisposer.h"

// RTTI N9NpcDJCtrl18SingletonDisposer_E @ 0x008D40E4
// vtable 0x0090C268 (vptr 0x0090C270), offset_to_top 0, 2 entries
class NpcDJCtrl::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x006EF048 (unverified)
    virtual ~SingletonDisposer_(); // 0x006EF144 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006EF100 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
