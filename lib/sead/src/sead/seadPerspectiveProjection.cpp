#include "sead/seadPerspectiveProjection.h"

#include <cmath>

namespace sead {
// 0x0054B4D8 (name is ours)
sead::PerspectiveProjection::PerspectiveProjection()
    : Projection(), mNear(1.0f), mFar(10000.0f), mAspect(4.0f / 3.0f), mOffset(Vector2<f32>::zero)
{
    setFovy(3.14159265f / 4.0f);
}

// 0x0054B5B8 slot 0x08
// 0x0054B5B4 (deleting dtor)
sead::PerspectiveProjection::~PerspectiveProjection()
{
    // empty (in the original too)
}

// 0x0074D8E8 slot 0x10 (name is ours)
s32 sead::PerspectiveProjection::getProjectionType() const
{
    return 0;
}

// 0x0074D7DC slot 0x14 (name is ours)
void sead::PerspectiveProjection::doUpdateMatrix(Matrix44<f32>* dst) const
{
    f32 height = mNear * mTanFovyHalf * 2.0f;
    f32 width = height * mAspect;
    f32 offsetX = mOffset.x * width;
    f32 halfWidth = width * 0.5f;
    f32 left = offsetX - halfWidth;
    f32 right = halfWidth + offsetX;
    f32 offsetY = mOffset.y * height;
    f32 halfHeight = height * 0.5f;
    f32 top = halfHeight + offsetY;
    f32 bottom = offsetY - halfHeight;

    f32 invWidth = 1.0f / (right - left);
    dst->m[0][0] = mNear * (invWidth * 2.0f);
    dst->m[0][1] = 0.0f;
    dst->m[0][2] = (right + left) * invWidth;
    f32 invHeight = 1.0f / (top - bottom);
    dst->m[0][3] = 0.0f;
    dst->m[1][0] = 0.0f;
    dst->m[1][1] = mNear * (invHeight * 2.0f);
    dst->m[1][2] = (top + bottom) * invHeight;
    dst->m[1][3] = 0.0f;
    f32 invDepth = 1.0f / (mFar - mNear);
    dst->m[2][0] = 0.0f;
    dst->m[2][1] = 0.0f;
    dst->m[2][2] = -(mFar * invDepth);
    dst->m[2][3] = -(mFar * mNear * invDepth);
    dst->m[3][0] = 0.0f;
    dst->m[3][1] = 0.0f;
    dst->m[3][2] = -1.0f;
    dst->m[3][3] = 0.0f;
}

// 0x0074D93C slot 0x1C (name is ours)
void sead::PerspectiveProjection::doScreenPosToCameraPosTo(Vector3<f32>* dst, const Vector2<f32>& screenPos) const
{
    dst->x = 0.0f;
    dst->y = 0.0f;
    dst->z = -mNear;
    dst->y = mNear * mTanFovyHalf * 2.0f * 0.5f * (screenPos.y + mOffset.y * 2.0f);
    dst->x = mNear * mTanFovyHalf * 2.0f * mAspect * ((screenPos.x + mOffset.x * 2.0f) * 0.5f);
}
} // namespace sead
