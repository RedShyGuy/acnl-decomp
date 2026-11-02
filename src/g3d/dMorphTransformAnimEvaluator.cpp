#include "g3d/dMorphTransformAnimEvaluator.h"
#include "g3d/dGroupTransformAnim.h"

namespace g3d {
// 0x00953A80
const nw::gfx::TypeInfo MorphTransformAnimEvaluator::s_TypeInfo(&nw::gfx::TransformAnimEvaluator::s_TypeInfo);
// 0x00953A84
const nw::gfx::TypeInfo GroupTransformAnim::s_TypeInfo(&nw::gfx::AnimObject::s_TypeInfo);

// sets both type infos
// 0x00786218 (static initializer)

// 0x004F0F48 slot 0x14 (name is ours)
void g3d::MorphTransformAnimEvaluator::UpdateFrame()
{
    if (m_AnimFrameController.m_Speed != 0.0f) {
        m_AnimFrameController.UpdateFrame();
        m_IsCacheDirty = true;
    }
    if (m_CalcRatio.IsMorphing()) {
        m_CalcRatio.Update();
        m_IsCacheDirty = true;
    }
}

// 0x004F0FF4 slot 0x0C (name is ours)
u32 g3d::MorphTransformAnimEvaluator::TryBind(nw::gfx::AnimGroup* pAnimGroup)
{
    u32 result = nw::gfx::BaseAnimEvaluator::TryBind(pAnimGroup);
    if (!(result & nw::gfx::RESULT_FAILURE_MASK)) {
        m_pAnimGroup = pAnimGroup;
    }
    return result;
}

// 0x004F1018 slot 0x00
// 0x004F1014 (deleting dtor)
g3d::MorphTransformAnimEvaluator::~MorphTransformAnimEvaluator()
{
    // (only the members and the base are destroyed)
}

} // namespace g3d
