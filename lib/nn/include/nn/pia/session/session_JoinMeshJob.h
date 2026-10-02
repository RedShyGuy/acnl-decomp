#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session11JoinMeshJobE @ 0x008CFF30
// vtable 0x00901630 (vptr 0x00901638), offset_to_top 0, 10 entries
class JoinMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    JoinMeshJob(); // ctor candidate(s) 0x0042C030 (unverified)
    virtual ~JoinMeshJob(); // 0x0042C304 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0042C1FC slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00733834 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void StartupImpl(); // 0x0042A19C slot 0x18 | fefates:bytes-fuzzy
    virtual void CleanupImpl(); // 0x0042A198 slot 0x1C | slot vf_0x1C of nn::pia::session::JoinMeshJob
    virtual void SetHostInfoToMonitoringData(const nn::pia::transport::StationConnectionInfo&); // 0x0042BA68 slot 0x20 | slot vf_0x20 of nn::pia::session::JoinMeshJob
    virtual void vf_0x24(); // 0x0042BC08 slot 0x24 | virtual slot, introduced by nn::pia::session::JoinMeshJob
    void WaitJoinResponse(); // 0x0042A7AC | fefates:bytes [tier B]
    void CheckContextCallCanncelled(); // 0x0042B884 | fefates:bytes-fuzzy [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
