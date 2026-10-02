#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_LeaveWithHostMigrationJob.h"

namespace nn {
namespace pia {
namespace session {
// 0x004435DC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::LeaveWithHostMigrationJob::~LeaveWithHostMigrationJob()
{
}

// 0x00734174 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::LeaveWithHostMigrationJob::Trace(unsigned long long) const
{
}

// 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
void nn::pia::session::LeaveWithHostMigrationJob::vf_0x18()
{
}

// 0x00442DF4 slot 0x1C | fefates:bytes
void nn::pia::session::LeaveWithHostMigrationJob::CleanupStatus()
{
}

// 0x00442EE8 | fefates:bytes [tier B]
void nn::pia::session::LeaveWithHostMigrationJob::ReceiveMigrationResponse(nn::pia::StationIndex)
{
}

// 0x00443420 | fefates:bytes [tier B]
void nn::pia::session::LeaveWithHostMigrationJob::Cleanup()
{
}

// 0x00443570 | fefates:bytes [tier B]
nn::pia::session::LeaveWithHostMigrationJob::LeaveWithHostMigrationJob()
{
}

} // namespace session
} // namespace pia
} // namespace nn
