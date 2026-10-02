#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session12LeaveMeshJobE @ 0x008CFF48
// vtable 0x00901660 (vptr 0x00901668), offset_to_top 0, 6 entries
class LeaveMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~LeaveMeshJob(); // 0x0042CAFC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0042CAEC slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00733838 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void SendLeaveRequest(); // 0x0042C4C0 | fefates:bytes [tier B]
    void WaitLeaveResponse(); // 0x0042C544 | fefates:bytes [tier B]
    void WaitLeavingProcess(); // 0x0042C660 | fefates:bytes [tier B]
    void RegisterExtraCallback(nn::pia::common::CallContext*); // 0x0042C744 | fefates:bytes [tier B]
    void StartDisconnectStations(); // 0x0042C76C | fefates:bytes [tier B]
    void Cleanup(); // 0x0042C8EC | fefates:bytes [tier B]
    LeaveMeshJob(); // 0x0042CAA8 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
