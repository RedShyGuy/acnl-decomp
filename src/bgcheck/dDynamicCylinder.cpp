#include "collision/dWorld_Body.h"
#include "collision/dWorld.h"
#include "bgcheck/dDynamicCylinder.h"

namespace bgcheck {
// ctor candidate(s) 0x00615040 (unverified)
bgcheck::DynamicCylinder::DynamicCylinder()
{
}

// 0x00615084 slot 0x00 | virtual slot, introduced by collision::World::Body
void bgcheck::DynamicCylinder::vf_0x00()
{
}

// 0x00615060 slot 0x04 | virtual slot, introduced by collision::World::Body
void bgcheck::DynamicCylinder::vf_0x04()
{
}

} // namespace bgcheck
