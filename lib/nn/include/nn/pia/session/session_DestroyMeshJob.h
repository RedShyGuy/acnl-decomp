#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session14DestroyMeshJobE @ 0x008CFF6C
// vtable 0x009016D8 (vptr 0x009016E0), offset_to_top 0, 6 entries
class DestroyMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~DestroyMeshJob(); // 0x0043159C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043158C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00733870 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void AssociateSystemWith(nn::pia::common::CallContext*); // 0x004312D4 | fefates:bytes [tier B]
    void WaitDestroyResponse(); // 0x00431318 | fefates:bytes [tier B]
    void ReceiveDestroyResponse(nn::pia::StationIndex); // 0x004313D4 | fefates:bytes [tier B]
    void Cleanup(); // 0x004313EC | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*, bool); // 0x00431444 | fefates:bytes-fuzzy [tier B]
    DestroyMeshJob(); // 0x00431540 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
