#pragma once

#include "decomp.h"
#include "g3d/dMorphTransformAnimEvaluator.h"
#include "nw/gfx/gfx_AnimObject.h"

namespace g3d {
// the animation object of up to four morphing transform evaluators: each member of the group takes its result from
// one of them (the evaluator of the member while it morphs into another evaluator is kept apart; names are ours)
// RTTI N3g3d18GroupTransformAnimE @ 0x008D0D24
// vtable 0x00903AE4 (vptr 0x00903AEC), offset_to_top 0, 9 entries
class GroupTransformAnim : public ::nw::gfx::AnimObject
{
public:
    // the evaluators of the object
    static const s32 EVALUATOR_COUNT = 4;
    // the bits of an entry of m_pMemberEvaluators: the evaluator, and whether the member morphs into it from
    // evaluator 0
    static const u8 EVALUATOR_MASK = 0x3;
    static const u8 MORPH_FLAG = 0x80;

    // (inline in GroupSkeletalAnim::Build) no member has an evaluator yet
    GroupTransformAnim(nw::os::IAllocator* pAllocator, s32 memberCount) : nw::gfx::AnimObject(pAllocator, ANIM_TYPE_MORPH)
    {
        m_pMemberEvaluators = static_cast<u8*>(GetAllocator()->Alloc(memberCount, 4));
        m_MemberCount = memberCount;
        m_EnabledEvaluators = 0;
        for (s32 j = 0; j < m_MemberCount; j++) {
            m_pMemberEvaluators[j] = 0;
        }
        for (s32 i = 0; i < EVALUATOR_COUNT; i++) {
            m_pEvaluators[i] = NULL;
        }
    }
    virtual ~GroupTransformAnim(); // 0x004F08F4 slot 0x00
    // 0x004F08E4 (deleting dtor)
    DECOMP_EMIT_COPY virtual const nw::gfx::TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00746F94 slot 0x08
    // (not bound to a group: the evaluators are)
    u32 TryBind(nw::gfx::AnimGroup* pAnimGroup) override; // 0x004F08BC slot 0x0C
    void Release() override; // 0x004F08B8 slot 0x10
    // the frames of the evaluators; an evaluator whose morph has ended gives its members back to evaluator 0
    void UpdateFrame() override; // 0x004F06DC slot 0x14
    DECOMP_EMIT_COPY virtual const nw::anim::AnimResult* GetResult(void* pTarget, int memberIdx) const // 0x00746FA0 slot 0x18
    {
        const u8 entry = m_pMemberEvaluators[memberIdx];
        const MorphTransformAnimEvaluator* evaluator = m_pEvaluators[entry & EVALUATOR_MASK];
        const f32 ratio = evaluator->m_CalcRatio.m_Ratio;
        if (entry & MORPH_FLAG) {
            evaluator = m_pEvaluators[0];
        }
        if (!evaluator->GetMorphResult(static_cast<nw::gfx::CalculatedTransform*>(pTarget), memberIdx, ratio)) {
            return NULL;
        }
        return static_cast<const nw::anim::AnimResult*>(pTarget);
    }
    DECOMP_EMIT_COPY virtual bool HasMemberAnim(int memberIdx) const // 0x00746F40 slot 0x1C
    {
        for (s32 i = 0; i < EVALUATOR_COUNT; i++) {
            if (m_pEvaluators[i] != NULL && m_pEvaluators[i]->HasMemberAnim(memberIdx)) {
                return true;
            }
        }
        return false;
    }
    void UpdateCache() override; // 0x004F06A4 slot 0x20

    // the members of evaluatorIdx start to morph into it (duration 0 or less: at once)
    void StartMorph(s32 evaluatorIdx, u8 type, f32 duration); // 0x004F07B8
    // whether the evaluator is used (evaluator 0 always is)
    bool IsEvaluatorEnabled(s32 evaluatorIdx) const; // 0x004F08C4

    // (inline in GroupSkeletalAnim; names are ours)
    void SetEvaluator(u8 evaluatorIdx, MorphTransformAnimEvaluator* evaluator) { m_pEvaluators[evaluatorIdx] = evaluator; }
    // the members morphing into the evaluator are given back to evaluator 0 and the evaluator is not used
    void EndMorph(s32 evaluatorIdx)
    {
        const u8 morphing = evaluatorIdx | MORPH_FLAG;
        for (s32 j = 0; j < m_MemberCount; j++) {
            if (m_pMemberEvaluators[j] == morphing) {
                m_pMemberEvaluators[j] = 0;
            }
        }
        m_EnabledEvaluators &= ~(1 << evaluatorIdx);
    }
    void EnableEvaluator(s32 evaluatorIdx)
    {
        if (evaluatorIdx != 0) {
            m_EnabledEvaluators |= 1 << evaluatorIdx;
        }
    }
    // frees the member table
    void Clear()
    {
        if (m_pMemberEvaluators != NULL) {
            GetAllocator()->Free(m_pMemberEvaluators);
        }
        m_pMemberEvaluators = NULL;
        m_MemberCount = 0;
        for (s32 i = 0; i < EVALUATOR_COUNT; i++) {
            m_pEvaluators[i] = NULL;
        }
    }

    static const nw::gfx::TypeInfo s_TypeInfo;

    MorphTransformAnimEvaluator* m_pEvaluators[EVALUATOR_COUNT];  // 0x10
    u8* m_pMemberEvaluators;                                     // 0x20 per member
    s32 m_MemberCount;                                           // 0x24
    u8 m_EnabledEvaluators;                                      // 0x28 bit per evaluator
};
ASSERT_SIZE(GroupTransformAnim, 0x2C);
} // namespace g3d
