#include "nw/snd/internal/snd_DriverCommandManager.h"

#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_Thread.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// how long FlushCommand sleeps while the ring is full (ns)
const s64 ALLOC_RETRY_SLEEP_NS = 1000000;
} // namespace

// 0x001415C0 | nintendogs:callgraph [tier A]
nw::snd::internal::DriverCommandManager& nw::snd::internal::DriverCommandManager::GetInstance()
{
    // 0x00AF0988
    static DriverCommandManager s_DriverCommandManager;
    return s_DriverCommandManager;
}

// 0x00141618 | nintendogs:callgraph [tier A]
u32 nw::snd::internal::DriverCommandManager::FlushCommand(bool isForce)
{
    if (m_pListHead == nullptr) {
        if (!isForce) {
            return 0;
        }
        DriverCommand* pCommand = static_cast<DriverCommand*>(TryAllocMemory(sizeof(DriverCommand) / sizeof(u32)));
        while (pCommand == nullptr) {
            nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromNanoSeconds(ALLOC_RETRY_SLEEP_NS));
            RecvCommandReply();
            pCommand = static_cast<DriverCommand*>(TryAllocMemory(sizeof(DriverCommand) / sizeof(u32)));
        }
        pCommand->memoryNext = m_WriteIndex;
        pCommand->id = DriverCommand::ID_DUMMY;
        PushCommand(pCommand);
    }

    DriverCommand* pList = m_pListHead;
    const u32 tag = m_SentTag;
    nn::os::detail::DataMemoryBarrier();
    if (isForce) {
        m_SendQueue.Enqueue(reinterpret_cast<uptr>(pList));
    } else if (!m_SendQueue.TryEnqueue(reinterpret_cast<uptr>(pList))) {
        return 0;
    }
    m_pListHead->tag = tag;
    ++m_SentTag;
    m_pListHead = nullptr;
    m_pListTail = nullptr;
    return tag;
}

// 0x00141764 | fefates:bytes [tier B]
void* nw::snd::internal::DriverCommandManager::TryAllocMemory(u32 count)
{
    if (m_IsMemoryFull) {
        return nullptr;
    }
    void* pMemory = nullptr;
    const u32 writeIndex = m_WriteIndex;
    const u32 readIndex = m_ReadIndex;
    if (readIndex > writeIndex) {
        // up to the part in use
        if (writeIndex + count > readIndex) {
            return nullptr;
        }
        m_WriteIndex = writeIndex + count;
        pMemory = &m_pMemory[writeIndex];
    } else if (writeIndex + count <= m_MemoryWordCount) {
        // up to the end
        m_WriteIndex = writeIndex + count;
        pMemory = &m_pMemory[writeIndex];
    } else if (count <= readIndex) {
        // from the start
        m_WriteIndex = count;
        pMemory = m_pMemory;
    }
    if (pMemory != nullptr && m_WriteIndex == readIndex) {
        m_IsMemoryFull = true;
    }
    return pMemory;
}

// 0x00141858 | nintendogs:callgraph [tier A]
void nw::snd::internal::DriverCommandManager::WaitCommandReply(u32 tag)
{
    DriverCommand* pCommand;
    do {
        pCommand = reinterpret_cast<DriverCommand*>(m_RecvQueue.Dequeue());
        ReleaseCommandList(pCommand);
    } while (pCommand->tag != tag);
}

// 0x001418BC | nintendogs:bytes-fuzzy [tier A]
nw::snd::internal::DriverCommandManager::DriverCommandManager() : m_IsInitialized(false)
{
}

// 0x00143B88 | nintendogs:bytes [tier A]
nw::snd::internal::DriverCommandManager::~DriverCommandManager()
{
    // (only the queues are destroyed)
}

// 0x004C8E88 | nintendogs:callseq [tier A]
void nw::snd::internal::DriverCommandManager::Initialize(void* pBuffer, u32 size)
{
    m_pListHead = nullptr;
    m_pListTail = nullptr;
    m_SentTag = 0;
    m_WriteIndex = 0;
    m_ReadIndex = 0;
    m_IsMemoryFull = false;
    m_pMemory = static_cast<u32*>(pBuffer);
    m_MemoryWordCount = size / sizeof(u32);
    m_SendQueue.Initialize(m_SendQueueBuffer, SEND_QUEUE_SIZE);
    m_RecvQueue.Initialize(m_RecvQueueBuffer, RECV_QUEUE_SIZE);
    m_IsInitialized = true;
}

// 0x004C8EE4 | nintendogs:callseq-callee [tier A]
void* nw::snd::internal::DriverCommandManager::AllocMemory(u32 count)
{
    void* pMemory = TryAllocMemory(count);
    if (pMemory != nullptr) {
        return pMemory;
    }
    RecvCommandReply();
    pMemory = TryAllocMemory(count);
    if (pMemory != nullptr) {
        return pMemory;
    }
    for (;;) {
        const u32 tag = FlushCommand(true);
        WaitCommandReply(tag);
        pMemory = TryAllocMemory(count);
        if (pMemory != nullptr) {
            return pMemory;
        }
    }
}

// 0x004C91AC | nintendogs:callseq-callee [tier A]
u32 nw::snd::internal::DriverCommandManager::PushCommand(DriverCommand* pCommand)
{
    if (m_pListTail == nullptr) {
        m_pListHead = pCommand;
    } else {
        m_pListTail->next = pCommand;
    }
    m_pListTail = pCommand;
    pCommand->next = nullptr;
    return m_SentTag;
}

// 0x004C91D0 | nintendogs:bytes [tier A]
void nw::snd::internal::DriverCommandManager::ProcessCommand()
{
    uptr message;
    while (m_SendQueue.TryDequeue(&message)) {
        DriverCommand::ProcessCommandList(reinterpret_cast<DriverCommand*>(message));
        nn::os::detail::DataMemoryBarrier();
        m_RecvQueue.Enqueue(message);
    }
}

// 0x004C921C | nintendogs:callgraph [tier A]
void nw::snd::internal::DriverCommandManager::RecvCommandReply()
{
    uptr message;
    while (m_RecvQueue.TryDequeue(&message)) {
        ReleaseCommandList(reinterpret_cast<DriverCommand*>(message));
    }
}

// 0x004C9288 | nintendogs:callgraph [tier A]
nw::snd::internal::DriverCommandManager& nw::snd::internal::DriverCommandManager::GetInstanceForTaskThread()
{
    // 0x00AF0B18
    static DriverCommandManager s_InstanceForTaskThread;
    return s_InstanceForTaskThread;
}

// 0x004C92E4 | nintendogs:callseq [tier A]
void nw::snd::internal::DriverCommandManager::Finalize()
{
    m_SendQueue.Finalize();
    m_RecvQueue.Finalize();
    m_IsInitialized = false;
}

} // namespace internal
} // namespace snd
} // namespace nw
