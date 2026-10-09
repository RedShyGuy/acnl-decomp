#include "nn/gxlow/CTR/gxlow_InterruptReceiver.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/gxlow/CTR/detail/detail_Api.h"
#include "nn/gxlow/CTR/gxlow_Gpu.h"
#include "nn/os/os_Atomic.h"

namespace nn {
namespace gxlow {
namespace CTR {
namespace {
// results (module 10; the names are ours)
const bit32 RESULT_QUEUE_NOT_INITIALIZED = 0xD8A02BF8; // permanent, invalid state, 1016
const bit32 RESULT_INTERRUPTS_LOST = 0xD8A02A02;       // permanent, invalid state, 514
const bit32 RESULT_QUEUE_EMPTY = 0x00002BEF;           // success, 1007
// RegisterInterruptRelayQueue: this process is the first to use the module
const bit32 RESULT_FIRST_INITIALIZATION = 0x00002A07;

// RegisterInterruptRelayQueue flags
const u32 RELAY_FLAG_APPLICATION = 1 << 0;
const u32 RELAY_FLAG_APPLET = 1 << 1;
const u32 RELAY_FLAG_FATAL_ERR_MODE = 1 << 2;

// the shared memory of the module: per thread the interrupt queue, the framebuffer infos of the
// two screens and the command queue (3dbrew "GSP Shared Memory")
const size_t SHARED_MEMORY_SIZE = 0x1000;
const uptr INTERRUPT_QUEUE_OFFSET = 0x000;
const size_t INTERRUPT_QUEUE_SIZE = 0x40;
const uptr FRAMEBUFFER_INFO_OFFSET = 0x200;
const size_t FRAMEBUFFER_INFO_SIZE = 0x80;
const size_t FRAMEBUFFER_INFO_SCREEN_SIZE = 0x40;
const uptr CMD_REQ_QUEUE_OFFSET = 0x800;
const size_t CMD_REQ_QUEUE_SIZE = 0x200;

// the interrupt ids follow the header word
const u32 INTERRUPT_IDS_OFFSET = 0x0C;

// library priority 2 of the first system range (see nn::os::detail::ConvertLibraryToSvcPriority)
const s32 THREAD_PRIORITY = 0x5109D502;
} // namespace

// (inline: only in ReceiverThreadFunc)
inline nn::Result nn::gxlow::CTR::InterruptRelayQueueRx::TryDequeue(u8* pId)
{
    if (m_pQueue == 0) {
        return RESULT_QUEUE_NOT_INITIALIZED;
    }
    volatile u8* bytes = reinterpret_cast<volatile u8*>(m_pQueue);
    if (bytes[1] == 0) {
        return RESULT_QUEUE_EMPTY;
    }
    *pId = bytes[INTERRUPT_IDS_OFFSET + bytes[0]];
    s32 header;
    do {
        header = nn::os::detail::LoadExclusive(m_pQueue);
        u32 first = ((header & 0xFF) + 1) % CAPACITY;
        header = (header & ~0xFF) | first;
        u32 count = ((header >> 8) & 0xFF) - 1;
        header = (header & ~0xFF00) | ((count << 8) & 0xFF00);
    } while (nn::os::detail::StoreExclusive(m_pQueue, header));
    if (bytes[2] == 1) {
        return RESULT_INTERRUPTS_LOST;
    }
    return nn::Result();
}

// 0x0013123C | nintendogs:callgraph [tier A]
void nn::gxlow::CTR::InterruptReceiver::Initialize()
{
    m_Lock.Initialize();
    m_Lock.Enter();
    for (u32 i = 0; i < HANDLER_COUNT; i++) {
        m_Handlers[i] = 0;
    }
    m_Event.Initialize(nn::os::RESET_TYPE_ONESHOT);
    m_IsStopRequested = false;
    m_HandlerState = HANDLER_STATE_NONE;
    m_HandlerDoneEvent.Initialize(false);

    Gpu* gpu = detail::GetGpuIpc();
    u32 flags = detail::IsAppletMode() ? RELAY_FLAG_APPLET : RELAY_FLAG_APPLICATION;
    if (detail::IsFatalErrMode()) {
        flags |= RELAY_FLAG_FATAL_ERR_MODE;
    }
    nn::Handle sharedMemory;
    int threadIndex;
    nn::Result result = gpu->RegisterInterruptRelayQueue(m_Event.GetHandle(), flags, &sharedMemory, &threadIndex);
    m_ThreadIndex = threadIndex;
    m_SharedMemory.AttachAndMap(sharedMemory, SHARED_MEMORY_SIZE, false);

    uptr memory = m_SharedMemory.GetAddress();
    m_InterruptQueue.m_Event = m_Event.GetHandle();
    m_InterruptQueue.m_pQueue = reinterpret_cast<volatile s32*>(memory + INTERRUPT_QUEUE_OFFSET + threadIndex * INTERRUPT_QUEUE_SIZE);
    m_CmdReqQueue.Initialize(reinterpret_cast<void*>(memory + CMD_REQ_QUEUE_OFFSET + threadIndex * CMD_REQ_QUEUE_SIZE));
    uptr framebufferInfo = memory + FRAMEBUFFER_INFO_OFFSET + threadIndex * FRAMEBUFFER_INFO_SIZE;
    m_pFramebufferInfo[0] = reinterpret_cast<FramebufferInfoQueue*>(framebufferInfo);
    m_pFramebufferInfo[1] = reinterpret_cast<FramebufferInfoQueue*>(framebufferInfo + FRAMEBUFFER_INFO_SCREEN_SIZE);
    m_IsFirstInitialization = (result == nn::Result(RESULT_FIRST_INITIALIZATION));

    m_Thread.Start(ReceiverThreadFunc, reinterpret_cast<uptr>(this), reinterpret_cast<uptr>(m_Stack + STACK_SIZE), THREAD_PRIORITY);
    m_Lock.Exit();
}

// 0x0013142C | fefates:bytes [tier B]
void nn::gxlow::CTR::InterruptReceiver::WaitAnyHandlerDone()
{
    m_Lock.Enter();
    if (m_HandlerState == HANDLER_STATE_DONE) {
        m_HandlerState = HANDLER_STATE_NONE;
        m_Lock.Exit();
        return;
    }
    m_HandlerState = HANDLER_STATE_WAITING;
    m_Lock.Exit();
    m_HandlerDoneEvent.Wait();
}

// 0x00137054 | fefates:bytes [tier B]
void nn::gxlow::CTR::InterruptReceiver::ReceiverThreadFunc(uptr param)
{
    InterruptReceiver* self = reinterpret_cast<InterruptReceiver*>(param);
    for (;;) {
        self->m_Event.Wait();
        self->m_Event.ClearSignal();
        if (self->m_IsStopRequested) {
            return;
        }
        for (;;) {
            u8 id;
            nn::Result result = self->m_InterruptQueue.TryDequeue(&id);
            if (result == nn::Result(RESULT_QUEUE_EMPTY)) {
                break;
            }
            self->m_Lock.Enter();
            if (self->m_Handlers[id] != 0) {
                self->m_Handlers[id]();
            }
            HandlerState state = self->m_HandlerState;
            self->m_HandlerState = HANDLER_STATE_DONE;
            if (state == HANDLER_STATE_WAITING) {
                self->m_HandlerDoneEvent.Signal();
            }
            self->m_Lock.Exit();
        }
    }
}

} // namespace CTR
} // namespace gxlow
} // namespace nn
