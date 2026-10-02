#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_RelayRouteManageJob.h"

namespace nn {
namespace pia {
namespace session {
// 0x0043A904 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::RelayRouteManageJob::~RelayRouteManageJob()
{
}

// 0x00733928 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::RelayRouteManageJob::Trace(unsigned long long) const
{
}

// 0x0043A094 | fefates:bytes [tier B]
void nn::pia::session::RelayRouteManageJob::UpdateConnectionReport(nn::pia::StationIndex, const unsigned char*, unsigned int)
{
}

// 0x0043A2E4 | fefates:bytes [tier B]
void nn::pia::session::RelayRouteManageJob::PrepareForBecomingNewHost()
{
}

// 0x0043A5B0 | fefates:bytes [tier B]
void nn::pia::session::RelayRouteManageJob::Cleanup()
{
}

// 0x0043A5EC | fefates:bytes [tier B]
void nn::pia::session::RelayRouteManageJob::Startup(nn::pia::StationIndex, unsigned int, unsigned int)
{
}

// 0x0043A6C4 | fefates:bytes [tier B]
nn::pia::session::RelayRouteManageJob::RelayRouteManageJob()
{
}

} // namespace session
} // namespace pia
} // namespace nn
