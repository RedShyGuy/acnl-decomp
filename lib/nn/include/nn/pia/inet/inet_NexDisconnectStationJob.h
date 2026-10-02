#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet23NexDisconnectStationJobE @ 0x008CF9A8
// vtable 0x00900468 (vptr 0x00900470), offset_to_top 0, 8 entries
class NexDisconnectStationJob : public ::nn::pia::transport::DisconnectStationJob
{
public:
    NexDisconnectStationJob(); // ctor candidate(s) 0x00404058 (unverified)
    virtual ~NexDisconnectStationJob(); // 0x004588E4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00404070 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F518 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00403EF8 slot 0x18 | virtual slot, introduced by nn::pia::transport::DisconnectStationJob
};
} // namespace inet
} // namespace pia
} // namespace nn
