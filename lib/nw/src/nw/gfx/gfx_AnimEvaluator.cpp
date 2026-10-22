#include "nw/gfx/gfx_AnimEvaluator.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the size of a cache entry: the AnimResult header and the value, a multiple of four
inline u32 GetCacheEntrySize(anim::res::ResMemberAnim memberAnim)
{
    return (memberAnim.GetPrimitiveSize() + 8 + 3) & ~3u;
}
} // namespace

// 0x0097604C
const TypeInfo nw::gfx::AnimEvaluator::s_TypeInfo(&BaseAnimEvaluator::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C164 (static initializer)

// 0x00491E88 slot 0x24 | slot vf_0x24 of nw::gfx::BaseAnimEvaluator
u32 nw::gfx::AnimEvaluator::ChangeAnim(anim::res::ResAnim animData)
{
    const u32 result = BaseAnimEvaluator::ChangeAnim(animData);
    m_CachePtrs.Resize(animData.GetMemberAnimCount());
    if (!m_IsCacheExternal && m_pCacheBuffer != nullptr) {
        GetAllocator()->Free(m_pCacheBuffer);
        m_pCacheBuffer = nullptr;
        if (animData.GetMemberAnimCount() != 0) {
            m_pCacheBuffer = GetAllocator()->Alloc(GetCacheSize(), 4);
            SetCacheBufferPointers();
        }
    }
    return result;
}

// 0x004920A0 (name is ours)
u32 nw::gfx::AnimEvaluator::Initialize(const anim::res::ResAnim& animData, int maxMembers, int maxMemberAnims, bool allocateCache)
{
    u32 result = RESULT_SUCCESS;
    SetAnimData(animData);

    void* pBuffer = GetAllocator()->Alloc(maxMembers * sizeof(int), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (!(result & RESULT_FAILURE_MASK)) {
        {
            ut::MoveArray<int> bindIndices(pBuffer, maxMembers, GetAllocator());
            m_BindIndices = bindIndices;
        }
        pBuffer = GetAllocator()->Alloc(maxMemberAnims * sizeof(int), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (!(result & RESULT_FAILURE_MASK)) {
            ut::MoveArray<int> animMemberIndices(pBuffer, maxMemberAnims, GetAllocator());
            m_AnimMemberIndices = animMemberIndices;
        }
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    pBuffer = GetAllocator()->Alloc(maxMemberAnims * sizeof(anim::AnimResult*), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<anim::AnimResult*> cachePtrs(pBuffer, maxMemberAnims, GetAllocator());
        m_CachePtrs = cachePtrs;
    }
    m_CachePtrs.Resize(animData.GetMemberAnimCount());

    if (allocateCache && animData.GetMemberAnimCount() != 0) {
        m_pCacheBuffer = GetAllocator()->Alloc(GetCacheSize(), 4);
        if (m_pCacheBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        SetCacheBufferPointers();
    }
    return result;
}

// 0x004924E8 slot 0x30 (name is ours)
void nw::gfx::AnimEvaluator::SetCacheBuffer(void* pBuffer, u32 /*size*/)
{
    m_pCacheBuffer = pBuffer;
    if (pBuffer != nullptr) {
        m_IsCacheDirty = true;
        m_IsCacheExternal = true;
        SetCacheBufferPointers();
    }
}

// 0x00492508 slot 0x20 | slot vf_0x20 of nw::gfx::BaseAnimEvaluator
void nw::gfx::AnimEvaluator::UpdateCache()
{
    UpdateCacheNonVirtual();
}

// 0x0049250C | nintendogs:bytes [tier A]
void nw::gfx::AnimEvaluator::UpdateCacheNonVirtual()
{
    if (!m_IsCacheDirty) {
        return;
    }
    if (m_pCacheBuffer != nullptr) {
        const s32 memberAnimCount = m_AnimData.GetMemberAnimCount();
        for (int animIdx = 0; animIdx < memberAnimCount; ++animIdx) {
            anim::AnimResult* pResult = m_CachePtrs[animIdx];
            pResult->m_EnabledFlags = 0;
            const int memberIdx = m_AnimMemberIndices[animIdx];
            if (memberIdx != NO_MEMBER_ANIM) {
                GetResult(pResult, memberIdx);
            }
        }
    } else if (m_UseCacheFrame && m_pCacheFrame != nullptr) {
        if (m_pCacheFrame->isDirty || GetFrame() != m_pCacheFrame->frame || m_AnimFrameController.GetSpeed() != m_pCacheFrame->speed) {
            const s32 memberAnimCount = m_AnimData.GetMemberAnimCount();
            for (int animIdx = 0; animIdx < memberAnimCount; ++animIdx) {
                anim::AnimResult* pResult = m_CachePtrs[animIdx];
                pResult->m_EnabledFlags = 0;
                const int memberIdx = m_AnimMemberIndices[animIdx];
                if (memberIdx != NO_MEMBER_ANIM) {
                    GetResult(pResult, memberIdx);
                }
            }
            m_pCacheFrame->frame = GetFrame();
            m_pCacheFrame->speed = m_AnimFrameController.GetSpeed();
            m_pCacheFrame->isDirty = false;
        }
    }
    m_IsCacheDirty = false;
}

// 0x00492654 | nintendogs:bytes [tier A]
void nw::gfx::AnimEvaluator::SetCacheBufferPointers()
{
    u8* pCache = static_cast<u8*>(m_pCacheBuffer);
    const s32 memberAnimCount = m_AnimData.GetMemberAnimCount();
    for (int animIdx = 0; animIdx < memberAnimCount; ++animIdx) {
        m_CachePtrs[animIdx] = reinterpret_cast<anim::AnimResult*>(pCache);
        pCache += GetCacheEntrySize(m_AnimData.GetMemberAnim(animIdx));
    }
}

// 0x004926E0 slot 0x2C (name is ours)
u32 nw::gfx::AnimEvaluator::GetCacheSize() const
{
    return GetCacheBufferSizeNeeded(m_AnimData);
}

// 0x004926E8 | nintendogs:bytes [tier A]
u32 nw::gfx::AnimEvaluator::GetCacheBufferSizeNeeded(const anim::res::ResAnim& animData)
{
    u32 size = 0;
    const s32 memberAnimCount = animData.GetMemberAnimCount();
    for (int animIdx = 0; animIdx < memberAnimCount; ++animIdx) {
        size += GetCacheEntrySize(animData.GetMemberAnim(animIdx));
    }
    return size;
}

// 0x00492770 (name is ours)
nw::gfx::AnimEvaluator::AnimEvaluator(os::IAllocator* pAllocator)
    : BaseAnimEvaluator(pAllocator, ANIM_TYPE_EVALUATOR), m_pCacheBuffer(nullptr), m_pCacheFrame(nullptr)
{
}

// 0x00492918 slot 0x00 | nintendogs:bytes
// 0x00492840 (deleting dtor)
nw::gfx::AnimEvaluator::~AnimEvaluator()
{
    if (!m_IsCacheExternal && m_pCacheBuffer != nullptr) {
        GetAllocator()->Free(m_pCacheBuffer);
    }
}

} // namespace gfx
} // namespace nw
