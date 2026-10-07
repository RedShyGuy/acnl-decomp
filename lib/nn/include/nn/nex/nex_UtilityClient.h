#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13UtilityClientE @ 0x008CE2B4
// vtable 0x008FC9F0 (vptr 0x008FC9F8), offset_to_top 0, 10 entries
class UtilityClient : public ::nn::nex::ServiceClient
{
public:
    UtilityClient(); // ctor candidate(s) 0x0036F368 (unverified)
    virtual ~UtilityClient(); // 0x0036F41C slot 0x00 | slot vf_0x00 of nn::nex::ServiceClient
    // 0x0036F3B8 slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual bool Bind(nn::nex::Credentials*); // 0x0036F2FC slot 0x0C | slot vf_0x0C of nn::nex::ServiceClient
    virtual void Unbind(); // 0x0036F330 slot 0x10 | slot vf_0x10 of nn::nex::ServiceClient
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials*); // 0x00370DCC slot 0x20 | slot vf_0x20 of nn::nex::ServiceClient
};
} // namespace nex
} // namespace nn
