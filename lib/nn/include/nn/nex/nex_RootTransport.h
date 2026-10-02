#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13RootTransportE @ 0x008CE278
// vtable 0x008FC928 (vptr 0x008FC930), offset_to_top 0, 17 entries
class RootTransport : public ::nn::nex::RootObject
{
public:
    RootTransport(); // ctor address unknown
    virtual ~RootTransport(); // 0x0036BCD0 slot 0x00 | slot vf_0x00 of nn::nex::RootTransport
    // 0x0036BCA0 slot 0x04 | slot vf_0x04 of nn::nex::RootTransport (deleting dtor)
    virtual void vf_0x08(); // 0x0036AED4 slot 0x08 | virtual slot, introduced by nn::nex::RootTransport
    virtual void StartListen(unsigned short, unsigned short*, bool, unsigned int, bool); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void Receive(unsigned short, nn::nex::Buffer*, const nn::nex::InetAddress*); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0036B69C slot 0x28 | virtual slot, introduced by nn::nex::RootTransport
    virtual void vf_0x2C(); // 0x0036B694 slot 0x2C | virtual slot, introduced by nn::nex::RootTransport
    virtual void vf_0x30(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x34(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x38(); // 0x0036B678 slot 0x38 | virtual slot, introduced by nn::nex::RootTransport
    virtual void vf_0x3C(); // 0x0036B59C slot 0x3C | virtual slot, introduced by nn::nex::RootTransport
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    void GetNextPortNumber(); // 0x0036B24C | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
