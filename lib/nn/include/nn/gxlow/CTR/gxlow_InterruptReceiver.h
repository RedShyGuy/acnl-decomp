#pragma once

#include "decomp.h"
#include "nn/gxlow/CTR/gxlow_CmdReqQueueTx.h"
#include "nn/gxlow/CTR/gxlow_InterruptRelayQueueRx.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Event.h"
#include "nn/os/os_LightEvent.h"
#include "nn/os/os_SharedMemoryBlock.h"
#include "nn/os/os_Thread.h"

namespace nn {
namespace gxlow {
namespace CTR {
struct FramebufferInfoQueue;

// The connection to the GPU module: the shared memory with the interrupt queue, the command queue
// and the framebuffer infos, and a thread that calls the registered handler of each interrupt.
// One object (0x0094CC40, constructed in nn::gxlow::CTR::Initialize); the members are ours.
class InterruptReceiver
{
public:
    typedef void (*Handler)();

    static const u32 HANDLER_COUNT = 7;
    static const size_t STACK_SIZE = 0x1000;

    // what the thread reports to WaitAnyHandlerDone (values ours)
    enum HandlerState : u8
    {
        HANDLER_STATE_NONE = 0,
        HANDLER_STATE_WAITING = 1,
        HANDLER_STATE_DONE = 2,
    };

    void Initialize(); // 0x0013123C | nintendogs:callgraph [tier A]
    // waits until the thread has called a handler (returns at once if it did since the last call)
    void WaitAnyHandlerDone(); // 0x0013142C | fefates:bytes [tier B]
    static void ReceiverThreadFunc(uptr param); // 0x00137054 | fefates:bytes [tier B]

    nn::os::CriticalSection m_Lock;                   // 0x00
    u32 m_Unknown0C;                                  // 0x0C (not used)
    Handler m_Handlers[HANDLER_COUNT];                // 0x10, by nngxlowInterrupt
    nn::os::Event m_Event;                            // 0x2C, signalled by the module
    nn::gxlow::CTR::InterruptRelayQueueRx m_InterruptQueue; // 0x30
    nn::os::SharedMemoryBlock m_SharedMemory;         // 0x38
    u32 m_Unknown54;                                  // 0x54 (not used)
    nn::gxlow::CTR::CmdReqQueueTx m_CmdReqQueue;      // 0x58
    FramebufferInfoQueue* m_pFramebufferInfo[2];      // 0x5C, top and bottom screen
    nn::os::LightEvent m_HandlerDoneEvent;            // 0x64
    nn::os::Thread m_Thread;                          // 0x6C
    u8 m_ThreadIndex;                                 // 0x74, of this process in the shared memory
    bool m_IsFirstInitialization;                     // 0x75, the module was set up by this process
    HandlerState m_HandlerState;                      // 0x76
    bool m_IsStopRequested;                           // 0x77
    u8 m_Stack[STACK_SIZE];                           // 0x78, of m_Thread
};
ASSERT_OFFSET(InterruptReceiver, m_InterruptQueue, 0x30);
ASSERT_OFFSET(InterruptReceiver, m_CmdReqQueue, 0x58);
ASSERT_OFFSET(InterruptReceiver, m_Thread, 0x6C);
ASSERT_SIZE(InterruptReceiver, 0x1078);
} // namespace CTR
} // namespace gxlow
} // namespace nn
