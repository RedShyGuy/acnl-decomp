#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace transport {
// 0x007352E4 slot 0x00 | virtual slot, introduced by nn::pia::transport::StationManager
void nn::pia::transport::StationManager::vf_0x00()
{
}

// 0x0044F6E4 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::GetStation(nn::pia::StationId)
{
}

// 0x0044F754 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::GetStation(const nn::pia::common::StationAddress&)
{
}

// 0x0044F7BC | fefates:bytes [tier B]
void nn::pia::transport::StationManager::Initialize(nn::pia::transport::NetworkFactory*, unsigned int)
{
}

// 0x0044F980 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::CreateStation()
{
}

// 0x0044FA88 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::DestroyStation(nn::pia::transport::Station*)
{
}

// 0x0044FBD4 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::DestroyInstance()
{
}

// 0x0044FC28 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::CreateLocalStation()
{
}

// 0x0044FCE4 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::Finalize()
{
}

// 0x0044FDAC | fefates:bytes [tier B]
nn::pia::transport::StationManager::StationManager()
{
}

// 0x00735194 | libgarden [tier A]
void nn::pia::transport::StationManager::GetStationAddress(nn::pia::common::StationAddress*, nn::pia::StationId) const
{
}

// 0x007351C0 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::GetStationAddress(nn::pia::common::StationAddress*, nn::pia::StationIndex) const
{
}

// 0x00735270 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::GetParticipatingStationBitmap(bool) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
