#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local30LocalForceDisconnectNetworkJobE @ 0x008CFD54
// vtable 0x00901320 (vptr 0x00901328), offset_to_top 0, 6 entries
class LocalForceDisconnectNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    struct ProcType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    LocalForceDisconnectNetworkJob(); // ctor candidate(s) 0x0042388C (unverified)
    virtual ~LocalForceDisconnectNetworkJob(); // 0x004238BC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004238AC slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0073176C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void WaitDisconnected(); // 0x00423614 | fefates:bytes [tier B]
    void WaitHostMigrationEnd(); // 0x004236AC | fefates:bytes [tier B]
    void Cleanup(); // 0x00423778 | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*, nn::pia::local::LocalForceDisconnectNetworkJob::ProcType); // 0x004237AC | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
