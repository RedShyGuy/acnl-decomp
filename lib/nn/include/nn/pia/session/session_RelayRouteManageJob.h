#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session19RelayRouteManageJobE @ 0x008D0050
// vtable 0x009019F4 (vptr 0x009019FC), offset_to_top 0, 6 entries
//
// The relay routes on the host: collects the connection reports of the stations (the round trip
// time to every other station, MeshProtocol), adds its own row and lets RelayRouteManager compute
// the routes, then sends the directions. The steps are from the strings of the binary; the layout
// is from the constructor, the member names are ours.
class RelayRouteManageJob : public ::nn::pia::common::StepSequenceJob
{
public:
    RelayRouteManageJob(); // 0x0043A6C4 | fefates:bytes [tier B]
    virtual ~RelayRouteManageJob(); // 0x0043A904 slot 0x00
    // 0x0043A860 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x00733928 slot 0x14

    // false if the job runs or the report is not newer
    bool Startup(nn::pia::StationIndex stationIndex, unsigned int version, unsigned int sequence); // 0x0043A5EC | fefates:bytes [tier B]
    void Cleanup(); // 0x0043A5B0 | fefates:bytes [tier B]
    void UpdateConnectionReport(nn::pia::StationIndex stationIndex, const unsigned char* pReport, unsigned int size); // 0x0043A094 | fefates:bytes [tier B]
    void PrepareForBecomingNewHost(); // 0x0043A2E4 | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult WaitAllDirectConnectionReport(); // 0x0043A310
    common::ExecuteResult SendRelayRouteDirections(); // 0x0043A164

    u32 m_StationNum;                    // 0x40 (Transport)
    u32 m_Version;                       // 0x44, of the reports (high word of the directions version)
    u32 m_DirectionsVersion;             // 0x48, the low word
    u8* m_pRttTable;                     // 0x4C, a row per station: the round trip time to each in units of 4 ms
    u8* m_pReportFormats;                // 0x50, byte 12 of the report of each station
    u32* m_pReportSequences;             // 0x54, of the last report of each station (0: none)
    u32 m_StationBitmap;                 // 0x58, for RelayRouteManager
    u32 m_KickoutStationBitmap;          // 0x5C, the stations without a route (MeshProtocol kicks them out)
    u8* m_pKickoutReasons;               // 0x60, per station
    common::Time m_NextCheckTime;        // 0x68, of the stations without a round trip time
};
ASSERT_OFFSET(RelayRouteManageJob, m_KickoutStationBitmap, 0x5C);
ASSERT_OFFSET(RelayRouteManageJob, m_NextCheckTime, 0x68);
ASSERT_SIZE(RelayRouteManageJob, 0x70);
} // namespace session
} // namespace pia
} // namespace nn
