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
    // the default values (inline, e.g. in the constructor of the game's RootTask 0x0011F16C; names are ours)
    Parameter()
        : TaskParameter(), field_0x04(0x10000), field_0x08(0x2000), field_0x0C(0x2000), field_0x10(0x2332),
          field_0x14(1), field_0x18(0x140D), field_0x1C(true)
    {
    }
    // whether the class is the one of typeInfo or derives from it
    DECOMP_EMIT_COPY virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0075099C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return TaskParameter::isKindOf(typeInfo);
    }
    DECOMP_EMIT_COPY virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x00750950 (name is ours)
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

    u32 field_0x04;  // 0x04
    u32 field_0x08;  // 0x08
    u32 field_0x0C;  // 0x0C
    u32 field_0x10;  // 0x10 (9010)
    u32 field_0x14;  // 0x14
    u32 field_0x18;  // 0x18 (5133)
    bool field_0x1C; // 0x1C
};
ASSERT_SIZE(sead::HostIOMgr::Parameter, 0x20);
