#include "nw/gfx/gfx_TransformAnimBlendOp.h"

#include <cmath>

#include "nn/math/math_Quaternion.h"

namespace nw {
namespace gfx {
namespace {
// the weight of a part that is left out
const f32 WEIGHT_NONE = -1.0f;
// the cosine above which the quaternions are interpolated linearly
const f32 SLERP_EPSILON_COS = 0.99999f;
} // namespace

// 0x0073B2D8 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendTranslate(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const
{
    if (weight == WEIGHT_NONE) {
        return;
    }
    const nn::math::VEC3 translate(pSrc->m_TransformMatrix.m[0][3] * weight, pSrc->m_TransformMatrix.m[1][3] * weight,
                                   pSrc->m_TransformMatrix.m[2][3] * weight);
    if (pDst->IsEnabledFlags(CalculatedTransform::FLAG_TRANSLATE_UNSET)) {
        pDst->SetTranslate(translate);
        pDst->DisableFlags(CalculatedTransform::FLAG_TRANSLATE_UNSET);
    } else {
        pDst->m_TransformMatrix.m[0][3] += translate.x;
        pDst->m_TransformMatrix.m[1][3] += translate.y;
        pDst->m_TransformMatrix.m[2][3] += translate.z;
    }
    pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x0073B364 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendRotateMatrix(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const
{
    if (weight == WEIGHT_NONE) {
        return;
    }
    // the first two rows (the third one follows from them in NormalizeRotateMatrix)
    f32(*dst)[4] = pDst->m_TransformMatrix.m;
    const f32(*src)[4] = pSrc->m_TransformMatrix.m;
    if (pDst->IsEnabledFlags(CalculatedTransform::FLAG_ROTATE_UNSET)) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 3; ++j) {
                dst[i][j] = src[i][j] * weight;
            }
        }
    } else {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 3; ++j) {
                dst[i][j] += src[i][j] * weight;
            }
        }
    }
    pDst->DisableFlags(CalculatedTransform::FLAG_ROTATE_UNSET);
    pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x0073B448 | nintendogs:bytes [tier B]
bool nw::gfx::TransformAnimBlendOp::OverrideTransform(CalculatedTransform* pDst, const CalculatedTransform* pSrc, u32 mode) const
{
    // the source has to be converted into the form of the destination
    const bool isConvert = pDst->IsEnabledFlags(CalculatedTransform::FLAG_IS_BLEND_FORM) && !pSrc->IsEnabledFlags(CalculatedTransform::FLAG_IS_BLEND_FORM);
    nn::math::MTX34 mtx;
    nn::math::ARMv6::MTX34CopyAsm(&mtx, &pDst->m_TransformMatrix);

    if (pDst->IsEnabledFlags(CalculatedTransform::FLAG_SCALE_UNSET) && !pSrc->IsEnabledFlags(CalculatedTransform::FLAG_SCALE_UNSET)) {
        pDst->m_Scale = pSrc->m_Scale;
        pDst->DisableFlags(CalculatedTransform::FLAG_SCALE_UNSET);
        pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
        if (isConvert && (mode & OVERRIDE_ACCURATE_SCALE)) {
            pDst->m_Scale.x = std::log(pDst->m_Scale.x);
            pDst->m_Scale.y = std::log(pDst->m_Scale.y);
            pDst->m_Scale.z = std::log(pDst->m_Scale.z);
            pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
        }
    }

    if (pDst->IsEnabledFlags(CalculatedTransform::FLAG_ROTATE_UNSET) && !pSrc->IsEnabledFlags(CalculatedTransform::FLAG_ROTATE_UNSET)) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                mtx.m[i][j] = pSrc->m_TransformMatrix.m[i][j];
            }
        }
        pDst->DisableFlags(CalculatedTransform::FLAG_ROTATE_UNSET);
        if (isConvert && (mode & OVERRIDE_QUATERNION) && !pDst->IsEnabledFlags(CalculatedTransform::FLAG_ROTATE_UNSET)) {
            // (the quaternion of the matrix the destination had before; the copy below replaces it)
            nn::math::QUAT quat;
            nn::math::ARMv6::MTX34ToQUATC_FAST(&quat, &pDst->m_TransformMatrix);
            pDst->m_TransformMatrix.m[0][0] = quat.x;
            pDst->m_TransformMatrix.m[0][1] = quat.y;
            pDst->m_TransformMatrix.m[0][2] = quat.z;
            pDst->m_TransformMatrix.m[1][0] = quat.w;
            pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
        }
    }

    if (pDst->IsEnabledFlags(CalculatedTransform::FLAG_TRANSLATE_UNSET) && !pSrc->IsEnabledFlags(CalculatedTransform::FLAG_TRANSLATE_UNSET)) {
        mtx.m[0][3] = pSrc->m_TransformMatrix.m[0][3];
        mtx.m[1][3] = pSrc->m_TransformMatrix.m[1][3];
        mtx.m[2][3] = pSrc->m_TransformMatrix.m[2][3];
        pDst->DisableFlags(CalculatedTransform::FLAG_TRANSLATE_UNSET);
    }

    nn::math::ARMv6::MTX34CopyAsm(&pDst->m_TransformMatrix, &mtx);
    pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
    return !pDst->IsEnabledFlags(CalculatedTransform::FLAG_SCALE_UNSET) && !pDst->IsEnabledFlags(CalculatedTransform::FLAG_ROTATE_UNSET) &&
           !pDst->IsEnabledFlags(CalculatedTransform::FLAG_TRANSLATE_UNSET);
}

