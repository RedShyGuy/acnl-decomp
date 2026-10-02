#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalHostMigrationJob.h"

namespace nn {
namespace pia {
namespace local {
// 0x0041C324 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::local::LocalHostMigrationJob::~LocalHostMigrationJob()
{
}

// 0x007311E0 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::local::LocalHostMigrationJob::Trace(unsigned long long) const
{
}

// 0x0041B000 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::ScanNetwork()
{
}

// 0x0041B1EC | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::CreateNetwork()
{
}

// 0x0041B310 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::WaitForCancel()
{
}

// 0x0041B360 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::ConnectNetwork()
{
}

// 0x0041B760 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::WaitAllClientsAck()
{
}

// 0x0041B83C | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::WaitCreateNetwork()
{
}

// 0x0041B9B0 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::WaitConnectNetwork()
{
}

// 0x0041BAF8 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::SearchNewHostNetwork()
{
}

// 0x0041BCA0 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::WaitDisconnectNetwork()
{
}

// 0x0041BE44 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::HostMigrationFailureProcess()
{
}

// 0x0041C18C | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::Cleanup()
{
}

// 0x0041C1E0 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::Startup(nn::pia::common::CallContext*, bool)
{
}

// 0x0041C284 | fefates:bytes [tier B]
nn::pia::local::LocalHostMigrationJob::LocalHostMigrationJob()
{
}

} // namespace local
} // namespace pia
} // namespace nn
