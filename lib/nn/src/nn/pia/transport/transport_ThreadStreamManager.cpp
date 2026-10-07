#include "nn/pia/transport/transport_ThreadStreamManager.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/common/common_IPacketOutput.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_Api.h"
#include "nn/pia/transport/transport_NetworkFactory.h"
#include "nn/pia/transport/transport_ReceiveThreadStream.h"
#include "nn/pia/transport/transport_SendThreadStream.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00975A90
nn::pia::transport::ThreadStreamManager* nn::pia::transport::ThreadStreamManager::s_pInstance;

inline nn::pia::transport::ThreadStreamManager::ThreadStreamManager(NetworkFactory* pFactory, unsigned int sendPacketNum, unsigned int receivePacketNum, unsigned int sendLatencyPacketNum, unsigned int receiveLatencyPacketNum, bool isDropEnabled)
    : m_pInput(nullptr), m_pOutput(nullptr), m_pReceiveStream(nullptr), m_pSendStream(nullptr)
{
    m_pInput = pFactory->CreateInputStream();
    m_pReceiveStream = new ReceiveThreadStream;
    m_pReceiveStream->Initialize(m_pInput, receivePacketNum, THREAD_PRIORITY, receiveLatencyPacketNum, isDropEnabled);
    m_pReceiveStream->m_WaitMSec = THREAD_WAIT_MSEC;
    m_pOutput = pFactory->CreateOutputStream();
    m_pSendStream = new SendThreadStream;
    m_pSendStream->Initialize(m_pOutput, sendPacketNum, THREAD_PRIORITY, sendLatencyPacketNum, isDropEnabled);
    m_pSendStream->m_WaitMSec = THREAD_WAIT_MSEC;
}

inline nn::pia::transport::ThreadStreamManager::~ThreadStreamManager()
{
    if (m_pSendStream != nullptr) {
        m_pSendStream->Finalize();
        delete m_pSendStream;
    }
    if (m_pReceiveStream != nullptr) {
        m_pReceiveStream->Finalize();
        delete m_pReceiveStream;
    }
    if (m_pOutput != nullptr) {
        delete m_pOutput;
        m_pOutput = nullptr;
    }
    if (m_pInput != nullptr) {
        delete m_pInput;
        m_pInput = nullptr;
    }
}

// 0x00457F58 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ThreadStreamManager::CreateInstance(nn::pia::transport::NetworkFactory* pFactory, unsigned int sendPacketNum, unsigned int receivePacketNum, unsigned int sendLatencyPacketNum, unsigned int receiveLatencyPacketNum, bool isDropEnabled)
{
    if (!common::IsValidPointer(pFactory)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new ThreadStreamManager(pFactory, sendPacketNum, receivePacketNum, sendLatencyPacketNum, receiveLatencyPacketNum,
                                          isDropEnabled);
    return nn::Result();
}

// 0x004580A4 | fefates:bytes [tier B]
void nn::pia::transport::ThreadStreamManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00458164 | fefates:bytes [tier B]
void nn::pia::transport::ThreadStreamManager::SetMonitoringData()
{
    if (common::IsValidPointer(m_pSendStream)) {
        m_pSendStream->SetMonitoringData();
    }
    if (common::IsValidPointer(m_pReceiveStream)) {
        m_pReceiveStream->SetMonitoringData();
    }
}

// 0x004581A0 (name is ours)
void nn::pia::transport::ThreadStreamManager::SetSendThreadPriority(int priority)
{
    m_pSendStream->m_pThread->setPriority(priority);
}

// 0x004581B4 (name is ours)
void nn::pia::transport::ThreadStreamManager::SetReceiveThreadPriority(int priority)
{
    m_pReceiveStream->m_pThread->setPriority(priority);
}

// 0x004581C8 (name is ours)
void nn::pia::transport::ThreadStreamManager::SetSendThreadWaitMSec(int waitMSec)
{
    m_pSendStream->m_WaitMSec = waitMSec;
}

// 0x004581D4 (name is ours)
void nn::pia::transport::ThreadStreamManager::SetReceiveThreadWaitMSec(int waitMSec)
{
    m_pReceiveStream->m_WaitMSec = waitMSec;
}

// 0x004581E0 | fefates:bytes [tier B]
void nn::pia::transport::ThreadStreamManager::Cleanup()
{
    if (common::IsValidPointer(m_pReceiveStream)) {
        m_pReceiveStream->Cleanup();
    }
    if (common::IsValidPointer(m_pSendStream)) {
        m_pSendStream->Cleanup();
    }
}

// 0x0045821C | fefates:bytes [tier B]
nn::Result nn::pia::transport::ThreadStreamManager::Startup()
{
    if (!common::IsValidPointer(m_pReceiveStream) || !common::IsValidPointer(m_pSendStream)) {
        return common::RESULT_INVALID_STATE;
    }
    m_pReceiveStream->Startup();
    m_pSendStream->Startup();
    return nn::Result();
}

// 0x007360A4 slot 0x00
void nn::pia::transport::ThreadStreamManager::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
