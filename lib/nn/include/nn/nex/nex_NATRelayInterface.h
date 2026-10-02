#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17NATRelayInterfaceE @ 0x008CE6D0
// vtable 0x008FD418 (vptr 0x008FD420), offset_to_top 0, 14 entries
class NATRelayInterface : public ::nn::nex::RootObject
{
public:
    NATRelayInterface(); // ctor candidate(s) 0x00387390 (unverified)
    virtual void vf_0x00(); // 0x003873BC slot 0x00 | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void vf_0x04(); // 0x003873A0 slot 0x04 | virtual slot, introduced by nn::nex::NATRelayInterface
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void UnregisterRelayClient(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void RelayProbeRequest(const nn::nex::qList<nn::nex::StationURL>&, const nn::nex::StationURL&); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void CheckCurrentPublicPort(nn::nex::CallContext*, const nn::nex::InetAddress&, bool); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void RequestProbe(const nn::nex::StationURL&); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x2C(); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void ReportNATTraversalResultDetail(const unsigned int&, const bool&, const nn::nex::NATTraversalResult&, unsigned int&); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void UpdateConnectionState(unsigned int, const nn::nex::StationURL&); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
};
} // namespace nex
} // namespace nn
