#pragma once

#include "decomp.h"
#include "pead/RuntimeTypeInfo/peadInterface.h"

namespace pead {
namespace RuntimeTypeInfo {
// The type information of a class below BaseType (BaseType::getRuntimeTypeInfoStatic gives the
// one of the base).
//
// Instantiations found in the binary:
//   pead::RuntimeTypeInfo::Derive<nn::pia::transport::PacketHandler>  typeinfo 0x008D11E4  vtable 0x00904B08
//   pead::RuntimeTypeInfo::Derive<pead::Heap>  typeinfo 0x008D11F0  vtable 0x00904B14
template <typename BaseType>
class Derive : public Interface
{
public:
    Derive() {}
    virtual bool isDerived(const Interface* typeInfo) const
    {
        if (this == typeInfo) {
            return true;
        }
        return BaseType::getRuntimeTypeInfoStatic()->isDerived(typeInfo);
    }
};
} // namespace RuntimeTypeInfo
} // namespace pead
