#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/local/local_LocalFacade.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x00414678 (unverified)
nn::pia::local::LocalFacade::LocalFacade()
{
}

// 0x004147F8 slot 0x00 | fefates:bytes-fuzzy
void nn::pia::local::LocalFacade::Startup()
{
}

// 0x004147C8 slot 0x04 | fefates:bytes
void nn::pia::local::LocalFacade::Cleanup()
{
}

// 0x0041493C slot 0x08 | virtual slot, introduced by nn::pia::local::LocalFacade
void nn::pia::local::LocalFacade::vf_0x08()
{
}

// 0x00414938 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalFacade
nn::pia::local::LocalFacade::~LocalFacade()
{
}

// 0x0072FBAC slot 0x10 | virtual slot, introduced by nn::pia::local::LocalFacade
void nn::pia::local::LocalFacade::vf_0x10()
{
}

// 0x00414678 | fefates:bytes [tier B]
void nn::pia::local::LocalFacade::CreateInstance()
{
}

// 0x00414710 | fefates:bytes [tier B]
void nn::pia::local::LocalFacade::DestroyInstance()
{
}

// 0x00414740 | fefates:bytes [tier B]
void nn::pia::local::LocalFacade::LocalFacadeUpdateEventCallback(nn::pia::local::LocalUpdateEvent, unsigned char, void*)
{
}

// 0x0072FAD4 | fefates:bytes [tier B]
void nn::pia::local::LocalFacade::GetHostStationConnectionInfo(nn::pia::transport::StationConnectionInfo*) const
{
}

} // namespace local
} // namespace pia
} // namespace nn
