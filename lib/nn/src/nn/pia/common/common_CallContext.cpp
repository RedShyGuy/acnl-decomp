#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_ErrorHandler.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace common {
// 0x004267CC | fefates:bytes [tier B]
bool nn::pia::common::CallContext::InitiateCall()
{
    m_State = STATE_CALL_IN_PROGRESS;
    m_IsCancelRequested = false;
    return true;
}

// 0x004267E4 | fefates:bytes [tier B]
void nn::pia::common::CallContext::SignalCancel()
{
    m_Result = RESULT_CANCELED;
    m_State = STATE_CALL_CANCEL;
    if (m_Callback != nullptr) {
        m_Callback(RESULT_CANCELED, m_pCallbackArg);
    }
}

// 0x00426814 | fefates:bytes [tier B]
void nn::pia::common::CallContext::SignalFailure(nn::Result result)
{
    if (!result.IsFailure()) {
        ErrorHandler::TraceResult(ErrorHandler::TRACE_FLAG_CALL_CONTEXT, result);
    }
    m_Result = result;
    m_State = STATE_CALL_FAILURE;
    if (m_Callback != nullptr) {
        m_Callback(result, m_pCallbackArg);
    }
}

// 0x00426860 | fefates:bytes [tier B]
void nn::pia::common::CallContext::SignalSuccess(nn::Result result)
{
    if (result.IsFailure()) {
        ErrorHandler::TraceResult(ErrorHandler::TRACE_FLAG_CALL_CONTEXT, result);
    }
    m_Result = result;
    m_State = STATE_CALL_SUCCESS;
    if (m_Callback != nullptr) {
        m_Callback(result, m_pCallbackArg);
    }
}

// 0x004268AC (name is ours)
void nn::pia::common::CallContext::RegisterCallback(Callback callback, void* pArg)
{
    m_Callback = callback;
    m_pCallbackArg = pArg;
}

// 0x0042691C | fefates:callgraph [tier C]
void nn::pia::common::CallContext::Reset()
{
    m_State = STATE_NOT_CALLED;
    m_Callback = nullptr;
    m_pCallbackArg = nullptr;
    m_Result = nn::Result();
    m_IsCancelRequested = false;
}

// 0x00426938 | fefates:callgraph [tier C]
void nn::pia::common::CallContext::Cancel()
{
    m_IsCancelRequested = true;
}

// 0x00426944 | fefates:callgraph [tier C]
nn::pia::common::CallContext::CallContext()
{
    Reset();
}

} // namespace common
} // namespace pia
} // namespace nn
