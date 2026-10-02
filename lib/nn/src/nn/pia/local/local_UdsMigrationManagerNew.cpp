#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_UdsMigrationManagerNew.h"

namespace nn {
namespace pia {
namespace local {
// 0x0041E908 slot 0x00 | fefates:bytes
nn::pia::local::UdsMigrationManagerNew::~UdsMigrationManagerNew()
{
}

// 0x0041E89C slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMigrationManager
void nn::pia::local::UdsMigrationManagerNew::vf_0x04()
{
}

// 0x0041E4F4 slot 0x08 | fefates:bytes
void nn::pia::local::UdsMigrationManagerNew::SetupParams()
{
}

// 0x0041E5D0 slot 0x0C | fefates:bytes
void nn::pia::local::UdsMigrationManagerNew::SetNetworkInfo(const nn::pia::local::LocalConnectNetworkSetting&)
{
}

// 0x0041E51C slot 0x10 | slot vf_0x10 of nn::pia::local::LocalMigrationManager
void nn::pia::local::UdsMigrationManagerNew::IsNextNetwork(const nn::pia::local::LocalNetworkDescription*)
{
}

// 0x0041E6F4 slot 0x14 | fefates:bytes
void nn::pia::local::UdsMigrationManagerNew::GetCreateNetworkSetting()
{
}

// 0x0041E790 slot 0x18 | fefates:bytes
void nn::pia::local::UdsMigrationManagerNew::GetConnectNetworkSetting(nn::pia::local::LocalNetworkDescription*)
{
}

// 0x0073165C slot 0x1C | slot vf_0x1C of nn::pia::local::LocalMigrationManager
void nn::pia::local::UdsMigrationManagerNew::GetSubId() const
{
}

// 0x0073163C slot 0x20 | fefates:bytes
void nn::pia::local::UdsMigrationManagerNew::GetLocalCommunicationId() const
{
}

// 0x007315C8 slot 0x24 | fefates:bytes
void nn::pia::local::UdsMigrationManagerNew::GetLocalNodeKey(unsigned short) const
{
}

// 0x0041E64C slot 0x28 | fefates:callseq
void nn::pia::local::UdsMigrationManagerNew::vf_0x28()
{
}

// 0x0041E7C0 | fefates:bytes [tier B]
nn::pia::local::UdsMigrationManagerNew::UdsMigrationManagerNew()
{
}

} // namespace local
} // namespace pia
} // namespace nn
