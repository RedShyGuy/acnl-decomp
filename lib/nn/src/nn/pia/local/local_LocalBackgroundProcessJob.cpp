#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace local {
// 0x0042005C | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalBackgroundProcessJob::PrepareDestroyNetwork()
{
    if (m_IsDestroyPrepared) {
        return nn::Result();
    }
    if (IsRunning()) {
        if (IsBackground()) {
            // it stops in the background thread
            m_IsCancelRequested = true;
            if (m_pCallContext != nullptr) {
                m_pCallContext->SignalFailure(common::RESULT_CANCELED);
            }
            return common::RESULT_INVALID_STATE;
        }
        if (m_pCallContext != nullptr) {
            m_pCallContext->Cancel();
        }
    }
    Reset(false);
    m_IsDestroyPrepared = true;
    return nn::Result();
}

// 0x004200E4 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalBackgroundProcessJob::PrepareDisconnectNetwork()
{
    if (m_IsDisconnectPrepared) {
        return nn::Result();
    }
    if (IsRunning()) {
        if (IsBackground()) {
            m_IsCancelRequested = true;
            if (m_pCallContext != nullptr) {
                m_pCallContext->SignalFailure(common::RESULT_CANCELED);
            }
            return common::RESULT_INVALID_STATE;
        }
        if (m_pCallContext != nullptr) {
            m_pCallContext->Cancel();
        }
    }
    Reset(false);
    m_IsDisconnectPrepared = true;
    return nn::Result();
}

// 0x0042016C | fefates:bytes [tier B]
void nn::pia::local::LocalBackgroundProcessJob::Cleanup()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
    m_JobPriority = JOB_PRIORITY_NONE;
    m_IsDestroyPrepared = false;
    m_IsDisconnectPrepared = false;
}

// 0x004201B0 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalBackgroundProcessJob::Startup(nn::pia::common::CallContext* pCallContext, nn::pia::local::LocalBackgroundProcessJob::JobPriority priority)
{
    if (IsRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    // after a Prepare...Network only that request (or one before it) may start
    if (m_IsDestroyPrepared && priority > JOB_PRIORITY_DESTROY_NETWORK) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_IsDisconnectPrepared && priority > JOB_PRIORITY_DISCONNECT_NETWORK) {
        return common::RESULT_INVALID_STATE;
    }
    Reset(false);
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    m_JobPriority = priority;
    return nn::Result();
}

// 0x00420228 | fefates:bytes [tier B]
nn::pia::local::LocalBackgroundProcessJob::LocalBackgroundProcessJob()
    : m_pCallContext(nullptr), m_JobPriority(JOB_PRIORITY_NONE), m_IsDestroyPrepared(false), m_IsDisconnectPrepared(false)
{
}

// 0x0042026C
// 0x00420258 (deleting dtor)
nn::pia::local::LocalBackgroundProcessJob::~LocalBackgroundProcessJob()
{
    // empty (in the original too)
}

// 0x0073169C
void nn::pia::local::LocalBackgroundProcessJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
