#include "Bs/dBsGrRenderer_Functor.h"
#include "Bs/dBsGrRenderer.h"
#include "Other/dBalloonString.h"

// 0x00213808 slot 0x00
// 0x002137C0 (deleting dtor)
BalloonString::~BalloonString()
{
}

// 0x00213358 slot 0x08 | virtual slot, introduced by BalloonString
void BalloonString::vf_0x08()
{
}

// 0x002134C4 slot 0x0C
const nn::math::MTX34* BalloonString::vf_0x0C(const nn::math::MTX34* matrix)
{
}

// 0x00213518 slot 0x10 | virtual slot, introduced by BalloonString
void BalloonString::vf_0x10()
{
}

// 0x002115FC slot 0x14 | virtual slot, introduced by BalloonString
void BalloonString::vf_0x14()
{
}

// 0x00212040 slot 0x18 | virtual slot, introduced by BalloonString
void BalloonString::vf_0x18()
{
}

// 0x00213160 (name is ours)
BalloonString* BalloonString::Create(sead::Heap* heap)
{
}

// 0x0021348C (name after the address)
bool BalloonString::Func21348C()
{
}

// 0x00213790 (name is ours)
void BalloonString::Release(math::Angle16 direction)
{
}
