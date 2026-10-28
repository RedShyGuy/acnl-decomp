#include "nw/gfx/gfx_WorldMatrixUpdater.h"

#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX33.h"
#include "nw/gfx/gfx_CalculatedTransform.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the matrix with its columns scaled (the translation kept; name is ours)
inline void MultScale(nn::math::MTX34* pOut, const nn::math::MTX34& m, const nn::math::VEC3& scale)
{
    pOut->m[0][0] = m.m[0][0] * scale.x;
    pOut->m[1][0] = m.m[1][0] * scale.x;
    pOut->m[2][0] = m.m[2][0] * scale.x;
    pOut->m[0][1] = m.m[0][1] * scale.y;
    pOut->m[1][1] = m.m[1][1] * scale.y;
    pOut->m[2][1] = m.m[2][1] * scale.y;
    pOut->m[0][2] = m.m[0][2] * scale.z;
    pOut->m[1][2] = m.m[1][2] * scale.z;
    pOut->m[2][2] = m.m[2][2] * scale.z;
    pOut->m[0][3] = m.m[0][3];
    pOut->m[1][3] = m.m[1][3];
    pOut->m[2][3] = m.m[2][3];
}

// (inline) the parent's world matrix with the translation moved by the transform's translation in
// the space scaled by the parent's own scale (name is ours)
inline void MultScaledTranslate(nn::math::MTX34* pWorldMatrix, const CalculatedTransform& parentWorldTransform,
                                const nn::math::VEC3& parentScale, nn::math::VEC3* pTranslate)
{
    nn::math::ARMv6::MTX34CopyAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix);
    nn::math::MTX33 rotate;
    nn::math::ARMv6::MTX34ToMTX33Asm(&rotate, pWorldMatrix);
    rotate.m[0][0] *= parentScale.x;
    rotate.m[1][0] *= parentScale.x;
    rotate.m[2][0] *= parentScale.x;
    rotate.m[0][1] *= parentScale.y;
    rotate.m[1][1] *= parentScale.y;
    rotate.m[2][1] *= parentScale.y;
    rotate.m[0][2] *= parentScale.z;
    rotate.m[1][2] *= parentScale.z;
    rotate.m[2][2] *= parentScale.z;
    nn::math::ARMv6::VEC3TransformAsm(pTranslate, &rotate, pTranslate);
    pWorldMatrix->m[0][3] += pTranslate->x;
    pWorldMatrix->m[1][3] += pTranslate->y;
    pWorldMatrix->m[2][3] += pTranslate->z;
}

// (inline) the world scale: the parent's world scale times the own scale
inline void CalculateWorldScale(nn::math::VEC3* pWorldScale, const CalculatedTransform& transform, const CalculatedTransform& parentWorldTransform)
{
    if (!parentWorldTransform.IsEnabledFlags(CalculatedTransform::FLAG_IS_SCALE_ONE)) {
        pWorldScale->x = parentWorldTransform.m_Scale.x * transform.m_Scale.x;
        pWorldScale->y = parentWorldTransform.m_Scale.y * transform.m_Scale.y;
        pWorldScale->z = parentWorldTransform.m_Scale.z * transform.m_Scale.z;
    } else {
        *pWorldScale = transform.m_Scale;
    }
}
} // namespace

// 0x0049FAA8 slot 0x00 (name is ours)
// 0x0049FAA4 (deleting dtor)
nw::gfx::WorldMatrixUpdater::~WorldMatrixUpdater()
{
    // empty (in the original too)
}

// 0x0073A8AC | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::WorldMatrixUpdater::CalculateWorldXsi(nn::math::MTX34* pWorldMatrix, nn::math::VEC3* pWorldScale, const CalculatedTransform& transform,
                                                    const CalculatedTransform& parentWorldTransform,
                                                    const CalculatedTransform& /*parentTransform*/) const
{
    if (transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_IDENTITY | CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO)) {
        nn::math::ARMv6::MTX34CopyAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix);
    } else {
        nn::math::VEC3 translate = transform.GetTranslate();
        const bool isParentScaleOne = parentWorldTransform.IsEnabledFlags(CalculatedTransform::FLAG_IS_SCALE_ONE);
        const nn::math::VEC3& parentScale = parentWorldTransform.m_Scale;
        if (!transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_ROTATE_ZERO)) {
            if (isParentScaleOne) {
                nn::math::ARMv6::MTX34MultAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix, &transform.m_TransformMatrix);
            } else {
                nn::math::MTX34 local(transform.m_TransformMatrix);
                local.m[0][3] *= parentScale.x;
                local.m[1][3] *= parentScale.y;
                local.m[2][3] *= parentScale.z;
                nn::math::ARMv6::MTX34MultAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix, &local);
            }
        } else {
            if (!isParentScaleOne) {
                translate.x *= parentScale.x;
                translate.y *= parentScale.y;
                translate.z *= parentScale.z;
            }
            nn::math::ARMv6::MTX34MultTranslateAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix, &translate);
        }
    }
    CalculateWorldScale(pWorldScale, transform, parentWorldTransform);
}

