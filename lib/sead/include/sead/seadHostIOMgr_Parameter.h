#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadHostIOMgr.h"
#include "sead/seadTaskParameter.h"

// RTTI N4sead9HostIOMgr9ParameterE @ 0x008D237C
// vtable 0x00907100 (vptr 0x00907108), offset_to_top 0, 2 entries
class sead::HostIOMgr::Parameter : public ::sead::TaskParameter
{
public:
    Parameter(); // ctor address unknown
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0075099C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return TaskParameter::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x00750950 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835FC (guard 0x009835F8)
        static const RuntimeTypeInfo::Derive<TaskParameter> s_TypeInfo;
        return &s_TypeInfo;
    }
};
