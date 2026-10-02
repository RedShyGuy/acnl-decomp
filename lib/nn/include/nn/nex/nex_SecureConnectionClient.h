#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex22SecureConnectionClientE @ 0x008CEC50
// vtable 0x008FE180 (vptr 0x008FE188), offset_to_top 0, 10 entries
class SecureConnectionClient : public ::nn::nex::ServiceClient
{
public:
    SecureConnectionClient(); // ctor address unknown
    virtual ~SecureConnectionClient(); // 0x0039DDF4 slot 0x00 | slot vf_0x00 of nn::nex::ServiceClient
    // 0x0039DDC4 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials*); // 0x0039DDBC slot 0x20 | slot vf_0x20 of nn::nex::ServiceClient
    void ReplaceURL(nn::nex::ProtocolCallContext*, const nn::nex::StationURL&, const nn::nex::StationURL&); // 0x0039DB10 | fefates:bytes [tier B]
    void SendReport(unsigned int, const void*, unsigned int); // 0x0039DB7C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
