#pragma once

#include <new>

#include "decomp.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_BaseAnimEvaluator.h"

namespace nw {
namespace gfx {
// The frame an evaluator cached its values at last, shared by several evaluators (name and
// members are ours).
struct AnimCacheFrame
{
    u8 m_Unknown0x0[0x10]; // 0x0
    f32 frame;             // 0x10
    f32 speed;             // 0x14
    bool isDirty;          // 0x18
};

// RTTI N2nw3gfx13AnimEvaluatorE @ 0x008D05BC
// vtable 0x00902458 (vptr 0x00902460), offset_to_top 0, 13 entries
// Evaluates an animation of any member type; it can keep the values of the last frame in a cache
// (per member animation an AnimResult header and the value). The function names are from the
// reference symbols, the rest is ours.
class AnimEvaluator : public BaseAnimEvaluator
{
public:
    class Builder;

    explicit AnimEvaluator(os::IAllocator* pAllocator);
    virtual ~AnimEvaluator();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738704 slot 0x08
    virtual const anim::AnimResult* GetResult(void* pTarget, int memberIdx) const // 0x00738710 slot 0x18 | nintendogs:bytes
    {
        if (!HasMemberAnim(memberIdx)) {
            return nullptr;
        }
        const anim::AnimBlendOp* pBlendOp = m_pAnimGroup->GetBlendOperation(memberIdx);
        const int animIdx = GetMemberAnimIndex(memberIdx);
        if (m_pCacheBuffer == nullptr || m_IsCacheDirty) {
            anim::res::ResMemberAnim memberAnim = m_AnimData.GetMemberAnim(animIdx);
            const void* pOriginalValue = m_pAnimGroup->GetOriginalValue(memberIdx);
            if (pBlendOp != nullptr) {
                anim::AnimResult* pResult = static_cast<anim::AnimResult*>(pTarget);
                pResult->m_EnabledFlags = memberAnim.EvaluateResultForType(pResult->GetValueBuffer(), pResult->m_EnabledFlags, GetFrame(), pOriginalValue);
                return pResult;
            }
            memberAnim.EvaluateResultForType(pTarget, 0, GetFrame(), pOriginalValue);
            return static_cast<const anim::AnimResult*>(pTarget);
        }
        if (pBlendOp != nullptr) {
            return m_CachePtrs[animIdx];
        }
        anim::res::ResMemberAnim memberAnim = m_AnimData.GetMemberAnim(animIdx);
        memberAnim.ApplyCacheForType(pTarget, m_CachePtrs[animIdx]);
        return static_cast<const anim::AnimResult*>(pTarget);
    }
    virtual bool HasMemberAnim(int memberIdx) const { return GetMemberAnimIndex(memberIdx) != NO_MEMBER_ANIM; } // 0x007386D8 slot 0x1C | nintendogs:bytes
    virtual void UpdateCache();
    virtual u32 ChangeAnim(anim::res::ResAnim animData);
    virtual void* GetCacheBuffer() const { return m_pCacheBuffer; } // 0x007386FC slot 0x28 (name is ours)
    virtual u32 GetCacheSize() const;
    virtual void SetCacheBuffer(void* pBuffer, u32 size);

    // the arrays for up to maxMembers group members and maxMemberAnims member animations, and the
    // cache if allocateCache
    u32 Initialize(const anim::res::ResAnim& animData, int maxMembers, int maxMemberAnims, bool allocateCache);
    void UpdateCacheNonVirtual();
    // the cache entry of each member animation in the cache buffer
    void SetCacheBufferPointers();
    static u32 GetCacheBufferSizeNeeded(const anim::res::ResAnim& animData);

    static const TypeInfo s_TypeInfo;

    void* m_pCacheBuffer;                         // 0x5C
    AnimCacheFrame* m_pCacheFrame;                // 0x60
    ut::MoveArray<anim::AnimResult*> m_CachePtrs; // 0x64 per member animation
};
ASSERT_SIZE(AnimEvaluator, 0x74);

// (inline) builds an evaluator: the members are set one after the other on the stack and Create allocates and
// initializes it (the game code uses it; class and member names are ours)
class AnimEvaluator::Builder
{
public:
    Builder() : m_MaxMembers(0), m_MaxMemberAnims(0), m_AllocateCache(false) {}

    Builder& AnimData(const anim::res::ResAnim& animData)
    {
        m_AnimData = animData;
        return *this;
    }
    Builder& MaxMembers(int maxMembers)
    {
        m_MaxMembers = maxMembers;
        return *this;
    }
    Builder& MaxMemberAnims(int maxMemberAnims)
    {
        m_MaxMemberAnims = maxMemberAnims;
        return *this;
    }
    Builder& AllocateCache(bool allocateCache)
    {
        m_AllocateCache = allocateCache;
        return *this;
    }
    // the evaluator (NULL: no memory; the result of Initialize is not checked)
    AnimEvaluator* Create(os::IAllocator* pAllocator)
    {
        void* pMemory = pAllocator->Alloc(sizeof(AnimEvaluator), 4);
        if (pMemory == nullptr) {
            return nullptr;
        }
        AnimEvaluator* pEvaluator = new (pMemory) AnimEvaluator(pAllocator);
        pEvaluator->Initialize(m_AnimData, m_MaxMembers, m_MaxMemberAnims, m_AllocateCache);
        return pEvaluator;
    }

    anim::res::ResAnim m_AnimData; // 0x0
    int m_MaxMembers;              // 0x4
    int m_MaxMemberAnims;          // 0x8
    bool m_AllocateCache;          // 0xC
};
} // namespace gfx
} // namespace nw
