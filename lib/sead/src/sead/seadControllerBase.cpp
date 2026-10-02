#include "sead/seadControllerBase.h"

namespace sead {
// ctor candidate(s) 0x0054471C (unverified)
sead::ControllerBase::ControllerBase()
{
}

// 0x0074BD64 slot 0x00 | virtual slot, introduced by sead::ControllerBase
void sead::ControllerBase::vf_0x00()
{
}

// 0x0074BD18 slot 0x04 | virtual slot, introduced by sead::ControllerBase
void sead::ControllerBase::vf_0x04()
{
}

// 0x00544264 | nintendogs:bytes [tier A]
void sead::ControllerBase::getStickHold_(unsigned, const sead::Vector2<float>&, float, float, int)
{
}

// 0x00544554 | nintendogs:callseq [tier A]
void sead::ControllerBase::updateDerivativeParams_(unsigned, bool)
{
}

// 0x0054471C | nintendogs:callseq [tier A]
sead::ControllerBase::ControllerBase(int, int, int, int)
{
}

} // namespace sead
