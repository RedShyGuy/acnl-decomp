#include "nn/nex/nex_MessageDeliveryProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_AnyDataAdapter.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x0096B12C (name is ours)
MessageDeliveryProtocolClient::ProtoReturnFunction MessageDeliveryProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
} // namespace

using namespace ClientProtocolCall;

// 0x003C0F0C
bool nn::nex::MessageDeliveryProtocolClient::CallDeliverMessage(nn::nex::ProtocolCallContext* pContext, const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_DELIVER_MESSAGE)) {
        return false;
    }
    oUserMessage.StreamIn(&message);
    s_ProtoReturnFunctions[METHOD_DELIVER_MESSAGE - 1] = ProtoReturn_DeliverMessage;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C10E0
void nn::nex::MessageDeliveryProtocolClient::ProtoReturn_DeliverMessage(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C105C slot 0x50
void nn::nex::MessageDeliveryProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    u32 methodId = ExtractMethodID(pMessage) & 0x7FFF;
    if (methodId == 0 || methodId > METHOD_COUNT) {
        SetCallError(qResult(RESULT_UNKNOWN_METHOD));
        return;
    }
    ProtoReturnFunction pFunction = s_ProtoReturnFunctions[methodId - 1];
    if (pFunction != nullptr) {
        pFunction(pMessage, pContext);
    }
}

// 0x003C10E4
nn::nex::MessageDeliveryProtocolClient::MessageDeliveryProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        MessageDeliveryProtocolClient* pResponder = new MessageDeliveryProtocolClient(0);
        RVClientCore::GetInstanceInline()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003C11DC slot 0x00
// 0x003C11CC (deleting dtor)
nn::nex::MessageDeliveryProtocolClient::~MessageDeliveryProtocolClient()
{
    // nothing to do
}

// 0x0072DE8C slot 0x54
nn::nex::ClientProtocol* nn::nex::MessageDeliveryProtocolClient::CreateResponder() const
{
    return new MessageDeliveryProtocolClient(0);
}

} // namespace nex
} // namespace nn
