#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadInterface.h"

namespace sead {
namespace RuntimeTypeInfo {
// RTTI N4sead15RuntimeTypeInfo4RootE @ 0x008D18D0
// vtable 0x00905B58 (vptr 0x00905B60), offset_to_top 0, 1 entries
//
// The type information of the base of a hierarchy.
class Root : public ::sead::RuntimeTypeInfo::Interface
{
public:
    Root() {}
    virtual bool isDerived(const Interface* typeInfo) const; // 0x0074C770 slot 0x00
};
} // namespace RuntimeTypeInfo
} // namespace sead
