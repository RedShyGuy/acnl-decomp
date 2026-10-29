#include "sead/seadLookAtCamera.h"

namespace sead {
// 0x00540F04 slot 0x08
// 0x00540F00 (deleting dtor)
sead::LookAtCamera::~LookAtCamera()
{
    // empty (in the original too)
}

// 0x0074A9DC slot 0x10 | mk7dlp:bytes
void sead::LookAtCamera::doUpdateMatrix(Matrix34<f32>* dst) const
{
    if (mPos.x == mAt.x && mPos.y == mAt.y && mPos.z == mAt.z) {
        return;
    }
    // the axes of the camera: z away from the point looked at, x to the right, y up
    Vector3<f32> dir = mPos;
    Vector3CalcCtr<f32>::sub(toVEC3(dir), toVEC3(dir), toVEC3(mAt));
    Vector3CalcCtr<f32>::normalize(toVEC3(dir));
    Vector3<f32> right;
    Vector3CalcCtr<f32>::cross(toVEC3(right), toVEC3(mUp), toVEC3(dir));
    Vector3CalcCtr<f32>::normalize(toVEC3(right));
    Vector3<f32> up;
    Vector3CalcCtr<f32>::cross(toVEC3(up), toVEC3(dir), toVEC3(right));

    dst->m[0][0] = right.x;
    dst->m[0][1] = right.y;
    dst->m[0][2] = right.z;
    dst->m[0][3] = -(right.x * mPos.x + right.y * mPos.y + right.z * mPos.z);
    dst->m[1][0] = up.x;
    dst->m[1][1] = up.y;
    dst->m[1][2] = up.z;
    dst->m[1][3] = -(up.x * mPos.x + up.y * mPos.y + up.z * mPos.z);
    dst->m[2][0] = dir.x;
    dst->m[2][1] = dir.y;
    dst->m[2][2] = dir.z;
    dst->m[2][3] = -(dir.x * mPos.x + dir.y * mPos.y + dir.z * mPos.z);
}
} // namespace sead

// the type information of the classes below Camera
// 0x008296A8 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Camera>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
