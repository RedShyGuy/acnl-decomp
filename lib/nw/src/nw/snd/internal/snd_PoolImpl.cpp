#include "nw/snd/internal/snd_PoolImpl.h"

#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004D4A80 | nintendogs:bytes [tier A]
u32 nw::snd::internal::PoolImpl::CreateImpl(void* pMemory, u32 size, u32 objectSize)
{
    u8* p = ut::RoundUp(static_cast<u8*>(pMemory), 4);
    const u32 alignedSize = ut::RoundUp(objectSize, 4);
    const u32 count = (size - (p - static_cast<u8*>(pMemory))) / alignedSize;
    for (u32 i = 0; i < count; ++i) {
        PoolImpl* pObject = reinterpret_cast<PoolImpl*>(p);
        pObject->m_pNext = m_pNext;
        m_pNext = pObject;
        p += alignedSize;
    }
    return count;
}

// 0x004D4AE0 | nintendogs:bytes [tier A]
void nw::snd::internal::PoolImpl::DestroyImpl(void* pMemory, u32 size)
{
    const void* pEnd = static_cast<u8*>(pMemory) + size;
    PoolImpl* pPrev = this;
    for (PoolImpl* p = m_pNext; p != nullptr; p = p->m_pNext) {
        if (pMemory <= p && p < pEnd) {
            pPrev->m_pNext = p->m_pNext;
        } else {
            pPrev = p;
        }
    }
}

// 0x004D4B14 | nintendogs:bytes [tier A]
void nw::snd::internal::PoolImpl::FreeImpl(void* p)
{
    PoolImpl* pObject = static_cast<PoolImpl*>(p);
    pObject->m_pNext = m_pNext;
    m_pNext = pObject;
}

// 0x004D4B24 | nintendogs:bytes [tier A]
void* nw::snd::internal::PoolImpl::AllocImpl()
{
    PoolImpl* p = m_pNext;
    if (p != nullptr) {
        m_pNext = p->m_pNext;
    }
    return p;
}

// 0x007430A4 | nintendogs:bytes [tier A]
int nw::snd::internal::PoolImpl::CountImpl() const
{
    int count = 0;
    for (PoolImpl* p = m_pNext; p != nullptr; p = p->m_pNext) {
        ++count;
    }
    return count;
}

} // namespace internal
} // namespace snd
} // namespace nw
