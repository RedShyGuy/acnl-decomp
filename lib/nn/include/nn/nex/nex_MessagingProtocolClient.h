#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
class ProtocolCallContext;

// The client of the messaging protocol (protocol id 23). None of its methods is called in this
// build. The names of the members are ours.
// RTTI N2nn3nex23MessagingProtocolClientE @ 0x008CED00
// vtable 0x008FE5AC (vptr 0x008FE5B4), offset_to_top 0, 23 entries
class MessagingProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol
    static const u32 METHOD_COUNT = 7;
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);

    // id: the protocol id (0: the responder, not registered)
    explicit DECOMP_NOINLINE MessagingProtocolClient(unsigned short id); // 0x003B11F8
    virtual ~MessagingProtocolClient(); // 0x003B12F0 slot 0x00
    // 0x003B12E0 slot 0x04 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003B1174 slot 0x50
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072D25C slot 0x54

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
ASSERT_SIZE(MessagingProtocolClient, 0x44);
} // namespace nex
} // namespace nn
