#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace transport {
class Station;

// RTTI N2nn3pia9transport20DisconnectStationJobE @ 0x008D020C
// vtable 0x00901EC4 (vptr 0x00901ECC), offset_to_top 0, 8 entries
//
// Disconnects a station: sends the disconnection request every m_IntervalMSec until the response
// (StationProtocol) or the timeout, then removes the station. A station behind a relay is cut off
// at once. The steps are from the strings of the binary; the layout is from the constructor, the
// member names and the names marked so are ours.
class DisconnectStationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    DisconnectStationJob(); // 0x00458884 | fefates:bytes [tier B]
    virtual ~DisconnectStationJob(); // 0x004588E8 slot 0x00
    // 0x004588D4 slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x007360A8 slot 0x14
    // called when the station is gone (empty here; name is ours)
    virtual void OnDisconnected(nn::pia::transport::Station* pStation); // 0x0045841C slot 0x18
    virtual nn::Result StartupImpl(nn::pia::transport::Station* pStation); // 0x00458274 slot 0x1C | fefates:bytes

    // (name is ours)
    nn::Result Startup(nn::pia::transport::Station* pStation); // 0x00458878
    void Cleanup(); // 0x0045885C | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult SendDisconnectionRequest(); // 0x00458664 | fefates:bytes [tier B]
    common::ExecuteResult CutRouteOfRelayConnection(); // 0x004587B8 | fefates:bytes [tier B]
    common::ExecuteResult WaitForDisconnection(); // 0x00458420 | fefates:bytes [tier B]
    common::ExecuteResult DisconnectionSucceeded(); // 0x00458588 | fefates:bytes [tier B]

    common::Time m_Deadline;      // 0x40
    s32 m_TimeoutMSec;            // 0x48 (4000)
    Station* m_pStation;          // 0x4C
    common::Time m_NextSendTime;  // 0x50
    s32 m_IntervalMSec;           // 0x58 (500)
    bool m_IsWaitingResponse;     // 0x5C, cleared by StationProtocol on the response
};
ASSERT_OFFSET(DisconnectStationJob, m_pStation, 0x4C);
ASSERT_OFFSET(DisconnectStationJob, m_IsWaitingResponse, 0x5C);
} // namespace transport
} // namespace pia
} // namespace nn
