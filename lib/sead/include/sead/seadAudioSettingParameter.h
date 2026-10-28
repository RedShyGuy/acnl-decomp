#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadTaskParameter.h"

namespace sead {
// RTTI N4sead21AudioSettingParameterE @ 0x008D1E58
// vtable 0x0090651C (vptr 0x00906524), offset_to_top 0, 4 entries
class AudioSettingParameter : public ::sead::TaskParameter
{
public:
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074D4D8 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return TaskParameter::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074D48C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098350C (guard 0x00983508)
        static const RuntimeTypeInfo::Derive<TaskParameter> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0054B178 slot 0x08 | virtual slot, introduced by sead::AudioSettingParameter
    virtual void vf_0x0C(); // 0x0054B174 slot 0x0C | virtual slot, introduced by sead::AudioSettingParameter
    void appendSubset(sead::AudioSubsetBase*); // 0x00125C20 | nintendogs:bytes [tier A]
    AudioSettingParameter(); // 0x0012CBD0 | nintendogs:bytes [tier A]
};
} // namespace sead
