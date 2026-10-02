#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex40NATTraversalReportInternalProtocolClientE @ 0x008CF494
// vtable 0x008FF8E0 (vptr 0x008FF8E8), offset_to_top 0, 23 entries
class NATTraversalReportInternalProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    NATTraversalReportInternalProtocolClient(); // ctor candidate(s) 0x003CD0F4 (unverified)
    virtual ~NATTraversalReportInternalProtocolClient(); // 0x003CD1EC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003CD1DC slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003CD070 slot 0x50 | fefates:bytes
    virtual void CreateResponder() const; // 0x0072E084 slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
};
} // namespace nex
} // namespace nn
