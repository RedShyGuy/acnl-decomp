#include "nn/pia/transport/transport_RelayRouteManager.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00454E64 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::Initialize(unsigned int, bool)
{
}

// 0x00455208 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::SwitchRelay(unsigned int, unsigned int, unsigned short, unsigned short)
{
}

// 0x004553C8 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::CalcStationType(unsigned int, unsigned int*, unsigned int*)
{
}

// 0x00455510 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::ProcStayStation(unsigned int, unsigned int*)
{
}

// 0x0045567C | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::UpdateRelayTable(unsigned int)
{
}

// 0x004557A0 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::ProcNewStationOne(unsigned int, unsigned int)
{
}

// 0x00455B24 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::ProcRefusedStation(unsigned int, unsigned int)
{
}

// 0x00455CCC | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::RelayRouteOptimization(unsigned int)
{
}

// 0x00455DF4 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::SearchRefugeRelayRoute(nn::pia::StationIndex)
{
}

// 0x00455FB8 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::InitRelayRouteManagerData()
{
}

// 0x004565F8 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::RelayRouteOptimizationRTTOne(unsigned int)
{
}

// 0x004568D0 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::DecideBrokenRouteRelayStation(unsigned int, unsigned int*)
{
}

// 0x00456F3C | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::UpdateRttBetweenRelayNodeData(const unsigned char*, unsigned int)
{
}

// 0x004571E8 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::Finalize()
{
}

// 0x00457330 | fefates:bytes [tier B]
nn::pia::transport::RelayRouteManager::RelayRouteManager()
{
}

// 0x007356DC | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::GetRelayRoute(nn::pia::StationIndex, nn::pia::StationIndex, nn::pia::StationIndex*) const
{
}

// 0x00735738 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::GetDestStationList(nn::pia::StationIndex, nn::pia::StationIndex, unsigned int*) const
{
}

// 0x007357C4 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::GetDirectStationList(nn::pia::StationIndex, unsigned int*) const
{
}

// 0x00735854 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::GetOriginalRelayRoute(nn::pia::StationIndex, nn::pia::StationIndex, nn::pia::StationIndex*) const
{
}

// 0x007358B0 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::GetRelayRouteDirectionsSize() const
{
}

// 0x007358F0 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::GetOriginalDirectStationList(nn::pia::StationIndex, unsigned int*) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
