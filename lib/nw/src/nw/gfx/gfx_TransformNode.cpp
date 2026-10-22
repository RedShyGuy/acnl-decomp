#include "nw/gfx/gfx_TransformNode.h"

#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_WorldMatrixUpdater.h"

namespace nw {
namespace gfx {
namespace {
// the smallest squared length of the world scale (a smaller scale keeps the signs with this length)
const f32 WORLD_SCALE_EPSILON = 1.00000011e-6f;
} // namespace

// 0x009760F8
const TypeInfo nw::gfx::TransformNode::s_TypeInfo(&SceneNode::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C25C (static initializer)

// ctor address unknown
nw::gfx::TransformNode::TransformNode()
{
}

// 0x004989E4 slot 0x24 | nintendogs:bytes
u32 nw::gfx::TransformNode::Initialize(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    result |= SceneNode::Initialize(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateCallbacks(pAllocator);
    return result;
}

// 0x00498A1C | nintendogs:callgraph [tier A]
u32 nw::gfx::TransformNode::CreateCallbacks(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    NodeCallbackList<TransformNode>* pCallbacks = NodeCallbackList<TransformNode>::Create(
        pAllocator, m_TransformDescription.isFixedSizeMemory, m_TransformDescription.maxCallbacks);
    if (pCallbacks == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    m_pCalculatedCallbacks = pCallbacks;
    return result;
}

// 0x00498B54 slot 0x28 | slot vf_0x28 of nw::gfx::TransformNode
void nw::gfx::TransformNode::UpdateDirection()
{
    // empty (in the original too)
}

// 0x00498B58 slot 0x0C | nintendogs:callseq
void nw::gfx::TransformNode::UpdateTransform(WorldMatrixUpdater* pWorldMatrixUpdater, SceneContext* pSceneContext)
{
    if (!m_Transform.IsEnabledFlags(CalculatedTransform::FLAG_UNKNOWN_0x1)) {
        m_TraversalResults &= ~TRAVERSAL_DIRTY;
    } else if (m_TraversalResults & TRAVERSAL_DIRTY) {
        const CalculatedTransform* pParentWorldTransform;
        const CalculatedTransform* pParentLocalTransform;
        if (m_pParent != nullptr) {
            pParentWorldTransform = &m_pParent->TrackbackWorldTransform();
            pParentLocalTransform = &m_pParent->TrackbackLocalTransform();
        } else {
            pParentWorldTransform = &CalculatedTransform::Identity();
            pParentLocalTransform = &CalculatedTransform::Identity();
        }
        nn::math::VEC3 scale;
        pWorldMatrixUpdater->CalculateWorldBasic(&m_WorldTransform.m_TransformMatrix, &scale, m_Transform, *pParentWorldTransform,
                                                 *pParentLocalTransform);
        if (scale.x * scale.x + scale.y * scale.y + scale.z * scale.z < WORLD_SCALE_EPSILON) {
            scale.x = scale.x < 0.0f ? -WORLD_SCALE_EPSILON : WORLD_SCALE_EPSILON;
            scale.y = scale.y < 0.0f ? -WORLD_SCALE_EPSILON : WORLD_SCALE_EPSILON;
            scale.z = scale.z < 0.0f ? -WORLD_SCALE_EPSILON : WORLD_SCALE_EPSILON;
        }
        m_WorldTransform.m_Scale = scale;
        nn::math::ARMv6::MTX34MultScaleAsm(&m_WorldMatrix, &m_WorldTransform.m_TransformMatrix, &m_Transform.m_Scale);
        m_WorldTransform.DisableFlags(CalculatedTransform::FLAG_IS_IDENTITY | CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO |
                                      CalculatedTransform::FLAG_IS_ROTATE_ZERO | CalculatedTransform::FLAG_IS_TRANSLATE_ZERO |
                                      CalculatedTransform::FLAG_IS_SCALE_ONE | CalculatedTransform::FLAG_IS_UNIFORM_SCALE);
        m_WorldTransform.UpdateScaleFlags();
        m_Unknown0x12C = false;
    }
    UpdateDirection();
    m_Transform.DisableFlags(CalculatedTransform::FLAG_IS_DIRTY);
    m_pCalculatedCallbacks->Invoke(this, pSceneContext);
}

// 0x00498E18 slot 0x20 | nintendogs:bytes
void nw::gfx::TransformNode::InheritTraversalResults()
{
    SceneNode::InheritTraversalResults();
    if (m_Transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_DIRTY) ||
        (m_pParent != nullptr && (m_pParent->m_TraversalResults & TRAVERSAL_DIRTY))) {
        m_TraversalResults |= TRAVERSAL_DIRTY;
    } else {
        m_TraversalResults &= ~TRAVERSAL_DIRTY;
    }
}

// 0x00498E98 | nintendogs:bytes [tier A]
void nw::gfx::TransformNode::GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, res::ResTransformNode resNode, Description description)
{
    // the children (aligned to 32)
    const u32 alignment = pSize->GetAlignment();
    pSize->SetAlignment(32);
    pSize->Add(description.maxChildren * sizeof(SceneNode*));
    pSize->SetAlignment(alignment);

    // the callbacks of the scene node
    pSize->Add(sizeof(NodeCallbackList<SceneNode>));
    if (description.maxCallbacks != 0) {
        pSize->Add(description.maxCallbacks * sizeof(NodeCallback<SceneNode>*));
    }

    // the animation binding
    if (description.isAnimationEnabled && resNode.IsValid() && resNode.GetAnimGroupCount() != 0) {
        const s32 groupCount = resNode.GetAnimGroupCount();
        pSize->Add(sizeof(AnimBinding));
        pSize->Add(groupCount * sizeof(AnimGroup*));
        pSize->Add(groupCount * description.maxAnimObjectsPerGroup * sizeof(AnimObject*));
    }

    // the callbacks of the transform node
    pSize->Add(sizeof(NodeCallbackList<TransformNode>));
    if (description.maxCallbacks != 0) {
        pSize->Add(description.maxCallbacks * sizeof(NodeCallback<TransformNode>*));
    }
}

// 0x0049903C (name is ours)
void nw::gfx::TransformNode::ClearUnknown0x12C()
{
    m_Unknown0x12C = false;
}

// 0x00499048 slot 0x10 | nintendogs:bytes
void nw::gfx::TransformNode::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitTransformNode(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x0049909C (name is ours)
nw::gfx::TransformNode* nw::gfx::TransformNode::Create(SceneNode* pParent, res::ResTransformNode resNode, const Description& description,
                                                       os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(TransformNode), 4);
    TransformNode* pNode = pMemory != nullptr ? new (pMemory) TransformNode(pAllocator, resNode, description) : nullptr;
    pNode->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pNode);
    }
    return pNode;
}

// 0x0049926C | nintendogs:callgraph [tier A]
nw::gfx::TransformNode::TransformNode(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description)
    : SceneNode(pAllocator, resNode, description), m_Unknown0x12C(false), m_pCalculatedCallbacks(nullptr), m_Unknown0x134(true),
      m_TransformDescription(description)
{
    nn::math::ARMv6::MTX34CopyAsm(&m_Unknown0xFC, &nn::math::MTX34::Identity());
    if (resNode.IsValid()) {
        m_Transform.SetTransform(resNode.GetTransform());
        m_WorldMatrix = resNode.GetWorldMatrix();
    } else {
        nn::math::ARMv6::MTX34CopyAsm(&m_WorldMatrix, &nn::math::MTX34::Identity());
    }
}

// 0x004995F8 slot 0x00 | nintendogs:bytes
// 0x004995EC (deleting dtor)
nw::gfx::TransformNode::~TransformNode()
{
    if (m_pCalculatedCallbacks != nullptr) {
        m_pCalculatedCallbacks->Destroy();
        m_pCalculatedCallbacks = nullptr;
    }
}

} // namespace gfx
} // namespace nw
