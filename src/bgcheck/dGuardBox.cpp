#include "bgcheck/dMoveBg.h"
#include "bgcheck/dGuardBox.h"

namespace bgcheck {
// ctor candidate(s) 0x006154C0 (unverified)
bgcheck::GuardBox::GuardBox()
{
}

// 0x00317F74 slot 0x00 | virtual slot, introduced by collision::World::Body
void bgcheck::GuardBox::vf_0x00()
{
}

// 0x006154E0 slot 0x04 | virtual slot, introduced by collision::World::Body
void bgcheck::GuardBox::vf_0x04()
{
}

// 0x00615434 slot 0x0C | virtual slot, introduced by bgcheck::MoveBg
void bgcheck::GuardBox::vf_0x0C()
{
}

} // namespace bgcheck
