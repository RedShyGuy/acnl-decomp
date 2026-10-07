#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace common {
class CallContext;
}
namespace transport {
class Station;
class StationConnectionInfo;

// RTTI N2nn3pia9transport17ConnectStationJobE @ 0x008D01DC
// vtable 0x00901E74 (vptr 0x00901E7C), offset_to_top 0, 8 entries
//
// Connects a station: sends the connection request (resent by ResendingMessageManager until it is
// acked), then waits until StationProtocol has the station's connection response. The steps are
// from the strings of the binary; the layout is from the constructor, the member names are ours.
class ConnectStationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ConnectStationJob(); // 0x004540F4 | fefates:bytes [tier B]
    virtual ~ConnectStationJob(); // 0x0045414C slot 0x00 | fefates:callgraph
    // 0x0045413C slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x007356D4 slot 0x14
    virtual nn::Result StartupImpl(nn::pia::common::CallContext* pCallContext, nn::pia::transport::Station* pStation, const nn::pia::transport::StationConnectionInfo& info, bool isInverseConnection); // 0x004534A8 slot 0x18 | fefates:bytes
    virtual void CleanupImpl(); // 0x004534A4 slot 0x1C

    nn::Result Startup(nn::pia::common::CallContext* pCallContext, nn::pia::transport::Station* pStation, const nn::pia::transport::StationConnectionInfo& info, bool isInverseConnection, int timeoutMSec); // 0x004540D0 | fefates:bytes [tier B]
    nn::Result StartupRelayConnection(nn::pia::common::CallContext* pCallContext, nn::pia::transport::Station* pStation, const nn::pia::transport::StationConnectionInfo& info, bool isInverseConnection, int timeoutMSec); // 0x00453CD0 | fefates:bytes [tier B]
    void Cleanup(); // 0x00454024 | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult SendConnectionRequest(); // 0x00453AF4 | fefates:bytes [tier B]
    common::ExecuteResult SendRelayConnectionRequest(); // 0x00453D60 | fefates:bytes [tier B]
    common::ExecuteResult WaitRequestAck(); // 0x00453604 | fefates:bytes [tier B]
    common::ExecuteResult WaitForConnection(); // 0x004538FC | fefates:bytes [tier B]
    common::ExecuteResult ConnectionSucceeded(); // 0x00453A50 | fefates:bytes [tier B]
    common::ExecuteResult ConnectionFailed(); // 0x00453898 | fefates:bytes [tier B]

    // the request through ResendingMessageManager; false if it could not be set
    bool SendConnectionRequestMessage(); // 0x00453F18 | fefates:bytes [tier B]

    // a station that was being connected is given up (inline in the steps; name is ours)
    void GiveUpStation();

    common::Time m_Deadline;             // 0x40
    s32 m_TimeoutMSec;                   // 0x48
    u32 m_AckId;                         // 0x4C, of the request (0: none)
    common::CallContext* m_pCallContext; // 0x50, of the caller (null when signaled)
    Station* m_pStation;                 // 0x54
    bool m_IsInverseConnection;          // 0x58, byte 3 of the request
    u8 m_DenyReason;                     // 0x59, byte 1 of a denying response (StationProtocol)
};
ASSERT_OFFSET(ConnectStationJob, m_Deadline, 0x40);
ASSERT_OFFSET(ConnectStationJob, m_DenyReason, 0x59);
} // namespace transport
} // namespace pia
} // namespace nn
