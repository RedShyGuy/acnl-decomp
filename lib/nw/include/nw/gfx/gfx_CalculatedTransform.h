#pragma once

#include "decomp.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Transform.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"

namespace nw {
namespace gfx {
// A transform as matrix (rotation and translation) and scale, with flags that say which parts are
// trivial (the class name and the function names are from the reference symbols, the rest is ours).
class CalculatedTransform
{
public:
    enum Flag
    {
        FLAG_UNKNOWN_0x1 = 1 << 0,          // set by the constructors
        // the translation / scale / rotation has no value yet (blending writes the first one instead
        // of adding it; the flags of the part are not updated)
        FLAG_TRANSLATE_UNSET = 1 << 2,
        FLAG_SCALE_UNSET = 1 << 3,
        FLAG_ROTATE_UNSET = 1 << 4,
        FLAG_IS_IDENTITY = 1 << 5,
        FLAG_IS_ROTATE_TRANSLATE_ZERO = 1 << 6,
        FLAG_IS_ROTATE_ZERO = 1 << 7,
        FLAG_IS_TRANSLATE_ZERO = 1 << 8,
        FLAG_IS_SCALE_ONE = 1 << 9,
        FLAG_IS_UNIFORM_SCALE = 1 << 10,
        FLAG_IS_DIRTY = 1 << 11,            // the matrix changed
        FLAG_UNKNOWN_0x1000 = 1 << 12,      // (with FLAG_UNKNOWN_0x1: SkeletonUpdater::UpdateView looks at the bone)
        // the values are in the form for blending (the logarithm of the scale, the rotation as a
        // quaternion, see NormalizeRotateQuaternion)
        FLAG_IS_BLEND_FORM = 1 << 30,

        FLAG_DEFAULT = FLAG_UNKNOWN_0x1 | FLAG_IS_DIRTY,
        FLAG_IDENTITY = FLAG_UNKNOWN_0x1 | FLAG_IS_IDENTITY | FLAG_IS_ROTATE_TRANSLATE_ZERO | FLAG_IS_ROTATE_ZERO | FLAG_IS_TRANSLATE_ZERO |
                        FLAG_IS_SCALE_ONE | FLAG_IS_UNIFORM_SCALE,
    };

    // (inline) the identity
    CalculatedTransform() : m_TransformMatrix(nn::math::MTX34::Identity())
    {
        m_Scale = nn::math::VEC3(1.0f, 1.0f, 1.0f);
        m_Flags = FLAG_DEFAULT;
    }
    CalculatedTransform(const nn::math::MTX34& matrix, const nn::math::VEC3& scale, u32 flags = FLAG_DEFAULT) : m_TransformMatrix(matrix)
    {
        m_Scale = scale;
        m_Flags = flags;
    }
    // (the copy of the matrix uses MTX34CopyAsm, the assignment copies the members)
    // (inline) the transform of a bone
    explicit CalculatedTransform(res::ResBone bone)
    {
        m_Flags = FLAG_DEFAULT;
        SetTransform(bone);
    }
    CalculatedTransform(const CalculatedTransform& other) = default;
    CalculatedTransform& operator=(const CalculatedTransform& other) = default;
    // (ARMCC keeps empty loops over the elements of arrays when they are destroyed)
    ~CalculatedTransform() {}

    // the identity (inline: a function-local static; ARMCC inlines it in places and calls the copy
    // out of line elsewhere)
    DECOMP_EMIT_COPY static const CalculatedTransform& Identity() // 0x004A0968 (name is ours)
    {
        // 0x00AF927C (guard 0x009832CC)
        static const CalculatedTransform s_Identity(nn::math::MTX34::Identity(), nn::math::VEC3(1.0f, 1.0f, 1.0f), FLAG_IDENTITY);
        return s_Identity;
    }

    // (inline)
    nn::math::VEC3 GetTranslate() const
    {
        return nn::math::VEC3(m_TransformMatrix.m[0][3], m_TransformMatrix.m[1][3], m_TransformMatrix.m[2][3]);
    }
    void SetTranslate(const nn::math::VEC3& translate)
    {
        m_TransformMatrix.m[0][3] = translate.x;
        m_TransformMatrix.m[1][3] = translate.y;
        m_TransformMatrix.m[2][3] = translate.z;
    }
    void SetScale(const nn::math::VEC3& scale) { m_Scale = scale; }
    bool IsEnabledFlags(u32 flags) const { return (m_Flags & flags) != 0; }
    void EnableFlags(u32 flags) { m_Flags |= flags; }
    void DisableFlags(u32 flags) { m_Flags &= ~flags; }
    void SetFlags(u32 flags, bool isEnabled)
    {
        if (isEnabled) {
            EnableFlags(flags);
        } else {
            DisableFlags(flags);
        }
    }
    // the uniform-scale and scale-one flags (inline in the original)
    void UpdateScaleFlags()
    {
        if (!IsEnabledFlags(FLAG_SCALE_UNSET)) {
            DisableFlags(FLAG_IS_SCALE_ONE | FLAG_IS_UNIFORM_SCALE);
            if (m_Scale.x == m_Scale.y && m_Scale.x == m_Scale.z) {
                EnableFlags(FLAG_IS_UNIFORM_SCALE);
                if (m_Scale.x == 1.0f) {
                    EnableFlags(FLAG_IS_SCALE_ONE);
                }
            }
        }
    }

    void SetRotateXYZ(f32 x, f32 y, f32 z);
    void SetTransform(const nn::math::Transform3& transform);
    // the transform of a bone, with its flags (name is ours)
    void SetTransform(res::ResBone bone);
    void UpdateRotateFlags();
    void UpdateCompositeFlags();
    void UpdateTranslateFlags();
    // all the flags of the scale, rotation and translation (the game calls it; name is ours)
    void UpdateFlags(); // 0x0049FAAC
    // the rotation made orthonormal again; false if a row is too short
    bool NormalizeRotateMatrix();
    // the matrix of the quaternion that blending left in the rotation (x, y, z in the first row,
    // w in m[1][0]); false if a row is too short (name is ours)
    bool NormalizeRotateQuaternion();
    void SetRotateAndTranslate(const nn::math::VEC3& rotate, const nn::math::VEC3& translate);

    nn::math::MTX34 m_TransformMatrix; // 0x00
    nn::math::VEC3 m_Scale;            // 0x30
    u32 m_Flags;                       // 0x3C
};
ASSERT_OFFSET(CalculatedTransform, m_Scale, 0x30);
ASSERT_SIZE(CalculatedTransform, 0x40);
} // namespace gfx
} // namespace nw
