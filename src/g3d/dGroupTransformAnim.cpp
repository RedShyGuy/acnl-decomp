#include "g3d/dGroupTransformAnim.h"

namespace g3d {
// 0x004F06A4 slot 0x20 (name is ours)
void g3d::GroupTransformAnim::UpdateCache()
{
    for (s32 i = 0; i < EVALUATOR_COUNT; i++) {
        if (m_pEvaluators[i] != NULL) {
            m_pEvaluators[i]->UpdateCache();
        }
    }
}

// 0x004F06DC slot 0x14 (name is ours)
void g3d::GroupTransformAnim::UpdateFrame()
{
    for (s32 i = 0; i < EVALUATOR_COUNT; i++) {
        if (m_pEvaluators[i] == NULL) {
            continue;
        }
        bool wasMorphing = m_pEvaluators[i]->m_CalcRatio.IsMorphing();
        m_pEvaluators[i]->UpdateFrame();
        if (i == 0) {
            continue;
        }
        if (!wasMorphing || m_pEvaluators[i]->m_CalcRatio.IsMorphing()) {
            continue;
        }
        EndMorph(i);
    }
}

// 0x004F07B8 (name is ours)
void g3d::GroupTransformAnim::StartMorph(s32 evaluatorIdx, u8 type, f32 duration)
{
    const u8 morphing = evaluatorIdx | MORPH_FLAG;
    for (s32 j = 0; j < m_MemberCount; j++) {
        if (m_pMemberEvaluators[j] == evaluatorIdx) {
            m_pMemberEvaluators[j] = morphing;
        }
    }
    if (!(duration > 0.0f)) {
        EndMorph(evaluatorIdx);
    }
    m_pEvaluators[evaluatorIdx]->m_CalcRatio.Start(type, duration);
}

// 0x004F08B8 slot 0x10 (name is ours)
void g3d::GroupTransformAnim::Release()
{
    // empty (in the original too)
}

// 0x004F08BC slot 0x0C (name is ours)
u32 g3d::GroupTransformAnim::TryBind(nw::gfx::AnimGroup* /*pAnimGroup*/)
{
    return nw::gfx::RESULT_FAILURE_MASK | 2;
}

// 0x004F08C4 (name is ours)
bool g3d::GroupTransformAnim::IsEvaluatorEnabled(s32 evaluatorIdx) const
{
    if (evaluatorIdx == 0) {
        return true;
    }
    return (m_EnabledEvaluators & (1 << evaluatorIdx)) != 0;
}

// 0x004F08F4 slot 0x00
// 0x004F08E4 (deleting dtor)
g3d::GroupTransformAnim::~GroupTransformAnim()
{
    // empty (in the original too)
}

} // namespace g3d
