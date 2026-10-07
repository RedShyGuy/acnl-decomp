#include "nn/pia/inet/inet_NatDetectionJob.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/inet/inet_NatDetecter.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_SocketInputStream.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// the waits between the sends: after a failed send and while no reply arrived
const s32 SEND_RETRY_INTERVAL_MSEC = 20;
const s32 SEND_INTERVAL_MSEC = 100;
// the timeout grows with each retry
const u32 RETRY_TIMEOUT_MSEC = 500;
// StepWait waits in steps of this time
const u16 WAIT_MSEC = 5;

// the time msec milliseconds from now (inline; name is ours)
inline common::Time GetTimeAfter(s32 msec)
{
    common::TimeSpan span(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
    common::Time now;
    now.SetNow();
    return now + span;
}
} // namespace

// 0x003E70C8 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NatDetectionJob::StepPreTest()
{
    if (m_pNatTraverser->IsStartupCancelled()) {
        SetStep(&NatDetectionJob::StepComplete, "NatDetectionJob::StepComplete");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE_FOREGROUND);
    }
    nn::Result result = m_pDetecter->OpenSocket();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pDetecter->Cleanup();
        m_pDetecter = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NatDetectionJob::StepStart, "NatDetectionJob::StepStart");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x003E71BC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NatDetectionJob::StepComplete()
{
    if (m_pNatTraverser->IsStartupCancelled()) {
        m_pCallContext->SignalFailure(common::RESULT_CANCELED);
    } else if (m_IsCloseFailed) {
        m_pCallContext->SignalFailure(common::RESULT_NAT_CHECK_FAILED);
    } else {
        bool isSucceeded = m_pDetecter->HandleResult();
        // the replies of the servers are no packets for the transport
        common::SimpleContainer<common::InetAddress, 4>& servers = NexFacade::s_pInstance->m_pNatTraverser->m_ServerAddresses;
        for (common::InetAddress* it = servers.Begin(); it != servers.End(); it++) {
            SocketInputStream::AddIgnoreAddress(*it);
        }
        if (isSucceeded) {
            m_pCallContext->SignalSuccess(nn::Result());
        } else {
            m_pCallContext->SignalFailure(common::RESULT_NAT_CHECK_FAILED);
        }
    }
    m_pDetecter->Cleanup();
    m_pDetecter = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x003E72B0 | fefates:bytes
void nn::pia::inet::NatDetectionJob::CancelCleanup()
{
    if (m_pCallContext != nullptr && m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pCallContext->SignalCancel();
    }
    m_pDetecter->Cleanup();
    m_pDetecter = nullptr;
}

// 0x003E72EC | fefates:bytes [tier B]
void nn::pia::inet::NatDetectionJob::stopReceivingMessage()
{
    if (m_pDetecter->CheckRetry()) {
        m_pDetecter->Retry();
        SetStep(&NatDetectionJob::StepStart, "NatDetectionJob::StepStart");
    } else {
        SetStep(&NatDetectionJob::StepEnd, "NatDetectionJob::StepEnd");
    }
}

// 0x003E7358 (name is ours)
nn::Result nn::pia::inet::NatDetectionJob::Startup(nn::pia::common::CallContext* pCallContext, nn::pia::inet::NatDetecter* pDetecter, nn::pia::inet::NatTraverser* pNatTraverser)
{
    m_Deadline = common::Time();
    m_NextSendTime = common::Time();
    m_pDetecter = pDetecter;
    m_pNatTraverser = pNatTraverser;
    SetStep(&NatDetectionJob::StepPreTest, "NatDetectionJob::StepPreTest");
    common::StepSequenceJob::Reset(true);
    m_pCallContext = pCallContext;
    if (!pCallContext->InitiateCall()) {
        return common::RESULT_INVALID_STATE;
    }
    m_IsSending = false;
    m_IsReceived = false;
    return nn::Result();
}

// 0x003E73F0 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NatDetectionJob::StepEnd()
{
    m_IsCloseFailed = m_pDetecter->CloseSocket().IsFailure();
    SetStep(&NatDetectionJob::StepComplete, "NatDetectionJob::StepComplete");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE_FOREGROUND);
}

// 0x003E7464 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NatDetectionJob::StepSend()
{
    if (m_pNatTraverser->IsStartupCancelled()) {
        SetStep(&NatDetectionJob::StepEnd, "NatDetectionJob::StepEnd");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    common::Time now;
    now.SetNow();
    if (m_Deadline < now) {
        stopReceivingMessage();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (!m_IsSending) {
        m_IsSending = true;
        m_SendStartTime.SetNow();
    }
    if (!m_pDetecter->SendAllQueuedMessage()) {
        m_NextSendTime = GetTimeAfter(SEND_RETRY_INTERVAL_MSEC);
        SetStep(&NatDetectionJob::StepWait, "NatDetectionJob::StepWait");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (!m_pDetecter->ReceiveMessage()) {
        m_NextSendTime = GetTimeAfter(SEND_INTERVAL_MSEC);
        SetStep(&NatDetectionJob::StepWait, "NatDetectionJob::StepWait");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (!m_IsReceived) {
        m_IsReceived = true;
        NatDetecter* pDetecter = m_pDetecter;
        common::Time receiveTime;
        receiveTime.SetNow();
        pDetecter->m_Rtt = (receiveTime - m_SendStartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
    }
    if (m_pDetecter->CheckAllMessage()) {
        stopReceivingMessage();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    m_NextSendTime = GetTimeAfter(SEND_INTERVAL_MSEC);
    SetStep(&NatDetectionJob::StepWait, "NatDetectionJob::StepWait");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x003E7754 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NatDetectionJob::StepWait()
{
    if (m_pNatTraverser->IsStartupCancelled()) {
        SetStep(&NatDetectionJob::StepEnd, "NatDetectionJob::StepEnd");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    common::Time now;
    now.SetNow();
    if (m_NextSendTime < now) {
        SetStep(&NatDetectionJob::StepSend, "NatDetectionJob::StepSend");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_MSEC);
}

// 0x003E785C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NatDetectionJob::StepStart()
{
    if (m_pNatTraverser->IsStartupCancelled()) {
        SetStep(&NatDetectionJob::StepEnd, "NatDetectionJob::StepEnd");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    NatDetecter* pDetecter = m_pDetecter;
    m_Deadline = GetTimeAfter(pDetecter->GetDetectionTimeout() + RETRY_TIMEOUT_MSEC * pDetecter->m_RetryCount);
    m_pDetecter->InitializeReceiveMessage();
    m_pDetecter->StartSendingMessage();
    SetStep(&NatDetectionJob::StepSend, "NatDetectionJob::StepSend");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x003E7990 | fefates:bytes [tier B]
nn::pia::inet::NatDetectionJob::NatDetectionJob()
    : m_pDetecter(nullptr), m_Deadline(), m_NextSendTime(), m_SendStartTime()
{
}

// 0x0042759C | fefates:callgraph
// 0x003E79D8 (deleting dtor)
nn::pia::inet::NatDetectionJob::~NatDetectionJob()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
