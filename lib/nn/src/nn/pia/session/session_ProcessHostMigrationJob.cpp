#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"

namespace nn {
namespace pia {
namespace session {
// ctor candidate(s) 0x00442A04 (unverified)
nn::pia::session::ProcessHostMigrationJob::ProcessHostMigrationJob()
{
}

// 0x00442AF0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::ProcessHostMigrationJob::~ProcessHostMigrationJob()
{
}

// 0x00734154 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::ProcessHostMigrationJob::Trace(unsigned long long) const
{
}

// 0x00442668 slot 0x18 | slot vf_0x18 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::CleanupOldHostInfoCommonProc()
{
}

// 0x00441430 slot 0x1C | slot vf_0x1C of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::IsFatalErrorOccur()
{
}

// 0x00442664 slot 0x20 | virtual slot, introduced by nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::vf_0x20()
{
}

// 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
void nn::pia::session::ProcessHostMigrationJob::IsValidHostMigrationSetting()
{
}

// 0x004427F0 slot 0x28 | slot vf_0x28 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::UpdateHostStationIndexByLocalStationIndex()
{
}

// 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
void nn::pia::session::ProcessHostMigrationJob::StartupImpl(bool, nn::pia::StationIndex)
{
}

// 0x00441030 slot 0x30 | slot vf_0x30 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::CleanupImpl()
{
}

// 0x00441320 slot 0x34 | slot vf_0x34 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::CleanupStatus()
{
}

// 0x0044202C slot 0x38 | slot vf_0x38 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::CallUpdateSessionHost(unsigned int)
{
}

// 0x004426B8 slot 0x3C | slot vf_0x3C of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::IsCompletedUpdateSessionHost()
{
}

// 0x004413C0 slot 0x40 | fefates:callseq
void nn::pia::session::ProcessHostMigrationJob::vf_0x40()
{
}

// 0x004413B8 slot 0x44 | slot vf_0x44 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::StartupMultiImpl(bool, unsigned short)
{
}

// 0x00442638 slot 0x48 | slot vf_0x48 of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::CheckWhetherReselectNewHost()
{
}

// 0x004426C0 slot 0x4C | slot vf_0x4C of nn::pia::session::ProcessHostMigrationJob
void nn::pia::session::ProcessHostMigrationJob::CheckWhetherSendMigrationFinish()
{
}

// 0x00442034 | fefates:bytes [tier B]
void nn::pia::session::ProcessHostMigrationJob::WaitUpdateSessionHost()
{
}

// 0x004421DC | fefates:bytes-fuzzy [tier B]
void nn::pia::session::ProcessHostMigrationJob::MakeHostCandidateRanking(nn::pia::StationIndex, unsigned char*, unsigned int*, unsigned int*, bool)
{
}

// 0x004426C8 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::ProcessHostMigrationJob::PrepareForBecomingHostCommonProc()
{
}

// 0x0044281C | fefates:bytes [tier B]
void nn::pia::session::ProcessHostMigrationJob::Cleanup()
{
}

} // namespace session
} // namespace pia
} // namespace nn
