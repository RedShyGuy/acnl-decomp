#pragma once

#include "decomp.h"
#include "nn/nex/nex__Proto_NintendoNotificationEventProtocolServer.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex32NintendoNotificationEventManagerE @ 0x008CF29C
// vtable 0x008FF5E0 (vptr 0x008FF5E8), offset_to_top 0, 24 entries
class NintendoNotificationEventManager : public ::nn::nex::_Proto_NintendoNotificationEventProtocolServer
{
public:
    NintendoNotificationEventManager(); // ctor candidate(s) 0x003CA6A0 (unverified)
    virtual ~NintendoNotificationEventManager(); // 0x003CA7E0 slot 0x00 | fefates:bytes
    // 0x003CA7D0 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void vf_0x4C(); // 0x0072DF00 slot 0x4C | virtual slot, introduced by nn::nex::ServerProtocol
    virtual void DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*); // 0x003CD708 slot 0x50 | fefates:bytes
    virtual void ProcessNintendoNotificationEvent(const nn::nex::NintendoNotificationEvent&); // 0x003CA5F4 slot 0x5C | fefates:bytes
    NintendoNotificationEventManager(nn::nex::NintendoNotificationEventManager*); // 0x003CA6A0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