// 0x0073AA44 | nintendogs:bytes [tier A]
void nw::gfx::WorldMatrixUpdater::CalculateWorldBasic(nn::math::MTX34* pWorldMatrix, nn::math::VEC3* pWorldScale, const CalculatedTransform& transform,
                                                      const CalculatedTransform& parentWorldTransform,
                                                      const CalculatedTransform& parentTransform) const
{
    const bool isParentScaleOne = parentTransform.IsEnabledFlags(CalculatedTransform::FLAG_IS_SCALE_ONE);
    if (transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_IDENTITY | CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO)) {
        if (isParentScaleOne) {
            nn::math::ARMv6::MTX34CopyAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix);
        } else {
            MultScale(pWorldMatrix, parentWorldTransform.m_TransformMatrix, parentTransform.m_Scale);
        }
    } else if (!transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_ROTATE_ZERO)) {
        nn::math::VEC3 translate = transform.GetTranslate();
        if (isParentScaleOne) {
            nn::math::ARMv6::MTX34MultTranslateAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix, &translate);
        } else {
            MultScale(pWorldMatrix, parentWorldTransform.m_TransformMatrix, parentTransform.m_Scale);
            nn::math::ARMv6::MTX34MultTranslateAsm(pWorldMatrix, pWorldMatrix, &translate);
        }
        nn::math::ARMv6::MTX33MultAsm(pWorldMatrix, pWorldMatrix, &transform.m_TransformMatrix);
    } else {
        nn::math::VEC3 translate = transform.GetTranslate();
        if (isParentScaleOne) {
            nn::math::ARMv6::MTX34MultTranslateAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix, &translate);
        } else {
            MultScale(pWorldMatrix, parentWorldTransform.m_TransformMatrix, parentTransform.m_Scale);
            nn::math::ARMv6::MTX34MultTranslateAsm(pWorldMatrix, pWorldMatrix, &translate);
        }
    }
    CalculateWorldScale(pWorldScale, transform, parentWorldTransform);
}

// 0x0073ADC0 | nintendogs:bytes [tier A]
void nw::gfx::WorldMatrixUpdater::CalculateWorldMayaSsc(nn::math::MTX34* pWorldMatrix, nn::math::VEC3* pWorldScale, const CalculatedTransform& transform,
                                                        const CalculatedTransform& parentWorldTransform,
                                                        const CalculatedTransform& parentTransform) const
{
    if (transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_IDENTITY | CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO)) {
        nn::math::ARMv6::MTX34CopyAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix);
    } else {
        nn::math::VEC3 translate = transform.GetTranslate();
        const bool isParentScaleOne = parentTransform.IsEnabledFlags(CalculatedTransform::FLAG_IS_SCALE_ONE);
        if (!transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_ROTATE_ZERO)) {
            if (isParentScaleOne) {
                nn::math::ARMv6::MTX34MultTranslateAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix, &translate);
            } else {
                MultScaledTranslate(pWorldMatrix, parentWorldTransform, parentTransform.m_Scale, &translate);
            }
            nn::math::ARMv6::MTX33MultAsm(pWorldMatrix, pWorldMatrix, &transform.m_TransformMatrix);
        } else {
            if (isParentScaleOne) {
                nn::math::ARMv6::MTX34MultTranslateAsm(pWorldMatrix, &parentWorldTransform.m_TransformMatrix, &translate);
            } else {
                MultScaledTranslate(pWorldMatrix, parentWorldTransform, parentTransform.m_Scale, &translate);
            }
        }
    }
    CalculateWorldScale(pWorldScale, transform, parentWorldTransform);
}

} // namespace gfx
} // namespace nw
