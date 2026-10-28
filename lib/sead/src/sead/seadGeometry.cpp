#include "sead/seadGeometry.h"
#include "nn/math/math_Vector3.h"
#include "sead/seadMathCalcCommon.h"

namespace sead {
namespace {
// sead::Vector3<f32> is laid out like nn::math::VEC3, the type of the CalcCtr functions
inline nn::math::VEC3& toVEC3(Vector3<f32>& v)
{
    return reinterpret_cast<nn::math::VEC3&>(v);
}

inline const nn::math::VEC3& toVEC3(const Vector3<f32>& v)
{
    return reinterpret_cast<const nn::math::VEC3&>(v);
}

inline f32 dot(const Vector3<f32>& a, const Vector3<f32>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
} // namespace

// 0x00560F34 | nintendogs:bytes [tier A]
f32 sead::Geometry::calcSquaredDistancePointToSegment(const Vector3<f32>& point, const Segment<Vector3<f32>>& segment,
                                                      f32* pT)
{
    Vector3<f32> dir;
    Vector3CalcCtr<f32>::sub(toVEC3(dir), toVEC3(segment.mPos1), toVEC3(segment.mPos0));
    Vector3<f32> rel;
    Vector3CalcCtr<f32>::sub(toVEC3(rel), toVEC3(point), toVEC3(segment.mPos0));
    f32 proj = dot(rel, dir);
    Vector3<f32> nearest;
    f32 t;
    if (proj <= 0.0f) {
        nearest = segment.mPos0;
        t = 0.0f;
    } else {
        f32 lengthSq = dot(dir, dir);
        if (proj >= lengthSq) {
            nearest = segment.mPos1;
            t = 1.0f;
        } else {
            t = proj / lengthSq;
            nearest.setLerp(segment.mPos0, segment.mPos1, t);
        }
    }
    if (pT != nullptr) {
        *pT = t;
    }
    f32 dx = nearest.x - point.x;
    f32 dy = nearest.y - point.y;
    f32 dz = nearest.z - point.z;
    return dx * dx + dy * dy + dz * dz;
}

} // namespace sead
