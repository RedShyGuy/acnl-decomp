#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class RelayRouteManager
{
public:
    void Initialize(unsigned int, bool); // 0x00454E64 | fefates:bytes [tier B]
    void SwitchRelay(unsigned int, unsigned int, unsigned short, unsigned short); // 0x00455208 | fefates:bytes [tier B]
    void CalcStationType(unsigned int, unsigned int*, unsigned int*); // 0x004553C8 | fefates:bytes [tier B]
    void ProcStayStation(unsigned int, unsigned int*); // 0x00455510 | fefates:bytes [tier B]
    void UpdateRelayTable(unsigned int); // 0x0045567C | fefates:bytes [tier B]
    void ProcNewStationOne(unsigned int, unsigned int); // 0x004557A0 | fefates:bytes [tier B]
    void ProcRefusedStation(unsigned int, unsigned int); // 0x00455B24 | fefates:bytes [tier B]
    void RelayRouteOptimization(unsigned int); // 0x00455CCC | fefates:bytes [tier B]
    void SearchRefugeRelayRoute(nn::pia::StationIndex); // 0x00455DF4 | fefates:bytes [tier B]
    void InitRelayRouteManagerData(); // 0x00455FB8 | fefates:bytes [tier B]
    void RelayRouteOptimizationRTTOne(unsigned int); // 0x004565F8 | fefates:bytes [tier B]
    void DecideBrokenRouteRelayStation(unsigned int, unsigned int*); // 0x004568D0 | fefates:bytes [tier B]
    void UpdateRttBetweenRelayNodeData(const unsigned char*, unsigned int); // 0x00456F3C | fefates:bytes [tier B]
    void Finalize(); // 0x004571E8 | fefates:bytes [tier B]
    RelayRouteManager(); // 0x00457330 | fefates:bytes [tier B]
    void GetRelayRoute(nn::pia::StationIndex, nn::pia::StationIndex, nn::pia::StationIndex*) const; // 0x007356DC | fefates:bytes [tier B]
    void GetDestStationList(nn::pia::StationIndex, nn::pia::StationIndex, unsigned int*) const; // 0x00735738 | fefates:bytes [tier B]
    void GetDirectStationList(nn::pia::StationIndex, unsigned int*) const; // 0x007357C4 | fefates:bytes [tier B]
    void GetOriginalRelayRoute(nn::pia::StationIndex, nn::pia::StationIndex, nn::pia::StationIndex*) const; // 0x00735854 | fefates:bytes [tier B]
    void GetRelayRouteDirectionsSize() const; // 0x007358B0 | fefates:bytes [tier B]
    void GetOriginalDirectStationList(nn::pia::StationIndex, unsigned int*) const; // 0x007358F0 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
