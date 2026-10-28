// The math of sead in C++ (the angle index functions, quaternions and the Matrix34 constructions);
// the hand-written assembly is in seadMathCtrAsm.cpp.

#include "sead/seadMathCalcCommon.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Quaternion.h"
#include "nn/math/math_Vector3.h"
#include "sead/seadVector3.h"

namespace sead {
// 0x0054510C | nintendogs:bytes [tier A]
template <>
DECOMP_NOINLINE u32 sead::MathCommon<float>::atanIdx_(float x)
{
    f32 v = x * 128.0f;
    s32 i = static_cast<s32>(v);
    const MathAtanEntry& entry = s_MathAtanTable[i];
    return entry.idx + static_cast<u32>(entry.delta * (v - static_cast<f32>(i)));
}

// 0x00540680 | mk7dlp:bytes [tier B]
template <>
void sead::QuatCalcCtr<float>::slerpToIdx(nn::math::QUAT& o, const nn::math::QUAT& a, const nn::math::QUAT& b, float t)
{
    f32 dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    if (dot > 1.0f) {
        dot = 1.0f;
    } else if (dot < -1.0f) {
        dot = -1.0f;
    }
    bool isNegative = false;
    if (dot < 0.0f) {
        dot = -dot;
        isNegative = true;
    }
    u32 theta = MathCommon<f32>::acosIdx(dot);
    f32 sinTheta = MathCommon<f32>::sinIdx(theta);
    f32 scaleA;
    f32 scaleB;
    if (fabsf(sinTheta) < 1.1920929e-07f) {
        scaleA = 1.0f - t;
        scaleB = t;
    } else {
        f32 inv = 1.0f / sinTheta;
        s32 tTheta = static_cast<s32>(t * static_cast<f32>(theta));
        scaleA = MathCommon<f32>::sinIdx(theta - tTheta) * inv;
        scaleB = MathCommon<f32>::sinIdx(tTheta) * inv;
    }
    if (isNegative) {
        scaleB = -scaleB;
    }
    o.x = a.x * scaleA + b.x * scaleB;
    o.y = a.y * scaleA + b.y * scaleB;
    o.z = a.z * scaleA + b.z * scaleB;
    o.w = a.w * scaleA + b.w * scaleB;
}

// 0x005408CC | nintendogs:bytes [tier A]
template <>
void sead::QuatCalcCtr<float>::setAxisRotation(nn::math::QUAT& q, const nn::math::VEC3& axis, float angle)
{
    const f32 half = angle * 0.5f;
    const f32 c = cosf(half);
    const f32 s = sinf(half);
    q.w = c;
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
}

// 0x00540924 | nintendogs:bytes [tier A]
template <>
void sead::QuatCalcCtr<float>::mul(nn::math::QUAT& o, const nn::math::QUAT& a, const nn::math::QUAT& b)
{
    const f32 x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    const f32 y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    const f32 z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    const f32 w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    o.x = x;
    o.y = y;
    o.z = z;
    o.w = w;
}

// 0x00540984 | mk7dlp:bytes [tier B]
template <>
void sead::QuatCalcCtr<float>::setRPY(nn::math::QUAT& q, float roll, float pitch, float yaw)
{
    const f32 cy = cosf(yaw * 0.5f);
    const f32 cp = cosf(pitch * 0.5f);
    const f32 cr = cosf(roll * 0.5f);
    const f32 sy = sinf(yaw * 0.5f);
    const f32 sp = sinf(pitch * 0.5f);
    const f32 sr = sinf(roll * 0.5f);
    const f32 cycp = cy * cp;
    const f32 sycp = sy * cp;
    const f32 cysp = cy * sp;
    const f32 sysp = sy * sp;
    q.x = cycp * sr - sysp * cr;
    q.y = cysp * cr + sycp * sr;
    q.z = sycp * cr - cysp * sr;
    q.w = cycp * cr + sysp * sr;
}

// 0x00545D48 | mk7dlp:bytes [tier B]
template <>
void sead::Matrix34CalcCtr<float>::makeSRTIdx(nn::math::MTX34& o, const nn::math::VEC3& s, const Vector3<u32>& r,
                                              const nn::math::VEC3& t)
{
    f32 sinV[3];
    f32 cosV[3];
    MathCtr<f32>::sinCosIdx(&sinV[0], &cosV[0], r.x);
    MathCtr<f32>::sinCosIdx(&sinV[1], &cosV[1], r.y);
    MathCtr<f32>::sinCosIdx(&sinV[2], &cosV[2], r.z);
    o.m[0][0] = s.x * (cosV[1] * cosV[2]);
    o.m[1][0] = s.x * (cosV[1] * sinV[2]);
    o.m[2][0] = -s.x * sinV[1];
    o.m[0][1] = s.y * (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    o.m[1][1] = s.y * (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    o.m[2][1] = s.y * (sinV[0] * cosV[1]);
    o.m[0][2] = s.z * (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    o.m[1][2] = s.z * (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    o.m[2][2] = s.z * (cosV[0] * cosV[1]);
    o.m[0][3] = t.x;
    o.m[1][3] = t.y;
    o.m[2][3] = t.z;
}

// 0x00545E78 | nintendogs:bytes [tier A]
template <>
void sead::Matrix34CalcCtr<float>::scaleBases(nn::math::MTX34& o, float x, float y, float z)
{
    o.m[0][0] *= x;
    o.m[1][0] *= x;
    o.m[2][0] *= x;
    o.m[0][1] *= y;
    o.m[1][1] *= y;
    o.m[2][1] *= y;
    o.m[0][2] *= z;
    o.m[1][2] *= z;
    o.m[2][2] *= z;
}

// 0x00545EE4 | nintendogs:bytes [tier A]
template <>
void sead::Matrix34CalcCtr<float>::makeQ(nn::math::MTX34& o, const nn::math::QUAT& q)
{
    const f32 xx = q.x * q.x;
    const f32 yy = q.y * q.y;
    const f32 zz = q.z * q.z;
    const f32 xy = q.x * q.y;
    const f32 xz = q.x * q.z;
    const f32 yz = q.y * q.z;
    const f32 wx = q.w * q.x;
    const f32 wy = q.w * q.y;
    const f32 wz = q.w * q.z;
    o.m[0][0] = 1.0f - 2.0f * yy - 2.0f * zz;
    o.m[0][1] = 2.0f * xy - 2.0f * wz;
    o.m[0][2] = 2.0f * xz + 2.0f * wy;
    o.m[0][3] = 0.0f;
    o.m[1][0] = 2.0f * xy + 2.0f * wz;
    o.m[1][1] = 1.0f - 2.0f * xx - 2.0f * zz;
    o.m[1][2] = 2.0f * yz - 2.0f * wx;
    o.m[1][3] = 0.0f;
    o.m[2][0] = 2.0f * xz - 2.0f * wy;
    o.m[2][1] = 2.0f * yz + 2.0f * wx;
    o.m[2][2] = 1.0f - 2.0f * xx - 2.0f * yy;
    o.m[2][3] = 0.0f;
}

// 0x00545FB0 | nintendogs:bytes [tier A]
template <>
void sead::Matrix34CalcCtr<float>::makeR(nn::math::MTX34& o, const nn::math::VEC3& r)
{
    const f32 sx = sinf(r.x);
    const f32 sy = sinf(r.y);
    const f32 sz = sinf(r.z);
    const f32 cx = cosf(r.x);
    const f32 cy = cosf(r.y);
    const f32 cz = cosf(r.z);
    o.m[0][0] = cy * cz;
    o.m[1][0] = cy * sz;
    o.m[2][0] = -sy;
    o.m[0][1] = sx * sy * cz - cx * sz;
    o.m[1][1] = sx * sy * sz + cx * cz;
    o.m[2][1] = sx * cy;
    o.m[0][2] = cx * cz * sy + sx * sz;
    o.m[1][2] = cx * sz * sy - sx * cz;
    o.m[2][2] = cx * cy;
    o.m[0][3] = 0.0f;
    o.m[1][3] = 0.0f;
    o.m[2][3] = 0.0f;
}

// 0x00546080 | nintendogs:bytes [tier A]
template <>
void sead::Matrix34CalcCtr<float>::makeSRT(nn::math::MTX34& o, const nn::math::VEC3& s, const nn::math::VEC3& r,
                                           const nn::math::VEC3& t)
{
    const f32 sx = sinf(r.x);
    const f32 sy = sinf(r.y);
    const f32 sz = sinf(r.z);
    const f32 cx = cosf(r.x);
    const f32 cy = cosf(r.y);
    const f32 cz = cosf(r.z);
    o.m[0][0] = s.x * (cy * cz);
    o.m[1][0] = s.x * (cy * sz);
    o.m[2][0] = -s.x * sy;
    o.m[0][1] = s.y * (sx * sy * cz - cx * sz);
    o.m[1][1] = s.y * (sx * sy * sz + cx * cz);
    o.m[2][1] = s.y * (sx * cy);
    o.m[0][2] = s.z * (cx * cz * sy + sx * sz);
    o.m[1][2] = s.z * (cx * sz * sy - sx * cz);
    o.m[2][2] = s.z * (cx * cy);
    o.m[0][3] = t.x;
    o.m[1][3] = t.y;
    o.m[2][3] = t.z;
}

} // namespace sead
