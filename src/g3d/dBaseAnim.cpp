#include "g3d/dBaseAnim.h"
#include "nw/gfx/gfx_BaseAnimEvaluator.h"

namespace g3d {
// 0x004F2CA8 (name is ours)
void g3d::BaseAnim::UpdateFrame()
{
    m_pAnimObject->UpdateFrame();
}

// 0x004F2CB8 slot 0x08 (name is ours)
nw::gfx::BaseAnimEvaluator* g3d::BaseAnim::GetEvaluator()
{
    return m_pAnimObject;
}

// 0x004F2CC0 (name is ours)
void g3d::BaseAnim::UnbindMemberAnim(s32 animIdx)
{
    m_pAnimObject->m_AnimMemberIndices[animIdx] = nw::gfx::BaseAnimEvaluator::NO_MEMBER_ANIM;
}

// 0x004F2CE0 slot 0x14 (name is ours)
void g3d::BaseAnim::Finalize()
{
    nw::gfx::SafeDestroy(m_pAnimObject);
    m_HeapAllocator.DestroyHeap();
}

// 0x004F2D2C slot 0x10 (name is ours)
void g3d::BaseAnim::ChangeAnim(nw::anim::res::ResAnim anim)
{
    nw::gfx::SafeDestroy(m_pAnimObject);
    CreateAnimObject(anim, 0);
    InitPlayMode(anim);
    Bind();
}

// 0x004F2DCC slot 0x00
// 0x004F2DAC (deleting dtor)
g3d::BaseAnim::~BaseAnim()
{
    // (only the members are destroyed)
}

} // namespace g3d
