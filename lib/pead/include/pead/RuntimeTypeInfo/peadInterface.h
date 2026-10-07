#pragma once

#include "decomp.h"

namespace pead {
namespace RuntimeTypeInfo {
// RTTI N4pead15RuntimeTypeInfo9InterfaceE @ 0x008D11FC
//
// The type information of a class with the runtime type check of pead: one static object per
// class (Root for the base of a hierarchy, Derive for the classes below it). The name of the
// slot is ours.
class Interface
{
public:
    Interface() {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isDerived(const Interface* typeInfo) const = 0;
};
} // namespace RuntimeTypeInfo
} // namespace pead
