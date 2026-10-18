#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class Data;
class String;

// The receiver of the messages that the server delivers to the messaging client (the method
// names are ours, after the DeliverMessage method of the protocol).
// RTTI N2nn3nex28MessagingNotificationHandlerE @ 0x008CF0A8
// vtable 0x008FF084 (vptr 0x008FF08C), offset_to_top 0, 3 entries
class MessagingNotificationHandler : public ::nn::nex::RootObject
{
public:
    MessagingNotificationHandler(); // 0x003BDDE0
    virtual ~MessagingNotificationHandler(); // 0x003BDDF8 slot 0x00
    // 0x003BDDF0 slot 0x04 (deleting dtor)
    // a message from the server (under the lock of the scheduler)
    virtual void DeliverMessage(nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage) = 0; // slot 0x08
};
ASSERT_SIZE(MessagingNotificationHandler, 0x04);
} // namespace nex
} // namespace nn
