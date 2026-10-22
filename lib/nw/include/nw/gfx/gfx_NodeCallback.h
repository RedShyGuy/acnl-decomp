#pragma once

#include <new>

#include "decomp.h"
#include "nw/os/os_IAllocator.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class SceneContext;

// A callback of a node: Invoke in the first slot, the destructor after it (class and names are ours).
template <typename TNode, typename TArg = SceneContext*>
class NodeCallback
{
public:
    virtual void Invoke(TNode* pNode, TArg arg) = 0;
    virtual ~NodeCallback() {}

    os::IAllocator* m_pAllocator; // 0x4 (NULL: not destroyed with the list)
};

// The callbacks of a node in an array from the allocator (class and names are ours).
template <typename TNode, typename TArg = SceneContext*>
class NodeCallbackList
{
public:
    // (inline)
    NodeCallbackList(os::IAllocator* pAllocator, void* pBuffer, s32 capacity)
        : m_pAllocator(pAllocator), m_Callbacks(pBuffer, capacity, pAllocator)
    {
    }
    explicit NodeCallbackList(os::IAllocator* pAllocator) : m_pAllocator(pAllocator), m_Callbacks(pAllocator) {}

    // (inline) a list for maxCallbacks callbacks (one that grows if !isFixedSizeMemory); NULL: no memory
    static NodeCallbackList* Create(os::IAllocator* pAllocator, bool isFixedSizeMemory, s32 maxCallbacks)
    {
        if (!isFixedSizeMemory) {
            void* pMemory = pAllocator->Alloc(sizeof(NodeCallbackList), 4);
            return pMemory != nullptr ? new (pMemory) NodeCallbackList(pAllocator) : nullptr;
        }
        if (maxCallbacks == 0) {
            void* pMemory = pAllocator->Alloc(sizeof(NodeCallbackList), 4);
            return pMemory != nullptr ? new (pMemory) NodeCallbackList(pAllocator, nullptr, 0) : nullptr;
        }
        void* pMemory = pAllocator->Alloc(sizeof(NodeCallbackList), 4);
        if (pMemory == nullptr) {
            return nullptr;
        }
        void* pBuffer = pAllocator->Alloc(maxCallbacks * sizeof(NodeCallback<TNode, TArg>*), 4);
        if (pBuffer == nullptr) {
            pAllocator->Free(pMemory);
            return nullptr;
        }
        return new (pMemory) NodeCallbackList(pAllocator, pBuffer, maxCallbacks);
    }

    // (inline) the callbacks, then the list
    void Destroy()
    {
        for (NodeCallback<TNode, TArg>** ppCallback = m_Callbacks.begin(); ppCallback != m_Callbacks.end(); ++ppCallback) {
            NodeCallback<TNode, TArg>* pCallback = *ppCallback;
            os::IAllocator* pCallbackAllocator = pCallback->m_pAllocator;
            if (pCallbackAllocator != nullptr) {
                pCallback->~NodeCallback();
                pCallbackAllocator->Free(pCallback);
            }
        }
        os::IAllocator* pAllocator = m_pAllocator;
        this->~NodeCallbackList();
        pAllocator->Free(this);
    }

    // (inline)
    void Invoke(TNode* pNode, TArg arg)
    {
        for (NodeCallback<TNode, TArg>** ppCallback = m_Callbacks.begin(); ppCallback != m_Callbacks.end(); ++ppCallback) {
            (*ppCallback)->Invoke(pNode, arg);
        }
    }

    os::IAllocator* m_pAllocator;                    // 0x0
    ut::MoveArray<NodeCallback<TNode, TArg>*> m_Callbacks; // 0x4
};
} // namespace gfx
} // namespace nw
