#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_LatestMedian.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/session/session_SyncClock.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
class ProtocolEvent;
class ProtocolMessageReader;
} // namespace transport
namespace session {
// RTTI N2nn3pia7session17SyncClockProtocolE @ 0x008D0014
// vtable 0x0090195C (vptr 0x00901964), offset_to_top 0, 9 entries
//
// The clock shared by the stations: the clients ask the host for its clock every
// m_RequestIntervalMSec (16 bytes: the time of the request and the clock of the host, both big
// endian) and set their base time from the answer and half the median round trip time. The layout
// is from the constructor; the member names and the names marked so are ours.
class SyncClockProtocol : public ::nn::pia::transport::Protocol
{
public:
    SyncClockProtocol(); // 0x00439854 | fefates:bytes [tier B]
    virtual ~SyncClockProtocol(); // 0x004398C8 slot 0x00
    // 0x004398A4 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00733908 slot 0x08
    virtual u16 GetProtocolType() const; // 0x00733900 slot 0x0C
    virtual nn::Result Startup(nn::pia::StationIndex localStationIndex); // 0x004394B4 slot 0x10
    virtual void Cleanup(); // 0x0043949C slot 0x14 | fefates:bytes
    virtual nn::Result Dispatch(); // 0x004395DC slot 0x18 | fefates:callseq
    virtual nn::Result UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event); // 0x004393CC slot 0x1C

    // the client: the answer of the host
    void processByClient(const nn::pia::transport::ProtocolMessageReader* pReader); // 0x00439028 | fefates:bytes [tier B]
    // the host: the answer to a request
    nn::Result response(nn::pia::StationIndex stationIndex, unsigned long long requestTime); // 0x0043976C | fefates:callgraph
    // the client: the request to the host (name is ours)
    nn::Result SendRequest(); // 0x004394E0

    SyncClock m_SyncClock;                  // 0x18
    StationIndex m_LocalStationIndex;       // 0x30 (253: not started)
    s32 m_RequestIntervalMSec;              // 0x34 (2000, Mesh::SetSyncClockRequestInterval)
    common::Time m_LastRequestTime;         // 0x38
    bool m_IsRequesting;                    // 0x40, the host is there
    bool m_IsRequestSent;                   // 0x41
    common::LatestMedian<s32, 10> m_Rtts;   // 0x44, the round trip times of the requests in ms
};
ASSERT_OFFSET(SyncClockProtocol, m_SyncClock, 0x18);
ASSERT_OFFSET(SyncClockProtocol, m_Rtts, 0x44);
ASSERT_SIZE(SyncClockProtocol, 0x78);
} // namespace session
} // namespace pia
} // namespace nn
