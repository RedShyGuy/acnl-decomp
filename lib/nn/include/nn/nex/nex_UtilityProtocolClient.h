#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21UtilityProtocolClientE @ 0x008CEB68
// vtable 0x008FDF38 (vptr 0x008FDF40), offset_to_top 0, 23 entries
class UtilityProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    UtilityProtocolClient(); // ctor candidate(s) 0x0039B574 (unverified)
    virtual ~UtilityProtocolClient(); // 0x00370EEC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0039B65C slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x0039AFA4 slot 0x50 | slot vf_0x50 of nn::nex::ClientProtocol
    virtual void CreateResponder() const; // 0x0072CE48 slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
};
} // namespace nex
} // namespace nn
