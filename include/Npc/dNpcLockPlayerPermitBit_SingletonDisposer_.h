#pragma once

#include "decomp.h"
#include "Npc/dNpcLockPlayerPermitBit.h"
#include "sead/seadIDisposer.h"

// RTTI N22NpcLockPlayerPermitBit18SingletonDisposer_E @ 0x008CDB8C
// vtable 0x008FBBA4 (vptr 0x008FBBAC), offset_to_top 0, 2 entries
class NpcLockPlayerPermitBit::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00331DA0 (unverified)
    virtual ~SingletonDisposer_(); // 0x00331FA0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00331F5C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
