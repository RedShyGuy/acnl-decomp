#include <new>

#include "nw/gfx/gfx_AnimBinding.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/res/gfx_ResLight.h"

namespace nw {
namespace gfx {
// 0x0097F474
const TypeInfo nw::gfx::Light::s_TypeInfo(&TransformNode::s_TypeInfo);

// sets s_TypeInfo
// 0x00788548 (static initializer)

// 0x004AA760 | nintendogs:bytes-fuzzy [tier A]
u32 nw::gfx::Light::CreateAnimGroup(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    if (m_AnimBinding.Get() == nullptr) {
        return result;
    }

    anim::res::ResAnimGroup resAnimGroup = GetResSceneObject().GetAnimGroup(0);
    void* pMemory = pAllocator->Alloc(sizeof(AnimGroup), 4);
    AnimGroup* pAnimGroup = nullptr;
    if (pMemory != nullptr) {
        pAnimGroup = new (pMemory) AnimGroup(resAnimGroup, this, pAllocator);
        if (pAnimGroup->Initialize(true) & RESULT_FAILURE_MASK) {
            pAnimGroup->~AnimGroup();
            pAnimGroup->GetAllocator()->Free(pAnimGroup);
            pAnimGroup = nullptr;
        }
    }
    if (pAnimGroup == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    // the light's own members: its transform, and its resource (the copy for the original values)
    m_pAnimGroup = pAnimGroup;
    const int memberCount = pAnimGroup->GetMemberCount();
    for (int i = 0; i < memberCount; ++i) {
        anim::res::ResAnimGroupMember member = m_pAnimGroup->GetResAnimGroupMember(i);
        void* pObject;
        switch (member.GetMemberType()) {
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_SCENE_NODE:
            pObject = this;
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_LIGHT:
            pObject = const_cast<res::ResLightData*>(res::DynamicCastData<res::ResLightData>(GetResSceneObject().ptr(), res::ResLightData::TYPE));
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_FRAGMENT_LIGHT:
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_HEMISPHERE_LIGHT:
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_AMBIENT_LIGHT:
            pObject = const_cast<res::ResSceneObjectData*>(GetResSceneObject().ptr());
            break;
        default:
            pObject = nullptr;
            break;
        }
        m_pAnimGroup->m_Targets[i] = pObject;
        if (member.GetMemberType() == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_SCENE_NODE && member.GetField() == 0) {
            m_pAnimGroup->m_TargetValues[i] = &m_Transform;
        } else {
            m_pAnimGroup->m_TargetValues[i] = static_cast<u8*>(pObject) + member.ptr()->valueOffset;
        }
        m_pAnimGroup->m_TargetObjects[i] = nullptr;
        if (member.GetMemberType() == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_SCENE_NODE) {
            m_pAnimGroup->m_OriginalValues[i] = &m_OriginalTransform;
        } else {
            m_pAnimGroup->m_OriginalValues[i] = static_cast<u8*>(m_pOriginalValue) + member.ptr()->valueOffset;
        }
    }
    m_AnimBinding->m_AnimGroups[0] = m_pAnimGroup;
    return result;
}

// 0x004AA9B0 | nintendogs:bytes [tier A]
void nw::gfx::Light::DestroyOriginalValue()
{
    GetAllocator()->Free(m_pOriginalValue);
    m_pOriginalValue = nullptr;
}

// 0x004AA9D8 slot 0x10 | nintendogs:bytes
void nw::gfx::Light::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitLight(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x004AAA38 slot 0x00 | nintendogs:bytes
// 0x004AAA2C (deleting dtor)
nw::gfx::Light::~Light()
{
    if (m_pAnimGroup != nullptr) {
        m_pAnimGroup->~AnimGroup();
        m_pAnimGroup->GetAllocator()->Free(m_pAnimGroup);
        m_pAnimGroup = nullptr;
    }
}

} // namespace gfx
} // namespace nw
