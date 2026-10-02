#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport27ProcessConnectionRequestJobE @ 0x008D029C
// vtable 0x00902024 (vptr 0x0090202C), offset_to_top 0, 6 entries
class ProcessConnectionRequestJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~ProcessConnectionRequestJob(); // 0x0045F164 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0045F0FC slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00736D0C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void WaitResponseAck(); // 0x0045E904 | fefates:bytes [tier B]
    void WaitInverseConnection(); // 0x0045E9EC | fefates:bytes [tier B]
    void SendConnectionResponse(); // 0x0045EAF8 | fefates:bytes [tier B]
    void StartupRelayConnection(nn::pia::transport::Station*, int, bool); // 0x0045EC7C | fefates:bytes [tier B]
    void ConnectToRequesterStation(); // 0x0045EE2C | fefates:bytes [tier B]
    void Cleanup(); // 0x0045EE98 | fefates:bytes [tier B]
    void Startup(nn::pia::transport::Station*, int, bool); // 0x0045EEF4 | fefates:bytes [tier B]
    ProcessConnectionRequestJob(); // 0x0045F0A4 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
