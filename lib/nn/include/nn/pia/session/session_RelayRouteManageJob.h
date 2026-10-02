#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session19RelayRouteManageJobE @ 0x008D0050
// vtable 0x009019F4 (vptr 0x009019FC), offset_to_top 0, 6 entries
class RelayRouteManageJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~RelayRouteManageJob(); // 0x0043A904 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043A860 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00733928 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void UpdateConnectionReport(nn::pia::StationIndex, const unsigned char*, unsigned int); // 0x0043A094 | fefates:bytes [tier B]
    void PrepareForBecomingNewHost(); // 0x0043A2E4 | fefates:bytes [tier B]
    void Cleanup(); // 0x0043A5B0 | fefates:bytes [tier B]
    void Startup(nn::pia::StationIndex, unsigned int, unsigned int); // 0x0043A5EC | fefates:bytes [tier B]
    RelayRouteManageJob(); // 0x0043A6C4 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
