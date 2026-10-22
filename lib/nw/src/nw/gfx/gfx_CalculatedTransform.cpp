#include "nw/gfx/gfx_CalculatedTransform.h"

#include <cmath>

#include "nw/math/math_Api.h"

namespace nw {
namespace gfx {
namespace {
// the shortest row of a rotation that is normalized
// 0x008A39AC (name is ours)
const f32 s_NormalizeEpsilon = 1.0e-6f;

// (inline)
inline f32 LengthSquare(const f32* pRow)
{
    return pRow[0] * pRow[0] + pRow[1] * pRow[1] + pRow[2] * pRow[2];
}
} // namespace

// 0x0049FBBC | nintendogs:bytes [tier B]
void nw::gfx::CalculatedTransform::SetRotateXYZ(f32 x, f32 y, f32 z)
{
    const nn::math::VEC3 translate = GetTranslate();
    nw::math::MTX34RotateXYZRad(&m_TransformMatrix, x, y, z);
    SetTranslate(translate);
    EnableFlags(FLAG_IS_DIRTY);
}

// 0x0049FDE8 | nintendogs:bytes [tier A]
void nw::gfx::CalculatedTransform::SetTransform(const nn::math::Transform3& transform)
{
    m_Scale = transform.scale;
    nw::math::MTX34RotateXYZRad(&m_TransformMatrix, transform.rotate.x, transform.rotate.y, transform.rotate.z);
    SetTranslate(transform.translate);
    EnableFlags(FLAG_IS_DIRTY);
}

// 0x004A0020 | nintendogs:bytes [tier A]
void nw::gfx::CalculatedTransform::UpdateRotateFlags()
{
    if (IsEnabledFlags(FLAG_ROTATE_UNSET)) {
        return;
    }
    if (m_TransformMatrix.m[0][0] == 1.0f && m_TransformMatrix.m[1][1] == 1.0f) {
        EnableFlags(FLAG_IS_ROTATE_ZERO);
    } else {
        DisableFlags(FLAG_IS_ROTATE_ZERO);
    }
}

// 0x004A0120 | nintendogs:bytes [tier A]
void nw::gfx::CalculatedTransform::UpdateCompositeFlags()
{
    DisableFlags(FLAG_IS_IDENTITY | FLAG_IS_ROTATE_TRANSLATE_ZERO);
    if ((m_Flags & (FLAG_IS_ROTATE_ZERO | FLAG_IS_TRANSLATE_ZERO)) == (FLAG_IS_ROTATE_ZERO | FLAG_IS_TRANSLATE_ZERO)) {
        EnableFlags(FLAG_IS_ROTATE_TRANSLATE_ZERO);
        if (IsEnabledFlags(FLAG_IS_SCALE_ONE)) {
            EnableFlags(FLAG_IS_IDENTITY);
        }
    }
}

// 0x004A0154 | nintendogs:bytes [tier A]
void nw::gfx::CalculatedTransform::UpdateTranslateFlags()
{
    if (IsEnabledFlags(FLAG_TRANSLATE_UNSET)) {
        return;
    }
    if (m_TransformMatrix.m[0][3] == 0.0f && m_TransformMatrix.m[1][3] == 0.0f && m_TransformMatrix.m[2][3] == 0.0f) {
        EnableFlags(FLAG_IS_TRANSLATE_ZERO);
    } else {
        DisableFlags(FLAG_IS_TRANSLATE_ZERO);
    }
}

// 0x004A01B0 | nintendogs:bytes-fuzzy [tier A]
bool nw::gfx::CalculatedTransform::NormalizeRotateMatrix()
{
    if (IsEnabledFlags(FLAG_ROTATE_UNSET)) {
        return true;
    }
    f32(*m)[4] = m_TransformMatrix.m;

    // the third row from the first two
    m[2][0] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
    m[2][1] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
    m[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

    const f32 lengthSquare0 = LengthSquare(m[0]);
    const f32 lengthSquare2 = LengthSquare(m[2]);
    if (s_NormalizeEpsilon > lengthSquare0 || lengthSquare2 < s_NormalizeEpsilon) {
        return false;
    }
    const f32 inverse0 = 1.0f / std::sqrt(lengthSquare0);
    m[0][0] *= inverse0;
    m[0][1] *= inverse0;
    m[0][2] *= inverse0;
    const f32 inverse2 = 1.0f / std::sqrt(lengthSquare2);
    m[2][0] *= inverse2;
    m[2][1] *= inverse2;
    m[2][2] *= inverse2;

    // the second row from the third and the first
    m[1][0] = m[2][1] * m[0][2] - m[2][2] * m[0][1];
    m[1][1] = m[2][2] * m[0][0] - m[2][0] * m[0][2];
    m[1][2] = m[2][0] * m[0][1] - m[2][1] * m[0][0];
    EnableFlags(FLAG_IS_DIRTY);
    return true;
}

// 0x004A02E8 | nintendogs:callgraph [tier A]
void nw::gfx::CalculatedTransform::SetRotateAndTranslate(const nn::math::VEC3& rotate, const nn::math::VEC3& translate)
{
    nw::math::MTX34RotateXYZRad(&m_TransformMatrix, rotate.x, rotate.y, rotate.z);
    SetTranslate(translate);
    EnableFlags(FLAG_IS_DIRTY);
}

// 0x004A0510 (name is ours)
bool nw::gfx::CalculatedTransform::NormalizeRotateQuaternion()
{
    if (IsEnabledFlags(FLAG_ROTATE_UNSET)) {
        return true;
    }
    f32(*m)[4] = m_TransformMatrix.m;
    const f32 x = m[0][0];
    const f32 y = m[0][1];
    const f32 z = m[0][2];
    const f32 w = m[1][0];
    const nn::math::VEC3 translate = GetTranslate();

    const f32 s = 2.0f / (x * x + y * y + z * z + w * w);
    const f32 xs = x * s;
    const f32 ys = y * s;
    const f32 zs = z * s;
    const f32 xx = x * xs;
    const f32 yy = y * ys;
    const f32 zz = z * zs;
    const f32 xy = x * ys;
    const f32 xz = x * zs;
    const f32 yz = y * zs;
    const f32 wx = w * xs;
    const f32 wy = w * ys;
    const f32 wz = w * zs;
    m[0][0] = 1.0f - (yy + zz);
    m[0][1] = xy - wz;
    m[0][2] = xz + wy;
    m[1][0] = xy + wz;
    m[1][1] = 1.0f - (xx + zz);
    m[1][2] = yz - wx;
    m[2][0] = xz - wy;
    m[2][1] = yz + wx;
    m[2][2] = 1.0f - (xx + yy);
    SetTranslate(translate);

    if (s_NormalizeEpsilon > LengthSquare(m[0]) || LengthSquare(m[1]) < s_NormalizeEpsilon || LengthSquare(m[2]) < s_NormalizeEpsilon) {
        return false;
    }
    EnableFlags(FLAG_IS_DIRTY);
    return true;
}

// 0x004A0684 (name is ours)
void nw::gfx::CalculatedTransform::SetTransform(res::ResBone bone)
{
    const res::ResBoneData* pData = bone.ptr();
    m_Scale = pData->scale;
    nw::math::MTX34RotateXYZRad(&m_TransformMatrix, pData->rotate.x, pData->rotate.y, pData->rotate.z);
    SetTranslate(pData->translate);
    EnableFlags(FLAG_IS_DIRTY);

    bool isTranslateZero = false;
    bool isRotateZero = false;
    if (pData->flags & res::ResBoneData::FLAG_IS_IDENTITY) {
        EnableFlags(FLAG_IS_IDENTITY);
    }
    if (pData->flags & res::ResBoneData::FLAG_IS_TRANSLATE_ZERO) {
        EnableFlags(FLAG_IS_TRANSLATE_ZERO);
        isTranslateZero = true;
    }
    if (pData->flags & res::ResBoneData::FLAG_IS_ROTATE_ZERO) {
        EnableFlags(FLAG_IS_ROTATE_ZERO);
        isRotateZero = true;
    }
    if (pData->flags & res::ResBoneData::FLAG_IS_SCALE_ONE) {
        EnableFlags(FLAG_IS_SCALE_ONE);
    }
    if (pData->flags & res::ResBoneData::FLAG_IS_UNIFORM_SCALE) {
        EnableFlags(FLAG_IS_UNIFORM_SCALE);
    }
    if (isTranslateZero && isRotateZero) {
        EnableFlags(FLAG_IS_ROTATE_TRANSLATE_ZERO);
    }
}

} // namespace gfx
} // namespace nw
