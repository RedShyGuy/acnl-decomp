#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/inet/inet_JoinMeshJob.h"

namespace nn {
namespace pia {
namespace inet {
// ctor address unknown
nn::pia::inet::JoinMeshJob::JoinMeshJob()
{
}

// 0x0042C300 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::inet::JoinMeshJob::~JoinMeshJob()
{
}

// 0x003E26C0 slot 0x18 | slot vf_0x18 of nn::pia::session::JoinMeshJob
void nn::pia::inet::JoinMeshJob::StartupImpl()
{
}

// 0x003E26BC slot 0x1C | slot vf_0x1C of nn::pia::session::JoinMeshJob
void nn::pia::inet::JoinMeshJob::CleanupImpl()
{
}

// 0x003E2E4C slot 0x20 | slot vf_0x20 of nn::pia::session::JoinMeshJob
void nn::pia::inet::JoinMeshJob::SetHostInfoToMonitoringData(const nn::pia::transport::StationConnectionInfo&)
{
}

// 0x003E2E68 slot 0x24 | virtual slot, introduced by nn::pia::session::JoinMeshJob
void nn::pia::inet::JoinMeshJob::vf_0x24()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
