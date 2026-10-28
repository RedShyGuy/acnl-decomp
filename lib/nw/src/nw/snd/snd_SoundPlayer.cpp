#include "nw/snd/snd_SoundPlayer.h"

#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/snd_PlayerHeap.h"
#include "nw/snd/snd_SoundHandle.h"

namespace nw {
namespace snd {
// 0x00137B6C | fefates:bytes [tier B]
void nw::snd::SoundPlayer::detail_SortPriorityList(internal::BasicSound* pSound)
{
    m_PriorityList.Erase(pSound);
    InsertPriorityList(pSound);
}

// 0x00137B94 | fefates:bytes [tier B]
void nw::snd::SoundPlayer::InsertPriorityList(internal::BasicSound* pSound)
{
    PriorityList::iterator it = m_PriorityList.begin();
    for (; it != m_PriorityList.end(); ++it) {
        const int priority = pSound->CalcCurrentPlayerPriority();
        const int other = PriorityList::GetPointerFromNode(it.GetNode())->CalcCurrentPlayerPriority();
        if (m_IsFirstComeBased) {
            if (priority <= other) {
                break;
            }
        } else {
            if (priority < other) {
                break;
            }
        }
    }
    m_PriorityList.insert(it, PriorityList::GetNodeFromPointer(pSound));
}

// 0x004BF6B8 | fefates:bytes [tier B]
void nw::snd::SoundPlayer::StopAllSound(int fadeFrames)
{
    for (SoundList::iterator it = m_SoundList.begin(); it != m_SoundList.end();) {
        internal::BasicSound* pSound = SoundList::GetPointerFromNode(it.GetNode());
        ++it;
        pSound->Stop(fadeFrames);
    }
}

// 0x004BF6F4 (name after mk7dlp)
void nw::snd::SoundPlayer::PauseAllSound(bool isPause, int fadeFrames)
{
    for (SoundList::iterator it = m_SoundList.begin(); it != m_SoundList.end();) {
        internal::BasicSound* pSound = SoundList::GetPointerFromNode(it.GetNode());
        ++it;
        pSound->Pause(isPause, fadeFrames);
    }
}

// 0x004BF738 (name is ours)
void nw::snd::SoundPlayer::SetBiquadFilter(int type, f32 value)
{
    m_BiquadType = type;
    m_BiquadValue = value;
}

// 0x004BF744 | fefates:bytes [tier B]
bool nw::snd::SoundPlayer::detail_AppendSound(internal::BasicSound* pSound)
{
    const int priority = pSound->CalcCurrentPlayerPriority();
    if (m_PlayableCount == 0) {
        return false;
    }
    // the sounds with a lower priority make room
    while (m_SoundList.size() >= m_PlayableCount) {
        internal::BasicSound* pLowest = GetLowestPrioritySound();
        if (pLowest == nullptr) {
            return false;
        }
        const int lowest = pLowest->CalcCurrentPlayerPriority();
        if (m_IsFirstComeBased) {
            if (priority <= lowest) {
                return false;
            }
        } else {
            if (priority < lowest) {
                return false;
            }
        }
        pLowest->Finalize();
    }
    m_SoundList.PushBack(pSound);
    InsertPriorityList(pSound);
    pSound->m_pSoundPlayer = this;
    return true;
}

// 0x004BF804 | fefates:bytes [tier B]
bool nw::snd::SoundPlayer::detail_CanPlaySound(int priority)
{
    if (m_PlayableCount == 0) {
        return false;
    }
    if (m_PlayableCount > m_SoundList.size()) {
        return true;
    }
    internal::BasicSound* pLowest = GetLowestPrioritySound();
    if (pLowest == nullptr) {
        return false;
    }
    const int lowest = pLowest->CalcCurrentPlayerPriority();
    if (m_IsFirstComeBased) {
        return priority > lowest;
    }
    return lowest <= priority;
}

// 0x004BF86C | fefates:bytes [tier B]
void nw::snd::SoundPlayer::SetPlayableSoundCount(int count)
{
    if (count > m_PlayableLimit) {
        count = m_PlayableLimit;
    } else if (count < 0) {
        count = 0;
    }
    m_PlayableCount = count;
    while (m_SoundList.size() > m_PlayableCount) {
        GetLowestPrioritySound()->Finalize();
    }
}

// 0x004BF8B8 | fefates:bytes-fuzzy [tier B]
void nw::snd::SoundPlayer::detail_FreePlayerHeap(internal::PlayerHeap* pHeap)
{
    if (pHeap->m_State == internal::PlayerHeap::STATE_TASK_FINISHED) {
        m_PlayerHeapFreeList.PushBack(pHeap);
    } else {
        // the load task still uses it
        m_PlayerHeapUsedList.PushBack(pHeap);
    }
}

// 0x004BF8EC | fefates:bytes [tier B]
internal::PlayerHeap* nw::snd::SoundPlayer::detail_AllocPlayerHeap()
{
    if (m_PlayerHeapFreeList.empty()) {
        return nullptr;
    }
    ut::LinkListNode* pNode = m_PlayerHeapFreeList.begin().GetNode();
    m_PlayerHeapFreeList.erase(m_PlayerHeapFreeList.begin());
    return PlayerHeapList::GetPointerFromNode(pNode);
}

// 0x004BF918 | fefates:bytes [tier B]
void nw::snd::SoundPlayer::detail_AppendPlayerHeap(internal::PlayerHeap* pHeap)
{
    pHeap->m_pPlayer = this;
    m_PlayerHeapFreeList.PushBack(pHeap);
    ++m_PlayerHeapCount;
}

// 0x004BF94C | fefates:bytes [tier B]
void nw::snd::SoundPlayer::Update()
{
    // the heaps whose task is done are free
    for (PlayerHeapList::iterator it = m_PlayerHeapUsedList.begin(); it != m_PlayerHeapUsedList.end();) {
        internal::PlayerHeap* pHeap = PlayerHeapList::GetPointerFromNode(it.GetNode());
        ++it;
        if (pHeap->m_State == internal::PlayerHeap::STATE_TASK_FINISHED) {
            m_PlayerHeapUsedList.Erase(pHeap);
            m_PlayerHeapFreeList.PushBack(pHeap);
        }
    }
    for (SoundList::iterator it = m_SoundList.begin(); it != m_SoundList.end();) {
        internal::BasicSound* pSound = SoundList::GetPointerFromNode(it.GetNode());
        ++it;
        pSound->Update();
    }
    detail_SortPriorityList(false);
}

// 0x004BF9D8 | fefates:bytes [tier B]
DECOMP_NOINLINE void nw::snd::SoundPlayer::detail_SortPriorityList(bool reverse)
{
    if (m_PriorityList.size() < 2) {
        return;
    }
    // the sounds by priority
    // 0x00AE52B8
    static PriorityList s_SortPriorityLists[PRIORITY_COUNT];
    while (!m_PriorityList.empty()) {
        ut::LinkListNode* pNode = m_PriorityList.begin().GetNode();
        internal::BasicSound* pSound = PriorityList::GetPointerFromNode(pNode);
        m_PriorityList.erase(m_PriorityList.begin());
        PriorityList& list = s_SortPriorityLists[pSound->CalcCurrentPlayerPriority()];
        list.insert(list.end(), pNode);
    }
    for (int i = 0; i < PRIORITY_COUNT; ++i) {
        PriorityList& list = s_SortPriorityLists[i];
        while (!list.empty()) {
            ut::LinkListNode* pNode;
            if (reverse) {
                pNode = list.m_Node.m_pPrev;
            } else {
                pNode = list.begin().GetNode();
            }
            list.erase(PriorityList::iterator(pNode));
            m_PriorityList.insert(m_PriorityList.end(), pNode);
        }
    }
}

// 0x004BFAF8 (name is ours)
void nw::snd::SoundPlayer::SetFxSend(AuxBus bus, f32 send)
{
    m_FxSend[bus] = send;
}

// 0x004BFB44 | fefates:bytes [tier B]
void nw::snd::SoundPlayer::SetVolume(f32 volume)
{
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    m_Volume = volume;
}

// 0x004BFB60 | fefates:bytes [tier B]
nw::snd::SoundPlayer::SoundPlayer()
    : m_PlayableCount(1), m_PlayableLimit(0x7FFFFFFF), m_PlayerHeapCount(0), m_Volume(1.0f), m_LpfFreq(0.0f), m_BiquadType(-1),
      m_BiquadValue(0.0f), m_MainSend(0.0f), m_IsFirstComeBased(false)
{
    m_FxSend[0] = 0.0f;
    m_FxSend[1] = 0.0f;
}

// 0x004C4E98 | nintendogs:callseq [tier A]
void nw::snd::SoundPlayer::detail_RemoveSound(internal::BasicSound* pSound)
{
    m_PriorityList.Erase(pSound);
    m_SoundList.Erase(pSound);
    pSound->DetachSoundPlayer(this);
}

template <typename Function>
Function SoundPlayer::ForEachSound(Function function, bool reverse)
{
    if (reverse) {
        ut::LinkListNode* pNode = m_SoundList.end().GetNode();
        while (pNode != m_SoundList.begin().GetNode()) {
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle(SoundList::GetPointerFromNode(pNode->m_pPrev));
            function(handle);
            // (a stopped sound left the list: its predecessor is the next one)
            if (handle.m_pSound != nullptr) {
                pNode = pNode->m_pPrev;
            }
            handle.DetachSound();
        }
    } else {
        SoundList::iterator it = m_SoundList.begin();
        while (it != m_SoundList.end()) {
            internal::BasicSound* pSound = SoundList::GetPointerFromNode(it.GetNode());
            ++it;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle(pSound);
            function(handle);
            handle.DetachSound();
        }
    }
    return function;
}

// the function the game passes (name is ours)
typedef void (*SoundHandleFunction)(nw::snd::SoundHandle&);
// 0x007E776C (name is ours)
template SoundHandleFunction nw::snd::SoundPlayer::ForEachSound<void (*)(nw::snd::SoundHandle&)>(SoundHandleFunction function, bool reverse);

} // namespace snd
namespace ut {
// (the copy for the array of the priority lists)
// 0x007E5DE0 (name is ours)
template nw::ut::LinkList<nw::snd::internal::BasicSound, 264l>::LinkList();
} // namespace ut
} // namespace nw
