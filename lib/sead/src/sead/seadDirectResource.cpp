#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadResource.h"
#include "sead/seadDirectResource.h"

namespace sead {
// ctor candidate(s) 0x005448A8 (unverified)
sead::DirectResource::DirectResource()
{
}

// 0x005448FC slot 0x08 | virtual slot, introduced by sead::DirectResource
void sead::DirectResource::vf_0x08()
{
}

// 0x005448CC slot 0x0C | virtual slot, introduced by sead::DirectResource
void sead::DirectResource::vf_0x0C()
{
}

// 0x0074BE0C slot 0x10 | virtual slot, introduced by sead::DirectResource
void sead::DirectResource::vf_0x10()
{
}

// 0x00544890 slot 0x14 | virtual slot, introduced by sead::DirectResource
void sead::DirectResource::vf_0x14()
{
}

} // namespace sead

// the type information of the classes below Resource
// 0x00829804 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Resource>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
