#include "nn/nex/nex__Proto_MessageDeliveryProtocolServer.h"
#include "nn/nex/nex_AnyDataAdapter.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
// the method id of a response
const u32 METHOD_RESPONSE_FLAG = 0x8000;
} // namespace

// 0x003CC248 slot 0x50
void nn::nex::_Proto_MessageDeliveryProtocolServer::DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint)
{
    u16 methodId = Protocol::ExtractMethodID(pRequest);
    if (methodId != METHOD_DELIVER_MESSAGE) {
        SetCallError(qResult(RESULT_UNKNOWN_METHOD));
        return;
    }
    AnyObjectHolder<Data, String> oUserMessage;
    oUserMessage.StreamOut(pRequest);
    if (!pRequest->m_IsError) {
        DeliverMessage(oUserMessage);
        Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_DELIVER_MESSAGE);
    }
    *pIsHandled = true;
}

} // namespace nex
} // namespace nn
