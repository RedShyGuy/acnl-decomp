#include "nn/nex/nex_TransportBufferThread.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_QueuingSocketTransportBuffer.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/os/os_Api.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace nex {
namespace {
// the sleep while there is no socket
const s64 IDLE_SLEEP_NS = 100000000;
const s64 NS_PER_MS = 1000000;
} // namespace

// 0x0096C920 (name is ours)
u8 g_TransportBufferThreadMode = 1;
// 0x0096C930 (name is ours)
bool g_UsesTransportBuffer = true;
// 0x0096C9C4 (name is ours)
u32 g_TransportBufferCount = 64;
// 0x0096C9C8 (name is ours)
u32 g_TransportBufferThreadSleepTime = 2;
// 0x0096C9CC (name is ours)
s32 g_TransportBufferThreadPriority = 10;
// 0x0096C9D0 (name is ours)
s32 g_TransportBufferThreadSendPriority = 10;
// 0x0096C9D4 (name is ours)
u32 g_TransportReceiveCount = 32;
// 0x0096C9D8 (name is ours)
u32 g_TransportSendCount = 32;
// 0x0096C9DC (name is ours)
s32 g_TransportBufferThreadCoreNo = ObjectThreadRoot::CORE_NO_DEFAULT;

// (inline in SocketTransport)
nn::nex::TransportBufferThread::TransportBufferThread()
    : m_IsStopRequested(false), m_Context(PseudoSingleton::GetCurrentContext()), m_Lock(LOCK_LEVEL), m_Thread(String(L"Transport Buffer Thread"))
{
    m_Thread.m_Priority = g_TransportBufferThreadPriority;
    if (g_TransportBufferThreadCoreNo != ObjectThreadRoot::CORE_NO_DEFAULT) {
        m_Thread.m_CoreNo = g_TransportBufferThreadCoreNo;
    }
}

// 0x0039A998 slot 0x18
void nn::nex::TransportBufferThread::SetPriority(int priority)
{
    m_Thread.m_Priority = priority;
    nn::os::Thread* pThread = m_Thread.m_pData->m_pThread;
    nn::Result result = nn::svc::SetThreadPriority(pThread->GetHandle(), nn::os::detail::ConvertLibraryToSvcPriority(priority));
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
}

// 0x0039A9D4
void nn::nex::TransportBufferThread::Run(void* pArgument)
{
    if (PseudoSingleton::s_IsContextUsed) {
        PseudoSingleton::SetContext(m_Context);
    }
    while (!m_IsStopRequested) {
        if (m_Sockets.size() == 0) {
            OsSleepThread(IDLE_SLEEP_NS);
            continue;
        }
        if (g_TransportBufferThreadSleepTime != 0) {
            OsSleepThread(static_cast<s64>(static_cast<u64>(g_TransportBufferThreadSleepTime) * NS_PER_MS));
        } else {
            OsYieldThread();
        }
        ScopedCS lock(m_Lock);
        for (qSortedVector<u16, QueuingSocket*>::iterator it = m_Sockets.begin(); it != m_Sockets.end(); it++) {
            QueuingSocketTransportBuffer* pSocket = static_cast<QueuingSocketTransportBuffer*>(it->second);
            pSocket->ReceiveBuffers(true);
            pSocket->SendBuffers();
        }
    }
}

// 0x0039AAD4 slot 0x14
void nn::nex::TransportBufferThread::EraseSocketPort(unsigned short port)
{
    ScopedCS lock(m_Lock);
    m_Sockets.erase(port);
}

// 0x0039AB14 slot 0x10
bool nn::nex::TransportBufferThread::InsertSocketPort(unsigned short port, nn::nex::QueuingSocket* pSocket)
{
    ScopedCS lock(m_Lock);
    m_Sockets.insert(std::pair<u16, QueuingSocket*>(port, pSocket));
    return true;
}

// 0x0039AC34 slot 0x0C
void nn::nex::TransportBufferThread::Stop()
{
    if (m_Thread.m_IsLaunched) {
        m_IsStopRequested = true;
        m_Thread.Wait(static_cast<unsigned int>(-1));
    }
}

// 0x0039AC58 slot 0x08
bool nn::nex::TransportBufferThread::Start()
{
    m_IsStopRequested = false;
    m_Thread.SetMethod(this, &TransportBufferThread::Run, nullptr);
    return m_Thread.Launch(0);
}

// 0x0039AD70 slot 0x00
// 0x0039AC84 (deleting dtor)
nn::nex::TransportBufferThread::~TransportBufferThread()
{
    Stop();
}

// 0x0072CE40 slot 0x1C
int nn::nex::TransportBufferThread::GetPriority()
{
    return m_Thread.m_Priority;
}

// 0x007D4510
template void nn::nex::ObjectThread<nn::nex::TransportBufferThread, void*>::Run();

// 0x007D4568
// 0x007D4544 (deleting dtor)
template nn::nex::ObjectThread<nn::nex::TransportBufferThread, void*>::~ObjectThread();

} // namespace nex
} // namespace nn
