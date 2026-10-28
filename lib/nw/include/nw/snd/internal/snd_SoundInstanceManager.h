#pragma once

#include "decomp.h"
#include <new>

#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
// The sounds of one kind in a buffer: the free ones and the ones in use, sorted by priority (lowest
// first) (the class name is from the reference symbols, the members and the inline functions are ours).
template <typename TSound>
class SoundInstanceManager
{
public:
    typedef ut::LinkList<TSound, 0xF8> SoundList;
    // the priorities of the sounds
    static const int PRIORITY_MAX = 127;

    // (inline)
    SoundInstanceManager() : m_pBuffer(nullptr), m_BufferSize(0) {}

    // (inline) the sounds in the buffer; their count
    u32 Create(void* pBuffer, u32 size)
    {
        const u32 count = size / sizeof(TSound);
        u8* p = static_cast<u8*>(pBuffer);
        for (u32 i = 0; i < count; i++) {
            TSound* pSound = p != nullptr ? new (p) TSound(*this) : nullptr;
            m_FreeList.PushBack(pSound);
            p += sizeof(TSound);
        }
        m_pBuffer = pBuffer;
        m_BufferSize = size;
        return count;
    }
    // (inline) the sounds are destroyed
    void Destroy()
    {
        const u32 count = m_BufferSize / sizeof(TSound);
        TSound* pSound = static_cast<TSound*>(m_pBuffer);
        for (u32 i = 0; i < count; i++) {
            pSound->~TSound();
            pSound++;
        }
        m_FreeList.erase(m_FreeList.begin(), m_FreeList.end());
        m_PriorityList.erase(m_PriorityList.begin(), m_PriorityList.end());
    }
    // (inline) a free sound; without one the sound of the lowest priority stops if it is not above priority
    // (NULL: none)
    TSound* Alloc(int priority, int ambientPriority)
    {
        int allocPriority = priority + ambientPriority;
        if (allocPriority > PRIORITY_MAX) {
            allocPriority = PRIORITY_MAX;
        } else if (allocPriority < 0) {
            allocPriority = 0;
        }
        TSound* pSound = nullptr;
        while (pSound == nullptr) {
            if (!m_FreeList.empty()) {
                pSound = SoundList::GetPointerFromNode(m_FreeList.begin().GetNode());
                m_FreeList.erase(m_FreeList.begin());
            } else {
                if (m_PriorityList.empty()) {
                    return nullptr;
                }
                TSound* pLowest = SoundList::GetPointerFromNode(m_PriorityList.begin().GetNode());
                if (pLowest == nullptr || pLowest->CalcCurrentPlayerPriority() > allocPriority) {
                    return nullptr;
                }
                pLowest->Stop(0);
            }
        }
        pSound->Initialize();
        pSound->SetPriority(priority, ambientPriority);
        InsertPriorityList(pSound, allocPriority);
        return pSound;
    }
    // (inline) the sound at the place of its priority
    void InsertPriorityList(TSound* pSound, int priority)
    {
        typename SoundList::iterator it = m_PriorityList.begin();
        for (; it != m_PriorityList.end(); ++it) {
            if (priority < SoundList::GetPointerFromNode(it.GetNode())->CalcCurrentPlayerPriority()) {
                break;
            }
        }
        m_PriorityList.insert(it, SoundList::GetNodeFromPointer(pSound));
    }
    // (inline) the priority list sorted again (the priorities of the players change)
    DECOMP_ALWAYS_INLINE void SortPriorityList()
    {
        if (m_PriorityList.size() < 2) {
            return;
        }
        SoundList lists[PRIORITY_MAX + 1];
        while (!m_PriorityList.empty()) {
            TSound* pSound = SoundList::GetPointerFromNode(m_PriorityList.begin().GetNode());
            m_PriorityList.erase(m_PriorityList.begin());
            lists[pSound->CalcCurrentPlayerPriority()].PushBack(pSound);
        }
        for (int i = 0; i < PRIORITY_MAX + 1; i++) {
            while (!lists[i].empty()) {
                ut::LinkListNode* pNode = lists[i].begin().GetNode();
                lists[i].erase(lists[i].begin());
                m_PriorityList.insert(m_PriorityList.end(), pNode);
            }
        }
    }

    // (inline) the sound moves to the place of its new priority
    void UpdatePriority(TSound* pSound, int priority)
    {
        m_PriorityList.Erase(pSound);
        typename SoundList::iterator it = m_PriorityList.begin();
        for (; it != m_PriorityList.end(); ++it) {
            if (priority < SoundList::GetPointerFromNode(it.GetNode())->CalcCurrentPlayerPriority()) {
                break;
            }
        }
        m_PriorityList.insert(it, SoundList::GetNodeFromPointer(pSound));
    }
    // (inline) the sound goes back to the free list
    void Free(TSound* pSound)
    {
        m_PriorityList.Erase(pSound);
        pSound->Finalize();
        m_FreeList.PushBack(pSound);
    }

    void* m_pBuffer;          // 0x00
    u32 m_BufferSize;         // 0x04
    SoundList m_PriorityList; // 0x08
    SoundList m_FreeList;     // 0x14
};
} // namespace internal
} // namespace snd
} // namespace nw
