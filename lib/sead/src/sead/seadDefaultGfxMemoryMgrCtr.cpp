#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadGfxMemoryMgrCtr.h"
#include "sead/seadDefaultGfxMemoryMgrCtr.h"

namespace sead {
// ctor candidate(s) 0x0011EBAC (unverified)
sead::DefaultGfxMemoryMgrCtr::DefaultGfxMemoryMgrCtr()
{
}

// 0x0054BAC4 slot 0x08 | virtual slot, introduced by sead::DefaultGfxMemoryMgrCtr
void sead::DefaultGfxMemoryMgrCtr::vf_0x08()
{
}

// 0x0054BAB0 slot 0x0C | virtual slot, introduced by sead::DefaultGfxMemoryMgrCtr
void sead::DefaultGfxMemoryMgrCtr::vf_0x0C()
{
}

// 0x0054BC5C slot 0x10 | virtual slot, introduced by sead::DefaultGfxMemoryMgrCtr
void sead::DefaultGfxMemoryMgrCtr::vf_0x10()
{
}

// 0x0054BC58 slot 0x14 | virtual slot, introduced by sead::DefaultGfxMemoryMgrCtr
void sead::DefaultGfxMemoryMgrCtr::vf_0x14()
{
}

} // namespace sead

// the type information of the classes below GfxMemoryMgrCtr
// 0x00828FE0 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::GfxMemoryMgrCtr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
