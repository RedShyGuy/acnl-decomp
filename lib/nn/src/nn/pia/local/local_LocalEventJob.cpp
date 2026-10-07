#include "nn/pia/local/local_LocalEventJob.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
// 0x00416644
nn::pia::common::ExecuteResult nn::pia::local::LocalEventJob::WatchUpdateEvent()
{
    LocalNetworkManager* pManager = LocalNetwork::s_pInstance->m_pNetworkManager;
    if (pManager->m_IsEventSignaled) {
        common::CriticalSection& criticalSection = pManager->m_EventCriticalSection;
        criticalSection.Lock();
        LocalNetwork::s_pInstance->m_pNetworkManager->ProcessUpdateEvent();
        LocalNetwork::s_pInstance->m_pNetworkManager->m_IsEventSignaled = false;
        criticalSection.Unlock();
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004166C0 (name is ours)
nn::Result nn::pia::local::LocalEventJob::Startup()
{
    SetStep(&LocalEventJob::WatchUpdateEvent, "LocalEventJob::WatchUpdateEvent");
    return nn::Result();
}

// 0x00416700
nn::pia::local::LocalEventJob::LocalEventJob()
{
    // only the base and the vptr (in the original too)
}

// 0x00416728
// 0x00416718 (deleting dtor)
nn::pia::local::LocalEventJob::~LocalEventJob()
{
    // empty (in the original too)
}

// 0x00730080
void nn::pia::local::LocalEventJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
