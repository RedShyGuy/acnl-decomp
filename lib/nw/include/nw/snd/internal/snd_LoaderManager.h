#pragma once

#include "decomp.h"
#include <new>

#include "nw/snd/internal/driver/snd_SoundThread_SoundFrameCallback.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
using ut::LinkListNode;

// The loaders of a kind of sound in a buffer: the free ones and the ones in use; at the start of each sound
// frame the loaders whose task is done are finalized and freed (the class name and OnBeginSoundFrame are
// from the reference symbols, the members are ours). The other functions are inlined everywhere.
// Instantiations found in the binary:
//   nw::snd::internal::LoaderManager<nw::snd::internal::driver::SequenceSoundLoader>  typeinfo 0x008D09B4  vtable 0x00903108
//   nw::snd::internal::LoaderManager<nw::snd::internal::driver::WaveSoundLoader>  typeinfo 0x008D09A8  vtable 0x009030F0
template <typename TLoader>
class LoaderManager : public ::nw::snd::internal::driver::SoundThread::SoundFrameCallback
{
public:
    typedef ut::LinkList<TLoader, TLoader::LINK_OFFSET> LoaderList;

    // (inline)
    LoaderManager() : m_pBuffer(nullptr), m_BufferSize(0) {}
    virtual ~LoaderManager() {}

    virtual void OnBeginSoundFrame()
    {
        for (typename LoaderList::iterator it = m_UsedList.begin(); it != m_UsedList.end();) {
            TLoader* pLoader = LoaderList::GetPointerFromNode(it.GetNode());
            ++it;
            if (!pLoader->IsInUse()) {
                pLoader->Finalize();
                m_UsedList.Erase(pLoader);
                m_FreeList.PushBack(pLoader);
            }
        }
    }

    // (inline; name is ours) the loaders in the buffer; their count
    u32 Create(void* pBuffer, u32 size)
    {
        const u32 count = size / sizeof(TLoader);
        u8* p = static_cast<u8*>(pBuffer);
        for (u32 i = 0; i < count; i++) {
            TLoader* pLoader = p != nullptr ? new (p) TLoader : nullptr;
            m_FreeList.PushBack(pLoader);
            p += sizeof(TLoader);
        }
        m_pBuffer = pBuffer;
        m_BufferSize = size;
        return count;
    }
    // (inline; name is ours) the loaders are destroyed
    void Destroy()
    {
        const u32 count = m_BufferSize / sizeof(TLoader);
        TLoader* pLoader = static_cast<TLoader*>(m_pBuffer);
        for (u32 i = 0; i < count; i++) {
            pLoader->~TLoader();
            pLoader++;
        }
        m_FreeList.erase(m_FreeList.begin(), m_FreeList.end());
    }
    // (inline; name is ours) a free loader (NULL: none)
    TLoader* Alloc()
    {
        if (m_FreeList.empty()) {
            return nullptr;
        }
        LinkListNode* pNode = m_FreeList.begin().GetNode();
        m_FreeList.erase(m_FreeList.begin());
        return LoaderList::GetPointerFromNode(pNode);
    }
    // (inline; name is ours) the loader goes back to the free list, or to the used list while its task runs
    void Free(TLoader* pLoader)
    {
        if (!pLoader->IsInUse()) {
            pLoader->Finalize();
            m_FreeList.PushBack(pLoader);
        } else {
            m_UsedList.PushBack(pLoader);
        }
    }

    void* m_pBuffer;        // 0x0C
    u32 m_BufferSize;       // 0x10
    LoaderList m_FreeList;  // 0x14
    LoaderList m_UsedList;  // 0x20
};
} // namespace internal
} // namespace snd
} // namespace nw
