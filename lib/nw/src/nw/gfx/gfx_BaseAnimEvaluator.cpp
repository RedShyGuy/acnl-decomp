#include "nw/anim/res/anim_ResAnimGroup.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_BaseAnimEvaluator.h"
#include "nw/gfx/res/gfx_ResMaterial.h"

namespace nw {
namespace gfx {
// 0x00976058
const TypeInfo nw::gfx::BaseAnimEvaluator::s_TypeInfo(&AnimObject::s_TypeInfo);

// sets s_TypeInfo
// 0x0079E834 (static initializer)

// 0x008A39A8
const int nw::gfx::BaseAnimEvaluator::NO_MEMBER_ANIM = -1;

// 0x0049CD9C slot 0x24 | slot vf_0x24 of nw::gfx::BaseAnimEvaluator
u32 nw::gfx::BaseAnimEvaluator::ChangeAnim(anim::res::ResAnim animData)
{
    SetAnimData(animData);
    AnimGroup* pAnimGroup = m_pAnimGroup;
    Release();
    return TryBind(pAnimGroup);
}

// 0x0049CE2C slot 0x14 | nintendogs:bytes
void nw::gfx::BaseAnimEvaluator::UpdateFrame()
{
    if (m_AnimFrameController.GetSpeed() != 0.0f) {
        m_AnimFrameController.UpdateFrame();
        m_IsCacheDirty = true;
    }
}

// 0x0049CE80 slot 0x30 (name is ours)
void nw::gfx::BaseAnimEvaluator::SetCacheBuffer(void* /*pBuffer*/, u32 /*size*/)
{
    // empty (in the original too)
}

// 0x0049CE84 slot 0x10 | slot vf_0x10 of nw::gfx::BaseAnimEvaluator
void nw::gfx::BaseAnimEvaluator::Release()
{
    m_pAnimGroup = nullptr;
}

// 0x0049CE90 slot 0x0C | nintendogs:callgraph
u32 nw::gfx::BaseAnimEvaluator::TryBind(AnimGroup* pAnimGroup)
{
    const s32 memberCount = pAnimGroup->GetMemberCount();
    m_BindIndices.Resize(memberCount);
    const s32 memberAnimCount = m_AnimData.GetMemberAnimCount();
    m_AnimMemberIndices.Resize(memberAnimCount);
    for (int i = 0; i < memberCount; ++i) {
        m_BindIndices[i] = NO_MEMBER_ANIM;
    }

    int bindCount = 0;
    for (int animIdx = 0; animIdx < memberAnimCount; ++animIdx) {
        anim::res::ResMemberAnim memberAnim = m_AnimData.GetMemberAnim(animIdx);
        const char* pPath = memberAnim.ptr()->path.To<char>();
        const int memberIdx = pAnimGroup->GetResAnimGroup().GetMemberDic().GetIndex(pPath);
        if (memberIdx == -1) {
            m_AnimMemberIndices[animIdx] = NO_MEMBER_ANIM;
            continue;
        }
        m_BindIndices[memberIdx] = animIdx;
        m_AnimMemberIndices[animIdx] = memberIdx;
        ++bindCount;

        // the material sends the animated value again
        anim::res::ResAnimGroupMember member = pAnimGroup->GetResAnimGroupMember(memberIdx);
        res::ResMaterialData* pMaterial = static_cast<res::ResMaterialData*>(member.GetObject());
        switch (member.GetMemberType()) {
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MATERIAL_COLOR:
            pMaterial->materialColorHash = 0;
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_SAMPLER:
            pMaterial->textureSamplersHash = 0;
            pMaterial->textureMappersHash = 0;
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_MAPPER:
            pMaterial->textureMappersHash = 0;
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_BLEND_OPERATION:
            pMaterial->fragmentOperationHash = 0;
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_COORDINATOR:
            pMaterial->textureCoordinatorsHash = 0;
            break;
        default:
            break;
        }
    }
    m_pAnimGroup = pAnimGroup;

    if (m_AnimData.GetMemberAnimCount() == bindCount) {
        return RESULT_SUCCESS;
    }
    return bindCount == 0 ? RESULT_BIND_NONE : RESULT_BIND_PARTIAL;
}

// 0x0049D324 slot 0x00 | nintendogs:bytes
// 0x0049D2A8 (deleting dtor)
nw::gfx::BaseAnimEvaluator::~BaseAnimEvaluator()
{
    // empty (in the original too: only the members)
}

} // namespace gfx
} // namespace nw
