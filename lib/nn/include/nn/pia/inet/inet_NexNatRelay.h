#pragma once

#include "decomp.h"
#include "nn/pia/inet/inet_NatRelayInterface.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet11NexNatRelayE @ 0x008CF830
// vtable 0x008FFEB8 (vptr 0x008FFEC0), offset_to_top 0, 8 entries
class NexNatRelay : public ::nn::pia::inet::NatRelayInterface
{
public:
    virtual ~NexNatRelay(); // 0x003E4154 slot 0x00 | fefates:bytes
    // 0x003E4144 slot 0x04 | slot vf_0x04 of nn::pia::inet::NexNatRelay (deleting dtor)
    virtual void RequestProbe(const nn::pia::transport::StationLocation&); // 0x003E3B50 slot 0x08 | slot vf_0x08 of nn::pia::inet::NexNatRelay
    virtual void RelayProbeRequest(const nn::pia::transport::StationLocation&, const nn::pia::transport::StationLocation&); // 0x003E3B6C slot 0x0C | slot vf_0x0C of nn::pia::inet::NexNatRelay
    virtual void ReportNatTraversalResult(const unsigned int&, const bool&, const unsigned int&); // 0x003E3EC4 slot 0x10 | fefates:bytes
    virtual void ReportNatProperties(const unsigned int&, const unsigned int&, const unsigned int&); // 0x003E3E48 slot 0x14 | fefates:bytes
    virtual void SetLocalCID(unsigned int); // 0x003E3B3C slot 0x18 | slot vf_0x18 of nn::pia::inet::NexNatRelay
    virtual void AssociateProtocol(nn::pia::inet::NexNatTraversalProtocol*); // 0x003E3B64 slot 0x1C | slot vf_0x1C of nn::pia::inet::NexNatRelay
    NexNatRelay(); // 0x003E3F28 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
