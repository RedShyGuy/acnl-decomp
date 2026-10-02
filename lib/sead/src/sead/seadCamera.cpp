#include "sead/seadCamera.h"

namespace sead {
// ctor address unknown
sead::Camera::Camera()
{
}

// 0x0074EDBC slot 0x00 | virtual slot, introduced by sead::Camera
void sead::Camera::vf_0x00()
{
}

// 0x0074ED70 slot 0x04 | virtual slot, introduced by sead::Camera
void sead::Camera::vf_0x04()
{
}

// 0x0055D5A0 slot 0x08 | virtual slot, introduced by sead::Camera
void sead::Camera::vf_0x08()
{
}

// 0x0055D59C slot 0x0C | virtual slot, introduced by sead::Camera
void sead::Camera::vf_0x0C()
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void sead::Camera::doUpdateMatrix(sead::Matrix34<float>*) const
{
}

} // namespace sead
