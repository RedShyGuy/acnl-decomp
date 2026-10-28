#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCamera.h"

namespace sead {
// RTTI N4sead12LookAtCameraE @ 0x008D15A0
// vtable 0x0090522C (vptr 0x00905234), offset_to_top 0, 5 entries
class LookAtCamera : public ::sead::Camera
{
public:
    LookAtCamera(); // ctor candidate(s) 0x0054EBB0 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074AB68 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Camera::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074AB1C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833EC (guard 0x009833E8)
        static const RuntimeTypeInfo::Derive<Camera> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x00540F04 slot 0x08 | virtual slot, introduced by sead::Camera
    virtual void vf_0x0C(); // 0x00540F00 slot 0x0C | virtual slot, introduced by sead::Camera
    virtual void doUpdateMatrix(sead::Matrix34<float>*) const; // 0x0074A9DC slot 0x10 | mk7dlp:bytes
};
} // namespace sead
