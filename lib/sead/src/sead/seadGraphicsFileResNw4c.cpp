#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDirectResource.h"
#include "sead/seadGraphicsFileResNw4c.h"

namespace sead {
// ctor candidate(s) 0x0054A180 (unverified)
sead::GraphicsFileResNw4c::GraphicsFileResNw4c()
{
}

// 0x0054A1DC slot 0x08 | virtual slot, introduced by sead::DirectResource
void sead::GraphicsFileResNw4c::vf_0x08()
{
}

// 0x0054A1A8 slot 0x0C | virtual slot, introduced by sead::DirectResource
void sead::GraphicsFileResNw4c::vf_0x0C()
{
}

// 0x0074CE80 slot 0x10 | virtual slot, introduced by sead::DirectResource
void sead::GraphicsFileResNw4c::vf_0x10()
{
}

// 0x0054A164 slot 0x14 | virtual slot, introduced by sead::DirectResource
void sead::GraphicsFileResNw4c::vf_0x14()
{
}

} // namespace sead

// the type information of the classes below DirectResource
// 0x00828E84 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::DirectResource>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
