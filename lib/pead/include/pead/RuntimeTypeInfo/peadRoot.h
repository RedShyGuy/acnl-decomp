#pragma once

#include "decomp.h"
#include "pead/RuntimeTypeInfo/peadInterface.h"

namespace pead {
namespace RuntimeTypeInfo {
// RTTI N4pead15RuntimeTypeInfo4RootE @ 0x008D11D8
// vtable 0x00904AFC (vptr 0x00904B04), offset_to_top 0, 1 entries
class Root : public ::pead::RuntimeTypeInfo::Interface
{
public:
    Root() {}
    virtual bool isDerived(const Interface* typeInfo) const; // 0x00749478 slot 0x00
};
} // namespace RuntimeTypeInfo
} // namespace pead
