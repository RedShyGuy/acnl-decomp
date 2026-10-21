#include "nn/nex/nex_TransportBufferMultiThread.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_QueuingSocketTransportBuffer.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_TransportBufferThread.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/os/os_Api.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace nex {
namespace {
// the sleep of the threads
const s64 SLEEP_NS = 2000000;
// the sockets that are reserved for
const u32 RESERVED_SOCKET_COUNT = 2;

// (inline) the priority of the thread of the OS
inline void SetThreadPriority(ObjectThreadRoot* pThread, int priority)
{
    pThread->m_Priority = priority;
    nn::os::Thread* pOsThread = pThread->m_pData->m_pThread;
    nn::Result result = nn::svc::SetThreadPriority(pOsThread->GetHandle(), nn::os::detail::ConvertLibraryToSvcPriority(priority));
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
}

// (inline) waits for the condition with the lock left
inline void WaitCondition(Condition& condition, CriticalSection& lock)
{
    condition.m_Lock.Enter();
    condition.m_WaiterCount++;
    condition.m_Lock.Leave();
    lock.Leave();
    condition.m_pEvent->Wait();
    lock.Enter();
    condition.m_Lock.Enter();
    condition.m_WaiterCount--;
    condition.m_Lock.Leave();
}
} // namespace

// 0x003BA788 slot 0x18
void nn::nex::TransportBufferMultiThread::SetPriority(int priority)
{
    for (int i = 0; i < SLOT_COUNT; i++) {
        if (m_pReceiveThreads[i] != nullptr) {
            SetThreadPriority(m_pReceiveThreads[i], priority);
        }
    }
    SetThreadPriority(m_pSendThread, priority);
}

// 0x003BA81C slot 0x14
void nn::nex::TransportBufferMultiThread::EraseSocketPort(unsigned short port)
{
    ScopedCS lock(m_Lock);
    SocketEntry* pEntry = m_Entries.m_pBegin;
    while (pEntry != m_Entries.m_pEnd && pEntry->m_Port != port) {
        pEntry++;
    }
    if (pEntry == m_Entries.m_pEnd) {
        return;
    }
    int slot = pEntry->m_Slot;
    m_IsSlotErasing[slot] = true;
    m_ErasingPort = port;
    QueuingSocket* pSocket = pEntry->m_pSocket;
    if (pSocket->m_State != Socket::STATE_CLOSED && pSocket->m_IsAvailable && pSocket->m_pSocket != nullptr) {
        pSocket->m_pSocket->Shutdown();
    }
    if (m_Entries.m_pBegin != m_Entries.m_pEnd) {
        m_Entries.erase(pEntry);
    }
    m_IsSlotUsed[slot] = false;
    m_IsChanging = true;
    m_pSlotLocks[slot]->Enter();
    m_Slots[slot].m_Port = 0;
    m_Slots[slot].m_pSocket = nullptr;
    m_IsSlotErasing[slot] = false;
    m_pSlotLocks[slot]->Leave();
    m_SendLock.Enter();
    std::pair<u16, QueuingSocket*>* pSend = m_SendSockets.m_pBegin;
    while (pSend != m_SendSockets.m_pEnd && pSend->first != port) {
        pSend++;
    }
    if (m_SendSockets.m_pBegin != m_SendSockets.m_pEnd) {
        m_SendSockets.erase(pSend);
    }
    m_ErasingPort = -1;
    m_SendLock.Leave();
    m_IsChanging = false;
}

// 0x003BAA6C slot 0x10
bool nn::nex::TransportBufferMultiThread::InsertSocketPort(unsigned short port, nn::nex::QueuingSocket* pSocket)
{
    ScopedCS lock(m_Lock);
    int slot = m_IsSlotUsed[0] ? -1 : 0;
    if (!m_IsSlotUsed[1]) {
        slot = 1;
    } else if (slot == -1) {
        return false;
    }
    if (m_pReceiveThreads[slot] == nullptr) {
        m_pReceiveThreads[slot] = new ObjectThread<TransportBufferMultiThread, int>(String(L"Transport Buffer Thread (recv)"));
        m_pReceiveThreads[slot]->m_Priority = g_TransportBufferThreadPriority;
        if (g_TransportBufferThreadCoreNo != ObjectThreadRoot::CORE_NO_DEFAULT) {
            m_pReceiveThreads[slot]->m_CoreNo = g_TransportBufferThreadCoreNo;
        }
        m_pReceiveThreads[slot]->SetMethod(this, &TransportBufferMultiThread::RunReceive, slot);
        m_pReceiveThreads[slot]->Launch(0);
    }
    SocketEntry entry;
    entry.m_Port = port;
    entry.m_Slot = slot;
    entry.m_pSocket = pSocket;
    m_Entries.push_back(entry);
    m_IsSlotUsed[slot] = true;
    m_IsChanging = true;
    m_pSlotLocks[slot]->Enter();
    m_Slots[slot].m_Port = port;
    m_Slots[slot].m_pSocket = pSocket;
    m_pSlotLocks[slot]->Leave();
    m_SendLock.Enter();
    m_SendSockets.push_back(std::pair<u16, QueuingSocket*>(port, pSocket));
    m_SendLock.Leave();
    m_IsChanging = false;
    return true;
}