// 0x0073B668 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendScaleAccurate(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const
{
    if (weight == WEIGHT_NONE) {
        return;
    }
    const nn::math::VEC3 logScale(std::log(pSrc->m_Scale.x) * weight, std::log(pSrc->m_Scale.y) * weight, std::log(pSrc->m_Scale.z) * weight);
    if (pDst->IsEnabledFlags(CalculatedTransform::FLAG_SCALE_UNSET)) {
        pDst->m_Scale = logScale;
    } else {
        pDst->m_Scale.x += logScale.x;
        pDst->m_Scale.y += logScale.y;
        pDst->m_Scale.z += logScale.z;
    }
    pDst->DisableFlags(CalculatedTransform::FLAG_SCALE_UNSET);
    pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x0073B71C | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendScaleStandard(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const
{
    if (weight == WEIGHT_NONE) {
        return;
    }
    if (pDst->IsEnabledFlags(CalculatedTransform::FLAG_SCALE_UNSET)) {
        pDst->m_Scale.x = pSrc->m_Scale.x * weight;
        pDst->m_Scale.y = pSrc->m_Scale.y * weight;
        pDst->m_Scale.z = pSrc->m_Scale.z * weight;
    } else {
        pDst->m_Scale.x += pSrc->m_Scale.x * weight;
        pDst->m_Scale.y += pSrc->m_Scale.y * weight;
        pDst->m_Scale.z += pSrc->m_Scale.z * weight;
    }
    pDst->DisableFlags(CalculatedTransform::FLAG_SCALE_UNSET);
    pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x0073B7B4 slot 0x18 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::ConvertToAnimResult(anim::AnimResult* pResult, const void* pSource) const
{
    *static_cast<CalculatedTransform*>(pResult->GetValueBuffer()) = *static_cast<const CalculatedTransform*>(pSource);
}

// 0x0073B7D4 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendRotateQuaternion(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const
{
    if (weight == WEIGHT_NONE) {
        return;
    }
    // the quaternion of the source (already one in the form for blending)
    nn::math::QUAT srcQuat;
    if (pSrc->IsEnabledFlags(CalculatedTransform::FLAG_IS_BLEND_FORM)) {
        srcQuat.x = pSrc->m_TransformMatrix.m[0][0];
        srcQuat.y = pSrc->m_TransformMatrix.m[0][1];
        srcQuat.z = pSrc->m_TransformMatrix.m[0][2];
        srcQuat.w = pSrc->m_TransformMatrix.m[1][0];
    } else {
        nn::math::ARMv6::MTX34ToQUATC_FAST(&srcQuat, &pSrc->m_TransformMatrix);
    }

    // the sum of the weights so far is kept in m[1][1]
    f32* pTotalWeight = &pDst->m_TransformMatrix.m[1][1];
    nn::math::QUAT result;
    if (pDst->IsEnabledFlags(CalculatedTransform::FLAG_ROTATE_UNSET)) {
        *pTotalWeight = weight;
        result = srcQuat;
    } else {
        const f32 totalWeight = *pTotalWeight + weight;
        *pTotalWeight = totalWeight;
        const f32 t = (totalWeight == 0.0f) ? 0.0f : weight / totalWeight;

        const f32 dstX = pDst->m_TransformMatrix.m[0][0];
        const f32 dstY = pDst->m_TransformMatrix.m[0][1];
        const f32 dstZ = pDst->m_TransformMatrix.m[0][2];
        const f32 dstW = pDst->m_TransformMatrix.m[1][0];
        f32 cosine = dstX * srcQuat.x + dstY * srcQuat.y + dstZ * srcQuat.z + dstW * srcQuat.w;
        f32 sign = 1.0f;
        if (cosine < 0.0f) {
            cosine = -cosine;
            sign = -1.0f;
        }
        f32 dstRatio;
        f32 srcRatio;
        if (cosine > SLERP_EPSILON_COS) {
            srcRatio = sign * t;
            dstRatio = 1.0f - t;
        } else {
            const f32 theta = std::acos(cosine);
            const f32 sinTheta = std::sin(theta);
            dstRatio = std::sin((1.0f - t) * theta) / sinTheta;
            srcRatio = std::sin(t * theta) / sinTheta * sign;
        }
        result.x = dstX * dstRatio + srcRatio * srcQuat.x;
        result.y = dstY * dstRatio + srcRatio * srcQuat.y;
        result.z = dstZ * dstRatio + srcRatio * srcQuat.z;
        result.w = dstW * dstRatio + srcRatio * srcQuat.w;
    }
    pDst->m_TransformMatrix.m[0][0] = result.x;
    pDst->m_TransformMatrix.m[0][1] = result.y;
    pDst->m_TransformMatrix.m[0][2] = result.z;
    pDst->m_TransformMatrix.m[1][0] = result.w;
    pDst->DisableFlags(CalculatedTransform::FLAG_ROTATE_UNSET);
    // (the matrix copied onto itself)
    nn::math::ARMv6::MTX34CopyAsm(&pDst->m_TransformMatrix, &pDst->m_TransformMatrix);
    pDst->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x0073B970 | nintendogs:bytes [tier A]
bool nw::gfx::TransformAnimBlendOp::PostBlendAccurateScale(CalculatedTransform* pTransform) const
{
    pTransform->m_Scale.x = std::exp(pTransform->m_Scale.x);
    pTransform->m_Scale.y = std::exp(pTransform->m_Scale.y);
    pTransform->m_Scale.z = std::exp(pTransform->m_Scale.z);
    pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
    return true;
}

// 0x0073B9B4 slot 0x14 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::Apply(void* pTarget, const anim::AnimResult* pResult) const
{
    *static_cast<CalculatedTransform*>(pTarget) = *static_cast<const CalculatedTransform*>(pResult->GetValueBuffer());
}

} // namespace gfx
} // namespace nw
