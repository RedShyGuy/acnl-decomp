#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet17NatRelayInterfaceE @ 0x008CF8B4
//
// The relay of the NAT traversal through the nex server (NexNatRelay); NexNatTraversalProtocol
// uses it. The slot names are the ones of NexNatRelay.
class NatRelayInterface : public ::nn::pia::common::RootObject
{
public:
    NatRelayInterface() {}
    virtual ~NatRelayInterface() {} // slot 0x00
    // slot 0x04 (deleting dtor)
    virtual void RequestProbe(const nn::pia::transport::StationLocation& location) = 0; // slot 0x08
    virtual void RelayProbeRequest(const nn::pia::transport::StationLocation& target, const nn::pia::transport::StationLocation& self) = 0; // slot 0x0C
    virtual void ReportNatTraversalResult(const unsigned int& stationKey, const bool& isSucceeded, const unsigned int& rtt) = 0; // slot 0x10
    virtual void ReportNatProperties(const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt) = 0; // slot 0x14
    virtual void SetLocalCID(unsigned int cid) = 0; // slot 0x18
    virtual void AssociateProtocol(nn::pia::inet::NexNatTraversalProtocol* pProtocol) = 0; // slot 0x1C
};
} // namespace inet
} // namespace pia
} // namespace nn
