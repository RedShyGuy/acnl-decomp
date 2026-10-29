#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"
#include "sead/seadMathCalcCommon.h"

namespace sead {
// The matrices of sead. The f32 ones are the nn::math matrices (the CalcCtr functions take those, and
// ARMCC copies them with MTX34CopyAsm/MTX44CopyAsm); only these are used. The constants are defined
// in seadMathConstants.cpp (names are ours).
template <typename T>
class Matrix34;

template <>
class Matrix34<f32> : public nn::math::MTX34
{
public:
    Matrix34() {}
    Matrix34(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21, f32 m22,
             f32 m23)
        : nn::math::MTX34(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23)
    {
    }

    Matrix34(const Matrix34&) = default;
    // (an assignment calls Matrix34CalcCtr::copy, e.g. ptcl::EmitterSet::setMtx)
    Matrix34& operator=(const Matrix34& m)
    {
        Matrix34CalcCtr<f32>::copy(*this, m);
        return *this;
    }

    static const Matrix34 zero;
    static const Matrix34 ident;
};

template <typename T>
class Matrix44;

template <>
class Matrix44<f32> : public nn::math::MTX44
{
public:
    Matrix44() {}
    Matrix44(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21, f32 m22,
             f32 m23, f32 m30, f32 m31, f32 m32, f32 m33)
        : nn::math::MTX44(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
    {
    }

    static const Matrix44 zero;
    static const Matrix44 ident;
};

typedef Matrix34<f32> Matrix34f;
typedef Matrix44<f32> Matrix44f;
} // namespace sead
