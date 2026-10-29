#include "sead/hostio/seadCurve.h"
#include "sead/seadMathCalcCommon.h"

#include <math.h>

namespace sead {
namespace hostio {
namespace {
// 2 pi
const f32 PI2 = 6.2831855f;

// the cubic Hermite curve from p0 (slope m0) to p1 (slope m1) at s = 0..1
inline f32 hermite_(f32 p0, f32 m0, f32 p1, f32 m1, f32 s)
{
    return p0 * (2.0f * s * s * s - 3.0f * s * s + 1.0f) + p1 * (-2.0f * s * s * s + 3.0f * s * s) +
           m0 * (s * s * s - 2.0f * s * s + s) + m1 * (s * s * s - s * s);
}

// the vector a - b, scaled to length 1 (unless it is 0)
inline NonUniformSpline::Vec4 normalizedDiff_(const NonUniformSpline::Vec4& a, const NonUniformSpline::Vec4& b)
{
    NonUniformSpline::Vec4 v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    v.w = a.w - b.w;
    f32 length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    if (length > 0.0f) {
        f32 inv = 1.0f / length;
        v.x *= inv;
        v.y *= inv;
        v.z *= inv;
        v.w *= inv;
    }
    return v;
}

// the tangent at an end of the spline: from the chord (to - from over length) and the tangent of the
// neighbour
inline NonUniformSpline::Vec4 endTangent_(const NonUniformSpline::Vec4& from, const NonUniformSpline::Vec4& to,
                                          f32 length, const NonUniformSpline::Vec4& neighbour)
{
    f32 inv = 1.0f / length;
    NonUniformSpline::Vec4 v;
    v.x = ((to.x - from.x) * 3.0f * inv - neighbour.x) * 0.5f;
    v.y = ((to.y - from.y) * 3.0f * inv - neighbour.y) * 0.5f;
    v.z = ((to.z - from.z) * 3.0f * inv - neighbour.z) * 0.5f;
    v.w = ((to.w - from.w) * 3.0f * inv - neighbour.w) * 0.5f;
    return v;
}

// the Hermite basis of NonUniformSpline::calc (rows: the factors of t^3, t^2, t, 1)
// 0x00AE6618 (name is ours)
const Matrix44<f32> s_HermiteBasis(2.0f, -2.0f, 1.0f, 1.0f, -3.0f, 3.0f, -2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                                   0.0f, 0.0f, 0.0f);
} // namespace

// (the static initializer builds s_HermiteBasis)
// 0x0079A324 (static initializer)

// 0x0055DB00 (name is ours)
void sead::hostio::NonUniformSpline::prepare()
{
    for (s32 i = 1; i < mNumPoints - 1; i++) {
        Vec4 next = normalizedDiff_(mPoints[i + 1].pos, mPoints[i].pos);
        Vec4 prev = normalizedDiff_(mPoints[i - 1].pos, mPoints[i].pos);
        Vec4& tangent = mPoints[i].tangent;
        tangent.x = next.x - prev.x;
        tangent.y = next.y - prev.y;
        tangent.z = next.z - prev.z;
        tangent.w = next.w - prev.w;
        f32 length = sqrtf(tangent.x * tangent.x + tangent.y * tangent.y + tangent.z * tangent.z +
                           tangent.w * tangent.w);
        if (length > 0.0f) {
            f32 inv = 1.0f / length;
            tangent.x *= inv;
            tangent.y *= inv;
            tangent.z *= inv;
            tangent.w *= inv;
        }
    }
    mPoints[0].tangent = endTangent_(mPoints[0].pos, mPoints[1].pos, mPoints[0].length, mPoints[1].tangent);
    s32 n = mNumPoints;
    mPoints[n - 1].tangent =
        endTangent_(mPoints[n - 2].pos, mPoints[n - 1].pos, mPoints[n - 2].length, mPoints[n - 2].tangent);
}

// 0x0055DDE0 (name is ours)
sead::hostio::NonUniformSpline::Vec4 sead::hostio::NonUniformSpline::calc(f32 t) const
{
    f32 distance = mTotalLength * t;
    f32 start = 0.0f;
    s32 i = 0;
    while (mPoints[i].length + start < distance && i < mNumPoints - 2) {
        start += mPoints[i].length;
        i++;
    }
    const Point& p0 = mPoints[i];
    const Point& p1 = mPoints[i + 1];
    f32 length = p0.length;
    f32 s = (distance - start) / length;
    Matrix44<f32> points(p0.pos.x, p0.pos.y, p0.pos.z, 1.0f, p1.pos.x, p1.pos.y, p1.pos.z, 1.0f,
                         p0.tangent.x * length, p0.tangent.y * length, p0.tangent.z * length, 1.0f,
                         p1.tangent.x * length, p1.tangent.y * length, p1.tangent.z * length, 1.0f);
    Matrix44<f32> m;
    Matrix44CalcCtr<f32>::multiply(m, s_HermiteBasis, points);
    f32 s2 = s * s;
    f32 s3 = s2 * s;
    Vec4 v;
    v.x = s3 * m.m[0][0] + s2 * m.m[1][0] + s * m.m[2][0] + 1.0f * m.m[3][0];
    v.y = s3 * m.m[0][1] + s2 * m.m[1][1] + s * m.m[2][1] + 1.0f * m.m[3][1];
    v.z = s3 * m.m[0][2] + s2 * m.m[1][2] + s * m.m[2][2] + 1.0f * m.m[3][2];
    v.w = s3 * m.m[0][3] + s2 * m.m[1][3] + s * m.m[2][3] + 1.0f * m.m[3][3];
    return v;
}

template <typename T>
T curveLinear_(f32 t, const CurveDataInfo* info, const T* buf)
{
    if (t < 0.0f) {
        return buf[0];
    }
    s32 n = info->numUse - 1;
    s32 i = static_cast<s32>(t * n);
    if (i >= n) {
        return buf[i];
    }
    return buf[i] + (buf[i + 1] - buf[i]) * (t * n - i);
}

template <typename T>
T curveHermit_(f32 t, const CurveDataInfo* info, const T* buf)
{
    if (info->numUse % 2 != 0) {
        return 0.0f;
    }
    if (t < 0.0f) {
        return buf[0];
    }
    s32 n = info->numUse / 2 - 1;
    s32 i = static_cast<s32>(t * n);
    f32 s = t * n - i;
    if (i >= n) {
        return buf[i * 2];
    }
    return hermite_(buf[i * 2], buf[i * 2 + 1], buf[i * 2 + 2], buf[i * 2 + 3], s);
}

template <typename T>
T curveStep_(f32 t, const CurveDataInfo* info, const T* buf)
{
    if (t < 0.0f) {
        t = 0.0f;
    } else if (t > 1.0f) {
        t = 1.0f;
    }
    return buf[static_cast<s32>((info->numUse - 1) * t)];
}

template <typename T>
T curveSin_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return sinf(buf[0] * (t * PI2)) * buf[1];
}

template <typename T>
T curveCos_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return cosf(buf[0] * (t * PI2)) * buf[1];
}

