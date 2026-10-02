#pragma once

#include "decomp.h"
#include "aquarium/dAABB.h"

namespace aquarium {
// vtable +0x30000 in ModuleMusFish.cro, offset_to_top 0, 6 entries
class Capsule : public ::aquarium::AABB
{
public:
    Capsule(); // ctor address unknown
};
} // namespace aquarium
