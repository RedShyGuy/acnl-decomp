#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session16KickoutManageJobE @ 0x008CFFE4
// vtable 0x009018F4 (vptr 0x009018FC), offset_to_top 0, 8 entries
class KickoutManageJob : public ::nn::pia::common::StepSequenceJob
{
public:
    struct KickoutReason { u32 _unknown; }; // TODO: real type unknown (placeholder)
    KickoutManageJob(); // ctor candidate(s) 0x00438950 (unverified)
    virtual ~KickoutManageJob(); // 0x004389B4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043898C slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007338F8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00438608 slot 0x18 | virtual slot, introduced by nn::pia::session::KickoutManageJob
    virtual void vf_0x1C(); // 0x0043860C slot 0x1C | virtual slot, introduced by nn::pia::session::KickoutManageJob
    void StartKickout(nn::pia::StationIndex, nn::pia::session::KickoutManageJob::KickoutReason); // 0x004382C4 | fefates:bytes [tier B]
    void ClientWaitLeaveMesh(); // 0x00438610 | fefates:bytes [tier B]
    void AssociateKickoutWith(nn::pia::common::CallContext*); // 0x00438690 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
