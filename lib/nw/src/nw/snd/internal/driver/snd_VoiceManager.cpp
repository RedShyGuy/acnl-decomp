#include "nw/snd/internal/driver/snd_VoiceManager.h"

#include <new>

#include "nw/snd/internal/driver/snd_SoundThread.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the priority above which ChangeVoicePriority leaves the voices
const int PRIORITY_MAX = 0x7FFF;
} // namespace

// 0x004CE784 | nintendogs:callseq [tier A]
nw::snd::internal::driver::Voice* nw::snd::internal::driver::VoiceManager::AllocVoice(int channelCount, int priority, Voice::Callback pCallback,
                                                                                       void* pCallbackArg)
{
    if (m_VoiceCount - m_VoiceCountLimit >= m_FreeList.size()) {
        // no voice left: the lowest voice if it is not more important
        if (m_VoiceList.empty()) {
            return nullptr;
        }
        Voice* pLowest = VoiceList::GetPointerFromNode(m_VoiceList.begin().GetNode());
        if (pLowest->m_Priority > priority) {
            return nullptr;
        }
        const Voice::Callback pLowestCallback = pLowest->m_pCallback;
        const int dropCount = pLowest->m_VoiceCount;
        void* pLowestCallbackArg = pLowest->m_pCallbackArg;
        pLowest->Stop();
        pLowest->Free();
        if (pLowestCallback != nullptr) {
            pLowestCallback(pLowest, Voice::STATUS_DROP_VOICE, pLowestCallbackArg);
        }
        if (dropCount == 0) {
            return nullptr;
        }
    }
    Voice* pVoice = VoiceList::GetPointerFromNode(m_FreeList.begin().GetNode());
    if (!pVoice->Alloc(channelCount, priority, pCallback, pCallbackArg)) {
        return nullptr;
    }
    pVoice->m_Priority = static_cast<u8>(priority);
    AppendVoiceList(pVoice);
    return pVoice;
}

// 0x004CE878 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::VoiceManager::Initialize(void* pMemory, u32 size)
{
    if (m_IsInitialized) {
        return;
    }
    const int voiceCount = size / sizeof(Voice);
    u8* p = static_cast<u8*>(pMemory);
    for (int i = 0; i < voiceCount; ++i) {
        Voice* pVoice = new (p) Voice();
        m_FreeList.PushBack(pVoice);
        p += sizeof(Voice);
    }
    m_VoiceCountLimit = voiceCount;
    m_IsInitialized = true;
    m_VoiceCount = voiceCount;
}

// 0x004CE8F0 | nintendogs:callgraph [tier A]
nw::snd::internal::driver::VoiceManager& nw::snd::internal::driver::VoiceManager::GetInstance()
{
    // 0x00AF0CB4
    static VoiceManager s_VoiceManager;
    return s_VoiceManager;
}

// 0x004CE958 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::AppendVoiceList(Voice* pVoice)
{
    m_FreeList.erase(&pVoice->m_Link);
    // behind the voices of a lower or the same priority
    ut::internal::LinkListImpl::iterator it = m_VoiceList.end();
    while (it != m_VoiceList.begin()) {
        ut::LinkListNode* pPrev = it.GetNode()->m_pPrev;
        if (VoiceList::GetPointerFromNode(pPrev)->m_Priority <= pVoice->m_Priority) {
            break;
        }
        it = ut::internal::LinkListImpl::iterator(pPrev);
    }
    m_VoiceList.insert(it, &pVoice->m_Link);
}

// 0x004CE9B4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::UpdateAllVoices()
{
    for (ut::internal::LinkListImpl::iterator it = m_VoiceList.begin(); it != m_VoiceList.end();) {
        Voice* pVoice = VoiceList::GetPointerFromNode(it.GetNode());
        ++it;
        pVoice->StopFinished();
    }
    for (ut::internal::LinkListImpl::iterator it = m_VoiceList.begin(); it != m_VoiceList.end();) {
        Voice* pVoice = VoiceList::GetPointerFromNode(it.GetNode());
        ++it;
        pVoice->Calc();
    }
    for (ut::internal::LinkListImpl::iterator it = m_VoiceList.begin(); it != m_VoiceList.end();) {
        Voice* pVoice = VoiceList::GetPointerFromNode(it.GetNode());
        ++it;
        pVoice->Update();
    }
}

