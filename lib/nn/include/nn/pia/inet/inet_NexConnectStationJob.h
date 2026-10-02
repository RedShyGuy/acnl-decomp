#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet20NexConnectStationJobE @ 0x008CF960
// vtable 0x00900360 (vptr 0x00900368), offset_to_top 0, 8 entries
class NexConnectStationJob : public ::nn::pia::transport::ConnectStationJob
{
public:
    virtual ~NexConnectStationJob(); // 0x00401380 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00401338 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F174 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void StartupImpl(nn::pia::common::CallContext*, nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&, bool); // 0x004001F0 slot 0x18 | slot vf_0x18 of nn::pia::transport::ConnectStationJob
    virtual void CleanupImpl(); // 0x00400140 slot 0x1C | fefates:callseq
    void tryCurrentAddress(); // 0x004004FC | fefates:bytes [tier B]
    void testCurrentAddress(); // 0x004005CC | fefates:bytes [tier B]
    void resolveCurrentAddress(); // 0x00400A24 | fefates:bytes [tier B]
    NexConnectStationJob(); // 0x004012E4 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
