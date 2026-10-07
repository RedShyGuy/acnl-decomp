#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13ServiceClientE @ 0x008CE290
// vtable 0x008FC990 (vptr 0x008FC998), offset_to_top 0, 10 entries
class ServiceClient : public ::nn::nex::RootObject
{
public:
    ServiceClient(); // ctor candidate(s) 0x0036D600 (unverified)
    virtual ~ServiceClient(); // 0x0036D6F4 slot 0x00 | fefates:bytes
    // 0x0036D6E4 slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual void vf_0x08(); // 0x0036D484 slot 0x08 | virtual slot, introduced by nn::nex::ServiceClient
    virtual bool Bind(nn::nex::Credentials*); // 0x0036D528 slot 0x0C | slot vf_0x0C of nn::nex::ServiceClient
    virtual void Unbind(); // 0x0036D5A4 slot 0x10 | slot vf_0x10 of nn::nex::ServiceClient
    virtual void IsConnected() const; // 0x0072AA08 slot 0x14 | slot vf_0x14 of nn::nex::ServiceClient
    virtual void IsFaulty() const; // 0x0072AB28 slot 0x18 | fefates:bytes
    virtual void ConnectionStateHasChanged(); // 0x0036D524 slot 0x1C | slot vf_0x1C of nn::nex::ServiceClient
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials*); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0072AAEC slot 0x24 | virtual slot, introduced by nn::nex::ServiceClient
    void SetDefaultCredentials(nn::nex::Credentials*); // 0x0036D488 | fefates:bytes [tier B]
    void RegisterProtocol(nn::nex::Protocol*); // 0x00383520 | fefates:bytes [tier B]
    void GetConnection(unsigned short) const; // 0x0072AAA4 | fefates:bytes [tier B]

    // (only the members pia uses; the names are ours)
    u32 m_Unknown0x4;                     // 0x4
    Credentials* m_pDefaultCredentials;   // 0x8
};
} // namespace nex
} // namespace nn