// 0x003BAF54 (name is ours)
void nn::nex::TransportBufferMultiThread::RunReceive(int slot)
{
    if (PseudoSingleton::s_IsContextUsed) {
        PseudoSingleton::SetContext(m_Context);
    }
    while (!m_IsStopRequested) {
        bool isReceived = false;
        if (!m_IsSlotErasing[slot]) {
            m_pSlotLocks[slot]->Enter();
            if (m_Slots[slot].m_pSocket != nullptr) {
                isReceived = static_cast<QueuingSocketTransportBuffer*>(m_Slots[slot].m_pSocket)->ReceiveBuffers(false);
            }
            m_pSlotLocks[slot]->Leave();
        }
        if (!isReceived || m_IsChanging) {
            OsSleepThread(SLEEP_NS);
        }
    }
}

// 0x003BB010 (name is ours)
void nn::nex::TransportBufferMultiThread::RunSend(int unused)
{
    if (PseudoSingleton::s_IsContextUsed) {
        PseudoSingleton::SetContext(m_Context);
    }
    while (!m_IsStopRequested) {
        bool isSent = false;
        m_SendLock.Enter();
        for (std::pair<u16, QueuingSocket*>* p = m_SendSockets.m_pBegin; p != m_SendSockets.m_pEnd; p++) {
            if (p->second != nullptr && m_ErasingPort != p->first) {
                if (static_cast<QueuingSocketTransportBuffer*>(p->second)->SendBuffers()) {
                    isSent = true;
                }
            }
        }
        m_SendLock.Leave();
        if (m_IsChanging) {
            OsSleepThread(SLEEP_NS);
        }
        if (!isSent) {
            m_SendWaitLock.Enter();
            WaitCondition(m_SendCondition, m_SendWaitLock);
            m_SendWaitLock.Leave();
        }
    }
}

// 0x003BB148 slot 0x0C | fefates:callseq
void nn::nex::TransportBufferMultiThread::Stop()
{
    if (m_IsStopRequested) {
        return;
    }
    int count = m_Entries.size();
    m_IsStopRequested = true;
    u16 ports[SLOT_COUNT];
    for (int i = 0; i < count; i++) {
        ports[i] = m_Entries.m_pBegin[i].m_Port;
    }
    for (int i = 0; i < count; i++) {
        EraseSocketPort(ports[i]);
    }
    m_SendCondition.Signal();
    if (m_pSendThread->m_IsLaunched) {
        m_pSendThread->Wait(static_cast<unsigned int>(-1));
    }
    for (int i = 0; i < SLOT_COUNT; i++) {
        if (m_pReceiveThreads[i] != nullptr && m_pReceiveThreads[i]->m_IsLaunched) {
            m_pReceiveThreads[i]->Wait(static_cast<unsigned int>(-1));
        }
    }
}

// 0x003BB244 slot 0x08
bool nn::nex::TransportBufferMultiThread::Start()
{
    m_pSendThread->SetMethod(this, &TransportBufferMultiThread::RunSend, 0);
    return m_pSendThread->Launch(0);
}

// 0x003BB274
nn::nex::TransportBufferMultiThread::TransportBufferMultiThread()
    : m_IsStopRequested(false), m_IsChanging(false), m_Context(PseudoSingleton::GetCurrentContext()), m_Lock(LOCK_LEVEL), m_ErasingPort(-1), m_Entries(0),
      m_SendSockets(0), m_SendLock(SLOT_LOCK_LEVEL), m_SendWaitLock(SLOT_LOCK_LEVEL)
{
    m_Entries.reserve(RESERVED_SOCKET_COUNT);
    m_SendSockets.reserve(RESERVED_SOCKET_COUNT);
    for (int i = 0; i < SLOT_COUNT; i++) {
        m_pReceiveThreads[i] = nullptr;
        m_pSlotLocks[i] = new ForcedCriticalSection(SLOT_LOCK_LEVEL);
        m_IsSlotUsed[i] = false;
        m_IsSlotErasing[i] = false;
        m_Slots[i].m_Port = 0;
        m_Slots[i].m_pSocket = nullptr;
    }
    m_pSendThread = new ObjectThread<TransportBufferMultiThread, int>(String(L"Transport Buffer Thread (send)"));
    m_pSendThread->m_Priority = g_TransportBufferThreadSendPriority;
    if (g_TransportBufferThreadCoreNo != ObjectThreadRoot::CORE_NO_DEFAULT) {
        m_pSendThread->m_CoreNo = g_TransportBufferThreadCoreNo;
    }
}

// 0x003BB79C slot 0x00
// 0x003BB76C (deleting dtor)
nn::nex::TransportBufferMultiThread::~TransportBufferMultiThread()
{
    Stop();
    for (int i = 0; i < SLOT_COUNT; i++) {
        if (m_pReceiveThreads[i] != nullptr) {
            delete m_pReceiveThreads[i];
        }
        if (m_pSlotLocks[i] != nullptr) {
            delete m_pSlotLocks[i];
        }
    }
    if (m_pSendThread != nullptr) {
        delete m_pSendThread;
    }
}

// 0x0072DDDC slot 0x1C
int nn::nex::TransportBufferMultiThread::GetPriority()
{
    return m_pSendThread->m_Priority;
}

// 0x007D456C
template void nn::nex::ObjectThread<nn::nex::TransportBufferMultiThread, int>::Run();

// 0x007D45C4
// 0x007D45A0 (deleting dtor)
template nn::nex::ObjectThread<nn::nex::TransportBufferMultiThread, int>::~ObjectThread();

} // namespace nex
} // namespace nn
