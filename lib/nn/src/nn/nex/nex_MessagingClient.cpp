#include "nn/nex/nex_MessagingClient.h"
#include "nn/nex/nex_AnyDataAdapter.h"
#include "nn/nex/nex_BinaryMessage.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_MessageRecipient.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include "nn/nex/nex_StreamManager.h"
#include "nn/nex/nex_TextMessage.h"
#include "nn/nex/nex_UserMessage.h"

namespace nn {
namespace nex {
// 0x00ADDB9C (name is ours)
CriticalSection MessagingClient::s_Lock(0x20000000);
// 0x0096B128 (name is ours)
u16 MessagingClient::s_PrototypeUseCount = 0;

// 0x00379ED0 (name is ours)
bool nn::nex::MessagingClient::DeliverMessage(nn::nex::ProtocolCallContext* pContext, const nn::nex::MessageRecipient& recipient,
                                              const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage, unsigned int flags, unsigned int lifeTime)
{
    UserMessage* pMessage = static_cast<UserMessage*>(oUserMessage.m_pObject);
    if (pMessage == nullptr || pMessage->m_strSubject.GetLength() > MAX_SUBJECT_LENGTH) {
        return false;
    }
    if (pMessage->IsAKindOf(String(L"TextMessage"))) {
        if (static_cast<TextMessage*>(pMessage)->m_strTextBody.GetLength() > MAX_TEXT_BODY_LENGTH) {
            return false;
        }
    } else if (pMessage->IsAKindOf(String(L"BinaryMessage"))) {
        if (static_cast<BinaryMessage*>(pMessage)->m_binaryBody.size() > MAX_BINARY_BODY_SIZE) {
            return false;
        }
    }
    pMessage->m_idRecipient = recipient.m_idRecipient;
    pMessage->m_uiRecipientType = recipient.m_uiRecipientType;
    pMessage->m_uiFlags = flags;
    pMessage->m_uiLifeTime = lifeTime;
    return m_DeliveryProtocol.CallDeliverMessage(pContext, oUserMessage);
}

// 0x0037A000 slot 0x20
void nn::nex::MessagingClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    m_MessagingProtocol.SetDefaultCredentials(pCredentials);
    m_DeliveryProtocol.SetDefaultCredentials(pCredentials);
    if (!m_UsesMessageDelivery) {
        return;
    }
    // the server moves from the broker of the old credentials to the one of the new ones
    if (m_pDefaultCredentials != nullptr && m_pDefaultCredentials->m_pStreamManager != nullptr) {
        m_pDefaultCredentials->m_pStreamManager->m_pBroker->UnregisterServerProtocol(MESSAGE_DELIVERY_PROTOCOL_ID);
    }
    if (pCredentials != nullptr && pCredentials->m_pStreamManager != nullptr) {
        pCredentials->m_pStreamManager->m_pBroker->RegisterServerProtocol(MESSAGE_DELIVERY_PROTOCOL_ID, &m_DeliveryServer);
    }
}

// 0x0037A084 (name is ours)
void nn::nex::MessagingClient::RegisterNotificationHandler(nn::nex::MessagingNotificationHandler* pHandler)
{
    m_DeliveryServer.SetHandler(pHandler);
}

// 0x0037A0C0 (name is ours)
void nn::nex::MessagingClient::UnregisterNotificationHandler()
{
    m_DeliveryServer.SetHandler(nullptr);
}

// 0x0037A0FC
nn::nex::MessagingClient::MessagingClient(bool usesMessageDelivery)
    : ServiceClient(SERVICE_CLIENT_ID), m_MessagingProtocol(0), m_DeliveryProtocol(0), m_UsesMessageDelivery(usesMessageDelivery)
{
    AnyDataAdapter::Register();
    m_MessagingProtocol.m_ProtocolId = MESSAGING_PROTOCOL_ID;
    if (m_UsesMessageDelivery) {
        m_DeliveryServer.m_ProtocolId = MESSAGE_DELIVERY_PROTOCOL_ID;
        m_DeliveryServer.SetFlag(Protocol::FLAG_UNKNOWN_8);
        m_DeliveryProtocol.m_ProtocolId = MESSAGE_DELIVERY_PROTOCOL_ID;
        ScopedCS lock(s_Lock);
        if (s_PrototypeUseCount == 0) {
            AnyDataAdapter::s_Instance.RegisterPrototype(new UserMessage());
            AnyDataAdapter::s_Instance.RegisterPrototype(new TextMessage());
            AnyDataAdapter::s_Instance.RegisterPrototype(new BinaryMessage());
        }
        s_PrototypeUseCount++;
    }
    GetProtocolRegistry()->RegisterProtocol(MESSAGING_PROTOCOL_ID, new MessagingProtocolClient(0));
    GetProtocolRegistry()->RegisterProtocol(MESSAGE_DELIVERY_PROTOCOL_ID, new MessageDeliveryServer());
    GetProtocolRegistry()->RegisterProtocol(MESSAGE_DELIVERY_PROTOCOL_ID, new MessageDeliveryProtocolClient(0));
}

// 0x0037A4A8 slot 0x00
// 0x0037A498 (deleting dtor)
nn::nex::MessagingClient::~MessagingClient()
{
    {
        ScopedCS lock(s_Lock);
        if (s_PrototypeUseCount == 1) {
            AnyDataAdapter::s_Instance.UnregisterPrototype(String(L"UserMessage"));
            AnyDataAdapter::s_Instance.UnregisterPrototype(String(L"TextMessage"));
            AnyDataAdapter::s_Instance.UnregisterPrototype(String(L"BinaryMessage"));
        }
        s_PrototypeUseCount--;
    }
    UnregisterNotificationHandler();
    SetDefaultCredentials(nullptr);
    AnyDataAdapter::Unregister();
}

} // namespace nex
} // namespace nn
