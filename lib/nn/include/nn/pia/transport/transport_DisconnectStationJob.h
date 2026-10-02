#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport20DisconnectStationJobE @ 0x008D020C
// vtable 0x00901EC4 (vptr 0x00901ECC), offset_to_top 0, 8 entries
class DisconnectStationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~DisconnectStationJob(); // 0x004588E8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004588D4 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007360A8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0045841C slot 0x18 | virtual slot, introduced by nn::pia::transport::DisconnectStationJob
    virtual void StartupImpl(nn::pia::transport::Station*); // 0x00458274 slot 0x1C | fefates:bytes
    void WaitForDisconnection(); // 0x00458420 | fefates:bytes [tier B]
    void DisconnectionSucceeded(); // 0x00458588 | fefates:bytes [tier B]
    void SendDisconnectionRequest(); // 0x00458664 | fefates:bytes [tier B]
    void CutRouteOfRelayConnection(); // 0x004587B8 | fefates:bytes [tier B]
    void Cleanup(); // 0x0045885C | fefates:bytes [tier B]
    DisconnectStationJob(); // 0x00458884 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
