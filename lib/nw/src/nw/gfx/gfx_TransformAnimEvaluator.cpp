#include "nw/gfx/gfx_TransformAnimEvaluator.h"

#include "nw/anim/res/anim_ResAnimCurve.h"

namespace nw {
namespace gfx {
namespace {
// the components of a ResTransformAnimData (inline, names are ours)
inline bool IsComponentNotExist(u32 flags, int index)
{
    return (flags & (1 << (anim::res::ResTransformAnimData::NOT_EXIST_SHIFT + index))) != 0;
}
inline f32 EvaluateComponent(const anim::res::ResTransformAnimData* pData, int index, f32 frame)
{
    if (pData->flags & (1 << (anim::res::ResTransformAnimData::CONSTANT_SHIFT + index))) {
        return pData->components[index].constantValue;
    }
    return anim::res::CalcFloatCurve(pData->components[index].curve.To<anim::res::ResFloatCurveData>(), frame);
}

// the not-exist flags of the parts of a ResTransformAnimData
const u32 NOT_EXIST_SCALE = 0x7 << (anim::res::ResTransformAnimData::NOT_EXIST_SHIFT + anim::res::ResTransformAnimData::COMPONENT_SCALE_X);
const u32 NOT_EXIST_ROTATE = 0x7 << (anim::res::ResTransformAnimData::NOT_EXIST_SHIFT + anim::res::ResTransformAnimData::COMPONENT_ROTATE_X);
const u32 NOT_EXIST_TRANSLATE = 0x7 << (anim::res::ResTransformAnimData::NOT_EXIST_SHIFT + anim::res::ResTransformAnimData::COMPONENT_TRANSLATE_X);

// the kinds of transform animation (ResMemberAnimData::primitiveType; names are ours)
const u32 PRIMITIVE_BAKED_TRANSFORM = 8;
const u32 PRIMITIVE_FULL_BAKED_TRANSFORM = 9;

// (inline) whether a vector is (0, 0, 0)
inline bool IsZero(const nn::math::VEC3& v)
{
    return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
}
} // namespace

// 0x009760F4
const TypeInfo nw::gfx::TransformAnimEvaluator::s_TypeInfo(&BaseAnimEvaluator::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FB28 (static initializer)

// 0x004A0058 | nintendogs:callgraph [tier A]
void nw::gfx::TransformAnimEvaluator::UpdateFlags(CalculatedTransform* pTransform) const
{
    SetDisabledFlags(pTransform);
    pTransform->UpdateScaleFlags();
    pTransform->UpdateRotateFlags();
    pTransform->UpdateTranslateFlags();
    pTransform->UpdateCompositeFlags();
}

// 0x004A11FC slot 0x24 | mk7dlp:bytes
u32 nw::gfx::TransformAnimEvaluator::ChangeAnim(anim::res::ResAnim animData)
{
    if (!m_IsCacheExternal && !m_CacheTransforms.empty()) {
        m_CacheTransforms.Resize(animData.GetMemberAnimCount());
    }
    return BaseAnimEvaluator::ChangeAnim(animData);
}

// 0x004A12B4 (name is ours)
u32 nw::gfx::TransformAnimEvaluator::Initialize(const anim::res::ResAnim& animData, int maxMembers, int maxMemberAnims, bool allocateCache)
{
    u32 result = RESULT_SUCCESS;
    SetAnimData(animData);

    void* pBuffer = GetAllocator()->Alloc(maxMembers * sizeof(int), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (!(result & RESULT_FAILURE_MASK)) {
        {
            ut::MoveArray<int> bindIndices(pBuffer, maxMembers, GetAllocator());
            m_BindIndices = bindIndices;
        }
        pBuffer = GetAllocator()->Alloc(maxMemberAnims * sizeof(int), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (!(result & RESULT_FAILURE_MASK)) {
            ut::MoveArray<int> animMemberIndices(pBuffer, maxMemberAnims, GetAllocator());
            m_AnimMemberIndices = animMemberIndices;
        }
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    if (allocateCache) {
        pBuffer = GetAllocator()->Alloc(maxMemberAnims * sizeof(CalculatedTransform), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        {
            ut::MoveArray<CalculatedTransform> cacheTransforms(pBuffer, maxMemberAnims, GetAllocator());
            m_CacheTransforms = cacheTransforms;
        }
        m_CacheTransforms.Resize(animData.GetMemberAnimCount());
    }
    return result;
}

// 0x004A15D8 slot 0x20 | nintendogs:bytes
void nw::gfx::TransformAnimEvaluator::UpdateCache()
{
    if (m_CacheTransforms.empty() || !m_IsCacheDirty) {
        return;
    }
    if (m_AnimData.IsValid()) {
        for (int memberIdx = 0; memberIdx < m_pAnimGroup->GetMemberCount(); ++memberIdx) {
            const int animIdx = GetMemberAnimIndex(memberIdx);
            if (animIdx != NO_MEMBER_ANIM) {
                GetResult(&m_CacheTransforms[animIdx], memberIdx);
            }
        }
    }
    m_IsCacheDirty = false;
}

// 0x004A1664 slot 0x30 (name is ours)
void nw::gfx::TransformAnimEvaluator::SetCacheBuffer(void* pBuffer, u32 size)
{
    if (pBuffer != nullptr) {
        const s32 count = size / sizeof(CalculatedTransform);
        {
            ut::MoveArray<CalculatedTransform> cacheTransforms(pBuffer, count);
            m_CacheTransforms = cacheTransforms;
        }
        m_CacheTransforms.Resize(count);
        m_IsCacheDirty = true;
        m_IsCacheExternal = true;
    } else {
        ut::MoveArray<CalculatedTransform> cacheTransforms;
        m_CacheTransforms = cacheTransforms;
    }
}

// 0x004A1814 | nintendogs:callgraph [tier A]
void nw::gfx::TransformAnimEvaluator::ResetNoAnimMember(AnimGroup* pAnimGroup, anim::res::ResAnim animData)
{
    for (int memberIdx = 0; memberIdx < pAnimGroup->GetMemberCount(); ++memberIdx) {
        const int animIdx = GetMemberAnimIndex(memberIdx);
        if (animIdx == NO_MEMBER_ANIM) {
            continue;
        }
        anim::res::ResMemberAnim memberAnim = animData.GetMemberAnim(animIdx);
        const u32 type = memberAnim.GetPrimitiveType();
        if (type != anim::res::PRIMITIVE_TRANSFORM && type != PRIMITIVE_BAKED_TRANSFORM) {
            continue;
        }
        const nn::math::Transform3* pOriginal = static_cast<const nn::math::Transform3*>(pAnimGroup->m_OriginalValues[memberIdx]);
        CalculatedTransform* pTransform = static_cast<CalculatedTransform*>(pAnimGroup->m_TargetValues[memberIdx]);
        const u32 flags = memberAnim.ptr()->flags;
        u32 notExistRotateTranslate;
        u32 notExistScale;
        if (type == anim::res::PRIMITIVE_TRANSFORM) {
            notExistRotateTranslate = NOT_EXIST_ROTATE | NOT_EXIST_TRANSLATE;
            notExistScale = NOT_EXIST_SCALE;
        } else {
            notExistRotateTranslate = anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_TRANSLATE | anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_ROTATE;
            notExistScale = anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_SCALE;
        }

        bool isReset = false;
        if (flags & notExistRotateTranslate) {
            pTransform->SetRotateAndTranslate(pOriginal->rotate, pOriginal->translate);
            isReset = true;
        }
        if (flags & notExistScale) {
            pTransform->SetScale(pOriginal->scale);
            pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
        } else if (!isReset) {
            continue;
        }
        UpdateFlags(pTransform);
        if (!m_CacheTransforms.empty()) {
            m_CacheTransforms[animIdx] = *pTransform;
        }
    }
}

// 0x004A1A28 slot 0x0C | nintendogs:bytes
u32 nw::gfx::TransformAnimEvaluator::TryBind(AnimGroup* pAnimGroup)
{
    const u32 result = BaseAnimEvaluator::TryBind(pAnimGroup);
    if (static_cast<s32>(result & RESULT_FAILURE_MASK) < 0) {
        return result;
    }
    ResetNoAnimMember(pAnimGroup, m_AnimData);
    m_pAnimGroup = pAnimGroup;
    return result;
}

// 0x004A1A68 (name is ours)
nw::gfx::TransformAnimEvaluator::TransformAnimEvaluator(os::IAllocator* pAllocator)
    : BaseAnimEvaluator(pAllocator, ANIM_TYPE_TRANSFORM_EVALUATOR), m_IsScaleDisabled(false), m_IsRotateDisabled(false), m_IsTranslateDisabled(false)
{
}

// 0x004A1C2C slot 0x00 | slot vf_0x00 of nw::gfx::BaseAnimEvaluator
// 0x004A1B40 (deleting dtor)
nw::gfx::TransformAnimEvaluator::~TransformAnimEvaluator()
{
    // empty (in the original too: only the members)
}

// 0x0073BA54 | nintendogs:bytes [tier A]
const anim::AnimResult* nw::gfx::TransformAnimEvaluator::GetResultCommon(void* pTarget, int memberIdx, bool isComplete) const
{
    if (!HasMemberAnim(memberIdx)) {
        return nullptr;
    }
    CalculatedTransform* pTransform = static_cast<CalculatedTransform*>(pTarget);
    if (!m_CacheTransforms.empty() && !m_IsCacheDirty && m_AnimData.IsValid()) {
        *pTransform = m_CacheTransforms[GetMemberAnimIndex(memberIdx)];
    } else {
        const nn::math::Transform3* pOriginal = static_cast<const nn::math::Transform3*>(m_pAnimGroup->m_OriginalValues[memberIdx]);
        if (m_AnimData.IsValid()) {
            anim::res::ResMemberAnim memberAnim = m_AnimData.GetMemberAnim(GetMemberAnimIndex(memberIdx));
            switch (memberAnim.GetPrimitiveType()) {
            case anim::res::PRIMITIVE_TRANSFORM:
                EvaluateMemberAnim(pTransform, anim::res::ResTransformAnim(memberAnim.ptr()), GetFrame(), pOriginal, isComplete);
                break;
            case PRIMITIVE_BAKED_TRANSFORM:
                EvaluateMemberBakedAnim(pTransform, anim::res::ResBakedTransformAnim(memberAnim.ptr()), GetFrame(), pOriginal, isComplete);
                break;
            case PRIMITIVE_FULL_BAKED_TRANSFORM:
                anim::res::CalcTransformCurve(&pTransform->m_TransformMatrix, memberAnim.ptr()->curve.To<anim::res::ResFullBakedCurveData>(), GetFrame());
                break;
            default:
                break;
            }
        } else {
            pTransform->SetScale(pOriginal->scale);
            pTransform->SetRotateAndTranslate(pOriginal->rotate, pOriginal->translate);
            UpdateFlags(pTransform);
        }
    }
    return reinterpret_cast<const anim::AnimResult*>(pTransform);
}

// 0x0073BBE4 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::TransformAnimEvaluator::EvaluateMemberAnim(CalculatedTransform* pTransform, anim::res::ResTransformAnim transformAnim, f32 frame,
                                                         const nn::math::Transform3* pOriginal, bool isComplete) const
{
    const anim::res::ResTransformAnimData* pData = transformAnim.ptr();
    const u32 flags = pData->flags;
    if (isComplete) {
        // a new transform: the parts that are not animated from the original one
        nn::math::Transform3 transform;
        if (m_IsScaleDisabled) {
            transform.scale = pOriginal->scale;
        } else {
            transform.scale.x = IsComponentNotExist(flags, 0) ? pOriginal->scale.x : EvaluateComponent(pData, 0, frame);
            transform.scale.y = IsComponentNotExist(flags, 1) ? pOriginal->scale.y : EvaluateComponent(pData, 1, frame);
            transform.scale.z = IsComponentNotExist(flags, 2) ? pOriginal->scale.z : EvaluateComponent(pData, 2, frame);
        }
        if (m_IsRotateDisabled) {
            transform.rotate = pOriginal->rotate;
        } else {
            transform.rotate.x = IsComponentNotExist(flags, 3) ? pOriginal->rotate.x : EvaluateComponent(pData, 3, frame);
            transform.rotate.y = IsComponentNotExist(flags, 4) ? pOriginal->rotate.y : EvaluateComponent(pData, 4, frame);
            transform.rotate.z = IsComponentNotExist(flags, 5) ? pOriginal->rotate.z : EvaluateComponent(pData, 5, frame);
        }
        if (m_IsTranslateDisabled) {
            transform.translate = pOriginal->translate;
        } else {
            transform.translate.x = IsComponentNotExist(flags, 7) ? pOriginal->translate.x : EvaluateComponent(pData, 7, frame);
            transform.translate.y = IsComponentNotExist(flags, 8) ? pOriginal->translate.y : EvaluateComponent(pData, 8, frame);
            transform.translate.z = IsComponentNotExist(flags, 9) ? pOriginal->translate.z : EvaluateComponent(pData, 9, frame);
        }
        pTransform->SetScale(transform.scale);
        pTransform->SetRotateAndTranslate(transform.rotate, transform.translate);
    } else {
        // only the animated components change
        if (!m_IsScaleDisabled) {
            if (!IsComponentNotExist(flags, 0)) {
                pTransform->m_Scale.x = EvaluateComponent(pData, 0, frame);
            }
            if (!IsComponentNotExist(flags, 1)) {
                pTransform->m_Scale.y = EvaluateComponent(pData, 1, frame);
            }
            if (!IsComponentNotExist(flags, 2)) {
                pTransform->m_Scale.z = EvaluateComponent(pData, 2, frame);
            }
        }
        if (!m_IsRotateDisabled && (flags & NOT_EXIST_ROTATE) != NOT_EXIST_ROTATE) {
            const f32 x = IsComponentNotExist(flags, 3) ? pOriginal->rotate.x : EvaluateComponent(pData, 3, frame);
            const f32 y = IsComponentNotExist(flags, 4) ? pOriginal->rotate.y : EvaluateComponent(pData, 4, frame);
            const f32 z = IsComponentNotExist(flags, 5) ? pOriginal->rotate.z : EvaluateComponent(pData, 5, frame);
            pTransform->SetRotateXYZ(x, y, z);
        }
        if (!m_IsTranslateDisabled) {
            if (!IsComponentNotExist(flags, 7)) {
                pTransform->m_TransformMatrix.m[0][3] = EvaluateComponent(pData, 7, frame);
            }
            if (!IsComponentNotExist(flags, 8)) {
                pTransform->m_TransformMatrix.m[1][3] = EvaluateComponent(pData, 8, frame);
            }
            if (!IsComponentNotExist(flags, 9)) {
                pTransform->m_TransformMatrix.m[2][3] = EvaluateComponent(pData, 9, frame);
            }
        }
        pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
    }

    SetDisabledFlags(pTransform);
    if ((flags & NOT_EXIST_ROTATE) != NOT_EXIST_ROTATE) {
        pTransform->DisableFlags(CalculatedTransform::FLAG_IS_ROTATE_ZERO);
    }
    if ((flags & NOT_EXIST_TRANSLATE) != NOT_EXIST_TRANSLATE) {
        pTransform->DisableFlags(CalculatedTransform::FLAG_IS_TRANSLATE_ZERO);
    }
    if ((flags & NOT_EXIST_SCALE) != NOT_EXIST_SCALE) {
        pTransform->DisableFlags(CalculatedTransform::FLAG_IS_SCALE_ONE);
    }
    pTransform->UpdateCompositeFlags();
}

// 0x0073C19C | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::TransformAnimEvaluator::EvaluateMemberBakedAnim(CalculatedTransform* pTransform, anim::res::ResBakedTransformAnim transformAnim, f32 frame,
                                                              const nn::math::Transform3* pOriginal, bool isComplete) const
{
    const anim::res::ResBakedTransformAnimData* pData = transformAnim.ptr();
    const u32 flags = pData->flags;
    // the trivial parts; the curves clear and set them
    u32 bakedFlags = 0;
    if (!isComplete) {
        bakedFlags = pTransform->m_Flags;
    }

    if (!(flags & anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_ROTATE)) {
        bakedFlags &= ~CalculatedTransform::FLAG_IS_ROTATE_ZERO;
        anim::res::CalcRotateCurve(&pTransform->m_TransformMatrix, &bakedFlags,
                                   pData->rotateCurve.To<anim::res::ResBakedCurveData<nn::math::QUAT> >(),
                                   (pData->flags & anim::res::ResBakedTransformAnimData::FLAG_CONSTANT_ROTATE) ? 0.0f : frame);
        pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
    }
    if (!(flags & anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_TRANSLATE)) {
        bakedFlags &= ~CalculatedTransform::FLAG_IS_TRANSLATE_ZERO;
        anim::res::CalcTranslateCurve(&pTransform->m_TransformMatrix, &bakedFlags,
                                      pData->translateCurve.To<anim::res::ResBakedCurveData<nn::math::VEC3_> >(),
                                      (pData->flags & anim::res::ResBakedTransformAnimData::FLAG_CONSTANT_TRANSLATE) ? 0.0f : frame);
        pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
    }
    if (!(flags & anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_SCALE)) {
        bakedFlags &= ~(CalculatedTransform::FLAG_IS_SCALE_ONE | CalculatedTransform::FLAG_IS_UNIFORM_SCALE);
        anim::res::CalcVector3Curve(&pTransform->m_Scale, &bakedFlags, pData->scaleCurve.To<anim::res::ResBakedCurveData<nn::math::VEC3_> >(),
                                    (pData->flags & anim::res::ResBakedTransformAnimData::FLAG_CONSTANT_SCALE) ? 0.0f : frame);
        pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
    }

    if (isComplete) {
        // the parts that are not animated from the original transform
        if (flags & anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_ROTATE) {
            pTransform->SetRotateXYZ(pOriginal->rotate.x, pOriginal->rotate.y, pOriginal->rotate.z);
            if (IsZero(pOriginal->rotate)) {
                bakedFlags |= CalculatedTransform::FLAG_IS_ROTATE_ZERO;
            }
        }
        if (flags & anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_TRANSLATE) {
            pTransform->SetTranslate(pOriginal->translate);
            pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
            if (IsZero(pOriginal->translate)) {
                bakedFlags |= CalculatedTransform::FLAG_IS_TRANSLATE_ZERO;
            }
        }
        if (flags & anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_SCALE) {
            pTransform->SetScale(pOriginal->scale);
            pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY);
            if (pOriginal->scale.x == pOriginal->scale.y && pOriginal->scale.x == pOriginal->scale.z) {
                bakedFlags |= CalculatedTransform::FLAG_IS_UNIFORM_SCALE;
                if (pOriginal->scale.x == 1.0f) {
                    bakedFlags |= CalculatedTransform::FLAG_IS_SCALE_ONE;
                }
            }
        }
    }
    SetDisabledFlags(pTransform);
    anim::res::ResBakedTransformAnim::ApplyBakedFlags(pTransform, bakedFlags);
}

} // namespace gfx

namespace ut {
// (ARMCC calls it out of line, GCC inlines it)
// 0x007E5ED0 | mk7dlp:callgraph
template bool MoveArray<nw::gfx::CalculatedTransform>::Resize(int count);
} // namespace ut
} // namespace nw
