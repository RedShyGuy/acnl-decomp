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
    // (a nop in the original that falls into the destructor of MessagingNotificationHandler)
    virtual ~MessageRecvHandler(); // 0x003BDDF4 slot 0x00
    // 0x00513400 slot 0x04 (deleting dtor)
    virtual void DeliverMessage(nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage); // 0x00513328 slot 0x08
};
