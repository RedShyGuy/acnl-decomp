#pragma once

#include "decomp.h"

namespace sead {
namespace RuntimeTypeInfo {
// RTTI N4sead15RuntimeTypeInfo9InterfaceE @ 0x008D1AA4
//
// The type information of a class with the runtime type check of sead: one function-local static
// object per class (Root for the base of a hierarchy, Derive for the classes below it), returned by
// the inline getTypeInfoStatic of the class; slot 0 of the class (isKindOf) checks a type against
// the chain, slot 1 (getTypeInfo) returns the object. The names of the slot and of the functions of
// the classes are ours (the same mechanism as pead's copy in pia).
class Interface
{
public:
    Interface() {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isDerived(const Interface* typeInfo) const = 0;
};
} // namespace RuntimeTypeInfo
} // namespace sead
