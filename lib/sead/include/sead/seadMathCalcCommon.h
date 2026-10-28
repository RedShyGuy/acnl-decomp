#pragma once

#include "decomp.h"

#include <math.h>

namespace nn {
namespace math {
struct MTX22;
struct MTX33;
struct MTX34;
struct MTX44;
struct QUAT;
struct VEC2;
struct VEC3;
struct VEC4;
} // namespace math
} // namespace nn

namespace sead {
template <typename T>
class Vector3;
} // namespace sead

// the hand-written VFP assembly of sead's math (the *CalcCtr classes, on the nn::math types)
#define SEAD_MATH_ASM __attribute__((naked))

namespace sead {
// an entry of the table of sinCosIdx: one of 256 steps per turn (and the first again), with the
// change of sin and cos to the next step (names are ours)
struct MathSinCosEntry
{
    f32 sinValue;
    f32 sinDelta;
    f32 cosValue;
    f32 cosDelta;
};
ASSERT_SIZE(MathSinCosEntry, 0x10);

// an entry of the table of atanIdx_: the angle index of atan(i / 128) and its change to the next
// entry (names are ours)
struct MathAtanEntry
{
    u32 idx;
    f32 delta;
};
ASSERT_SIZE(MathAtanEntry, 0x8);

// (names are ours)
extern const MathSinCosEntry s_MathSinCosTable[257];
extern const MathAtanEntry s_MathAtanTable[129];

// The angle index of the *Idx functions is one turn in 2^32: the top 8 bits select the step of the
// table, the low 24 bits interpolate to the next one.
template <typename T>
class MathCtr
{
public:
    static void sinCosIdx(T* pSin, T* pCos, u32 idx);
    static T tanIdx(u32 idx);
};

template <typename T>
class MathCommon
{
public:
    // the angle index of atan(x) for x in [0, 1]
    static u32 atanIdx_(T x);

