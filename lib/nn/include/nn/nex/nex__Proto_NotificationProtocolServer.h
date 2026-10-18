#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServerProtocol.h"

namespace nn {
namespace nex {
class NotificationEvent;

// The server of the notification protocol (generated from the DDL): method 1 delivers an event.
// RTTI N2nn3nex33_Proto_NotificationProtocolServerE @ 0x008CF340
class _Proto_NotificationProtocolServer : public ::nn::nex::ServerProtocol
{
public:
    // (inline in the constructors of the derived classes)
    _Proto_NotificationProtocolServer() : ServerProtocol(FLAG_REGISTER) {}
    virtual void DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint); // 0x003CB6F8 slot 0x50 | fefates:bytes
    // the event of method 1 (the name is ours)
    virtual void ProcessNotificationEvent(const nn::nex::NotificationEvent& event) = 0; // slot 0x5C
};
} // namespace nex
} // namespace nn
