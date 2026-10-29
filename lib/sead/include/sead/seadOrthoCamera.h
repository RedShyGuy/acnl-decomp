#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadLookAtCamera.h"
#include "sead/seadVector2.h"

namespace sead {
class OrthoProjection;

// RTTI N4sead11OrthoCameraE @ 0x008D1410
// vtable 0x00905024 (vptr 0x0090502C), offset_to_top 0, 5 entries
//
// A camera that looks along -z at the middle of an orthographic projection (names are ours).
class OrthoCamera : public ::sead::LookAtCamera
{
public:
    // looks at pos from the distance
    OrthoCamera(const Vector2<f32>& pos, f32 distance); // 0x005405D0
    // looks at the middle of the projection from its near plane
    explicit OrthoCamera(const OrthoProjection& projection); // 0x005404FC
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
    virtual ~OrthoCamera(); // 0x00540678 slot 0x08
    // 0x00540674 slot 0x0C (deleting dtor)

    // the same as the constructor for an existing camera
    void setByOrthoProjection(const OrthoProjection& projection); // 0x0054046C
};
ASSERT_SIZE(OrthoCamera, 0x58);
} // namespace sead
