#pragma once

#include "decomp.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_ObjectThread.h"
#include "nn/nex/nex_TransportBufferThreadInterface.h"
#include "nn/nex/nex_qSortedVector.h"

namespace nn {
namespace nex {
// One thread that sends and fills the transport buffer sets of all sockets in turn. The names of
// the members and of the functions without a reference name are ours.
// RTTI N2nn3nex21TransportBufferThreadE @ 0x008CEB50
// vtable 0x008FDF10 (vptr 0x008FDF18), offset_to_top 0, 8 entries
class TransportBufferThread : public ::nn::nex::TransportBufferThreadInterface
{
public:
    static const u32 LOCK_LEVEL = 0x8000;

    // (inline in SocketTransport)
    TransportBufferThread();
    virtual ~TransportBufferThread(); // 0x0039AD70 slot 0x00
    // 0x0039AC84 slot 0x04 (deleting dtor)
    virtual bool Start(); // 0x0039AC58 slot 0x08
    virtual void Stop(); // 0x0039AC34 slot 0x0C
    virtual bool InsertSocketPort(unsigned short port, nn::nex::QueuingSocket* pSocket); // 0x0039AB14 slot 0x10
    virtual void EraseSocketPort(unsigned short port); // 0x0039AAD4 slot 0x14
    virtual void SetPriority(int priority); // 0x0039A998 slot 0x18
    virtual int GetPriority(); // 0x0072CE40 slot 0x1C

    // the function of the thread
    void Run(void* pArgument); // 0x0039A9D4

    bool m_IsStopRequested;                           // 0x04
    u32 m_Context;                                    // 0x08 (the pseudo singleton context of the creator)
    ForcedCriticalSection m_Lock;                     // 0x0C
    qSortedVector<u16, QueuingSocket*> m_Sockets;     // 0x34
    ObjectThread<TransportBufferThread, void*> m_Thread; // 0x44
};
ASSERT_OFFSET(TransportBufferThread, m_Sockets, 0x34);
ASSERT_OFFSET(TransportBufferThread, m_Thread, 0x44);
ASSERT_SIZE(TransportBufferThread, 0x78);

// the settings of the transport buffer threads (the names are ours)
extern u8 g_TransportBufferThreadMode;         // 1: TransportBufferMultiThread
extern bool g_UsesTransportBuffer;
extern u32 g_TransportBufferCount;
extern u32 g_TransportBufferThreadSleepTime;   // milliseconds between the rounds (0: a yield)
extern s32 g_TransportBufferThreadPriority;
extern s32 g_TransportBufferThreadSendPriority;       // (TransportBufferMultiThread)
extern u32 g_TransportReceiveCount;            // the buffers received per socket and round
extern u32 g_TransportSendCount;               // the buffers sent per socket and round
extern s32 g_TransportBufferThreadCoreNo;
} // namespace nex
} // namespace nn
