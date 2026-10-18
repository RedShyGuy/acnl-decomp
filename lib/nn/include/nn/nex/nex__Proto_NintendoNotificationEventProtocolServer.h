#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServerProtocol.h"

namespace nn {
namespace nex {
class EndPoint;
class Message;
class NintendoNotificationEvent;

// The server side of the Nintendo notification event protocol: the events are passed to the slot of
// the derived class (the method name is the one of the slot).
// RTTI N2nn3nex46_Proto_NintendoNotificationEventProtocolServerE @ 0x008CF508
class _Proto_NintendoNotificationEventProtocolServer : public ::nn::nex::ServerProtocol
{
public:
    // the methods of the protocol
    static const u16 METHOD_PROCESS_NINTENDO_NOTIFICATION_EVENT = 1;

    // (inline)
    _Proto_NintendoNotificationEventProtocolServer() : ServerProtocol(FLAG_REGISTER) {}
    virtual void DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint); // 0x003CD708 slot 0x50 | fefates:bytes
    virtual void ProcessNintendoNotificationEvent(const nn::nex::NintendoNotificationEvent& event) = 0; // slot 0x5C
};
ASSERT_SIZE(_Proto_NintendoNotificationEventProtocolServer, 0x40);
} // namespace nex
} // namespace nn
