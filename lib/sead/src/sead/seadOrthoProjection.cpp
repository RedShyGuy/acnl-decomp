#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadProjection.h"
#include "sead/seadOrthoProjection.h"

namespace sead {
// ctor candidate(s) 0x005461D0, 0x00546290 (unverified)
sead::OrthoProjection::OrthoProjection()
{
}

// 0x00546324 slot 0x08 | virtual slot, introduced by sead::Projection
void sead::OrthoProjection::vf_0x08()
{
}

// 0x00546320 slot 0x0C | virtual slot, introduced by sead::Projection
void sead::OrthoProjection::vf_0x0C()
{
}

// 0x0074C560 slot 0x10 | virtual slot, introduced by sead::Projection
void sead::OrthoProjection::vf_0x10()
{
}

// 0x0074C494 slot 0x14 | virtual slot, introduced by sead::Projection
void sead::OrthoProjection::vf_0x14()
{
}

// 0x0074C5B4 slot 0x1C | virtual slot, introduced by sead::Projection
void sead::OrthoProjection::vf_0x1C()
{
}

} // namespace sead

// the type information of the classes below Projection
// 0x00828914 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Projection>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
