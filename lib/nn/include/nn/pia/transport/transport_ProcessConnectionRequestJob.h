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

// RTTI N2nn3pia9transport27ProcessConnectionRequestJobE @ 0x008D029C
// vtable 0x00902024 (vptr 0x0090202C), offset_to_top 0, 6 entries
//
// Answers the connection request of a station: first connects back to it (the inverse connection,
// a ConnectStationJob with its own call context), then sends the connection response until it is
// acked. The steps are from the strings of the binary; the layout is from the constructor, the
// member names are ours.
class ProcessConnectionRequestJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ProcessConnectionRequestJob(); // 0x0045F0A4 | fefates:bytes [tier B]
    virtual ~ProcessConnectionRequestJob(); // 0x0045F164 slot 0x00
    // 0x0045F0FC slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x00736D0C slot 0x14

    // false without a station or for an inverse connection request of an unknown station
    bool Startup(nn::pia::transport::Station* pStation, int timeoutMSec, bool isInverseConnection); // 0x0045EEF4 | fefates:bytes [tier B]
    bool StartupRelayConnection(nn::pia::transport::Station* pStation, int timeoutMSec, bool isInverseConnection); // 0x0045EC7C | fefates:bytes [tier B]
    void Cleanup(); // 0x0045EE98 | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult ConnectToRequesterStation(); // 0x0045EE2C | fefates:bytes [tier B]
    common::ExecuteResult WaitInverseConnection(); // 0x0045E9EC | fefates:bytes [tier B]
    common::ExecuteResult SendConnectionResponse(); // 0x0045EAF8 | fefates:bytes [tier B]
    common::ExecuteResult WaitResponseAck(); // 0x0045E904 | fefates:bytes [tier B]

    common::Time m_Deadline;             // 0x40
    s32 m_TimeoutMSec;                   // 0x48
    u32 m_AckId;                         // 0x4C, of the response
    Station* m_pStation;                 // 0x50
    common::CallContext* m_pCallContext; // 0x54, of the inverse connection (its own)
    bool m_IsRelay;                      // 0x58
};
ASSERT_OFFSET(ProcessConnectionRequestJob, m_pCallContext, 0x54);
} // namespace transport
} // namespace pia
} // namespace nn
