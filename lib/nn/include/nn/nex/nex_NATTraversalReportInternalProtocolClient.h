#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
class ProtocolCallContext;

// The client of the internal NAT traversal report protocol. None of its methods is called in this
// build. The names of the members are ours.
// RTTI N2nn3nex40NATTraversalReportInternalProtocolClientE @ 0x008CF494
// vtable 0x008FF8E0 (vptr 0x008FF8E8), offset_to_top 0, 23 entries
class NATTraversalReportInternalProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol
    static const u32 METHOD_COUNT = 3;
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);

    // id: the protocol id (0: the responder, not registered)
    explicit DECOMP_NOINLINE NATTraversalReportInternalProtocolClient(unsigned short id); // 0x003CD0F4
    virtual ~NATTraversalReportInternalProtocolClient(); // 0x003CD1EC slot 0x00
    // 0x003CD1DC slot 0x04 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003CD070 slot 0x50
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072E084 slot 0x54

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
ASSERT_SIZE(NATTraversalReportInternalProtocolClient, 0x44);
} // namespace nex
} // namespace nn
