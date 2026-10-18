#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_MessageDeliveryProtocolClient.h"
#include "nn/nex/nex_MessageDeliveryServer.h"
#include "nn/nex/nex_MessagingProtocolClient.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
class Data;
class MessageRecipient;
class MessagingNotificationHandler;
class ProtocolCallContext;
class String;

// The client of the messaging service: sends user messages with the message delivery protocol
// and receives them with its server (when it uses the message delivery). The names of the members
// and of the functions are ours.
// RTTI N2nn3nex15MessagingClientE @ 0x008CE4C0
// vtable 0x008FCED4 (vptr 0x008FCEDC), offset_to_top 0, 10 entries
class MessagingClient : public ::nn::nex::ServiceClient
{
public:
    // the id of the service client
    static const u32 SERVICE_CLIENT_ID = 11;
    // the protocol ids
    static const u16 MESSAGING_PROTOCOL_ID = 23;
    static const u16 MESSAGE_DELIVERY_PROTOCOL_ID = 27;
    // the limits of the messages that are sent
    static const u32 MAX_SUBJECT_LENGTH = 256;
    static const u32 MAX_TEXT_BODY_LENGTH = 256;
    static const u32 MAX_BINARY_BODY_SIZE = 512;

    explicit MessagingClient(bool usesMessageDelivery); // 0x0037A0FC
    virtual ~MessagingClient(); // 0x0037A4A8 slot 0x00
    // 0x0037A498 slot 0x04 (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x0037A000 slot 0x20

    // the message (a UserMessage, TextMessage or BinaryMessage) is sent to the recipient; false if
    // it is too long
    bool DeliverMessage(nn::nex::ProtocolCallContext* pContext, const nn::nex::MessageRecipient& recipient, const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage,
                        unsigned int flags, unsigned int lifeTime); // 0x00379ED0
    // the receiver of the delivered messages
    void RegisterNotificationHandler(nn::nex::MessagingNotificationHandler* pHandler); // 0x0037A084
    void UnregisterNotificationHandler(); // 0x0037A0C0

    // the users of the prototypes of the messages (they are registered by the first one)
    static CriticalSection s_Lock;
    static u16 s_PrototypeUseCount;

    MessagingProtocolClient m_MessagingProtocol;      // 0x10
    MessageDeliveryServer m_DeliveryServer;           // 0x54
    MessageDeliveryProtocolClient m_DeliveryProtocol; // 0x98
    bool m_UsesMessageDelivery;                       // 0xDC
};
ASSERT_OFFSET(MessagingClient, m_DeliveryServer, 0x54);
ASSERT_OFFSET(MessagingClient, m_DeliveryProtocol, 0x98);
ASSERT_SIZE(MessagingClient, 0xE0);
} // namespace nex
} // namespace nn
