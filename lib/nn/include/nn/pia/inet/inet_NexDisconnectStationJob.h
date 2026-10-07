#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet23NexDisconnectStationJobE @ 0x008CF9A8
// vtable 0x00900468 (vptr 0x00900470), offset_to_top 0, 8 entries
//
// The disconnection of inet: the NAT traversal to the station that left ends too.
class NexDisconnectStationJob : public ::nn::pia::transport::DisconnectStationJob
{
public:
    NexDisconnectStationJob(); // 0x00404058
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexDisconnectStationJob(); // 0x004588E4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00404070 slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x0072F518 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void OnDisconnected(nn::pia::transport::Station* pStation); // 0x00403EF8 slot 0x18
};
} // namespace inet
} // namespace pia
} // namespace nn
