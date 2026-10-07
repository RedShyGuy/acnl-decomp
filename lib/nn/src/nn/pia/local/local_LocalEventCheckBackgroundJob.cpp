#include "nn/pia/local/local_LocalEventCheckBackgroundJob.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_Event.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace pia {
namespace local {
namespace {
// the event is polled every that many milliseconds; while the last one is not processed yet the
// job waits shorter
const u16 POLL_INTERVAL_MSEC = 15;
const u16 PENDING_INTERVAL_MSEC = 8;
// the description of the result of WaitSynchronization1 after the timeout
const bit32 DESCRIPTION_TIMEOUT = 0x3FE;
} // namespace

// 0x00422E98
nn::pia::common::ExecuteResult nn::pia::local::LocalEventCheckBackgroundJob::WatchUpdateEvent()
{
    if (!common::IsValidPointer(m_pEvent)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (LocalNetwork::s_pInstance->m_pNetworkManager->m_IsEventSignaled) {
        return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, PENDING_INTERVAL_MSEC);
    }
    nn::Handle handle = m_pEvent->GetHandle();
    if (handle.IsValid()) {
        nn::Result result = nn::svc::WaitSynchronization1(handle, 0);
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
        if (result.GetDescription() != DESCRIPTION_TIMEOUT) {
            common::CriticalSection& criticalSection = LocalNetwork::s_pInstance->m_pNetworkManager->m_EventCriticalSection;
            criticalSection.Lock();
            LocalNetwork::s_pInstance->m_pNetworkManager->UpdateConnectionStatus();
            LocalNetwork::s_pInstance->m_pNetworkManager->m_IsEventSignaled = true;
            criticalSection.Unlock();
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, POLL_INTERVAL_MSEC);
}

// 0x00422F94 (name is ours)
void nn::pia::local::LocalEventCheckBackgroundJob::Cleanup()
{
    m_pEvent = nullptr;
}

// 0x00422FA0 (name is ours)
nn::Result nn::pia::local::LocalEventCheckBackgroundJob::Startup(nn::os::Event* pEvent)
{
    m_pEvent = pEvent;
    Reset(false);
    SetStep(&LocalEventCheckBackgroundJob::WatchUpdateEvent, "LocalEventCheckBackgroundJob::WatchUpdateEvent");
    return nn::Result();
}

// 0x00423018
nn::pia::local::LocalEventCheckBackgroundJob::LocalEventCheckBackgroundJob() : m_pEvent(nullptr)
{
}

// 0x00423048
// 0x00423038 (deleting dtor)
nn::pia::local::LocalEventCheckBackgroundJob::~LocalEventCheckBackgroundJob()
{
    // empty (in the original too)
}

// 0x007316C8
void nn::pia::local::LocalEventCheckBackgroundJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
