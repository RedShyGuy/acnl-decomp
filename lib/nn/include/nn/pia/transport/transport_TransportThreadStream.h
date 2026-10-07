#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_PacketStream.h"
#include "pead/peadDelegate2.h"
#include "pead/peadDelegateThread.h"
#include "pead/peadEvent.h"
#include "pead/peadRandom.h"

namespace nn {
namespace pia {
namespace transport {
class LatencyEmulator;

// RTTI N2nn3pia9transport21TransportThreadStreamE @ 0x008D0254
// vtable 0x00901F88 (vptr 0x00901F90), offset_to_top 0, 2 entries
//
// A thread that moves packets between the PacketStream and the network (SendThreadStream,
// ReceiveThreadStream): it calls ProcessOne while it runs and waits m_WaitMSec when there is
// nothing to do. Optionally the packets are delayed (LatencyEmulator) or dropped (m_DropRate %).
// The layout is from the constructor and InitializeCore; the member names and the names marked so
// are ours.
class TransportThreadStream : public ::nn::pia::common::RootObject
{
public:
    // m_State; ChangeState sets the state with THREAD_STATE_REQUEST and the thread takes it over
    enum ThreadState : u8
    {
        THREAD_STATE_WAITING = 1,
        THREAD_STATE_RUNNING = 2,
        THREAD_STATE_ERROR = 3,
        THREAD_STATE_FINISHED = 4,
        THREAD_STATE_REQUEST = 0x10,
    };

    TransportThreadStream(); // 0x0045BD04 | fefates:bytes [tier B]
    ~TransportThreadStream(); // 0x0045BD90 | fefates:bytes [tier B]

    // called by Startup (name is ours)
    virtual void ResetMonitoringData(); // 0x0045BD00 slot 0x00
    // one packet; RESULT_NO_DATA / RESULT_BUFFER_IS_FULL make the thread wait (name is ours)
    virtual nn::Result ProcessOne() = 0; // slot 0x04

    nn::Result InitializeCore(const char* pName, int priority, unsigned int packetNum, unsigned int latencyPacketNum, bool isDropEnabled, unsigned int watermarkIndex); // 0x0045BB50 | fefates:bytes [tier B]
    void FinalizeCore(); // 0x0045BA6C | fefates:bytes [tier B]
    nn::Result Startup(); // 0x0045BC98 | fefates:bytes [tier B]
    void Cleanup(); // 0x0045BC4C | fefates:bytes [tier B]
    // the error that stopped the thread
    nn::Result GetLastResult() const; // 0x00736548 | fefates:bytes [tier B]

    // waits until the thread has taken over the state
    void ChangeState(nn::pia::transport::TransportThreadStream::ThreadState state); // 0x0045B9D4 | fefates:bytes-fuzzy [tier B]
    // whether to drop the packet (m_DropRate %)
    bool isDropPacket(); // 0x0045BB14 | fefates:bytes [tier B]
    // the function of the thread (name is ours)
    void ThreadFunc(pead::Thread* pThread, int message); // 0x0045B868

    PacketStream m_PacketStream;                                    // 0x04
    pead::Delegate2<TransportThreadStream, pead::Thread*, int> m_Delegate; // 0x30
    pead::DelegateThread* m_pThread;                                // 0x40
    pead::Event m_Event;                                            // 0x44
    ThreadState m_State;                                            // 0x58
    s32 m_WaitMSec;                                                 // 0x5C
    nn::Result m_LastResult;                                        // 0x60
    LatencyEmulator* m_pLatencyEmulator;                            // 0x64
    s32 m_DropRate;                                                 // 0x68
    pead::Random m_Random;                                          // 0x6C
    bool m_IsDropEnabled;                                           // 0x7C
};
ASSERT_OFFSET(TransportThreadStream, m_PacketStream, 0x04);
ASSERT_OFFSET(TransportThreadStream, m_Delegate, 0x30);
ASSERT_OFFSET(TransportThreadStream, m_Event, 0x44);
ASSERT_OFFSET(TransportThreadStream, m_State, 0x58);
ASSERT_SIZE(TransportThreadStream, 0x80);
} // namespace transport
} // namespace pia
} // namespace nn
