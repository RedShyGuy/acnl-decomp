#include "nn/nex/nex_SecureConnectionProtocolClient.h"
#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_ConnectionData.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"

namespace nn {
namespace nex {
// 0x00AE0724 (name is ours)
SecureConnectionProtocolClient::ProtoReturnFunction SecureConnectionProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);

// (inline) the connection data of the stations
inline void ExtractReturn(Message* pMessage, qList<ConnectionData>* pList)
{
    if (pList != nullptr) {
        _Type_std_list<_DDL_ConnectionData>::Extract(pMessage, pList);
    } else {
        qList<ConnectionData> temporary[1];
        _Type_std_list<_DDL_ConnectionData>::Extract(pMessage, temporary);
    }
}
} // namespace

using namespace ClientProtocolCall;

// 0x003C3188
bool nn::nex::SecureConnectionProtocolClient::CallRegister(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::qList<nn::nex::StationURL>& urls,
                                                           unsigned int* pConnectionId, nn::nex::StationURL* pPublicURL)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!Begin(this, &message, lock, pContext, METHOD_REGISTER)) {
        return false;
    }
    _Type_std_list<_Type_stationurl>::Add(&message, urls);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResult);
        pContext->AddReturnValue(pConnectionId);
        pContext->AddReturnValue(pPublicURL);
    }
    s_ProtoReturnFunctions[METHOD_REGISTER - 1] = ProtoReturn_Register;
    return End(this, &message, lock, pContext);
}

// 0x003C3318
bool nn::nex::SecureConnectionProtocolClient::CallRegisterEx(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult,
                                                             const nn::nex::qList<nn::nex::StationURL>& urls, unsigned int* pConnectionId, nn::nex::StationURL* pPublicURL,
                                                             const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& data)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!Begin(this, &message, lock, pContext, METHOD_REGISTER_EX)) {
        return false;
    }
    _Type_std_list<_Type_stationurl>::Add(&message, urls);
    data.StreamIn(&message);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResult);
        pContext->AddReturnValue(pConnectionId);
        pContext->AddReturnValue(pPublicURL);
    }
    s_ProtoReturnFunctions[METHOD_REGISTER_EX - 1] = ProtoReturn_RegisterEx;
    return End(this, &message, lock, pContext);
}

// 0x003C34CC
bool nn::nex::SecureConnectionProtocolClient::CallReplaceURL(nn::nex::ProtocolCallContext* pContext, const nn::nex::StationURL& target, const nn::nex::StationURL& url)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!Begin(this, &message, lock, pContext, METHOD_REPLACE_URL)) {
        return false;
    }
    _Type_stationurl::Add(&message, target);
    _Type_stationurl::Add(&message, url);
    s_ProtoReturnFunctions[METHOD_REPLACE_URL - 1] = ProtoReturn_ReplaceURL;
    return End(this, &message, lock, pContext);
}

// 0x003C3638
bool nn::nex::SecureConnectionProtocolClient::CallSendReport(nn::nex::ProtocolCallContext* pContext, const unsigned int& reportId, const nn::nex::qBuffer& data)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!Begin(this, &message, lock, pContext, METHOD_SEND_REPORT)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&reportId), sizeof(reportId));
    _Type_qBuffer::Add(&message, data);
    s_ProtoReturnFunctions[METHOD_SEND_REPORT - 1] = ProtoReturn_SendReport;
    return End(this, &message, lock, pContext);
}

// 0x003C37A8
void nn::nex::SecureConnectionProtocolClient::ProtoReturn_Register(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<qResult*>(pContext->m_ReturnValues[0]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<u32*>(pContext->m_ReturnValues[1]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<StationURL*>(pContext->m_ReturnValues[2]));
}

// 0x003C3914
void nn::nex::SecureConnectionProtocolClient::ProtoReturn_RegisterEx(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<qResult*>(pContext->m_ReturnValues[0]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<u32*>(pContext->m_ReturnValues[1]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<StationURL*>(pContext->m_ReturnValues[2]));
}

// 0x003C3A80
void nn::nex::SecureConnectionProtocolClient::ProtoReturn_ReplaceURL(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003C3A84
void nn::nex::SecureConnectionProtocolClient::ProtoReturn_SendReport(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003C3A88
bool nn::nex::SecureConnectionProtocolClient::CallRequestConnectionData(nn::nex::ProtocolCallContext* pContext, bool* pIsSuccess, const unsigned int& connectionId,
                                                                        const unsigned int& principalId, nn::nex::qList<nn::nex::ConnectionData>* pConnectionData)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!Begin(this, &message, lock, pContext, METHOD_REQUEST_CONNECTION_DATA)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&connectionId), sizeof(connectionId));
    message.AppendRaw(reinterpret_cast<const u8*>(&principalId), sizeof(principalId));
    if (pContext != nullptr) {
        pContext->AddReturnValue(pIsSuccess);
        pContext->AddReturnValue(pConnectionData);
    }
    s_ProtoReturnFunctions[METHOD_REQUEST_CONNECTION_DATA - 1] = ProtoReturn_RequestConnectionData;
    return End(this, &message, lock, pContext);
}

// 0x003C3C20 slot 0x50 | fefates:bytes
void nn::nex::SecureConnectionProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
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

// 0x003C3CAC | fefates:bytes [tier B]
void nn::nex::SecureConnectionProtocolClient::ProtoReturn_RequestConnectionData(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<bool*>(pContext->m_ReturnValues[0]));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn(pMessage, static_cast<qList<ConnectionData>*>(pContext->m_ReturnValues[1]));
}

// 0x003C42BC
nn::nex::SecureConnectionProtocolClient::SecureConnectionProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        SecureConnectionProtocolClient* pResponder = new SecureConnectionProtocolClient(0);
        RVClientCore::GetInstance()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003C4338 slot 0x00
// 0x003C4328 (deleting dtor)
nn::nex::SecureConnectionProtocolClient::~SecureConnectionProtocolClient()
{
    // nothing to do
}

// 0x0072DEB0 slot 0x54
nn::nex::ClientProtocol* nn::nex::SecureConnectionProtocolClient::CreateResponder() const
{
    return new SecureConnectionProtocolClient(0);
}

} // namespace nex
} // namespace nn
