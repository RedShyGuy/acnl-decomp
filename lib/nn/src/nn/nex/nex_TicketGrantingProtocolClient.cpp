#include "nn/nex/nex_TicketGrantingProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/_Type_buffer/_Type_buffer_Api.h"
#include "nn/nex/_Type_qresult/_Type_qresult_Api.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_RVConnectionData.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x00AE0708 (name is ours)
TicketGrantingProtocolClient::ProtoReturnFunction TicketGrantingProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);

// (inline) the connection data of a result
inline void ExtractReturn(Message* pMessage, RVConnectionData* pData)
{
    if (pData != nullptr) {
        _DDL_RVConnectionData::Extract(pMessage, pData);
    } else {
        RVConnectionData temporary[1];
        _DDL_RVConnectionData::Extract(pMessage, temporary);
    }
}
} // namespace

using namespace ClientProtocolCall;

// 0x003BE3CC
bool nn::nex::TicketGrantingProtocolClient::CallLoginEx(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::String& name,
                                                        unsigned int* pPrincipalId, nn::nex::Buffer* pTicket, nn::nex::RVConnectionData* pConnectionData,
                                                        nn::nex::String* pServerName, const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& data)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!Begin(this, &message, lock, pContext, METHOD_LOGIN_EX)) {
        return false;
    }
    _Type_string::Add(&message, name);
    data.StreamIn(&message);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResult);
        pContext->AddReturnValue(pPrincipalId);
        pContext->AddReturnValue(pTicket);
        pContext->AddReturnValue(pConnectionData);
        pContext->AddReturnValue(pServerName);
    }
    s_ProtoReturnFunctions[METHOD_LOGIN_EX - 1] = ProtoReturn_LoginEx;
    return End(this, &message, lock, pContext);
}

// 0x003BE594
void nn::nex::TicketGrantingProtocolClient::ProtoReturn_Login(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<qResult*>(pContext->m_ReturnValues[0]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<u32*>(pContext->m_ReturnValues[1]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<Buffer*>(pContext->m_ReturnValues[2]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<RVConnectionData*>(pContext->m_ReturnValues[3]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<String*>(pContext->m_ReturnValues[4]));
}

// 0x003BE934
void nn::nex::TicketGrantingProtocolClient::ProtoReturn_LoginEx(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<qResult*>(pContext->m_ReturnValues[0]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<u32*>(pContext->m_ReturnValues[1]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<Buffer*>(pContext->m_ReturnValues[2]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<RVConnectionData*>(pContext->m_ReturnValues[3]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<String*>(pContext->m_ReturnValues[4]));
}

// 0x003BECD4
bool nn::nex::TicketGrantingProtocolClient::CallLoginWithContext(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult,
                                                                 const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& data, unsigned int* pPrincipalId,
                                                                 nn::nex::Buffer* pTicket, nn::nex::RVConnectionData* pConnectionData)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!Begin(this, &message, lock, pContext, METHOD_LOGIN_WITH_CONTEXT)) {
        return false;
    }
    data.StreamIn(&message);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResult);
        pContext->AddReturnValue(pPrincipalId);
        pContext->AddReturnValue(pTicket);
        pContext->AddReturnValue(pConnectionData);
    }
    s_ProtoReturnFunctions[METHOD_LOGIN_WITH_CONTEXT - 1] = ProtoReturn_LoginWithContext;
    return End(this, &message, lock, pContext);
}

// 0x003BEE84 | fefates:bytes [tier B]
void nn::nex::TicketGrantingProtocolClient::ProtoReturn_RequestTicket(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<qResult*>(pContext->m_ReturnValues[0]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<Buffer*>(pContext->m_ReturnValues[1]));
}

// 0x003BF08C slot 0x50 | fefates:bytes
void nn::nex::TicketGrantingProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    u32 methodId;
    pMessage->ExtractPrimitive(&methodId);
    methodId &= 0x7FFF;
    if (methodId == 0 || methodId > METHOD_COUNT) {
        SetCallError(qResult(RESULT_UNKNOWN_METHOD));
        return;
    }
    ProtoReturnFunction pFunction = s_ProtoReturnFunctions[methodId - 1];
    if (pFunction != nullptr) {
        pFunction(pMessage, pContext);
    }
}

// 0x003BF118 | fefates:bytes [tier B]
void nn::nex::TicketGrantingProtocolClient::ProtoReturn_LoginWithContext(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<qResult*>(pContext->m_ReturnValues[0]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<u32*>(pContext->m_ReturnValues[1]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<Buffer*>(pContext->m_ReturnValues[2]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<RVConnectionData*>(pContext->m_ReturnValues[3]));
}

// 0x003BF438 | fefates:bytes [tier B]
bool nn::nex::TicketGrantingProtocolClient::RequestTicket_PrepareRequest(nn::nex::ProtocolCallContext* pContext, nn::nex::Message* pMessage, nn::nex::qResult* pResult,
                                                                         const unsigned int& sourceId, const unsigned int& targetId, nn::nex::Buffer* pTicket)
{
    ScopedCS lock(GetLock());
    if (!pContext->InitiateCall()) {
        return false;
    }
    u32 callId = pContext->m_Id;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&callId), sizeof(callId));
    u32 methodId = METHOD_REQUEST_TICKET;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&methodId), sizeof(methodId));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&sourceId), sizeof(sourceId));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&targetId), sizeof(targetId));
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResult);
        pContext->AddReturnValue(pTicket);
    }
    s_ProtoReturnFunctions[METHOD_REQUEST_TICKET - 1] = ProtoReturn_RequestTicket;
    return true;
}

// 0x003BF538
bool nn::nex::TicketGrantingProtocolClient::CallLogin(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::String& name,
                                                      unsigned int* pPrincipalId, nn::nex::Buffer* pTicket, nn::nex::RVConnectionData* pConnectionData,
                                                      nn::nex::String* pServerName)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!Begin(this, &message, lock, pContext, METHOD_LOGIN)) {
        return false;
    }
    _Type_string::Add(&message, name);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResult);
        pContext->AddReturnValue(pPrincipalId);
        pContext->AddReturnValue(pTicket);
        pContext->AddReturnValue(pConnectionData);
        pContext->AddReturnValue(pServerName);
    }
    s_ProtoReturnFunctions[METHOD_LOGIN - 1] = ProtoReturn_Login;
    return End(this, &message, lock, pContext);
}

// 0x003BF6DC
nn::nex::TicketGrantingProtocolClient::TicketGrantingProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        TicketGrantingProtocolClient* pResponder = new TicketGrantingProtocolClient(0);
        RVClientCore::GetInstance()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003BF758 slot 0x00
// 0x003BF748 (deleting dtor)
nn::nex::TicketGrantingProtocolClient::~TicketGrantingProtocolClient()
{
    // nothing to do
}

// 0x0072DE60 slot 0x54
nn::nex::ClientProtocol* nn::nex::TicketGrantingProtocolClient::CreateResponder() const
{
    return new TicketGrantingProtocolClient(0);
}

} // namespace nex
} // namespace nn
