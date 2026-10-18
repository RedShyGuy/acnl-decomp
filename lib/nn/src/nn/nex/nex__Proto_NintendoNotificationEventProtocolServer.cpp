#include "nn/nex/nex__Proto_NintendoNotificationEventProtocolServer.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_NintendoNotificationEvent.h"

namespace nn {
namespace nex {
namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
// the method id of a response
const u32 METHOD_RESPONSE_FLAG = 0x8000;
} // namespace

// 0x003CD708 slot 0x50 | fefates:bytes
void nn::nex::_Proto_NintendoNotificationEventProtocolServer::DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled,
                                                                                       nn::nex::EndPoint* pEndPoint)
{
    u16 methodId = Protocol::ExtractMethodID(pRequest);
    if (methodId != METHOD_PROCESS_NINTENDO_NOTIFICATION_EVENT) {
        SetCallErrorInline(qResult(RESULT_UNKNOWN_METHOD));
        return;
    }
    NintendoNotificationEvent eventObject;
    _DDL_NintendoNotificationEvent::Extract(pRequest, &eventObject);
    if (!pRequest->m_IsError) {
        ProcessNintendoNotificationEvent(eventObject);
        Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_PROCESS_NINTENDO_NOTIFICATION_EVENT);
    }
    *pIsHandled = true;
}

} // namespace nex
} // namespace nn
