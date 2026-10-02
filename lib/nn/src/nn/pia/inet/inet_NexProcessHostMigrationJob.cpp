#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/inet/inet_NexProcessHostMigrationJob.h"

namespace nn {
namespace pia {
namespace inet {
// ctor address unknown
nn::pia::inet::NexProcessHostMigrationJob::NexProcessHostMigrationJob()
{
}

// 0x0040F9FC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::inet::NexProcessHostMigrationJob::~NexProcessHostMigrationJob()
{
}

// 0x0072F85C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::inet::NexProcessHostMigrationJob::Trace(unsigned long long) const
{
}

// 0x0040D240 slot 0x1C | slot vf_0x1C of nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::IsFatalErrorOccur()
{
}

// 0x0040DEF8 slot 0x20 | virtual slot, introduced by nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::vf_0x20()
{
}

// 0x0040DE98 slot 0x24 | slot vf_0x24 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::IsValidHostMigrationSetting()
{
}

// 0x0040D024 slot 0x2C | slot vf_0x2C of nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::StartupImpl(bool, nn::pia::StationIndex)
{
}

// 0x0040CFF0 slot 0x30 | slot vf_0x30 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::CleanupImpl()
{
}

// 0x0040D0EC slot 0x34 | slot vf_0x34 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::CleanupStatus()
{
}

// 0x0040DA4C slot 0x38 | fefates:bytes-fuzzy
void nn::pia::inet::NexProcessHostMigrationJob::CallUpdateSessionHost(unsigned int)
{
}

// 0x0040E1B4 slot 0x3C | slot vf_0x3C of nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::IsCompletedUpdateSessionHost()
{
}

// 0x0040D138 slot 0x44 | fefates:bytes-fuzzy
void nn::pia::inet::NexProcessHostMigrationJob::StartupMultiImpl(bool, unsigned short)
{
}

// 0x0040DD88 slot 0x48 | slot vf_0x48 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::CheckWhetherReselectNewHost()
{
}

// 0x0040EB9C slot 0x4C | slot vf_0x4C of nn::pia::session::ProcessHostMigrationJob
void nn::pia::inet::NexProcessHostMigrationJob::CheckWhetherSendMigrationFinish()
{
}

// 0x0040DABC | fefates:bytes [tier B]
void nn::pia::inet::NexProcessHostMigrationJob::InetCleanupOldHostInfo()
{
}

// 0x0040ED0C | fefates:bytes [tier B]
void nn::pia::inet::NexProcessHostMigrationJob::WaitAfterPrepareForBecomingHost()
{
}

// 0x0040F1D4 | fefates:bytes [tier B]
void nn::pia::inet::NexProcessHostMigrationJob::InetCleanupOldHostInfoOnMultiCandidate()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
