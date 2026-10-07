#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace transport {
// The relay routes between the stations of the session: for each pair of stations the station the
// messages go through (the destination itself if they are connected directly; m_BrokenRoute if
// there is none), from the round trip times between all stations. The relay stations are chosen
// by the round trip time over them (at most m_RttLimit) and by how many routes already go through
// them (at most m_RelayCountMax, 2 per route). The tables are n * n with n the number of stations
// of the Transport. Layout from the constructor and Initialize; the member names and the names of
// the unnamed functions are ours.
class RelayRouteManager
{
public:
    RelayRouteManager(); // 0x00457330 | fefates:bytes [tier B]

    // isUsingBlockedTable: the table of the connections that are not to be used (0x14)
    nn::Result Initialize(unsigned int stationNum, bool isUsingBlockedTable); // 0x00454E64 | fefates:bytes [tier B]
    void Finalize(); // 0x004571E8 | fefates:bytes [tier B]
    // every station its own route, no stations
    void InitRelayRouteManagerData(); // 0x00455FB8 | fefates:bytes [tier B]

    void SetRttLimit(u16 rttLimit); // 0x0045731C (name is ours)
    void SetRelayCountMax(u16 relayCountMax); // 0x004553B0 (name is ours)
    nn::Result SetRelayRoute(nn::pia::StationIndex from, nn::pia::StationIndex to, nn::pia::StationIndex relay); // 0x0045537C (name is ours)

    // the round trip times between the stations (rttTable: n * n, in units of 4 ms) and the new
    // routes; pRefusedBitmap: the stations without a route; pReasons: why (per station)
    nn::Result UpdateDirectConnectionData(const unsigned char* rttTable, unsigned int size, unsigned int stationBitmap, unsigned int* pRefusedBitmap, nn::pia::StationIndex firstStationIndex, unsigned char* pReasons); // 0x00456060 | fefates:bytes [tier B]
    // the round trip times to the relay node (n, in units of 4 ms)
    nn::Result UpdateRttBetweenRelayNodeData(const unsigned char* rttTable, unsigned int size); // 0x00456F3C | fefates:bytes [tier B]
    // the routes as GetRelayRouteDirectionsSize packs them
    nn::Result SetRelayRouteDirections(const unsigned char* pData, unsigned int size); // 0x00456FB8 (name is ours)

    // the stations whose routes stay (UpdateDirectConnectionData passes them to ProcStayStation)
    // and the ones whose routes are reset
    void CalcStationType(unsigned int stationBitmap, unsigned int* pStayBitmap, unsigned int* pResetBitmap); // 0x004553C8 | fefates:bytes [tier B]
    void ProcStayStation(unsigned int stationBitmap, unsigned int* pRefusedBitmap); // 0x00455510 | fefates:bytes [tier B]
    bool ProcNewStationOne(unsigned int stationIndex, unsigned int stationBitmap); // 0x004557A0 | fefates:bytes [tier B]
    void ProcRefusedStation(unsigned int refusedBitmap, unsigned int stationBitmap); // 0x00455B24 | fefates:bytes [tier B]
    void UpdateRelayTable(unsigned int stationBitmap); // 0x0045567C | fefates:bytes [tier B]
    // moves a route that goes through relay to another relay station
    bool SwitchRelay(unsigned int relay, unsigned int stationBitmap, unsigned short rttLimit, unsigned short relayCountLimit); // 0x00455208 | fefates:bytes [tier B]
    void RelayRouteOptimization(unsigned int stationBitmap); // 0x00455CCC | fefates:bytes [tier B]
    bool RelayRouteOptimizationRTTOne(unsigned int stationBitmap); // 0x004565F8 | fefates:bytes [tier B]
    // a new relay for a broken route; false if a station had to be refused
    bool DecideBrokenRouteRelayStation(unsigned int stationBitmap, unsigned int* pRefusedBitmap); // 0x004568D0 | fefates:bytes [tier B]
    nn::Result SearchRefugeRelayRoute(nn::pia::StationIndex brokenStationIndex); // 0x00455DF4 | fefates:bytes [tier B]

    nn::Result GetRelayRoute(nn::pia::StationIndex from, nn::pia::StationIndex to, nn::pia::StationIndex* pRelay) const; // 0x007356DC | fefates:bytes [tier B]
    // the stations whose messages from "from" go through relay
    nn::Result GetDestStationList(nn::pia::StationIndex from, nn::pia::StationIndex relay, unsigned int* pBitmap) const; // 0x00735738 | fefates:bytes [tier B]
    nn::Result GetDirectStationList(nn::pia::StationIndex from, unsigned int* pBitmap) const; // 0x007357C4 | fefates:bytes [tier B]
    nn::Result GetOriginalRelayRoute(nn::pia::StationIndex from, nn::pia::StationIndex to, nn::pia::StationIndex* pRelay) const; // 0x00735854 | fefates:bytes [tier B]
    nn::Result GetOriginalDirectStationList(nn::pia::StationIndex from, unsigned int* pBitmap) const; // 0x007358F0 | fefates:bytes [tier B]
    u32 GetRelayRouteDirectionsSize() const; // 0x007358B0 | fefates:bytes [tier B]
    // the routes packed for MeshProtocol (name is ours)
    nn::Result GetRelayRouteDirections(unsigned char* pBuffer, unsigned int bufferSize, unsigned int* pSize) const; // 0x00735980

    u16* m_pRttTable;              // 0x00, n * n, 0: no connection
    u16* m_pRelayNodeRttTable;     // 0x04, n
    u8* m_pRelayRouteTable;        // 0x08, n * n
    u8* m_pRelayCount;             // 0x0C, n
    u32* m_pConnectionBitmaps;     // 0x10, n
    u8* m_pBlockedTable;           // 0x14, n * n (1: not to be used)
    u8* m_pRefuseReasons;          // 0x18, n (1: no route, 2: too slow, 3: too many routes)
    u16 m_RttLimit;                // 0x1C
    u16 m_RelayCountMax;           // 0x1E
    // the version of the relay route directions (RelayRouteManageJob, MeshProtocol)
    u32 m_DirectionsVersionLow;    // 0x20
    u32 m_DirectionsVersionHigh;   // 0x24
    StationIndex m_FirstStationIndex; // 0x28
    u8 m_RouteBits;                // 0x29, the bits of a route in the packed directions
    u8 m_BrokenRoute;              // 0x2A
    u8 m_Unknown0x2B;              // 0x2B
    u32 m_RoutedStationBitmap;     // 0x2C
    u32 m_RefusedStationBitmap;    // 0x30
    bool m_IsUsingBlockedTable;    // 0x34
    u8* m_pOriginalRelayRouteTable; // 0x38, n * n
};
ASSERT_OFFSET(RelayRouteManager, m_RttLimit, 0x1C);
ASSERT_OFFSET(RelayRouteManager, m_FirstStationIndex, 0x28);
ASSERT_SIZE(RelayRouteManager, 0x3C);
} // namespace transport
} // namespace pia
} // namespace nn
