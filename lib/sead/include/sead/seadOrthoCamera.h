#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadLookAtCamera.h"

namespace sead {
// RTTI N4sead11OrthoCameraE @ 0x008D1410
// vtable 0x00905024 (vptr 0x0090502C), offset_to_top 0, 5 entries
class OrthoCamera : public ::sead::LookAtCamera
{
public:
    OrthoCamera(); // ctor candidate(s) 0x005404FC, 0x005405D0 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074A744 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return LookAtCamera::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074A6F8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833DC (guard 0x009833D8)
        static const RuntimeTypeInfo::Derive<LookAtCamera> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x00540678 slot 0x08 | virtual slot, introduced by sead::Camera
    virtual void vf_0x0C(); // 0x00540674 slot 0x0C | virtual slot, introduced by sead::Camera
};
} // namespace sead
