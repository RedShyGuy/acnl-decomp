#include "nn/pia/inet/inet_NatServerAddressResolveJob.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/inet/inet_NatTraverser.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// the time the name resolution may take
const s32 RESOLVE_TIMEOUT_MSEC = 30000;
} // namespace

// 0x0040C388
nn::pia::common::ExecuteResult nn::pia::inet::NatServerAddressResolveJob::StepResolve()
{
    if (m_pNatTraverser != nullptr && m_pCallContext != nullptr && !m_pNatTraverser->IsStartupCancelled()) {
        common::TimeSpan timeout(common::TimeSpan::GetTicksPerMSec().GetTick() * RESOLVE_TIMEOUT_MSEC);
        common::Time now;
        now.SetNow();
        m_Deadline = now + timeout;
        m_Result = m_pNatTraverser->UpdateNatServerAddress();
        if (m_Result.IsSuccess()) {
            if (m_pNatTraverser->m_ServerAddresses.GetNum() == 0) {
                m_Result = common::RESULT_NAT_SERVER_NOT_FOUND;
            } else {
                m_pNatTraverser->StartNatTraversal();
            }
        }
        m_Deadline = common::Time();
    }
    SetStep(&NatServerAddressResolveJob::StepComplete, "NatServerAddressResolveJob::StepComplete");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE_FOREGROUND);
}

// 0x0040C4A0 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NatServerAddressResolveJob::StepComplete()
{
    if (m_pNatTraverser != nullptr && m_pCallContext != nullptr) {
        if (m_pNatTraverser->IsStartupCancelled()) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        } else if (m_Result.IsSuccess()) {
            m_pCallContext->SignalSuccess(nn::Result());
        } else {
            m_pCallContext->SignalFailure(m_Result);
        }
    }
    m_pCallContext = nullptr;
    m_pNatTraverser = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0040C524 | fefates:bytes
void nn::pia::inet::NatServerAddressResolveJob::CancelCleanup()
{
    if (m_pCallContext != nullptr && m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pCallContext->SignalCancel();
    }
    m_pCallContext = nullptr;
    m_pNatTraverser = nullptr;
}

// 0x0040C554 | fefates:callgraph [tier C]
nn::Result nn::pia::inet::NatServerAddressResolveJob::Startup(nn::pia::common::CallContext* pCallContext, nn::pia::inet::NatTraverser* pNatTraverser)
{
    if (pCallContext == nullptr || pNatTraverser == nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pCallContext = pCallContext;
    if (!pCallContext->InitiateCall()) {
        return common::RESULT_INVALID_STATE;
    }
    m_Deadline = common::Time();
    m_pNatTraverser = pNatTraverser;
    m_Result = nn::Result();
    SetStep(&NatServerAddressResolveJob::StepResolve, "NatServerAddressResolveJob::StepResolve");
    common::StepSequenceJob::Reset(true);
    return nn::Result();
}

// 0x0040C5FC | fefates:bytes [tier B]
nn::pia::inet::NatServerAddressResolveJob::NatServerAddressResolveJob()
    : m_pNatTraverser(nullptr), m_pCallContext(nullptr), m_Deadline(), m_Result(common::RESULT_NOT_SET)
{
}

// 0x0040C648 | fefates:callgraph
// 0x0040C638 (deleting dtor)
nn::pia::inet::NatServerAddressResolveJob::~NatServerAddressResolveJob()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
