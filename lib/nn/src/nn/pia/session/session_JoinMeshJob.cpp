#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_JoinMeshJob.h"

namespace nn {
namespace pia {
namespace session {
// ctor candidate(s) 0x0042C030 (unverified)
nn::pia::session::JoinMeshJob::JoinMeshJob()
{
}

// 0x0042C304 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::JoinMeshJob::~JoinMeshJob()
{
}

// 0x00733834 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::JoinMeshJob::Trace(unsigned long long) const
{
}

// 0x0042A19C slot 0x18 | fefates:bytes-fuzzy
void nn::pia::session::JoinMeshJob::StartupImpl()
{
}

// 0x0042A198 slot 0x1C | slot vf_0x1C of nn::pia::session::JoinMeshJob
void nn::pia::session::JoinMeshJob::CleanupImpl()
{
}

// 0x0042BA68 slot 0x20 | slot vf_0x20 of nn::pia::session::JoinMeshJob
void nn::pia::session::JoinMeshJob::SetHostInfoToMonitoringData(const nn::pia::transport::StationConnectionInfo&)
{
}

// 0x0042BC08 slot 0x24 | virtual slot, introduced by nn::pia::session::JoinMeshJob
void nn::pia::session::JoinMeshJob::vf_0x24()
{
}

// 0x0042A7AC | fefates:bytes [tier B]
void nn::pia::session::JoinMeshJob::WaitJoinResponse()
{
}

// 0x0042B884 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::JoinMeshJob::CheckContextCallCanncelled()
{
}

} // namespace session
} // namespace pia
} // namespace nn
