#pragma once

#include "decomp.h"
#include "nn/math/math_Transform.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "nw/anim/res/anim_ResBakedTransformAnim.h"
#include "nw/anim/res/anim_ResTransformAnim.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_BaseAnimEvaluator.h"
#include "nw/gfx/gfx_CalculatedTransform.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx22TransformAnimEvaluatorE @ 0x008D0724
// vtable 0x00902800 (vptr 0x00902808), offset_to_top 0, 13 entries
// Evaluates a skeletal animation: the members are CalculatedTransforms (the bones); a bone without a
// member animation takes its original transform. The cache keeps one CalculatedTransform per member
// animation. The parts can be switched off (the transforms then say the part has no value). The
// function names are from the reference symbols, the rest is ours.
class TransformAnimEvaluator : public BaseAnimEvaluator
{
public:
    explicit TransformAnimEvaluator(os::IAllocator* pAllocator);
    virtual ~TransformAnimEvaluator();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C190 slot 0x08
    virtual u32 TryBind(AnimGroup* pAnimGroup);
    // the complete transform (the parts that are not animated from the original transform)
    virtual const anim::AnimResult* GetResult(void* pTarget, int memberIdx) const // 0x0073C474 slot 0x18
    {
        return GetResultCommon(pTarget, memberIdx, true);
    }
    virtual bool HasMemberAnim(int memberIdx) const // 0x0073B9F0 slot 0x1C | nintendogs:bytes
    {
        if (m_AnimData.IsValid()) {
            return GetMemberAnimIndex(memberIdx) != NO_MEMBER_ANIM;
        }
        // without an animation every bone takes its original transform
        return memberIdx >= 0 && memberIdx < m_pAnimGroup->GetMemberCount();
    }
    virtual void UpdateCache();
    virtual u32 ChangeAnim(anim::res::ResAnim animData);
    virtual void* GetCacheBuffer() const { return const_cast<CalculatedTransform*>(m_CacheTransforms.begin()); } // 0x0073BA44 slot 0x28 (name is ours)
    virtual u32 GetCacheSize() const { return m_AnimData.GetMemberAnimCount() * sizeof(CalculatedTransform); } // 0x0073C464 slot 0x2C (name is ours)
    virtual void SetCacheBuffer(void* pBuffer, u32 size);

    // the animated parts written into the transform at pTarget, the others left as they are
    DECOMP_EMIT_COPY const anim::AnimResult* GetResultDirect(void* pTarget, int memberIdx) const // 0x0073BA4C (name is ours)
    {
        return GetResultCommon(pTarget, memberIdx, false);
    }

    // the arrays for up to maxMembers group members and maxMemberAnims member animations, and the
    // cache if allocateCache
    u32 Initialize(const anim::res::ResAnim& animData, int maxMembers, int maxMemberAnims, bool allocateCache);
    // the flags of the switched-off parts and the trivial parts
    void UpdateFlags(CalculatedTransform* pTransform) const;
    // the parts of the bound bones that the animation does not animate: from the original transform
    void ResetNoAnimMember(AnimGroup* pAnimGroup, anim::res::ResAnim animData);
    const anim::AnimResult* GetResultCommon(void* pTarget, int memberIdx, bool isComplete) const;
    void EvaluateMemberAnim(CalculatedTransform* pTransform, anim::res::ResTransformAnim transformAnim, f32 frame, const nn::math::Transform3* pOriginal, bool isComplete) const;
    void EvaluateMemberBakedAnim(CalculatedTransform* pTransform, anim::res::ResBakedTransformAnim transformAnim, f32 frame, const nn::math::Transform3* pOriginal, bool isComplete) const;

    // (inline) the parts switched off, not in the form for blending
    void SetDisabledFlags(CalculatedTransform* pTransform) const
    {
        pTransform->DisableFlags(CalculatedTransform::FLAG_IS_BLEND_FORM);
        pTransform->SetFlags(CalculatedTransform::FLAG_SCALE_UNSET, m_IsScaleDisabled);
        pTransform->SetFlags(CalculatedTransform::FLAG_ROTATE_UNSET, m_IsRotateDisabled);
        pTransform->SetFlags(CalculatedTransform::FLAG_TRANSLATE_UNSET, m_IsTranslateDisabled);
    }

    static const TypeInfo s_TypeInfo;

    bool m_IsScaleDisabled;                              // 0x5B
    bool m_IsRotateDisabled;                             // 0x5C
    bool m_IsTranslateDisabled;                          // 0x5D
    ut::MoveArray<CalculatedTransform> m_CacheTransforms; // 0x60 per member animation
};
ASSERT_OFFSET(TransformAnimEvaluator, m_IsScaleDisabled, 0x5B);
ASSERT_SIZE(TransformAnimEvaluator, 0x70);
} // namespace gfx
} // namespace nw