// 0x004CEA40 | nintendogs:callseq-callee [tier A]
u32 nw::snd::internal::driver::VoiceManager::GetRequiredMemSize(int voiceCount)
{
    return voiceCount * sizeof(Voice);
}

// 0x004CEA4C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::ChangeVoicePriority(Voice* pVoice)
{
    m_VoiceList.erase(&pVoice->m_Link);
    m_FreeList.insert(m_FreeList.end(), &pVoice->m_Link);
    AppendVoiceList(pVoice);
    // the voices from this one on get their priority again
    for (ut::LinkListNode* pNode = &pVoice->m_Link; pNode != m_VoiceList.end().GetNode(); pNode = pNode->m_pNext) {
        Voice* p = VoiceList::GetPointerFromNode(pNode);
        if (p->m_Priority <= Voice::PRIORITY_RELEASE) {
            break;
        }
        if (p->m_Priority != PRIORITY_MAX) {
            p->UpdateVoicesPriority();
        }
    }
}

// 0x004CEAC0 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::UpdateAllVoicesSync(u32 syncFlag)
{
    for (ut::internal::LinkListImpl::iterator it = m_VoiceList.begin(); it != m_VoiceList.end();) {
        Voice* pVoice = VoiceList::GetPointerFromNode(it.GetNode());
        ++it;
        if (pVoice->m_IsActive) {
            pVoice->m_SyncFlag |= syncFlag;
        }
    }
}

// 0x004CEB0C | nintendogs:bytes [tier A]
int nw::snd::internal::driver::VoiceManager::DropLowestPriorityVoice(int priority)
{
    if (m_VoiceList.empty()) {
        return 0;
    }
    Voice* pVoice = VoiceList::GetPointerFromNode(m_VoiceList.begin().GetNode());
    if (pVoice->m_Priority > priority) {
        return 0;
    }
    const int dropCount = pVoice->m_VoiceCount;
    const Voice::Callback pCallback = pVoice->m_pCallback;
    void* pCallbackArg = pVoice->m_pCallbackArg;
    pVoice->Stop();
    pVoice->Free();
    if (pCallback != nullptr) {
        pCallback(pVoice, Voice::STATUS_DROP_VOICE, pCallbackArg);
    }
    return dropCount;
}

// 0x004CEB7C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    while (!m_VoiceList.empty()) {
        Voice* pVoice = VoiceList::GetPointerFromNode(m_VoiceList.begin().GetNode());
        pVoice->Stop();
        if (pVoice->m_pCallback != nullptr) {
            pVoice->m_pCallback(pVoice, Voice::STATUS_CANCEL, pVoice->m_pCallbackArg);
        }
        pVoice->Free();
    }
    while (!m_FreeList.empty()) {
        Voice* pVoice = VoiceList::GetPointerFromNode(m_FreeList.begin().GetNode());
        m_FreeList.erase(m_FreeList.begin());
        pVoice->~Voice();
    }
    m_IsInitialized = false;
}

// 0x004CEC1C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::FreeVoice(Voice* pVoice)
{
    m_VoiceList.erase(&pVoice->m_Link);
    m_FreeList.insert(m_FreeList.end(), &pVoice->m_Link);
}

// 0x007414D8 | nintendogs:bytes [tier A]
u32 nw::snd::internal::driver::VoiceManager::GetVoiceCount() const
{
    SoundThreadLock lock;
    u32 count = 0;
    for (ut::internal::LinkListImpl::iterator it = const_cast<VoiceList&>(m_VoiceList).begin(); it != const_cast<VoiceList&>(m_VoiceList).end();) {
        const Voice* pVoice = VoiceList::GetPointerFromNode(it.GetNode());
        ++it;
        count += pVoice->m_VoiceCount;
    }
    return count;
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