template <typename T>
T curveSinPow2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    f32 s = sinf(buf[0] * (t * PI2));
    return buf[1] * (s * s);
}

template <typename T>
T curveLinear2D_(f32 t, const CurveDataInfo* info, const T* buf)
{
    s32 n = info->numUse / 2;
    if (buf[0] >= t) {
        return buf[1];
    }
    if (buf[n * 2 - 2] <= t) {
        return buf[n * 2 - 1];
    }
    for (s32 i = 0; i < n; i++) {
        if (buf[i * 2 + 2] > t) {
            const T* p = &buf[i * 2];
            return p[1] + (p[3] - p[1]) * ((t - p[0]) / (p[2] - p[0]));
        }
    }
    return 0.0f;
}

template <typename T>
T curveHermit2D_(f32 t, const CurveDataInfo* info, const T* buf)
{
    s32 n = info->numUse / 3;
    if (buf[0] >= t) {
        return buf[1];
    }
    if (buf[(n - 1) * 3] <= t) {
        return buf[(n - 1) * 3 + 1];
    }
    for (s32 i = 0; i < n; i++) {
        if (buf[i * 3 + 3] > t) {
            const T* p = &buf[i * 3];
            f32 s = (t - p[0]) / (p[3] - p[0]);
            return hermite_(p[1], p[2], p[4], p[5], s);
        }
    }
    return 0.0f;
}

template <typename T>
T curveStep2D_(f32 t, const CurveDataInfo* info, const T* buf)
{
    s32 n = info->numUse / 2;
    if (buf[0] >= t) {
        return buf[1];
    }
    if (buf[n * 2 - 2] <= t) {
        return buf[n * 2 - 1];
    }
    for (s32 i = 0; i < n; i++) {
        if (buf[i * 2 + 2] > t) {
            return buf[i * 2 + 1];
        }
    }
    return 0.0f;
}

template <typename T>
T curveSpline_(f32, const CurveDataInfo*, const T*)
{
    return 0.0f;
}

template <typename T>
T curveHermitSlope2D_(f32 t, const CurveDataInfo* info, const T* buf)
{
    s32 n = info->numUse / 3;
    if (buf[0] >= t) {
        return buf[1];
    }
    if (buf[(n - 1) * 3] <= t) {
        return buf[(n - 1) * 3 + 1];
    }
    for (s32 i = 0; i < n; i++) {
        if (buf[i * 3 + 3] > t) {
            const T* p = &buf[i * 3];
            f32 width = p[3] - p[0];
            f32 s = (t - p[0]) / width;
            return hermite_(p[1], p[2] * width, p[4], p[5] * width, s);
        }
    }
    return 0.0f;
}

