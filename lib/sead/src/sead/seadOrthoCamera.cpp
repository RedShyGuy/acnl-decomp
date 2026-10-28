#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadLookAtCamera.h"
#include "sead/seadOrthoCamera.h"

namespace sead {
// ctor candidate(s) 0x005404FC, 0x005405D0 (unverified)
sead::OrthoCamera::OrthoCamera()
{
}

// 0x00540678 slot 0x08 | virtual slot, introduced by sead::Camera
void sead::OrthoCamera::vf_0x08()
{
}

// 0x00540674 slot 0x0C | virtual slot, introduced by sead::Camera
void sead::OrthoCamera::vf_0x0C()
{
}

} // namespace sead

// the type information of the classes below LookAtCamera
// 0x00828AE4 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::LookAtCamera>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
