#include "collision/dWorld_Body.h"
#include "collision/dWorld.h"
#include "bgcheck/dMoveBg.h"

namespace bgcheck {
// ctor candidate(s) 0x0061532C (unverified)
bgcheck::MoveBg::MoveBg()
{
}

// 0x00615384 slot 0x00 | virtual slot, introduced by collision::World::Body
void bgcheck::MoveBg::vf_0x00()
{
}

// 0x00615360 slot 0x04 | virtual slot, introduced by collision::World::Body
void bgcheck::MoveBg::vf_0x04()
{
}

// 0x00615270 slot 0x08 | virtual slot, introduced by bgcheck::MoveBg
void bgcheck::MoveBg::vf_0x08()
{
}

// 0x0061526C slot 0x0C | virtual slot, introduced by bgcheck::MoveBg
void bgcheck::MoveBg::vf_0x0C()
{
}

} // namespace bgcheck
