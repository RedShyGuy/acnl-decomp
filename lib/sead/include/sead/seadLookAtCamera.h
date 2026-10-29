#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCamera.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadVector3.h"

namespace sead {
// RTTI N4sead12LookAtCameraE @ 0x008D15A0
// vtable 0x0090522C (vptr 0x00905234), offset_to_top 0, 5 entries
//
// A camera at a position that looks at a point (the up vector is kept normalized; member names are
// ours).
class LookAtCamera : public ::sead::Camera
{
public:
    // in front of the origin, looking at it (inline in ptcl::PtclEditor)
    LookAtCamera() : Camera(), mPos(0.0f, 0.0f, 10.0f), mAt(0.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f) {}
    LookAtCamera(const Vector3<f32>& pos, const Vector3<f32>& at, const Vector3<f32>& up)
        : Camera(), mPos(pos), mAt(at), mUp(up)
    {
        Vector3CalcCtr<f32>::normalize(toVEC3(mUp));
    }
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
    virtual ~LookAtCamera(); // 0x00540F04 slot 0x08
    // 0x00540F00 slot 0x0C (deleting dtor)
    // the view matrix (unchanged if the position is the point looked at)
    virtual void doUpdateMatrix(Matrix34<f32>* dst) const; // 0x0074A9DC slot 0x10 | mk7dlp:bytes

    Vector3<f32> mPos; // 0x34
    Vector3<f32> mAt;  // 0x40
    Vector3<f32> mUp;  // 0x4C
};
ASSERT_SIZE(LookAtCamera, 0x58);
} // namespace sead
