#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_KickoutManageJob.h"

namespace nn {
namespace pia {
namespace session {
// ctor candidate(s) 0x00438950 (unverified)
nn::pia::session::KickoutManageJob::KickoutManageJob()
{
}

// 0x004389B4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::KickoutManageJob::~KickoutManageJob()
{
}

// 0x007338F8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::KickoutManageJob::Trace(unsigned long long) const
{
}

// 0x00438608 slot 0x18 | virtual slot, introduced by nn::pia::session::KickoutManageJob
void nn::pia::session::KickoutManageJob::vf_0x18()
{
}

// 0x0043860C slot 0x1C | virtual slot, introduced by nn::pia::session::KickoutManageJob
void nn::pia::session::KickoutManageJob::vf_0x1C()
{
}

// 0x004382C4 | fefates:bytes [tier B]
void nn::pia::session::KickoutManageJob::StartKickout(nn::pia::StationIndex, nn::pia::session::KickoutManageJob::KickoutReason)
{
}

// 0x00438610 | fefates:bytes [tier B]
void nn::pia::session::KickoutManageJob::ClientWaitLeaveMesh()
{
}

// 0x00438690 | fefates:bytes [tier B]
void nn::pia::session::KickoutManageJob::AssociateKickoutWith(nn::pia::common::CallContext*)
{
}

} // namespace session
} // namespace pia
} // namespace nn
