#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/local/local_LocalMigrationManager.h"

namespace nn {
namespace pia {
namespace local {
// 0x0041D524 slot 0x00 | fefates:bytes
nn::pia::local::LocalMigrationManager::~LocalMigrationManager()
{
}

// 0x0041D4E8 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMigrationManager
void nn::pia::local::LocalMigrationManager::vf_0x04()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::SetupParams()
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::SetNetworkInfo(const nn::pia::local::LocalConnectNetworkSetting&)
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::IsNextNetwork(const nn::pia::local::LocalNetworkDescription*)
{
}

// 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::GetCreateNetworkSetting()
{
}

// 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::GetConnectNetworkSetting(nn::pia::local::LocalNetworkDescription*)
{
}

// 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::GetSubId() const
{
}

// 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::GetLocalCommunicationId() const
{
}

// 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::GetLocalNodeKey(unsigned short) const
{
}

// 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalMigrationManager::vf_0x28()
{
}

// 0x0041CBE4 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::Initialize()
{
}

// 0x0041CD78 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ClearNodeInfo(unsigned char)
{
}

// 0x0041CDF8 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ClearNodeInfo()
{
}

// 0x0041D198 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::CancelHostMigration()
{
}

// 0x0041D1AC | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::SetTransportIdToNodeIdTable(unsigned char, unsigned short)
{
}

// 0x0041D1D0 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ClearTransportIdToNodeIdTable(unsigned char)
{
}

// 0x0041D208 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ClearTransportIdToNodeIdTable()
{
}

// 0x0041D258 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::Cleanup()
{
}

// 0x0041D2A8 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::Startup()
{
}

// 0x0041D3A0 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::Finalize()
{
}

// 0x0041D478 | fefates:bytes [tier B]
nn::pia::local::LocalMigrationManager::LocalMigrationManager()
{
}

// 0x00731238 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::GetMigrationState(unsigned char) const
{
}

// 0x00731264 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::IsExistStateMigrating() const
{
}

// 0x007312A8 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::RemoveBeaconSystemData(void*, const void*, unsigned int) const
{
}

// 0x007313D4 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ConvertLocalNodeIdToTransportId(unsigned short) const
{
}

// 0x00731444 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ConvertTransportIdToLocalNodeId(unsigned char) const
{
}

// 0x0073147C | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::GetNextHostCandidateTransportId() const
{
}

} // namespace local
} // namespace pia
} // namespace nn
