#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
class IPacketInput;
class IPacketOutput;
} // namespace common
namespace transport {
class NetworkFactory;
class ReceiveThreadStream;
class SendThreadStream;

// RTTI N2nn3pia9transport19ThreadStreamManagerE @ 0x008D0200
// vtable 0x00901EB8 (vptr 0x00901EC0), offset_to_top 0, 1 entries
//
// The send and the receive thread with the input / output of the network factory. One instance
// (CreateInstance). The layout is from CreateInstance; the member names and the names marked so are
// ours. The destructor is not virtual (the vtable only has Trace).
class ThreadStreamManager : public ::nn::pia::common::RootObject
{
public:
    // the priority of both threads
    static const int THREAD_PRIORITY = 10;
    // the wait of the threads when there is nothing to do (ms)
    static const int THREAD_WAIT_MSEC = 5;

    // (inline in CreateInstance)
    ThreadStreamManager(NetworkFactory* pFactory, unsigned int sendPacketNum, unsigned int receivePacketNum, unsigned int sendLatencyPacketNum, unsigned int receiveLatencyPacketNum, bool isDropEnabled);
    // (inline in DestroyInstance)
    ~ThreadStreamManager();
    virtual void Trace(u64 flag) const; // 0x007360A4 slot 0x00

    static nn::Result CreateInstance(nn::pia::transport::NetworkFactory* pFactory, unsigned int sendPacketNum, unsigned int receivePacketNum, unsigned int sendLatencyPacketNum, unsigned int receiveLatencyPacketNum, bool isDropEnabled); // 0x00457F58 | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x004580A4 | fefates:bytes [tier B]
    nn::Result Startup(); // 0x0045821C | fefates:bytes [tier B]
    void Cleanup(); // 0x004581E0 | fefates:bytes [tier B]
    void SetMonitoringData(); // 0x00458164 | fefates:bytes [tier B]

    // (names are ours)
    void SetSendThreadPriority(int priority); // 0x004581A0
    void SetReceiveThreadPriority(int priority); // 0x004581B4
    void SetSendThreadWaitMSec(int waitMSec); // 0x004581C8
    void SetReceiveThreadWaitMSec(int waitMSec); // 0x004581D4

    static ThreadStreamManager* s_pInstance;

    common::IPacketInput* m_pInput;          // 0x04
    common::IPacketOutput* m_pOutput;        // 0x08
    ReceiveThreadStream* m_pReceiveStream;   // 0x0C
    SendThreadStream* m_pSendStream;         // 0x10
};
ASSERT_SIZE(ThreadStreamManager, 0x14);
} // namespace transport
} // namespace pia
} // namespace nn
