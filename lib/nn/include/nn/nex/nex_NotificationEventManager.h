#pragma once

#include "decomp.h"
#include "nn/nex/nex__Proto_NotificationProtocolServer.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24NotificationEventManagerE @ 0x008CEE08
// vtable 0x008FE898 (vptr 0x008FE8A0), offset_to_top 0, 25 entries
class NotificationEventManager : public ::nn::nex::_Proto_NotificationProtocolServer
{
public:
    NotificationEventManager(); // ctor candidate(s) 0x003B5244 (unverified)
    virtual ~NotificationEventManager(); // 0x003B5438 slot 0x00 | fefates:bytes
    // 0x003B5428 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void vf_0x4C(); // 0x0072D548 slot 0x4C | virtual slot, introduced by nn::nex::ServerProtocol
    virtual void DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*); // 0x003CB6F8 slot 0x50 | fefates:bytes
    virtual void vf_0x5C(); // 0x003B4EA4 slot 0x5C | virtual slot, introduced by nn::nex::NotificationEventManager
    virtual void vf_0x60(); // 0x003B4EA8 slot 0x60 | virtual slot, introduced by nn::nex::NotificationEventManager
    void IsEventValid(const nn::nex::NotificationEvent&); // 0x003B4C5C | fefates:bytes [tier B]
    NotificationEventManager(nn::nex::NotificationEventManager*); // 0x003B5244 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
