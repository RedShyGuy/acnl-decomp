#pragma once

#include "decomp.h"
#include "nn/nex/nex_MessagingNotificationHandler.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex__Proto_MessageDeliveryProtocolServer.h"

namespace nn {
namespace nex {
// The server of the message delivery protocol in the messaging client: passes the delivered
// messages to the handler of the client. The member names are ours.
// RTTI N2nn3nex21MessageDeliveryServerE @ 0x008CEB18
// vtable 0x008FDE70 (vptr 0x008FDE78), offset_to_top 0, 24 entries
class MessageDeliveryServer : public ::nn::nex::_Proto_MessageDeliveryProtocolServer
{
public:
    // (inline)
    MessageDeliveryServer() : m_pHandler(nullptr) {}
    virtual ~MessageDeliveryServer(); // 0x0039A678 slot 0x00
    // 0x0039A624 slot 0x04 (deleting dtor)
    virtual nn::nex::Protocol* Clone(); // 0x0072CE0C slot 0x4C
    virtual void DeliverMessage(nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage); // 0x0039A5D0 slot 0x5C

    // (inline) the handler is set under the lock of the scheduler
    void SetHandler(MessagingNotificationHandler* pHandler)
    {
        ScopedCS lock(Scheduler::GetInstance()->m_Lock);
        m_pHandler = pHandler;
    }

    MessagingNotificationHandler* m_pHandler; // 0x40
};
ASSERT_SIZE(MessageDeliveryServer, 0x44);
} // namespace nex
} // namespace nn
