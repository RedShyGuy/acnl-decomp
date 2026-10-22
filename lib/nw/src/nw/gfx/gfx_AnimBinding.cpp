#include "nw/gfx/gfx_AnimBinding.h"

#include "nw/gfx/gfx_AnimEvaluator.h"
#include "nw/gfx/gfx_AnimOverrider.h"
#include "nw/gfx/gfx_TransformAnimEvaluator.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the value of a member from an animation object, written to the member (texture mappers
// take the texture through a local first)
DECOMP_ALWAYS_INLINE void EvaluateMember(AnimGroup* pAnimGroup, const AnimObject* pAnimObject, int memberIdx)
{
    anim::res::ResAnimGroupMember member = pAnimGroup->GetResAnimGroupMember(memberIdx);
    void* texture;
    void* pTarget;
    if (member.GetMemberType() == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_MAPPER && member.GetField() == 0) {
        pTarget = &texture;
    } else {
        pTarget = pAnimGroup->m_TargetValues[memberIdx];
    }
    const anim::AnimBlendOp* pBlendOp = pAnimGroup->GetBlendOperation(memberIdx);
    const anim::AnimResult* pResult;
    if (pBlendOp != nullptr) {
        anim::AnimResult result;
        result.m_EnabledFlags = 0;
        pResult = pAnimObject->GetResult(&result, memberIdx);
        if (pResult != nullptr) {
            pBlendOp->Apply(pTarget, pResult);
        }
    } else {
        pResult = pAnimObject->GetResult(pTarget, memberIdx);
    }
    if (!(member.GetMemberType() == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_SCENE_NODE && member.GetField() == 0) && pResult != nullptr) {
        member.SetValueForType(pAnimGroup->m_Targets[memberIdx], pTarget);
    }
}

// (inline) the members bound to an evaluator; the enable test is done once per target object
DECOMP_ALWAYS_INLINE void EvaluateAnimEvaluator(AnimGroup* pAnimGroup, AnimEvaluator* pEvaluator)
{
    void* pLastObject = reinterpret_cast<void*>(-1);
    bool isDisabled = false;
    const s32 memberAnimCount = pEvaluator->GetAnimData().GetMemberAnimCount();
    for (int animIdx = 0; animIdx < memberAnimCount; ++animIdx) {
        const int memberIdx = pEvaluator->m_AnimMemberIndices[animIdx];
        if (memberIdx == BaseAnimEvaluator::NO_MEMBER_ANIM) {
            continue;
        }
        if (pAnimGroup->m_pIsTargetEnabled != nullptr) {
            void* pObject = pAnimGroup->m_TargetObjects[memberIdx];
            if (pObject != pLastObject) {
                isDisabled = !pAnimGroup->m_pIsTargetEnabled(pAnimGroup, pObject);
                pLastObject = pObject;
            }
            if (isDisabled) {
                continue;
            }
        }
        EvaluateMember(pAnimGroup, pEvaluator, memberIdx);
    }
}
} // namespace

// 0x0048BB64 (name is ours)
void nw::gfx::AnimBinding::Evaluate(u8 evaluationTiming)
{
    for (int groupIdx = 0; groupIdx < m_AnimGroups.size(); ++groupIdx) {
        AnimGroup* pAnimGroup = m_AnimGroups[groupIdx];
        if (pAnimGroup == nullptr || pAnimGroup->GetResAnimGroup().GetEvaluationTiming() != evaluationTiming) {
            continue;
        }
        for (int objectIdx = 0; objectIdx < m_AnimObjectCountPerGroup; ++objectIdx) {
            AnimObject* pAnimObject = GetAnimObject(groupIdx, objectIdx);
            if (pAnimObject == nullptr) {
                continue;
            }
            switch (pAnimObject->m_AnimType) {
            case AnimObject::ANIM_TYPE_EVALUATOR: {
                AnimEvaluator* pEvaluator = static_cast<AnimEvaluator*>(pAnimObject);
                pEvaluator->UpdateCacheNonVirtual();
                EvaluateAnimEvaluator(pAnimGroup, pEvaluator);
                break;
            }
            case AnimObject::ANIM_TYPE_TRANSFORM_EVALUATOR: {
                TransformAnimEvaluator* pEvaluator = static_cast<TransformAnimEvaluator*>(pAnimObject);
                pEvaluator->TransformAnimEvaluator::UpdateCache();
                const s32 memberAnimCount = pEvaluator->GetAnimData().GetMemberAnimCount();
                for (int animIdx = 0; animIdx < memberAnimCount; ++animIdx) {
                    const int memberIdx = pEvaluator->m_AnimMemberIndices[animIdx];
                    if (memberIdx == BaseAnimEvaluator::NO_MEMBER_ANIM) {
                        continue;
                    }
                    if (pAnimGroup->m_pIsTargetEnabled == nullptr ||
                        pAnimGroup->m_pIsTargetEnabled(pAnimGroup, pAnimGroup->m_TargetObjects[memberIdx])) {
                        pEvaluator->GetResultDirect(pAnimGroup->m_TargetValues[memberIdx], memberIdx);
                    }
                }
                break;
            }
            default: {
                pAnimObject->UpdateCache();
                if (pAnimGroup->GetResAnimGroup().GetFlags() & anim::res::ResAnimGroupData::FLAG_DIRECT_RESULT) {
                    const s32 memberCount = pAnimGroup->GetMemberCount();
                    for (int memberIdx = 0; memberIdx < memberCount; ++memberIdx) {
                        if (pAnimGroup->m_pIsTargetEnabled == nullptr ||
                            pAnimGroup->m_pIsTargetEnabled(pAnimGroup, pAnimGroup->m_TargetObjects[memberIdx])) {
                            pAnimObject->GetResult(pAnimGroup->m_TargetValues[memberIdx], memberIdx);
                        }
                    }
                    break;
                }
                // an overrider: the evaluators in it one after the other
                AnimOverrider* pOverrider = DynamicCast<AnimOverrider>(pAnimObject);
                if (pOverrider != nullptr) {
                    const s32 count = pOverrider->m_AnimObjects.size();
                    for (int i = 0; i < count; ++i) {
                        AnimObject* pChild = pOverrider->m_AnimObjects[i];
                        if (pChild != nullptr && pChild->GetRuntimeTypeInfo() == &AnimEvaluator::s_TypeInfo) {
                            EvaluateAnimEvaluator(pAnimGroup, static_cast<AnimEvaluator*>(pChild));
                        }
                    }
                    break;
                }
                void* pLastObject = reinterpret_cast<void*>(-1);
                bool isDisabled = false;
                const s32 memberCount = pAnimGroup->GetMemberCount();
                for (int memberIdx = 0; memberIdx < memberCount; ++memberIdx) {
                    if (pAnimGroup->m_pIsTargetEnabled != nullptr) {
                        void* pObject = pAnimGroup->m_TargetObjects[memberIdx];
                        if (pObject != pLastObject) {
                            isDisabled = !pAnimGroup->m_pIsTargetEnabled(pAnimGroup, pObject);
                            pLastObject = pObject;
                        }
                        if (isDisabled) {
                            continue;
                        }
                    }
                    EvaluateMember(pAnimGroup, pAnimObject, memberIdx);
                }
                break;
            }
            }
        }
    }
}

// 0x0048C42C slot 0x00 | nintendogs:bytes
// 0x0048C3BC (deleting dtor)
nw::gfx::AnimBinding::~AnimBinding()
{
    // empty (in the original too: only the arrays)
}

} // namespace gfx
} // namespace nw
