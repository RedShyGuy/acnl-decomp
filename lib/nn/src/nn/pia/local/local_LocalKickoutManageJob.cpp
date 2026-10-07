#include "nn/pia/local/local_LocalKickoutManageJob.h"
#include "nn/pia/local/local_LocalNetwork.h"

namespace nn {
namespace pia {
namespace local {
// 0x0041C360
void nn::pia::local::LocalKickoutManageJob::StartupImpl()
{
    // a lost connection is not taken for a host that left
    LocalNetwork::s_pInstance->m_Unknown0x99 = true;
}

// 0x0041C378
void nn::pia::local::LocalKickoutManageJob::OnKickout(const nn::pia::common::StationAddress& address)
{
    LocalNetwork::s_pInstance->EjectClient(address);
}

// 0x0041C388
nn::pia::local::LocalKickoutManageJob::LocalKickoutManageJob()
{
    // only the base and the vptr (in the original too)
}

// 0x004389B0
// 0x0041C3A0 (deleting dtor)
nn::pia::local::LocalKickoutManageJob::~LocalKickoutManageJob()
{
    // empty (in the original too)
}

// 0x007311E4
void nn::pia::local::LocalKickoutManageJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
