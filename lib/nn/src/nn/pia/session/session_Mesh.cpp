#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/session/session_Mesh.h"

namespace nn {
namespace pia {
namespace session {
// ctor candidate(s) 0x00449D4C (unverified)
nn::pia::session::Mesh::Mesh()
{
}

// 0x00734334 slot 0x00 | virtual slot, introduced by nn::pia::session::Mesh
void nn::pia::session::Mesh::vf_0x00()
{
}

// 0x00449E50 slot 0x04 | slot vf_0x04 of nn::pia::session::Mesh
nn::pia::session::Mesh::~Mesh()
{
}

// 0x004480D8 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::CleanupJobs()
{
}

// 0x00448AB4 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::FixConnectedId(nn::pia::StationIndex)
{
}

// 0x00448CD8 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::NoticeMeshEvent(nn::pia::session::Mesh::EventType, nn::pia::StationIndex)
{
}

// 0x004491D8 | fefates:bytes [tier B]
void nn::pia::session::Mesh::CleanupStationsJobs()
{
}

// 0x0044929C | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::UnfixDisconnectedId(nn::pia::StationIndex)
{
}

// 0x004494E8 | fefates:bytes [tier B]
void nn::pia::session::Mesh::NotifyLeaveStationAddress(const nn::pia::common::StationAddress&)
{
}

// 0x00449C04 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::StartUse(nn::pia::StationIndex)
{
}

// 0x00734338 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::GetTime() const
{
}

} // namespace session
} // namespace pia
} // namespace nn
