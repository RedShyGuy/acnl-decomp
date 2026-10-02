#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_Station.h"

namespace nn {
namespace pia {
namespace transport {
// ctor candidate(s) 0x0045F7E4 (unverified)
nn::pia::transport::Station::Station()
{
}

// 0x00736DC4 slot 0x00 | virtual slot, introduced by nn::pia::transport::Station
void nn::pia::transport::Station::vf_0x00()
{
}

// 0x0045F338 | fefates:bytes [tier B]
void nn::pia::transport::Station::Initialize(nn::pia::transport::NetworkFactory*)
{
}

// 0x0045F3A0 | fefates:bytes [tier B]
void nn::pia::transport::Station::CleanupJobs()
{
}

// 0x0045F43C | fefates:bytes [tier B]
void nn::pia::transport::Station::GetPrincipalId(unsigned int*)
{
}

// 0x0045F5C0 | fefates:bytes [tier B]
void nn::pia::transport::Station::GetPlayerName(nn::pia::transport::Station::PlayerName*)
{
}

// 0x0045F5FC | fefates:bytes [tier B]
void nn::pia::transport::Station::Cleanup()
{
}

// 0x0045F658 | fefates:bytes [tier B]
void nn::pia::transport::Station::Startup(nn::pia::transport::StationProtocol*)
{
}

// 0x0045F6D4 | fefates:bytes [tier B]
void nn::pia::transport::Station::Startup(nn::pia::transport::StationProtocol*, nn::pia::StationIndex, const nn::pia::common::StationAddress&)
{
}

// 0x0045F740 | fefates:bytes [tier B]
void nn::pia::transport::Station::Startup(nn::pia::transport::StationProtocol*, const nn::pia::common::StationAddress&)
{
}

// 0x0045F784 | fefates:bytes [tier B]
void nn::pia::transport::Station::Finalize()
{
}

// 0x0045F894 | fefates:bytes [tier B]
nn::pia::transport::Station::~Station()
{
}

// 0x00736D90 | fefates:bytes [tier B]
void nn::pia::transport::Station::IsConnectionRouteRelay() const
{
}

// 0x00736DAC | fefates:bytes [tier B]
void nn::pia::transport::Station::IsConnectionRouteDirect() const
{
}

// 0x00736DC8 | fefates:bytes [tier B]
void nn::pia::transport::Station::GetRtt(unsigned int) const
{
}

// 0x00736E24 | fefates:bytes [tier B]
void nn::pia::transport::Station::GetRtt() const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