template <typename T>
Vector2<T> curveLinearVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveLinear_(t, info, buf));
}

template <typename T>
Vector2<T> curveHermitVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveHermit_(t, info, buf));
}

template <typename T>
Vector2<T> curveStepVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveStep_(t, info, buf));
}

template <typename T>
Vector2<T> curveSinVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveSin_(t, info, buf));
}

template <typename T>
Vector2<T> curveCosVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveCos_(t, info, buf));
}

template <typename T>
Vector2<T> curveSinPow2Vec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveSinPow2_(t, info, buf));
}

template <typename T>
Vector2<T> curveLinear2DVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveLinear2D_(t, info, buf));
}

template <typename T>
Vector2<T> curveHermit2DVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveHermit2D_(t, info, buf));
}

template <typename T>
Vector2<T> curveStep2DVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveStep2D_(t, info, buf));
}

template <typename T>
Vector2<T> curveSplineVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    NonUniformSpline spline;
    for (s32 i = 0; i < info->numUse / 2; i++) {
        NonUniformSpline::Vec4 pos;
        pos.x = buf[i * 2];
        pos.y = buf[i * 2 + 1];
        pos.z = 0.0f;
        pos.w = 1.0f;
        spline.addPoint(pos);
    }
    spline.prepare();
    NonUniformSpline::Vec4 v = spline.calc(t);
    return Vector2<T>(v.x, v.y);
}

template <typename T>
Vector2<T> curveHermitSlope2DVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveHermitSlope2D_(t, info, buf));
}

// 0x0097636C (name is ours)
f32 (*sCurveFunctionTbl_f32[CURVE_TYPE_NUM])(f32, const CurveDataInfo*, const f32*) = {
    curveLinear_<f32>,  curveHermit_<f32>,   curveStep_<f32>,     curveSin_<f32>,
    curveCos_<f32>,     curveSinPow2_<f32>,  curveLinear2D_<f32>, curveHermit2D_<f32>,
    curveStep2D_<f32>,  curveSpline_<f32>,   curveHermitSlope2D_<f32>,
};

// 0x00976398 (name is ours)
Vector2<f32> (*sCurveFunctionTbl_Vec2f[CURVE_TYPE_NUM])(f32, const CurveDataInfo*, const f32*) = {
    curveLinearVec2_<f32>,    curveHermitVec2_<f32>,  curveStepVec2_<f32>,     curveSinVec2_<f32>,
    curveCosVec2_<f32>,       curveSinPow2Vec2_<f32>, curveLinear2DVec2_<f32>, curveHermit2DVec2_<f32>,
    curveStep2DVec2_<f32>,    curveSplineVec2_<f32>,  curveHermitSlope2DVec2_<f32>,
};
} // namespace hostio
} // namespace sead

// 0x008135FC
template f32 sead::hostio::curveLinear_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x0081350C
template f32 sead::hostio::curveHermit_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x008134B8
template f32 sead::hostio::curveStep_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813E98 (name is ours)
template f32 sead::hostio::curveSin_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813E6C (name is ours)
template f32 sead::hostio::curveCos_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x0081372C
template f32 sead::hostio::curveSinPow2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x008138B4
template f32 sead::hostio::curveLinear2D_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813798
template f32 sead::hostio::curveHermit2D_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x0081366C
template f32 sead::hostio::curveStep2D_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813D0C (name is ours)
template f32 sead::hostio::curveSpline_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813BE8 (name is ours)
template f32 sead::hostio::curveHermitSlope2D_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);

// 0x008139D8 (name is ours)
template sead::Vector2<float> sead::hostio::curveLinearVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x008139B8 (name is ours)
template sead::Vector2<float> sead::hostio::curveHermitVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x0081395C (name is ours)
template sead::Vector2<float> sead::hostio::curveStepVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x0081375C (name is ours)
template sead::Vector2<float> sead::hostio::curveSinVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x008136F0 (name is ours)
template sead::Vector2<float> sead::hostio::curveCosVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813AD8 (name is ours)
template sead::Vector2<float> sead::hostio::curveSinPow2Vec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813B38 (name is ours)
template sead::Vector2<float> sead::hostio::curveLinear2DVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813B18 (name is ours)
template sead::Vector2<float> sead::hostio::curveHermit2DVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813A4C (name is ours)
template sead::Vector2<float> sead::hostio::curveStep2DVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813D38 (name is ours)
template sead::Vector2<float> sead::hostio::curveSplineVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);
// 0x00813D18 (name is ours)
template sead::Vector2<float> sead::hostio::curveHermitSlope2DVec2_<float>(f32, const sead::hostio::CurveDataInfo*, const float*);

// 0x00829E38 slot 0x00 (name is ours)
template f32 sead::hostio::Curve<float>::interpolateToF32(f32);
// 0x00829E54 slot 0x04 (name is ours)
template sead::Vector2<float> sead::hostio::Curve<float>::interpolateToVec2f(f32);
