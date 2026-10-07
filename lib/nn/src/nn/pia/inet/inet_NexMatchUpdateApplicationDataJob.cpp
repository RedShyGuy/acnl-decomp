#include "nn/pia/inet/inet_NexMatchUpdateApplicationDataJob.h"
#include <string.h>
#include "nn/nex/nex_qVector.h"
#include "nn/nstd/nstd_String.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace inet {
// 0x00411900
nn::Result nn::pia::inet::NexMatchUpdateApplicationDataJob::vf_0x1C(const void* pData, u32 size)
{
    if (!common::IsValidPointer(pData) || size > APPLICATION_DATA_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    session::Session* pSession = session::Session::s_pInstance;
    m_pSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    nnnstdMemCpy(&m_Buffer[APPLICATION_DATA_OFFSET], pData, size);
    m_DataSize = APPLICATION_DATA_OFFSET + size - m_DataOffset;
    SetStep(&NexMatchUpdateApplicationDataJob::UpdateApplicationBuffer, "NexMatchUpdateApplicationDataJob::UpdateApplicationBuffer");
    return nn::Result();
}

// ARMCC keeps the length check of qVector::reserve (GCC drops it: it can never be true), unrolls
// the copy loops of the vector and frees the vector per return; the rest is the same.
// 0x004119CC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchUpdateApplicationDataJob::UpdateApplicationBuffer()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        Cleanup();
        m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nex::qVector<u8> data;
    data.reserve(m_DataSize);
    for (u32 i = m_DataOffset; i < m_DataOffset + m_DataSize; i++) {
        data.push_back(m_Buffer[i]);
    }
    nn::Result result = m_pSession->UpdateApplicationBufferAsync(&m_CallContext, m_SessionId, data);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NexMatchUpdateApplicationDataJob::WaitUpdateApplicationBuffer, "NexMatchUpdateApplicationDataJob::WaitUpdateApplicationBuffer");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00411D8C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchUpdateApplicationDataJob::WaitUpdateApplicationBuffer()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        Cleanup();
        m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pSession->IsUpdateApplicationBufferCompleted()) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
                m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
            } else {
                m_pCallContext->SignalFailure(m_CallContext.m_Result);
            }
            SetStep(&UpdateApplicationDataJob::FailureProcess, "UpdateApplicationDataJob::FailureProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            SetStep(&UpdateApplicationDataJob::CompleteProcess, "UpdateApplicationDataJob::CompleteProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00411F4C
void nn::pia::inet::NexMatchUpdateApplicationDataJob::Cleanup()
{
    UpdateApplicationDataJob::Cleanup();
    m_pSession = nullptr;
    m_DataSize = 0;
    m_DataOffset = APPLICATION_DATA_OFFSET;
    memset(m_Buffer, 0, sizeof(m_Buffer));
}

// 0x00411F78
nn::pia::inet::NexMatchUpdateApplicationDataJob::NexMatchUpdateApplicationDataJob()
    : m_pSession(nullptr), m_DataSize(0), m_DataOffset(APPLICATION_DATA_OFFSET)
{
}

// 0x00442D1C
// 0x00411FA4 (deleting dtor)
nn::pia::inet::NexMatchUpdateApplicationDataJob::~NexMatchUpdateApplicationDataJob()
{
    // empty (in the original too)
}

// 0x0072FA6C
void nn::pia::inet::NexMatchUpdateApplicationDataJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
