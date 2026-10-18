#pragma once

#include "decomp.h"
#include "nn/nex/nex_Condition.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_ObjectThread.h"
#include "nn/nex/nex_TransportBufferThreadInterface.h"
#include "nn/nex/nex_qVector.h"

#include <utility>

namespace nn {
namespace nex {
// The transport buffer threads with one receive thread per socket (at most two sockets) and one
// send thread for all of them. The names of the members and of the functions without a reference
// name are ours.
// RTTI N2nn3nex26TransportBufferMultiThreadE @ 0x008CEF94
// vtable 0x008FEE38 (vptr 0x008FEE40), offset_to_top 0, 8 entries
class TransportBufferMultiThread : public ::nn::nex::TransportBufferThreadInterface
{
public:
    // the sockets that have a receive thread
    static const int SLOT_COUNT = 2;
    static const u32 LOCK_LEVEL = 0x8000;
    static const u32 SLOT_LOCK_LEVEL = 0x10000;

    // a socket and its receive thread
    struct SocketEntry
    {
        u16 m_Port;               // 0x00
        int m_Slot;               // 0x04
        QueuingSocket* m_pSocket; // 0x08
    };
    // the socket of a receive thread
    struct Slot
    {
        // (inline)
        Slot() : m_Port(0), m_pSocket(nullptr) {}

        u16 m_Port;               // 0x00
        QueuingSocket* m_pSocket; // 0x04
    };

    TransportBufferMultiThread(); // 0x003BB274 (symbols.json names it the constructor of ObjectThread<TransportBufferMultiThread, int>)
    virtual ~TransportBufferMultiThread(); // 0x003BB79C slot 0x00
    // 0x003BB76C slot 0x04 (deleting dtor)
    virtual bool Start(); // 0x003BB244 slot 0x08
    virtual void Stop(); // 0x003BB148 slot 0x0C | fefates:callseq
    virtual bool InsertSocketPort(unsigned short port, nn::nex::QueuingSocket* pSocket); // 0x003BAA6C slot 0x10
    virtual void EraseSocketPort(unsigned short port); // 0x003BA81C slot 0x14
    virtual void SetPriority(int priority); // 0x003BA788 slot 0x18
    virtual int GetPriority(); // 0x0072DDDC slot 0x1C

    // the function of a receive thread
    void RunReceive(int slot); // 0x003BAF54
    // the function of the send thread
    void RunSend(int unused); // 0x003BB010

    bool m_IsStopRequested;                                        // 0x04
    bool m_IsChanging;                                             // 0x05 (a socket is inserted or erased)
    u32 m_Context;                                                 // 0x08 (the pseudo singleton context of the creator)
    ForcedCriticalSection m_Lock;                                  // 0x0C
    bool m_IsSlotUsed[SLOT_COUNT];                                 // 0x34
    bool m_IsSlotErasing[SLOT_COUNT];                              // 0x36
    s32 m_ErasingPort;                                             // 0x38 (-1: none)
    qVector<SocketEntry> m_Entries;                                // 0x3C
    ForcedCriticalSection* m_pSlotLocks[SLOT_COUNT];               // 0x48
    Slot m_Slots[SLOT_COUNT];                                      // 0x50
    qVector<std::pair<u16, QueuingSocket*> > m_SendSockets;        // 0x60
    ForcedCriticalSection m_SendLock;                              // 0x6C
    ObjectThread<TransportBufferMultiThread, int>* m_pReceiveThreads[SLOT_COUNT]; // 0x94
    Condition m_SendCondition;                                     // 0x9C (signalled when there is something to send)
    ForcedCriticalSection m_SendWaitLock;                          // 0xD4
    ObjectThread<TransportBufferMultiThread, int>* m_pSendThread;  // 0xFC
};
ASSERT_OFFSET(TransportBufferMultiThread, m_Entries, 0x3C);
ASSERT_OFFSET(TransportBufferMultiThread, m_SendSockets, 0x60);
ASSERT_OFFSET(TransportBufferMultiThread, m_SendCondition, 0x9C);
ASSERT_SIZE(TransportBufferMultiThread, 0x100);
} // namespace nex
} // namespace nn
