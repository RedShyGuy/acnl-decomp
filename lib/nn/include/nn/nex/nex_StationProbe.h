#pragma once

#include "decomp.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// Picks the URL of a station with the shortest round trip time of the NAT traversal probes. Only
// the two steps are left in the binary (through the shared step tables; no vtable or
// constructor), so the base part and the member names are ours.
class StationProbe
{
public:
    void Reset(); // 0x00360F58 | fefates:bytes [tier B]
    // the URL of the probe with the shortest round trip time; finished when no probe is pending
    void Update(); // 0x00360F9C (name is ours)

    u8 m_Unknown0x00[0x1C];          // 0x00 (unknown base)
    qList<StationURL>::Node* m_pURLs; // 0x1C (the head node of the URLs of the station)
    u32 m_Unknown0x20;               // 0x20
    StationURL m_URL;                // 0x24 (the chosen URL)
    u32 m_RoundTripTime;             // 0x8C (of the chosen URL; -1: none)
    u32 m_Unknown0x90;               // 0x90
    bool m_IsFinished;               // 0x94
};
ASSERT_OFFSET(StationProbe, m_URL, 0x24);
ASSERT_OFFSET(StationProbe, m_RoundTripTime, 0x8C);
ASSERT_OFFSET(StationProbe, m_IsFinished, 0x94);
} // namespace nex
} // namespace nn
