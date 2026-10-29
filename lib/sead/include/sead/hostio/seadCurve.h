#pragma once

#include <math.h>

#include "decomp.h"
#include "sead/hostio/seadICurve.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"

namespace sead {
namespace hostio {
// The kinds of curves: the index into the tables of curve functions (names are ours; the functions
// named after the reference symbols are curveLinear_, curveHermit_, curveStep_, curveSinPow2_,
// curveLinear2D_, curveHermit2D_ and curveStep2D_).
enum CurveType : u8
{
    CURVE_LINEAR = 0,         // values at equal distances, linear in between
    CURVE_HERMIT = 1,         // (value, slope) pairs at equal distances
    CURVE_STEP = 2,           // values at equal distances, constant in between
    CURVE_SIN = 3,            // amplitude * sin(frequency * 2pi * t)
    CURVE_COS = 4,            // amplitude * cos(frequency * 2pi * t)
    CURVE_SIN_POW2 = 5,       // amplitude * sin(frequency * 2pi * t)^2
    CURVE_LINEAR_2D = 6,      // (x, y) pairs, linear in between
    CURVE_HERMIT_2D = 7,      // (x, y, slope) triples
    CURVE_STEP_2D = 8,        // (x, y) pairs, constant in between
    CURVE_SPLINE = 9,         // (x, y) points of a NonUniformSpline (only as a 2D curve; 0 as a float curve)
    CURVE_HERMIT_2D_SLOPE = 10, // (x, y, slope) triples, the slopes per unit of x
    CURVE_TYPE_NUM = 11,
};

// The description of the values of a curve (member names are ours).
struct CurveDataInfo
{
    u8 curveType;   // CurveType
    u8 field_0x01;
    u8 field_0x02;
    u8 numUse;      // the number of values
};

// The value of a curve at t: the curves over equal distances run over t = 0..1, the 2D curves over x.
template <typename T>
T curveLinear_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveHermit_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveStep_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveSin_(f32 t, const CurveDataInfo* info, const T* buf); // (name is ours)
template <typename T>
T curveCos_(f32 t, const CurveDataInfo* info, const T* buf); // (name is ours)
template <typename T>
T curveSinPow2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveLinear2D_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveHermit2D_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveStep2D_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveSpline_(f32 t, const CurveDataInfo* info, const T* buf); // (name is ours)
template <typename T>
T curveHermitSlope2D_(f32 t, const CurveDataInfo* info, const T* buf); // (name is ours)

// The point (t, value) of a curve; the spline gives its own point (names are ours).
template <typename T>
Vector2<T> curveLinearVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveHermitVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveStepVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveSinVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveCosVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveSinPow2Vec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveLinear2DVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveHermit2DVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveStep2DVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveSplineVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveHermitSlope2DVec2_(f32 t, const CurveDataInfo* info, const T* buf);

// the curve functions by CurveType (names are ours)
extern f32 (*sCurveFunctionTbl_f32[CURVE_TYPE_NUM])(f32, const CurveDataInfo*, const f32*);
extern Vector2<f32> (*sCurveFunctionTbl_Vec2f[CURVE_TYPE_NUM])(f32, const CurveDataInfo*, const f32*);

// A spline through up to 64 points whose parameter runs over the length of the polyline through
// them; points and tangents have four components (names are ours).
class NonUniformSpline
{
public:
    struct Vec4
    {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };

    struct Point
    {
        Point() {} // 0x0055DAFC (empty; ARMCC builds the array with it)

        Vec4 pos;      // 0x00
        Vec4 tangent;  // 0x10
        f32 length;    // 0x20, the distance to the next point
    };

    static const s32 POINT_MAX = 64;

    NonUniformSpline() : mTotalLength(0.0f), mNumPoints(0) {}

    void addPoint(const Vec4& pos)
    {
        if (mNumPoints == 0) {
            mTotalLength = 0.0f;
        } else {
            Point& prev = mPoints[mNumPoints - 1];
            f32 dx = prev.pos.x - pos.x;
            f32 dy = prev.pos.y - pos.y;
            f32 dz = prev.pos.z - pos.z;
            f32 dw = prev.pos.w - pos.w;
            prev.length = sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
            mTotalLength += prev.length;
        }
        mPoints[mNumPoints++].pos = pos;
    }
    // the tangents of the points
    void prepare(); // 0x0055DB00
    // the point at t = 0..1 of the length
    Vec4 calc(f32 t) const; // 0x0055DDE0

    Point mPoints[POINT_MAX];  // 0x000
    f32 mTotalLength;          // 0x900
    s32 mNumPoints;            // 0x904
};
ASSERT_SIZE(NonUniformSpline, 0x908);

// Instantiations found in the binary:
//   sead::hostio::Curve<float>  typeinfo 0x008D211C  vtable 0x00906BD8
//
// A curve over the values of a buffer (member names and slot names are ours).
template <typename T>
class Curve : public ICurve
{
public:
    virtual f32 interpolateToF32(f32 t) { return sCurveFunctionTbl_f32[mInfo.curveType](t, &mInfo, mFloats); }
    virtual Vector2<f32> interpolateToVec2f(f32 t)
    {
        return sCurveFunctionTbl_Vec2f[mInfo.curveType](t, &mInfo, mFloats);
    }

    const T* mFloats;     // 0x04
    CurveDataInfo mInfo;  // 0x08
};
} // namespace hostio
} // namespace sead
