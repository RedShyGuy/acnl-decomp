#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_ProcessUpdateMeshJob.h"

namespace nn {
namespace pia {
namespace session {
// ctor candidate(s) 0x0043DE40 (unverified)
nn::pia::session::ProcessUpdateMeshJob::ProcessUpdateMeshJob()
{
}

// 0x0043E14C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::ProcessUpdateMeshJob::~ProcessUpdateMeshJob()
{
}

// 0x007339E8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::ProcessUpdateMeshJob::Trace(unsigned long long) const
{
}

// 0x0043B8E0 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::ProcessUpdateMeshJob::CalcTimeLimit(bool)
{
}

// 0x0043BAA8 | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::ClearStationIndex(nn::pia::StationIndex)
{
}

// 0x0043BF90 | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::SetStationDataList(const unsigned char*, unsigned int)
{
}

// 0x0043C2F8 | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::UpdateDataTakeover(unsigned int)
{
}

// 0x0043D490 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::ProcessUpdateMeshJob::UpdateStationDataList(const unsigned char*, unsigned int)
{
}

// 0x0043D9CC | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::SetConnectionFailureNotice(nn::pia::StationIndex, unsigned char)
{
}

// 0x0043DC68 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::ProcessUpdateMeshJob::Startup(const unsigned char*, unsigned int, bool)
{
}

// 0x00733930 | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::CheckEdmByStationIndex(nn::pia::StationIndex) const
{
}

// 0x00733990 | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::GetStationIndexByPrincipalID(unsigned int) const
{
}

} // namespace session
} // namespace pia
} // namespace nn
