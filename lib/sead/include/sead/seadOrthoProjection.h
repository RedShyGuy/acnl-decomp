#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadProjection.h"

namespace sead {
class Viewport;

// RTTI N4sead15OrthoProjectionE @ 0x008D189C
// vtable 0x00905AFC (vptr 0x00905B04), offset_to_top 0, 8 entries
//
// An orthographic projection of a box (names are ours).
class OrthoProjection : public ::sead::Projection
{
public:
    // the box from -0.5 to 0.5 and from 0 to 1 in depth
    OrthoProjection(); // 0x00546290
    // the box of the viewport around the origin
    OrthoProjection(f32 near, f32 far, const Viewport& viewport); // 0x005461D0
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074C614 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Projection::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074C568 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834A4 (guard 0x009834A0)
        static const RuntimeTypeInfo::Derive<Projection> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~OrthoProjection(); // 0x00546324 slot 0x08
    // 0x00546320 slot 0x0C (deleting dtor)
    virtual s32 getProjectionType() const; // 0x0074C560 slot 0x10
    virtual void doUpdateMatrix(Matrix44<f32>* dst) const; // 0x0074C494 slot 0x14
    virtual void doScreenPosToCameraPosTo(Vector3<f32>* dst, const Vector2<f32>& screenPos) const; // 0x0074C5B4 slot 0x1C

    // the box of the viewport around the origin
    void setByViewport(const Viewport& viewport); // 0x00546188

    f32 mNear;   // 0x94
    f32 mFar;    // 0x98
    f32 mTop;    // 0x9C
    f32 mBottom; // 0xA0
    f32 mLeft;   // 0xA4
    f32 mRight;  // 0xA8
};
ASSERT_SIZE(OrthoProjection, 0xAC);
} // namespace sead
