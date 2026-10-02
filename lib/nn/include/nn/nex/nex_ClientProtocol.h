#pragma once

#include "decomp.h"
#include "nn/nex/nex_Protocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14ClientProtocolE @ 0x008CE2D8
// vtable 0x008FCA54 (vptr 0x008FCA5C), offset_to_top 0, 23 entries
class ClientProtocol : public ::nn::nex::Protocol
{
public:
    ClientProtocol(); // ctor candidate(s) 0x00370E64 (unverified)
    virtual ~ClientProtocol(); // 0x00370EF0 slot 0x00 | fefates:bytes
    // 0x00370EDC slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x08(); // 0x0072AE64 slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x0C(); // 0x0072AE8C slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x10(); // 0x00370E60 slot 0x10 | virtual slot, introduced by nn::nex::SystemComponent
    virtual void BeginInitialization(); // 0x003D5D8C slot 0x20 | fefates:bytes
    virtual void BeginTermination(); // 0x003D5BD0 slot 0x28 | fefates:bytes
    virtual void GetProtocolType() const; // 0x0072AE5C slot 0x40 | slot vf_0x40 of nn::nex::ClientProtocol
    virtual void vf_0x44(); // 0x003D5DF8 slot 0x44 | virtual slot, introduced by nn::nex::ClientProtocol
    virtual void vf_0x48(); // 0x003D5A48 slot 0x48 | virtual slot, introduced by nn::nex::ClientProtocol
    virtual void vf_0x4C(); // 0x0072E9E8 slot 0x4C | virtual slot, introduced by nn::nex::ClientProtocol
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
    virtual void CreateResponder() const; // 0x0011C12F slot 0x54 | slot vf_0x00 of ChangeRentalBase
    virtual void SetDefaultCredentials(nn::nex::Credentials*); // 0x00370DD4 slot 0x58 | fefates:bytes
    void ProcessResponse(nn::nex::Message*, nn::nex::EndPoint*); // 0x00370A18 | fefates:bytes [tier B]
    void SendOverLocalLoopback(nn::nex::ProtocolCallContext*, nn::nex::Message*); // 0x00370C88 | fefates:bytes [tier B]
    ClientProtocol(unsigned int); // 0x00370E64 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
