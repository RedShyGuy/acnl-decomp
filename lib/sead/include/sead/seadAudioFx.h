#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead7AudioFxE @ 0x008D2130
// vtable 0x00906BE8 (vptr 0x00906BF0), offset_to_top 0, 4 entries
class AudioFx
{
public:
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074EECC (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074EE80 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835B4 (guard 0x009835B0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0014397C slot 0x08 | virtual slot, introduced by sead::AudioFx
    virtual void vf_0x0C(); // 0x0055DF98 slot 0x0C | virtual slot, introduced by sead::AudioFx
    AudioFx(); // 0x001391DC | nintendogs:bytes [tier B]
};
} // namespace sead
