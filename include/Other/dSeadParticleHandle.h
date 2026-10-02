#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

// RTTI 18SeadParticleHandle @ 0x008CC8BC
// vtable 0x008F4A94 (vptr 0x008F4A9C), offset_to_top 0, 2 entries
class SeadParticleHandle : public ::ssys::st::ListNode
{
public:
    SeadParticleHandle(); // ctor candidate(s) 0x00316940 (unverified)
    virtual ~SeadParticleHandle(); // 0x002E6F10 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x002E6EC4 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
