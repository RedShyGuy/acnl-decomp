#pragma once

#include <new>

#include "decomp.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_AnimObject.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx11AnimBindingE @ 0x008D055C
// vtable 0x00902364 (vptr 0x0090236C), offset_to_top 0, 2 entries
// The animation groups of a scene node and the animation objects bound to each of them (a fixed
// number of slots per group). The class name is from the RTTI, the rest is ours.
class AnimBinding : public GfxObject
{
public:
    // (inline)
    AnimBinding(os::IAllocator* pAllocator, int animObjectCountPerGroup)
        : GfxObject(pAllocator), m_AnimObjectCountPerGroup(animObjectCountPerGroup)
    {
    }
    virtual ~AnimBinding();

    // (inline) a binding for groupCount groups (NULL: no memory)
    DECOMP_ALWAYS_INLINE static AnimBinding* Create(int groupCount, int animObjectCountPerGroup, os::IAllocator* pAllocator)
    {
        void* pMemory = pAllocator->Alloc(sizeof(AnimBinding), 4);
        if (pMemory == nullptr) {
            return nullptr;
        }
        AnimBinding* pBinding = new (pMemory) AnimBinding(pAllocator, animObjectCountPerGroup);
        if (pBinding->Initialize(groupCount) & RESULT_FAILURE_MASK) {
            pBinding->~AnimBinding();
            pBinding->GetAllocator()->Free(pBinding);
            return nullptr;
        }
        return pBinding;
    }
    // (inline) the arrays, all slots empty
    DECOMP_ALWAYS_INLINE u32 Initialize(int groupCount)
    {
        u32 result = RESULT_SUCCESS;
        const int animObjectCount = groupCount * m_AnimObjectCountPerGroup;
        void* pBuffer = GetAllocator()->Alloc(groupCount * sizeof(AnimGroup*), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        {
            ut::MoveArray<AnimGroup*> animGroups(pBuffer, groupCount, GetAllocator());
            m_AnimGroups = animGroups;
        }
        pBuffer = GetAllocator()->Alloc(animObjectCount * sizeof(AnimObject*), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        {
            ut::MoveArray<AnimObject*> animObjects(pBuffer, animObjectCount, GetAllocator());
            m_AnimObjects = animObjects;
        }
        for (int i = 0; i < groupCount; ++i) {
            m_AnimGroups.PushBackFast(nullptr);
        }
        for (int i = 0; i < animObjectCount; ++i) {
            m_AnimObjects.PushBackFast(nullptr);
        }
        return result;
    }

    // the values of the groups that are animated in this pass of the scene update (name is ours)
    void Evaluate(u8 evaluationTiming);

    // (inline)
    AnimObject* GetAnimObject(int groupIdx, int objectIdx) const
    {
        return m_AnimObjects[groupIdx * m_AnimObjectCountPerGroup + objectIdx];
    }

    ut::MoveArray<AnimGroup*> m_AnimGroups;   // 0x8
    ut::MoveArray<AnimObject*> m_AnimObjects; // 0x18 m_AnimObjectCountPerGroup per group
    int m_AnimObjectCountPerGroup;            // 0x28
};
ASSERT_SIZE(AnimBinding, 0x2C);
} // namespace gfx
} // namespace nw
