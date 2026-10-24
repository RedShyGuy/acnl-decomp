#include "nw/snd/internal/driver/snd_ChannelManager.h"

#include <new>

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CCB48 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::ChannelManager::Initialize(void* pMemory, u32 size)
{
    if (m_IsInitialized) {
        return;
    }
    m_ChannelCount = m_Pool.CreateImpl(pMemory, size, sizeof(Channel));
    m_pMemory = pMemory;
    m_IsInitialized = true;
    m_MemorySize = size;
}

// 0x004CCB88 | nintendogs:callseq-callee [tier A]
nw::snd::internal::driver::ChannelManager& nw::snd::internal::driver::ChannelManager::GetInstance()
{
    // 0x00AF0768
    static ChannelManager s_ChannelManager;
    return s_ChannelManager;
}

// 0x004CCBE4 | fefates:bytes [tier B]
void nw::snd::internal::driver::ChannelManager::UpdateAllChannel()
{
    for (ut::internal::LinkListImpl::iterator it = m_ChannelList.begin(); it != m_ChannelList.end();) {
        Channel* pChannel = ChannelList::GetPointerFromNode(it.GetNode());
        ++it;
        pChannel->Update(true);
    }
}

// 0x004CCC1C | nintendogs:callseq-callee [tier A]
u32 nw::snd::internal::driver::ChannelManager::GetRequiredMemSize(int channelCount)
{
    return sizeof(Channel) * (channelCount + 1);
}

// 0x004CCCB4 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::ChannelManager::Free(Channel* pChannel)
{
    m_ChannelList.erase(&pChannel->m_Link);
    if (pChannel != nullptr) {
        pChannel->~Channel();
        m_Pool.FreeImpl(pChannel);
    }
}

// 0x004CCCF0 | nintendogs:bytes-fuzzy [tier A]
nw::snd::internal::driver::Channel* nw::snd::internal::driver::ChannelManager::Alloc()
{
    void* pMemory = m_Pool.AllocImpl();
    Channel* pChannel = pMemory != nullptr ? new (pMemory) Channel() : nullptr;
    m_ChannelList.PushBack(pChannel);
    return pChannel;
}

// 0x004CCD28 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::ChannelManager::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    for (ut::internal::LinkListImpl::iterator it = m_ChannelList.begin(); it != m_ChannelList.end();) {
        Channel* pChannel = ChannelList::GetPointerFromNode(it.GetNode());
        ++it;
        pChannel->Stop();
    }
    m_Pool.DestroyImpl(m_pMemory, m_MemorySize);
    m_IsInitialized = false;
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
