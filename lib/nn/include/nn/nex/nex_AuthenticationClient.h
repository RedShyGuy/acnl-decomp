#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20AuthenticationClientE @ 0x008CE838
// vtable 0x008FD750 (vptr 0x008FD758), offset_to_top 0, 10 entries
class AuthenticationClient : public ::nn::nex::ServiceClient
{
public:
    AuthenticationClient(); // ctor address unknown
    virtual ~AuthenticationClient(); // 0x003954BC slot 0x00 | fefates:bytes
    // 0x003954AC slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials*); // 0x00395390 slot 0x20 | slot vf_0x20 of nn::nex::ServiceClient
};
} // namespace nex
} // namespace nn
