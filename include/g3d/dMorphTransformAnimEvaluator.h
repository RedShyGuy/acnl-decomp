#pragma once

#include "decomp.h"
#include "g3d/dCalcRatio.h"
#include <cmath>
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Quaternion.h"
#include "nw/gfx/gfx_TransformAnimEvaluator.h"

namespace g3d {
// the anim type of the morphing animation objects (AnimObject::m_AnimType; name is ours)
const int ANIM_TYPE_MORPH = 2;

// a transform evaluator that blends the previous pose into its result while it morphs (names are ours)
// RTTI N3g3d27MorphTransformAnimEvaluatorE @ 0x008D0D48
// vtable 0x00903B58 (vptr 0x00903B60), offset_to_top 0, 13 entries
class MorphTransformAnimEvaluator : public ::nw::gfx::TransformAnimEvaluator
{
public:
    // (inline in MorphSkeletalAnim::CreateAnimObject)
    explicit MorphTransformAnimEvaluator(nw::os::IAllocator* pAllocator) : nw::gfx::TransformAnimEvaluator(pAllocator) {}
    virtual ~MorphTransformAnimEvaluator(); // 0x004F1018 slot 0x00
    // 0x004F1014 (deleting dtor; falls into the destructor)
    DECOMP_EMIT_COPY virtual const nw::gfx::TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00747458 slot 0x08
    // the group is remembered on success
    u32 TryBind(nw::gfx::AnimGroup* pAnimGroup) override; // 0x004F0FF4 slot 0x0C
    // the frame and the morph
    void UpdateFrame() override; // 0x004F0F48 slot 0x14
    DECOMP_EMIT_COPY virtual const nw::anim::AnimResult* GetResult(void* pTarget, int memberIdx) const // 0x00747464 slot 0x18
    {
        if (!GetMorphResult(static_cast<nw::gfx::CalculatedTransform*>(pTarget), memberIdx, m_CalcRatio.m_Ratio)) {
            return NULL;
        }
        return static_cast<const nw::anim::AnimResult*>(pTarget);
    }

    // the result of the member blended with the transform the target has (the previous pose) by ratio: rotations by
    // a slerp of their quaternions, translations linearly; false if the member is not animated
    DECOMP_EMIT_COPY bool GetMorphResult(nw::gfx::CalculatedTransform* pTarget, int memberIdx, f32 ratio) const; // 0x00746FE4

    static const nw::gfx::TypeInfo s_TypeInfo;

    CalcRatio m_CalcRatio; // 0x70
};
ASSERT_SIZE(MorphTransformAnimEvaluator, 0x88);

inline bool MorphTransformAnimEvaluator::GetMorphResult(nw::gfx::CalculatedTransform* pTarget, int memberIdx, f32 ratio) const
{
    if (!HasMemberAnim(memberIdx)) {
        return false;
    }
    if (!(ratio > 0.0f)) {
        TransformAnimEvaluator::GetResult(pTarget, memberIdx);
        return true;
    }
    const nw::gfx::CalculatedTransform prevTransform(*pTarget);
    // (0x00AF91F4, guard 0x009832C0)
    static const nn::math::MTX34 s_IdentityMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    nw::gfx::CalculatedTransform transform(s_IdentityMatrix, nn::math::VEC3(1.0f, 1.0f, 1.0f));
    TransformAnimEvaluator::GetResult(&transform, memberIdx);

    nn::math::MTX34 prevMatrix(prevTransform.m_TransformMatrix);
    const f32 prevTx = prevMatrix.m[0][3];
    const f32 prevTy = prevMatrix.m[1][3];
    const f32 prevTz = prevMatrix.m[2][3];
    nn::math::QUAT prevQuat;
    nn::math::ARMv6::MTX34ToQUATC_FAST(&prevQuat, &prevMatrix);
    nn::math::MTX34 matrix(transform.m_TransformMatrix);
    const f32 tx = matrix.m[0][3];
    const f32 ty = matrix.m[1][3];
    const f32 tz = matrix.m[2][3];
    nn::math::QUAT quat;
    nn::math::ARMv6::MTX34ToQUATC_FAST(&quat, &matrix);

    // the slerp from the new rotation to the previous one by ratio
    f32 sign = 1.0f;
    f32 cosTheta = quat.x * prevQuat.x + quat.y * prevQuat.y + quat.z * prevQuat.z + quat.w * prevQuat.w;
    if (cosTheta < 0.0f) {
        cosTheta = -cosTheta;
        sign = -1.0f;
    }
    const f32 inverseRatio = 1.0f - ratio;
    f32 newWeight;
    f32 prevWeight;
    if (cosTheta <= 0.99999f) {
        const f32 theta = std::acos(cosTheta);
        const f32 sinTheta = std::sin(theta);
        newWeight = std::sin(inverseRatio * theta) / sinTheta;
        prevWeight = std::sin(ratio * theta) / sinTheta * sign;
    } else {
        newWeight = inverseRatio;
        prevWeight = sign * ratio;
    }
    quat.x = quat.x * newWeight + prevWeight * prevQuat.x;
    quat.y = quat.y * newWeight + prevWeight * prevQuat.y;
    quat.z = quat.z * newWeight + prevWeight * prevQuat.z;
    quat.w = quat.w * newWeight + prevWeight * prevQuat.w;
    // (the quaternion in the matrix for NormalizeRotateQuaternion)
    matrix.m[0][0] = quat.x;
    matrix.m[0][1] = quat.y;
    matrix.m[0][2] = quat.z;
    matrix.m[1][0] = quat.w;
    matrix.m[1][1] = 1.0f;

    pTarget->DisableFlags(nw::gfx::CalculatedTransform::FLAG_ROTATE_UNSET);
    nn::math::ARMv6::MTX34CopyAsm(&pTarget->m_TransformMatrix, &matrix);
    pTarget->SetTranslate(nn::math::VEC3(prevTx * ratio + tx * inverseRatio, prevTy * ratio + ty * inverseRatio,
                                         prevTz * ratio + tz * inverseRatio));
    pTarget->EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
    pTarget->SetScale(transform.m_Scale);
    pTarget->EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
    if (!pTarget->NormalizeRotateQuaternion()) {
        *pTarget = prevTransform;
    }
    pTarget->UpdateFlags();
    return true;
}
} // namespace g3d
