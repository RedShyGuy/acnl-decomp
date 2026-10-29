#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadProjection.h"

#include <cmath>

namespace sead {
// RTTI N4sead21PerspectiveProjectionE @ 0x008D1ED8
// vtable 0x00906624 (vptr 0x0090662C), offset_to_top 0, 8 entries
//
// A perspective projection with a vertical field of view, an aspect ratio and an offset of the
// center (in halves of the screen; names are ours).
class PerspectiveProjection : public ::sead::Projection
{
public:
    // near 1, far 10000, 45 degrees, 4:3
    PerspectiveProjection(); // 0x0054B4D8
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074D9C4 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Projection::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074D8F0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983534 (guard 0x00983530)
        static const RuntimeTypeInfo::Derive<Projection> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~PerspectiveProjection(); // 0x0054B5B8 slot 0x08
    // 0x0054B5B4 slot 0x0C (deleting dtor)
    virtual s32 getProjectionType() const; // 0x0074D8E8 slot 0x10
    virtual void doUpdateMatrix(Matrix44<f32>* dst) const; // 0x0074D7DC slot 0x14
    virtual void doScreenPosToCameraPosTo(Vector3<f32>* dst, const Vector2<f32>& screenPos) const; // 0x0074D93C slot 0x1C

    // the clip planes, the field of view and the aspect ratio (name is ours)
    void set(f32 near, f32 far, f32 fovy, f32 aspect); // 0x0054B474
    // the edges of the near plane with the offset (inline; the copies are called; names are ours)
    DECOMP_EMIT_COPY f32 getTop() const // 0x0074DA78
    {
        f32 height = mNear * mTanFovyHalf * 2.0f;
        return mOffset.y * height + height * 0.5f;
    }
    DECOMP_EMIT_COPY f32 getBottom() const // 0x0074DB18
    {
        f32 height = mNear * mTanFovyHalf * 2.0f;
        return mOffset.y * height - height * 0.5f;
    }
    DECOMP_EMIT_COPY f32 getLeft() const // 0x0074DAA8
    {
        f32 width = mNear * mTanFovyHalf * 2.0f * mAspect;
        return mOffset.x * width - width * 0.5f;
    }
    DECOMP_EMIT_COPY f32 getRight() const // 0x0074DAE0
    {
        f32 width = mNear * mTanFovyHalf * 2.0f * mAspect;
        return mOffset.x * width + width * 0.5f;
    }
    // the vertical field of view (radians)
    void setFovy(f32 fovy)
    {
        mFovy = fovy;
        mSinFovyHalf = sinf(fovy * 0.5f);
        mCosFovyHalf = cosf(fovy * 0.5f);
        mTanFovyHalf = tanf(fovy * 0.5f);
        mDirty = true;
    }

    f32 mNear;             // 0x94
    f32 mFar;              // 0x98
    f32 mFovy;             // 0x9C
    f32 mSinFovyHalf;      // 0xA0
    f32 mCosFovyHalf;      // 0xA4
    f32 mTanFovyHalf;      // 0xA8
    f32 mAspect;           // 0xAC
    Vector2<f32> mOffset;  // 0xB0
};
ASSERT_SIZE(PerspectiveProjection, 0xB8);
} // namespace sead
