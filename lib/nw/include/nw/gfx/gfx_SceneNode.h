#pragma once

#include <algorithm>
#include <new>

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nw/gfx/gfx_AnimBinding.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_ISceneVisitor.h"
#include "nw/gfx/gfx_NodeCallback.h"
#include "nw/gfx/gfx_SceneObject.h"
#include "nw/os/os_MemorySizeCalculator.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class SceneContext;
class WorldMatrixUpdater;

// RTTI N2nw3gfx9SceneNodeE @ 0x008D0834
// vtable 0x00902AF8 (vptr 0x00902B00), offset_to_top 0, 10 entries
// A node of the scene tree: its children, the animations bound to it and the traversal results
// (visible, dirty). The function names are from the reference symbols, the rest is ours.
class SceneNode : public ::nw::gfx::SceneObject
{
public:
    // how the node is built (names are ours)
    struct Description
    {
        bool isFixedSizeMemory;     // 0x0 the arrays do not grow
        bool isAnimationEnabled;    // 0x1
        s32 maxCallbacks;           // 0x4
        s32 maxChildren;            // 0x8
        s32 maxAnimObjectsPerGroup; // 0xC
    };

    // the traversal results (names are ours)
    enum TraversalResult
    {
        TRAVERSAL_VISIBLE = 1 << 0, // the node and all its parents are visible
        TRAVERSAL_DIRTY = 1 << 1,   // the world matrix has to be calculated again
    };

    // The children: the node they take as parent (set by Initialize) and the array. Destroying it
    // detaches the children (class and names are ours).
    class ChildList
    {
    public:
        // (inline)
        ChildList() {}
        ChildList(void* pBuffer, s32 capacity, os::IAllocator* pAllocator = nullptr)
            : m_pOwner(nullptr), m_Children(pBuffer, capacity, pAllocator)
        {
        }
        explicit ChildList(os::IAllocator* pAllocator) : m_pOwner(nullptr), m_Children(pAllocator) {}
        ~ChildList()
        {
            for (int i = 0; i < m_Children.size(); ++i) {
                if (m_Children[i] != nullptr) {
                    m_Children[i]->m_pParent = nullptr;
                }
            }
        }
        ChildList& operator=(ChildList& other)
        {
            m_pOwner = other.m_pOwner;
            m_Children = other.m_Children;
            return *this;
        }

        bool push_back(SceneNode* pChild)
        {
            const bool isSuccess = m_Children.push_back(pChild);
            if (isSuccess) {
                pChild->m_pParent = m_pOwner;
            }
            return isSuccess;
        }
        // removes the child; true if it was there
        bool erase(SceneNode* pChild)
        {
            SceneNode** pEnd = m_Children.end();
            SceneNode** pNewEnd = std::remove(m_Children.begin(), pEnd, pChild);
            m_Children.m_pEnd = pNewEnd;
            return pEnd != pNewEnd;
        }

        SceneNode** begin() { return m_Children.begin(); }
        SceneNode** end() { return m_Children.end(); }

        SceneNode* m_pOwner;                  // 0x0
        ut::MoveArray<SceneNode*> m_Children; // 0x4
    };

    // (inline)
    SceneNode(os::IAllocator* pAllocator, res::ResSceneObject resObject, const Description& description)
        : SceneObject(pAllocator, resObject), m_pParent(nullptr), m_IsBranchVisible(true), m_pCallbacks(nullptr),
          m_TraversalResults(TRAVERSAL_VISIBLE | TRAVERSAL_DIRTY), m_Description(description)
    {
        if (resObject.IsValid()) {
            m_IsBranchVisible = resObject.GetFlags() & res::ResSceneObjectData::FLAG_IS_BRANCH_VISIBLE;
        }
    }
    SceneNode(); // ctor address unknown (for the derived classes not written yet)
    virtual ~SceneNode();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C96C slot 0x08
    virtual void UpdateTransform(WorldMatrixUpdater* pWorldMatrixUpdater, SceneContext* pSceneContext);
    virtual void Accept(ISceneVisitor* pVisitor);
    // the world matrix of the nearest node that has one (the identity)
    virtual const nn::math::MTX34& TrackbackWorldMatrix() const // 0x0073C998 slot 0x14 | nintendogs:bytes
    {
        if (m_pParent != nullptr) {
            return m_pParent->TrackbackWorldMatrix();
        }
        return nn::math::MTX34::Identity();
    }
    virtual const CalculatedTransform& TrackbackWorldTransform() const // 0x0073CA54 slot 0x18 | nintendogs:bytes-fuzzy
    {
        if (m_pParent != nullptr) {
            return m_pParent->TrackbackWorldTransform();
        }
        return CalculatedTransform::Identity();
    }
    virtual const CalculatedTransform& TrackbackLocalTransform() const // 0x0073C9C4 slot 0x1C | nintendogs:bytes-fuzzy
    {
        if (m_pParent != nullptr) {
            return m_pParent->TrackbackLocalTransform();
        }
        return CalculatedTransform::Identity();
    }
    virtual void InheritTraversalResults();
    virtual u32 Initialize(os::IAllocator* pAllocator);

