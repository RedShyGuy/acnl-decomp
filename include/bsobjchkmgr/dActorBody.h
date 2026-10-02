#pragma once

#include "decomp.h"
#include "collision/dWorld.h"
#include "collision/dWorld_Body.h"

namespace bsobjchkmgr {
// RTTI N11bsobjchkmgr9ActorBodyE @ 0x008CD8F4
// vtable 0x008FAE4C (vptr 0x008FAE54), offset_to_top 0, 2 entries
class ActorBody : public ::collision::World::Body
{
public:
    ActorBody(); // ctor candidate(s) 0x001E5E88 (unverified)
    virtual void vf_0x00(); // 0x006F89D8 slot 0x00 | virtual slot, introduced by collision::World::Body
    virtual void vf_0x04(); // 0x001E5EA8 slot 0x04 | virtual slot, introduced by collision::World::Body
};
} // namespace bsobjchkmgr
