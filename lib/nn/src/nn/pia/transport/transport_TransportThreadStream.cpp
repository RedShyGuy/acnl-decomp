#include "nn/pia/transport/transport_TransportThreadStream.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_LatencyEmulator.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadSafeStringBase.h"
#include "pead/peadTickSpan.h"
#include <new>

// 0x00810120
template void pead::Delegate2<nn::pia::transport::TransportThreadStream, pead::Thread*, int>::invoke(pead::Thread*, int);

namespace nn {
namespace pia {
namespace transport {
namespace {
// the thread's stack and the arguments of its message queue
const int THREAD_STACK_SIZE = 0x2000;
const int THREAD_MESSAGE_QUEUE_SIZE = 32;

// a result of pia with the description of the given one (any level and summary)
bool IsSameDescription(nn::Result result, bit32 value)
{
    return result.GetModule() == nn::Result(value).GetModule() && result.GetDescription() == nn::Result(value).GetDescription();
}
} // namespace

// 0x0045B868 (name is ours)
void nn::pia::transport::TransportThreadStream::ThreadFunc(pead::Thread* pThread, int)
{
    for (;;) {
        switch (m_State) {
        case THREAD_STATE_WAITING | THREAD_STATE_REQUEST:
            m_State = THREAD_STATE_WAITING;
            break;
        case THREAD_STATE_RUNNING | THREAD_STATE_REQUEST:
            m_State = THREAD_STATE_RUNNING;
            break;
        case THREAD_STATE_FINISHED | THREAD_STATE_REQUEST:
            pThread->quit(false);
            m_State = THREAD_STATE_FINISHED;
            return;
        case THREAD_STATE_WAITING:
        case THREAD_STATE_ERROR:
            m_Event.Wait();
            break;
        case THREAD_STATE_RUNNING: {
            nn::Result result = ProcessOne();
            if (common::IsValidPointer(m_pLatencyEmulator)) {
                nn::Result latencyResult = m_pLatencyEmulator->Dispatch();
                if (latencyResult.IsFailure()) {
                    result = latencyResult;
                }
            }
            if (result.IsFailure()) {
                // a broken packet is skipped
                if (IsSameDescription(result, common::RESULT_INVALID_FORMAT)) {
                    break;
                }
                if (IsSameDescription(result, common::RESULT_NO_DATA) || IsSameDescription(result, common::RESULT_BUFFER_IS_FULL)) {
                    m_Event.Wait(static_cast<s64>(m_WaitMSec) * pead::TickSpan::sFrequency / 1000);
                } else {
                    m_LastResult = result;
                    m_State = THREAD_STATE_ERROR;
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

// 0x0045B9D4 | fefates:bytes-fuzzy [tier B]
void nn::pia::transport::TransportThreadStream::ChangeState(nn::pia::transport::TransportThreadStream::ThreadState state)
{
    if (m_State == state) {
        return;
    }
    pead::TickSpan span = pead::TickSpan::fromMicroSeconds(100);
    do {
        m_State = static_cast<ThreadState>(state | THREAD_STATE_REQUEST);
        m_Event.Signal();
        pead::SleepThread(span);
        // a thread that is started and fails at once does not run
    } while (!(state == THREAD_STATE_RUNNING && m_State == THREAD_STATE_ERROR) && m_State != state);
}

// 0x0045BA6C | fefates:bytes [tier B]
void nn::pia::transport::TransportThreadStream::FinalizeCore()
{
    if (common::IsValidPointer(m_pThread)) {
        ChangeState(THREAD_STATE_FINISHED);
        m_pThread->waitDone();
        if (m_pThread != nullptr) {
            m_pThread->~DelegateThread();
            pead::FreeMemory(m_pThread);
        }
        m_pThread = nullptr;
    }
    m_PacketStream.Finalize();
    m_LastResult = nn::Result();
    if (common::IsValidPointer(m_pLatencyEmulator)) {
        delete m_pLatencyEmulator;
        m_pLatencyEmulator = nullptr;
    }
}

// 0x0045BB14 | fefates:bytes [tier B]
bool nn::pia::transport::TransportThreadStream::isDropPacket()
{
    if (m_IsDropEnabled && m_DropRate != 0) {
        if (m_DropRate > static_cast<s32>(m_Random.getU32(100))) {
            return true;
        }
    }
    return false;
}

// 0x0045BB50 | fefates:bytes [tier B]
nn::Result nn::pia::transport::TransportThreadStream::InitializeCore(const char* pName, int priority, unsigned int packetNum, unsigned int latencyPacketNum, bool isDropEnabled, unsigned int watermarkIndex)
{
    if (m_State != THREAD_STATE_FINISHED) {
        return common::RESULT_INVALID_STATE;
    }
    m_pLatencyEmulator = latencyPacketNum != 0 ? new LatencyEmulator(latencyPacketNum) : nullptr;
    m_IsDropEnabled = isDropEnabled;
    pead::Heap* pHeap = common::HeapManager::GetHeap();
    void* pBuffer = pead::AllocMemory(sizeof(pead::DelegateThread), common::HeapManager::GetHeap());
    if (pBuffer != nullptr) {
        pBuffer = ::new (pBuffer) pead::DelegateThread(pead::SafeStringBase<char>(pName), &m_Delegate, pHeap, priority, 1, 0x7FFFFFFF,
                                                       THREAD_STACK_SIZE, THREAD_MESSAGE_QUEUE_SIZE);
    }
    m_pThread = static_cast<pead::DelegateThread*>(pBuffer);
    m_PacketStream.Initialize(packetNum, watermarkIndex);
    m_LastResult = nn::Result();
    m_State = THREAD_STATE_WAITING;
    m_pThread->start();
    return nn::Result();
}

// 0x0045BC4C | fefates:bytes [tier B]
void nn::pia::transport::TransportThreadStream::Cleanup()
{
    if (m_State == THREAD_STATE_FINISHED) {
        return;
    }
    ChangeState(THREAD_STATE_WAITING);
    m_PacketStream.Cleanup();
    if (common::IsValidPointer(m_pLatencyEmulator)) {
        m_pLatencyEmulator->Clear();
    }
}

// 0x0045BC98 | fefates:bytes [tier B]
nn::Result nn::pia::transport::TransportThreadStream::Startup()
{
    if (m_State != THREAD_STATE_WAITING) {
        return common::RESULT_INVALID_STATE;
    }
    if (common::IsValidPointer(m_pLatencyEmulator)) {
        m_pLatencyEmulator->Clear();
    }
    m_PacketStream.Startup();
    m_LastResult = nn::Result();
    ResetMonitoringData();
    ChangeState(THREAD_STATE_RUNNING);
    return nn::Result();
}

// 0x0045BD00 slot 0x00
void nn::pia::transport::TransportThreadStream::ResetMonitoringData()
{
    // empty (in the original too)
}

// 0x0045BD04 | fefates:bytes [tier B]
nn::pia::transport::TransportThreadStream::TransportThreadStream()
    : m_Delegate(this, &TransportThreadStream::ThreadFunc), m_pThread(nullptr), m_Event(false), m_State(THREAD_STATE_FINISHED),
      m_WaitMSec(5), m_LastResult(common::RESULT_NOT_SET), m_pLatencyEmulator(nullptr), m_DropRate(0), m_IsDropEnabled(false)
{
}

// 0x0045BD90 | fefates:bytes [tier B]
nn::pia::transport::TransportThreadStream::~TransportThreadStream()
{
    // empty (in the original too)
}

// 0x00736548 | fefates:bytes [tier B]
nn::Result nn::pia::transport::TransportThreadStream::GetLastResult() const
{
    if (m_State == THREAD_STATE_WAITING || m_State == THREAD_STATE_ERROR) {
        return m_LastResult;
    }
    return nn::Result();
}

} // namespace transport
} // namespace pia
} // namespace nn
