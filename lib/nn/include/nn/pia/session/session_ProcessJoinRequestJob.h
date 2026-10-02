#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session21ProcessJoinRequestJobE @ 0x008D00A4
// vtable 0x00901AA4 (vptr 0x00901AAC), offset_to_top 0, 6 entries
class ProcessJoinRequestJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~ProcessJoinRequestJob(); // 0x004406B8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00440644 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0073405C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void InitialStep(); // 0x0043F848 | fefates:bytes [tier B]
    void WaitResponseAck(); // 0x0043F96C | fefates:bytes [tier B]
    void CancellationNotice(nn::pia::StationIndex); // 0x00440320 | fefates:bytes [tier B]
    void SendDenyingJoinResponse(); // 0x00440334 | fefates:bytes [tier B]
    void Cleanup(); // 0x00440408 | fefates:bytes [tier B]
    void Startup(); // 0x00440488 | fefates:bytes [tier B]
    ProcessJoinRequestJob(); // 0x00440520 | fefates:bytes-fuzzy [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