    // a node made from the resource, attached to pParent (name is ours)
    static SceneNode* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator);
    // (inline) the memory Initialize takes: the children (aligned to 32), the callbacks and the
    // animation binding
    static void GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, res::ResSceneObject resObject, Description description)
    {
        const u32 alignment = pSize->GetAlignment();
        pSize->SetAlignment(32);
        pSize->Add(description.maxChildren * sizeof(SceneNode*));
        pSize->SetAlignment(alignment);

        pSize->Add(sizeof(NodeCallbackList<SceneNode>));
        if (description.maxCallbacks != 0) {
            pSize->Add(description.maxCallbacks * sizeof(NodeCallback<SceneNode>*));
        }

        if (description.isAnimationEnabled && resObject.IsValid() && resObject.GetAnimGroupCount() != 0) {
            const s32 groupCount = resObject.GetAnimGroupCount();
            pSize->Add(sizeof(AnimBinding));
            pSize->Add(groupCount * sizeof(AnimGroup*));
            pSize->Add(groupCount * description.maxAnimObjectsPerGroup * sizeof(AnimObject*));
        }
    }
    // false if the child has a parent or is above this node
    bool AttachChild(SceneNode* pChild);
    // the frames of the bound animations
    void UpdateFrame();
    u32 CreateChildren(os::IAllocator* pAllocator);
    u32 CreateCallbacks(os::IAllocator* pAllocator);
    u32 CreateAnimBinding(os::IAllocator* pAllocator);
    // whether pNode is above this node
    DECOMP_EMIT_COPY bool IsCircularReference(const SceneNode* pNode) const // 0x0073C978 | nintendogs:bytes [tier A]
    {
        if (m_pParent == nullptr) {
            return false;
        }
        if (m_pParent == pNode) {
            return true;
        }
        return m_pParent->IsCircularReference(pNode);
    }

    // (inline)
    SceneNode* GetParent() const { return m_pParent; }
    bool IsVisible() const { return (m_TraversalResults & TRAVERSAL_VISIBLE) != 0; }
    AnimBinding* GetAnimBinding() const { return m_AnimBinding.Get(); }

    static const TypeInfo s_TypeInfo;

    SceneNode* m_pParent;                          // 0xC
    ChildList m_Children;                          // 0x10
    u32 m_Unknown0x24;                             // 0x24
    GfxPtr<AnimBinding> m_AnimBinding;             // 0x28
    bool m_IsBranchVisible;                        // 0x30
    NodeCallbackList<SceneNode>* m_pCallbacks;     // 0x34
    u32 m_TraversalResults;                        // 0x38 TraversalResult
    Description m_Description;                     // 0x3C
};
ASSERT_SIZE(SceneNode, 0x4C);
// (inline) the node as a T if it is one or derives from one (NULL: not, or no node; name is ours)
template <typename T>
T* DynamicCast(SceneNode* pNode)
{
    if (pNode == nullptr) {
        return nullptr;
    }
    const TypeInfo* pTypeInfo = pNode->GetRuntimeTypeInfo();
    do {
        if (pTypeInfo == &T::s_TypeInfo) {
            return static_cast<T*>(pNode);
        }
        pTypeInfo = pTypeInfo->pParent;
    } while (pTypeInfo != nullptr);
    return nullptr;
}
} // namespace gfx
} // namespace nw
