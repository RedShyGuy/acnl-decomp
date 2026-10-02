#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/local/local_LocalNetwork.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x004164D0 (unverified)
nn::pia::local::LocalNetwork::LocalNetwork()
{
}

// 0x00416600 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalNetwork
void nn::pia::local::LocalNetwork::vf_0x00()
{
}

// 0x004165D8 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalNetwork
void nn::pia::local::LocalNetwork::vf_0x04()
{
}

// 0x00730034 slot 0x08 | virtual slot, introduced by nn::pia::local::LocalNetwork
void nn::pia::local::LocalNetwork::vf_0x08()
{
}

// 0x00414AB4 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::CreateJobs()
{
}

// 0x00414C50 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::CleanupJobs()
{
}

// 0x00414D70 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::DestroyJobs()
{
}

// 0x00414EF0 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::ScanNetwork(nn::pia::common::CallContext*, unsigned int, unsigned char)
{
}

// 0x00415350 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::DestroyNetwork(nn::pia::common::CallContext*)
{
}

// 0x004157F8 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::DestroyInstance()
{
}

// 0x00415BF4 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::AllowParticipating()
{
}

// 0x00415D80 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::DisallowParticipating(bool)
{
}

// 0x00415F20 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::GetNetworkDescription(unsigned int)
{
}

// 0x00416210 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::Cleanup()
{
}

// 0x0072FD00 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::GetApplicationData(void*, unsigned int*, unsigned int, const nn::pia::local::LocalNetworkDescription*) const
{
}

// 0x0072FE50 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::IsDuringHostMigration() const
{
}

// 0x0072FE90 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::IsEnableHostMigration() const
{
}

// 0x0072FF98 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::IsEnableAroundNetworkSearch() const
{
}

} // namespace local
} // namespace pia
} // namespace nn
