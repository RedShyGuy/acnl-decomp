#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session13CreateMeshJobE @ 0x008CFF60
// vtable 0x009016AC (vptr 0x009016B4), offset_to_top 0, 9 entries
class CreateMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    CreateMeshJob(); // ctor candidate(s) 0x00431098 (unverified)
    virtual ~CreateMeshJob(); // 0x004310CC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004310B8 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0073386C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void StartupImpl(); // 0x00430A38 slot 0x18 | fefates:bytes
    virtual void CleanupImpl(); // 0x00430A34 slot 0x1C | slot vf_0x1C of nn::pia::session::CreateMeshJob
    virtual void vf_0x20(); // 0x00430FE0 slot 0x20 | virtual slot, introduced by nn::pia::session::CreateMeshJob
    void SetupSystemProtocols(); // 0x00430EEC | fefates:bytes [tier B]
    void Cleanup(); // 0x00430FE4 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
