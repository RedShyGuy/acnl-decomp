#pragma once

#include "decomp.h"
#include "nn/nex/nex_NATRelayInterface.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet20NexNatRelayInterfaceE @ 0x008CF96C
// vtable 0x00900388 (vptr 0x00900390), offset_to_top 0, 15 entries
class NexNatRelayInterface : public ::nn::nex::NATRelayInterface
{
public:
    NexNatRelayInterface(); // ctor address unknown
    virtual void vf_0x00(); // 0x003873B8 slot 0x00 | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void vf_0x04(); // 0x00401560 slot 0x04 | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void vf_0x08(); // 0x00401480 slot 0x08 | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void UnregisterRelayClient(); // 0x004014E0 slot 0x0C | slot vf_0x0C of nn::nex::NATRelayInterface
    virtual void vf_0x10(); // 0x00401438 slot 0x10 | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void RelayProbeRequest(const nn::nex::qList<nn::nex::StationURL>&, const nn::nex::StationURL&); // 0x0040143C slot 0x14 | fefates:bytes
    virtual void CheckCurrentPublicPort(nn::nex::CallContext*, const nn::nex::InetAddress&, bool); // 0x00401508 slot 0x18 | slot vf_0x18 of nn::nex::NATRelayInterface
    virtual void vf_0x1C(); // 0x00401558 slot 0x1C | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void RequestProbe(const nn::nex::StationURL&); // 0x004013C8 slot 0x20 | fefates:bytes
    virtual void vf_0x24(); // 0x0040150C slot 0x24 | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void vf_0x28(); // 0x0040148C slot 0x28 | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void vf_0x2C(); // 0x004014D8 slot 0x2C | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void ReportNATTraversalResultDetail(const unsigned int&, const bool&, const nn::nex::NATTraversalResult&, unsigned int&); // 0x0040155C slot 0x30 | slot vf_0x30 of nn::nex::NATRelayInterface
    virtual void UpdateConnectionState(unsigned int, const nn::nex::StationURL&); // 0x004014EC slot 0x34 | fefates:bytes
    virtual void vf_0x38(); // 0x0040142C slot 0x38 | virtual slot, introduced by nn::pia::inet::NexNatRelayInterface
};
} // namespace inet
} // namespace pia
} // namespace nn
