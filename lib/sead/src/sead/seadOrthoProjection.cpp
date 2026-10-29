#include "sead/seadOrthoProjection.h"
#include "sead/seadViewport.h"

namespace sead {
// 0x00546188 (name is ours)
void sead::OrthoProjection::setByViewport(const Viewport& viewport)
{
    f32 halfHeight = (viewport.mMax.y - viewport.mMin.y) * 0.5f;
    f32 halfWidth = (viewport.mMax.x - viewport.mMin.x) * 0.5f;
    mTop = halfHeight;
    mBottom = -halfHeight;
    mLeft = -halfWidth;
    mRight = halfWidth;
    mDirty = true;
}

// 0x005461D0 (name is ours)
sead::OrthoProjection::OrthoProjection(f32 near, f32 far, const Viewport& viewport)
    : Projection(), mNear(near), mFar(far)
{
    mTop = (viewport.mMax.y - viewport.mMin.y) * 0.5f;
    mBottom = -((viewport.mMax.y - viewport.mMin.y) * 0.5f);
    mLeft = -((viewport.mMax.x - viewport.mMin.x) * 0.5f);
    mRight = (viewport.mMax.x - viewport.mMin.x) * 0.5f;
    mDevicePosture = viewport.mDevicePosture;
}

// 0x00546290 (name is ours)
sead::OrthoProjection::OrthoProjection()
    : Projection(), mNear(0.0f), mFar(1.0f), mTop(0.5f), mBottom(-0.5f), mLeft(-0.5f), mRight(0.5f)
{
}

// 0x00546324 slot 0x08
// 0x00546320 (deleting dtor)
sead::OrthoProjection::~OrthoProjection()
{
    // empty (in the original too)
}

// 0x0074C560 slot 0x10 (name is ours)
s32 sead::OrthoProjection::getProjectionType() const
{
    return 1;
}

// 0x0074C494 slot 0x14 (name is ours)
void sead::OrthoProjection::doUpdateMatrix(Matrix44<f32>* dst) const
{
    f32 halfWidth = (mRight - mLeft) * 0.5f;
    f32 centerX = -((mLeft + mRight) * 0.5f);
    dst->m[0][0] = 1.0f / halfWidth;
    dst->m[0][1] = 0.0f;
    dst->m[0][2] = 0.0f;
    dst->m[0][3] = centerX / halfWidth;
    f32 halfHeight = (mTop - mBottom) * 0.5f;
    f32 centerY = -((mTop + mBottom) * 0.5f);
    dst->m[1][0] = 0.0f;
    dst->m[1][1] = 1.0f / halfHeight;
    dst->m[1][2] = 0.0f;
    dst->m[1][3] = centerY / halfHeight;
    f32 invDepth = 1.0f / (mFar - mNear);
    dst->m[2][0] = 0.0f;
    dst->m[2][1] = 0.0f;
    dst->m[2][2] = -invDepth;
    dst->m[2][3] = -(mNear * invDepth);
    dst->m[3][0] = 0.0f;
    dst->m[3][1] = 0.0f;
    dst->m[3][2] = 0.0f;
    dst->m[3][3] = 1.0f;
}

// 0x0074C5B4 slot 0x1C (name is ours)
void sead::OrthoProjection::doScreenPosToCameraPosTo(Vector3<f32>* dst, const Vector2<f32>& screenPos) const
{
    dst->x = (mRight - mLeft) * 0.5f * screenPos.x + (mLeft + mRight) * 0.5f;
    dst->y = (mTop - mBottom) * 0.5f * screenPos.y + (mTop + mBottom) * 0.5f;
    dst->z = -mNear;
}
} // namespace sead

// the type information of the classes below Projection
// 0x00828914 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Projection>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
