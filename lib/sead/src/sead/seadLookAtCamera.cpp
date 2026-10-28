#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCamera.h"
#include "sead/seadLookAtCamera.h"

namespace sead {
// ctor candidate(s) 0x0054EBB0 (unverified)
sead::LookAtCamera::LookAtCamera()
{
}

// 0x00540F04 slot 0x08 | virtual slot, introduced by sead::Camera
void sead::LookAtCamera::vf_0x08()
{
}

// 0x00540F00 slot 0x0C | virtual slot, introduced by sead::Camera
void sead::LookAtCamera::vf_0x0C()
{
}

// 0x0074A9DC slot 0x10 | mk7dlp:bytes
void sead::LookAtCamera::doUpdateMatrix(sead::Matrix34<float>*) const
{
}

} // namespace sead

// the type information of the classes below Camera
// 0x008296A8 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Camera>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
