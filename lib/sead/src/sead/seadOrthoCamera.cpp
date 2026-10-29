#include "sead/seadOrthoCamera.h"
#include "sead/seadOrthoProjection.h"

namespace sead {
// 0x0054046C (name is ours)
void sead::OrthoCamera::setByOrthoProjection(const OrthoProjection& projection)
{
    mPos.x = (projection.mLeft + projection.mRight) * 0.5f;
    mPos.y = (projection.mTop + projection.mBottom) * 0.5f;
    mPos.z = projection.mNear;
    mAt.x = (projection.mLeft + projection.mRight) * 0.5f;
    mAt.y = (projection.mTop + projection.mBottom) * 0.5f;
    mAt.z = projection.mNear - 1.0f;
    mUp.x = 0.0f;
    mUp.y = 1.0f;
    mUp.z = 0.0f;
    Vector3CalcCtr<f32>::normalize(toVEC3(mUp));
}

// 0x005404FC (name is ours)
sead::OrthoCamera::OrthoCamera(const OrthoProjection& projection)
    : LookAtCamera(Vector3<f32>((projection.mLeft + projection.mRight) * 0.5f,
                                (projection.mTop + projection.mBottom) * 0.5f, projection.mNear),
                   Vector3<f32>((projection.mLeft + projection.mRight) * 0.5f,
                                (projection.mTop + projection.mBottom) * 0.5f, projection.mNear - 1.0f),
                   Vector3<f32>(0.0f, 1.0f, 0.0f))
{
}

// 0x005405D0 (name is ours)
sead::OrthoCamera::OrthoCamera(const Vector2<f32>& pos, f32 distance)
    : LookAtCamera(Vector3<f32>(pos.x, pos.y, distance), Vector3<f32>(pos.x, pos.y, distance - 1.0f),
                   Vector3<f32>(0.0f, 1.0f, 0.0f))
{
}

// 0x00540678 slot 0x08
// 0x00540674 (deleting dtor)
sead::OrthoCamera::~OrthoCamera()
{
    // empty (in the original too)
}
} // namespace sead

// the type information of the classes below LookAtCamera
// 0x00828AE4 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::LookAtCamera>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