    // (inline everywhere; names are ours)
    static T sinIdx(u32 idx)
    {
        const MathSinCosEntry& entry = s_MathSinCosTable[idx >> 24];
        return entry.sinValue + entry.sinDelta * (static_cast<T>(idx & 0xFFFFFF) * (1.0f / 0x1000000));
    }
    // the angle index of acos(x) for x in [-1, 1]
    static u32 acosIdx(T x)
    {
        if (x >= 0) {
            if (x > 0.70710677f) {
                return atanIdx_(sqrtf(1 - x * x) / x);
            }
            return 0x40000000 - atanIdx_(x / sqrtf(1 - x * x));
        }
        if (x < -0.70710677f) {
            return 0x80000000 - atanIdx_(-sqrtf(1 - x * x) / x);
        }
        return 0x40000000 + atanIdx_(-x / sqrtf(1 - x * x));
    }
};
template <>
u32 MathCommon<f32>::atanIdx_(f32 x);

template <typename T>
class Vector2CalcCtr
{
public:
    // (no change for the zero vector)
    static void normalize(nn::math::VEC2& v);
};

template <typename T>
class Vector3CalcCtr
{
public:
    static void add(nn::math::VEC3& o, const nn::math::VEC3& a, const nn::math::VEC3& b);
    static void sub(nn::math::VEC3& o, const nn::math::VEC3& a, const nn::math::VEC3& b);
    static void mul(nn::math::VEC3& o, const nn::math::VEC3& a, const nn::math::VEC3& b);
    static void mul(nn::math::VEC3& o, const nn::math::MTX34& m, const nn::math::VEC3& v);
    static void mul(nn::math::VEC3& o, const nn::math::MTX33& m, const nn::math::VEC3& v); // (name is ours)
    // m without the translation
    static void rotate(nn::math::VEC3& o, const nn::math::MTX34& m, const nn::math::VEC3& v);
    static void div(nn::math::VEC3& o, const nn::math::VEC3& a, const nn::math::VEC3& b);
    static void multScalar(nn::math::VEC3& o, const nn::math::VEC3& v, f32 s);
    static void divScalar(nn::math::VEC3& o, const nn::math::VEC3& v, f32 s);
    // o = s * a + b
    static void multScalarAdd(nn::math::VEC3& o, f32 s, const nn::math::VEC3& a, const nn::math::VEC3& b);
    static void normalize(nn::math::VEC3& v);
    static void cross(nn::math::VEC3& o, const nn::math::VEC3& a, const nn::math::VEC3& b);
};

template <typename T>
class QuatCalcCtr
{
public:
    // the rotation of from to to (unit vectors); the identity and false if they point in opposite
    // directions
    static bool makeVectorRotation(nn::math::QUAT& q, const nn::math::VEC3& from, const nn::math::VEC3& to);
    // the spherical interpolation at t between a and b (the shorter way)
    static void slerpToIdx(nn::math::QUAT& o, const nn::math::QUAT& a, const nn::math::QUAT& b, T t);
    // the rotation by angle (radians) about the unit vector axis
    static void setAxisRotation(nn::math::QUAT& q, const nn::math::VEC3& axis, T angle);
    static void mul(nn::math::QUAT& o, const nn::math::QUAT& a, const nn::math::QUAT& b);
    // the rotation by roll about x, then pitch about y, then yaw about z (radians)
    static void setRPY(nn::math::QUAT& q, T roll, T pitch, T yaw);
};

template <typename T>
class Matrix22CalcCtr
{
public:
    static void multiply(nn::math::MTX22& o, const nn::math::MTX22& a, const nn::math::MTX22& b);
};

template <typename T>
class Matrix33CalcCtr
{
public:
    static void copy(nn::math::MTX33& o, const nn::math::MTX33& n); // (name is ours)
    static void copy(nn::math::MTX33& o, const nn::math::MTX34& n);
    // the identity if n is singular
    static void inverse(nn::math::MTX33& o, const nn::math::MTX33& n);
    static void inverseTranspose(nn::math::MTX33& o, const nn::math::MTX33& n);
    static void multiply(nn::math::MTX33& o, const nn::math::MTX33& a, const nn::math::MTX33& b); // (name is ours)
    static void transposeTo(nn::math::MTX33& o, const nn::math::MTX33& n);
    static void makeZero(nn::math::MTX33& o);
};

template <typename T>
class Matrix34CalcCtr
{
public:
    static void copy(nn::math::MTX34& o, const nn::math::MTX34& n);
    static void copy(nn::math::MTX34& o, const nn::math::MTX33& n, const nn::math::VEC3& t);
    static void copy(nn::math::MTX34& o, const nn::math::MTX44& n); // (name is ours)
    static void inverse(nn::math::MTX34& o, const nn::math::MTX34& n);
    static void inverseTranspose(nn::math::MTX34& o, const nn::math::MTX34& n);
    static void multiply(nn::math::MTX34& o, const nn::math::MTX34& a, const nn::math::MTX34& b);
    static void transposeTo(nn::math::MTX34& o, const nn::math::MTX34& n);
    static void makeZero(nn::math::MTX34& o);
    static void makeIdentity(nn::math::MTX34& o);
    // scale, then the rotations about x, y and z (angle indices), then the translation
    static void makeSRTIdx(nn::math::MTX34& o, const nn::math::VEC3& s, const Vector3<u32>& r, const nn::math::VEC3& t);
    // multiplies the columns of the rotation part by x, y and z
    static void scaleBases(nn::math::MTX34& o, T x, T y, T z);
    // the rotation of q (no translation)
    static void makeQ(nn::math::MTX34& o, const nn::math::QUAT& q);
    // the rotations about x, y and z (radians; no translation)
    static void makeR(nn::math::MTX34& o, const nn::math::VEC3& r);
    static void makeSRT(nn::math::MTX34& o, const nn::math::VEC3& s, const nn::math::VEC3& r, const nn::math::VEC3& t);
};

template <typename T>
class Matrix44CalcCtr
{
public:
    static void copy(nn::math::MTX44& o, const nn::math::MTX44& n); // (name is ours)
    static void copy(nn::math::MTX44& o, const nn::math::MTX34& n, const nn::math::VEC4& w); // (name is ours)
    static void copy(nn::math::MTX44& o, const nn::math::MTX33& n, const nn::math::VEC3& t, const nn::math::VEC4& w);
    static void multiply(nn::math::MTX44& o, const nn::math::MTX44& a, const nn::math::MTX44& b);
    static void transpose(nn::math::MTX44& o);
    static void transposeTo(nn::math::MTX44& o, const nn::math::MTX44& n);
    static void makeZero(nn::math::MTX44& o);
    static void makeIdentity(nn::math::MTX44& o);
};
} // namespace sead
