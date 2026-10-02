#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15MessagingClientE @ 0x008CE4C0
// vtable 0x008FCED4 (vptr 0x008FCEDC), offset_to_top 0, 10 entries
class MessagingClient : public ::nn::nex::ServiceClient
{
public:
    MessagingClient(); // ctor address unknown
    virtual ~MessagingClient(); // 0x0037A4A8 slot 0x00 | slot vf_0x00 of nn::nex::ServiceClient
    // 0x0037A498 slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials*); // 0x0037A000 slot 0x20 | slot vf_0x20 of nn::nex::ServiceClient
};
} // namespace nex
} // namespace nn
