#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local27LocalAroundNetworkSearchJobE @ 0x008CFD0C
// vtable 0x00901214 (vptr 0x0090121C), offset_to_top 0, 6 entries
class LocalAroundNetworkSearchJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~LocalAroundNetworkSearchJob(); // 0x00422C08 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00422BC4 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007316C0 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void LifeTimeProcess(); // 0x0042151C | fefates:bytes [tier B]
    void SendAroundNetworkStatus(); // 0x004215F8 | fefates:bytes [tier B]
    void WaitAroundNetworkSearch(); // 0x0042164C | fefates:bytes [tier B]
    void ReceiveAroundNetworkInfo(); // 0x0042194C | fefates:bytes [tier B]
    void StartAroundNetworkSearch(); // 0x00421C04 | fefates:bytes [tier B]
    void WaitHostMigrationCompleted(); // 0x00421D88 | fefates:bytes [tier B]
    void WaitAroundNetworkSearchActivated(); // 0x00421ED0 | fefates:bytes [tier B]
    void SendStopAroundNetworkSearchMessage(); // 0x004220B8 | fefates:bytes [tier B]
    void SendStartAroundNetworkSearchMessage(); // 0x0042210C | fefates:bytes [tier B]
    void WaitSendAroundNetworkStatusCompleted(); // 0x00422160 | fefates:bytes [tier B]
    void WaitSendStopAroundNetworkSearchMessageCompleted(); // 0x00422504 | fefates:bytes [tier B]
    void WaitSendStartAroundNetworkSearchMessageCompleted(); // 0x00422750 | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*); // 0x00422AD8 | fefates:bytes [tier B]
    LocalAroundNetworkSearchJob(); // 0x00422B58 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
