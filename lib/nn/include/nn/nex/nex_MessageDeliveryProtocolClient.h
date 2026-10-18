#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
class Data;
class ProtocolCallContext;
class String;

// The client of the message delivery protocol (protocol id 27): DeliverMessage sends a user
// message. The method name is from the DDL strings, the other names are ours.
// RTTI N2nn3nex29MessageDeliveryProtocolClientE @ 0x008CF13C
// vtable 0x008FF24C (vptr 0x008FF254), offset_to_top 0, 23 entries
class MessageDeliveryProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol
    enum Method
    {
        METHOD_DELIVER_MESSAGE = 1,
        METHOD_COUNT = 1
    };
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);

    // id: the protocol id (0: the responder, not registered)
    explicit DECOMP_NOINLINE MessageDeliveryProtocolClient(unsigned short id); // 0x003C10E4
    virtual ~MessageDeliveryProtocolClient(); // 0x003C11DC slot 0x00
    // 0x003C11CC slot 0x04 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C105C slot 0x50
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072DE8C slot 0x54

    bool CallDeliverMessage(nn::nex::ProtocolCallContext* pContext, const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage); // 0x003C0F0C
    // (no results)
    static void ProtoReturn_DeliverMessage(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C10E0

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
ASSERT_SIZE(MessageDeliveryProtocolClient, 0x44);
} // namespace nex
} // namespace nn
