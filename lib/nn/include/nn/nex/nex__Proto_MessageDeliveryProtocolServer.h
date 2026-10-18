#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ServerProtocol.h"

namespace nn {
namespace nex {
class Data;
class String;

// The server side of the message delivery protocol: the requests of DeliverMessage are passed to
// the slot of the derived class (the method name is from the DDL strings).
// RTTI N2nn3nex36_Proto_MessageDeliveryProtocolServerE @ 0x008CF3E8
class _Proto_MessageDeliveryProtocolServer : public ::nn::nex::ServerProtocol
{
public:
    // the methods of the protocol
    static const u16 METHOD_DELIVER_MESSAGE = 1;

    // (inline)
    _Proto_MessageDeliveryProtocolServer() : ServerProtocol(FLAG_REGISTER) {}
    // (symbols.json names it the dispatch of _Proto_NintendoNotificationEventProtocolServer)
    virtual void DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint); // 0x003CC248 slot 0x50
    virtual void DeliverMessage(nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage) = 0; // slot 0x5C
};
ASSERT_SIZE(_Proto_MessageDeliveryProtocolServer, 0x40);
} // namespace nex
} // namespace nn
