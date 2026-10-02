#pragma once

#include "decomp.h"
#include "net/nex/dImpl.h"
#include "nn/nex/nex_MessagingNotificationHandler.h"

// RTTI N3net3nex4Impl18MessageRecvHandlerE @ 0x008D0FD8
// vtable 0x009046A4 (vptr 0x009046AC), offset_to_top 0, 3 entries
class net::nex::Impl::MessageRecvHandler : public ::nn::nex::MessagingNotificationHandler
{
public:
    MessageRecvHandler(); // ctor address unknown
    virtual void vf_0x00(); // 0x003BDDF4 slot 0x00 | virtual slot, introduced by nn::nex::MessagingNotificationHandler
    virtual void vf_0x04(); // 0x00513400 slot 0x04 | virtual slot, introduced by nn::nex::MessagingNotificationHandler
    virtual void vf_0x08(); // 0x00513328 slot 0x08 | virtual slot, introduced by nn::nex::MessagingNotificationHandler
};
