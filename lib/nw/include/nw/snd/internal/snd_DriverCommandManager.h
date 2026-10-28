#pragma once

#include "decomp.h"
#include "nn/os/detail/os_BlockingQueueBase.h"
#include "nw/snd/internal/snd_DriverCommand.h"

namespace nw {
namespace snd {
namespace internal {
// Sends the commands of the game threads to the sound thread (the class and function names are from
// the reference symbols, the members are ours). The commands are allocated from a ring of words; a
// flush sends the list through a queue and the sound thread sends it back when it ran it, which frees
// its memory. There is one instance for the game threads and one for the task thread.
class DriverCommandManager
{
public:
    static const int SEND_QUEUE_SIZE = 32;
    static const int RECV_QUEUE_SIZE = 33;

    // A queue that finalizes itself before the base destructor does it again (the destructor calls
    // Finalize, then ~BlockingQueueBase, for each queue; name is ours).
    class Queue : public nn::os::detail::BlockingQueueBase<nn::os::CriticalSection>
    {
    public:
        ~Queue() { Finalize(); }
    };

    static DriverCommandManager& GetInstance(); // 0x001415C0 | nintendogs:callgraph [tier A]
    static DriverCommandManager& GetInstanceForTaskThread(); // 0x004C9288 | nintendogs:callgraph [tier A]

    DECOMP_NOINLINE DriverCommandManager(); // 0x001418BC | nintendogs:bytes-fuzzy [tier A]
    ~DriverCommandManager(); // 0x00143B88 | nintendogs:bytes [tier A]

    void Initialize(void* pBuffer, u32 size); // 0x004C8E88 | nintendogs:callseq [tier A]
    void Finalize(); // 0x004C92E4 | nintendogs:callseq [tier A]

    // words of the ring; NULL if they are not free now
    void* TryAllocMemory(u32 count); // 0x00141764 | fefates:bytes [tier B]
    // words of the ring; flushes and waits until they are free
    void* AllocMemory(u32 count); // 0x004C8EE4 | nintendogs:callseq-callee [tier A]
    // a command at the end of the list; the tag of the next flush
    u32 PushCommand(DriverCommand* pCommand); // 0x004C91AC | nintendogs:callseq-callee [tier A]
    // sends the list (force: waits for room in the queue, an empty list becomes a dummy command);
    // the tag of the list (0: nothing sent)
    u32 FlushCommand(bool isForce); // 0x00141618 | nintendogs:callgraph [tier A]
    // frees the memory of the lists the sound thread sent back
    void RecvCommandReply(); // 0x004C921C | nintendogs:callgraph [tier A]
    // until the list with the tag came back
    void WaitCommandReply(u32 tag); // 0x00141858 | nintendogs:callgraph [tier A]
    // runs the sent lists (in the sound thread)
    void ProcessCommand(); // 0x004C91D0 | nintendogs:bytes [tier A]

    // (inline) a command of the type (name is ours)
    template <typename T>
    T* AllocCommand()
    {
        T* pCommand = static_cast<T*>(AllocMemory(sizeof(T) / sizeof(u32)));
        pCommand->memoryNext = m_WriteIndex;
        return pCommand;
    }
    // (inline) the memory of a list that came back is free (name is ours)
    void ReleaseCommandList(DriverCommand* pCommand)
    {
        m_FinishedTag = pCommand->tag;
        if (pCommand != nullptr) {
            while (pCommand->next != nullptr) {
                pCommand = pCommand->next;
            }
            m_ReadIndex = pCommand->memoryNext;
            m_IsMemoryFull = false;
            if (m_WriteIndex == m_ReadIndex) {
                m_ReadIndex = 0;
                m_WriteIndex = 0;
            }
        }
    }

    Queue m_SendQueue;                                                      // 0x000
    uptr m_SendQueueBuffer[SEND_QUEUE_SIZE];                                // 0x034
    Queue m_RecvQueue;                                                      // 0x0B4
    uptr m_RecvQueueBuffer[RECV_QUEUE_SIZE];                                // 0x0E8
    DriverCommand* m_pListHead;                                             // 0x16C the commands of the next flush
    DriverCommand* m_pListTail;                                             // 0x170
    u32 m_SentTag;                                                          // 0x174 the tag of the next flush
    u32 m_FinishedTag;                                                      // 0x178 of the last list that came back
    u32* m_pMemory;                                                         // 0x17C
    u32 m_MemoryWordCount;                                                  // 0x180
    u32 m_WriteIndex;                                                       // 0x184 the next free word
    u32 m_ReadIndex;                                                        // 0x188 the first word in use
    bool m_IsMemoryFull;                                                    // 0x18C
    bool m_IsInitialized;                                                   // 0x18D
};
ASSERT_SIZE(DriverCommandManager, 0x190);
} // namespace internal
} // namespace snd
} // namespace nw
