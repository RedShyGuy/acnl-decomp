#include "nw/gfx/gfx_SceneNode.h"

namespace nw {
namespace gfx {
// 0x009760E4
const TypeInfo nw::gfx::SceneNode::s_TypeInfo(&SceneObject::s_TypeInfo);

// sets s_TypeInfo
// 0x00797ADC (static initializer)

// 0x004B39D0 slot 0x24 | nintendogs:bytes
u32 nw::gfx::SceneNode::Initialize(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    result |= CreateChildren(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    m_Children.m_pOwner = this;
    result |= CreateCallbacks(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateAnimBinding(pAllocator);
    return result;
}

// 0x004B3A28 | nintendogs:callgraph [tier A]
bool nw::gfx::SceneNode::AttachChild(SceneNode* pChild)
{
    if (IsCircularReference(pChild)) {
        return false;
    }
    if (pChild->m_pParent != nullptr) {
        return false;
    }
    return m_Children.push_back(pChild);
}

// 0x004B3B94 | mk7dlp:bytes [tier B]
void nw::gfx::SceneNode::UpdateFrame()
{
    AnimBinding* pAnimBinding = m_AnimBinding.Get();
    if (pAnimBinding == nullptr) {
        return;
    }
    for (int i = 0; i < pAnimBinding->m_AnimObjects.size(); ++i) {
        AnimObject* pAnimObject = pAnimBinding->m_AnimObjects[i];
        if (pAnimObject != nullptr) {
            pAnimObject->UpdateFrame();
        }
    }
}

// 0x004B3BF4 | nintendogs:callgraph [tier A]
u32 nw::gfx::SceneNode::CreateChildren(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    if (m_Description.isFixedSizeMemory) {
        if (m_Description.maxChildren != 0) {
            void* pBuffer = pAllocator->Alloc(m_Description.maxChildren * sizeof(SceneNode*), 32);
            if (pBuffer == nullptr) {
                result |= RESULT_FAILURE_MASK;
            }
            if (!(result & RESULT_FAILURE_MASK)) {
                ChildList children(pBuffer, m_Description.maxChildren, pAllocator);
                m_Children = children;
            }
        } else {
            ChildList children(nullptr, 0);
            m_Children = children;
        }
    } else {
        ChildList children(pAllocator);
        m_Children = children;
    }
    return result;
}

// 0x004B4018 | nintendogs:callgraph [tier A]
u32 nw::gfx::SceneNode::CreateCallbacks(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    NodeCallbackList<SceneNode>* pCallbacks =
        NodeCallbackList<SceneNode>::Create(pAllocator, m_Description.isFixedSizeMemory, m_Description.maxCallbacks);
    if (pCallbacks == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    m_pCallbacks = pCallbacks;
    return result;
}

// 0x004B4150 slot 0x0C | slot vf_0x0C of nw::gfx::SceneNode
void nw::gfx::SceneNode::UpdateTransform(WorldMatrixUpdater* /*pWorldMatrixUpdater*/, SceneContext* /*pSceneContext*/)
{
    // empty (in the original too)
}

// 0x004B4154 | nintendogs:callgraph [tier A]
u32 nw::gfx::SceneNode::CreateAnimBinding(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    if (!m_Description.isAnimationEnabled || !m_ResSceneObject.IsValid()) {
        return result;
    }
    // only if one of the animation groups has members
    const s32 groupCount = m_ResSceneObject.GetAnimGroupCount();
    for (int i = 0; i < groupCount; ++i) {
        if (m_ResSceneObject.GetAnimGroup(i).GetMemberCount() > 0) {
            AnimBinding* pAnimBinding = AnimBinding::Create(groupCount, m_Description.maxAnimObjectsPerGroup, pAllocator);
            if (pAnimBinding == nullptr) {
                result |= RESULT_FAILURE_MASK;
            }
            if (result & RESULT_FAILURE_MASK) {
                return result;
            }
            m_AnimBinding = GfxPtr<AnimBinding>(pAnimBinding);
            break;
        }
    }
    return result;
}

// 0x004B4538 slot 0x20 | nintendogs:bytes
void nw::gfx::SceneNode::InheritTraversalResults()
{
    u32 results = m_TraversalResults;
    if ((m_pParent == nullptr || (m_pParent->m_TraversalResults & TRAVERSAL_VISIBLE)) && m_IsBranchVisible) {
        results |= TRAVERSAL_VISIBLE;
    } else {
        results &= ~TRAVERSAL_VISIBLE;
    }
    results &= ~TRAVERSAL_DIRTY;
    m_TraversalResults = results;
}

// 0x004B4578 slot 0x10 | nintendogs:bytes
void nw::gfx::SceneNode::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitSceneNode(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x004B45CC (name is ours)
nw::gfx::SceneNode* nw::gfx::SceneNode::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                               os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(SceneNode), 4);
    SceneNode* pNode = pMemory != nullptr ? new (pMemory) SceneNode(pAllocator, resObject, description) : nullptr;
    pNode->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pNode);
    }
    return pNode;
}

// 0x004B47FC slot 0x00 | nintendogs:bytes
// 0x004B47F0 (deleting dtor)
nw::gfx::SceneNode::~SceneNode()
{
    if (m_pParent != nullptr && m_pParent->m_Children.erase(this)) {
        m_pParent = nullptr;
    }
    if (m_pCallbacks != nullptr) {
        m_pCallbacks->Destroy();
        m_pCallbacks = nullptr;
    }
}

} // namespace gfx
} // namespace nw
