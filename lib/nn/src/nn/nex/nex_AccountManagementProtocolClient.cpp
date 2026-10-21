#include "nn/nex/nex_AccountManagementProtocolClient.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_AnyDataAdapter.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x00AE0514 (name is ours)
AccountManagementProtocolClient::ProtoReturnFunction AccountManagementProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

using namespace ClientProtocolCall;

namespace {
// (inline) a string result into the return value or into a temporary
inline void ExtractReturnString(Message* pMessage, String* pValue)
{
    if (pValue != nullptr) {
        _Type_string::Extract(pMessage, pValue);
    } else {
        String temporary[1];
        _Type_string::Extract(pMessage, temporary);
    }
}
} // namespace

// 0x003C4C10 | fefates:callgraph [tier C]
bool nn::nex::AccountManagementProtocolClient::CallCreateAccount(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pRetval, const nn::nex::String& strPrincipalName,
                                                                const nn::nex::String& strKey, const unsigned int& uiGroups, const nn::nex::String& strEmail)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CREATE_ACCOUNT)) {
        return false;
    }
    _Type_string::Add(&message, strPrincipalName);
    _Type_string::Add(&message, strKey);
    message.AppendRaw(reinterpret_cast<const u8*>(&uiGroups), sizeof(uiGroups));
    _Type_string::Add(&message, strEmail);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pRetval);
    }
    s_ProtoReturnFunctions[METHOD_CREATE_ACCOUNT - 1] = ProtoReturn_CreateAccount;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C4D9C | fefates:callgraph [tier C]
bool nn::nex::AccountManagementProtocolClient::CallUpdateAccount(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strKey, const nn::nex::String& strEmail,
                                                                const DataHolder& publicData, const DataHolder& privateData)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_UPDATE_ACCOUNT)) {
        return false;
    }
    _Type_string::Add(&message, strKey);
    _Type_string::Add(&message, strEmail);
    publicData.StreamIn(&message);
    privateData.StreamIn(&message);
    s_ProtoReturnFunctions[METHOD_UPDATE_ACCOUNT - 1] = ProtoReturn_UpdateAccount;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C4F30 (name after the DDL strings)
bool nn::nex::AccountManagementProtocolClient::CallChangePassword(nn::nex::ProtocolCallContext* pContext, bool* pRetval, const nn::nex::String& strNewKey)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CHANGE_PASSWORD)) {
        return false;
    }
    _Type_string::Add(&message, strNewKey);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pRetval);
    }
    s_ProtoReturnFunctions[METHOD_CHANGE_PASSWORD - 1] = ProtoReturn_ChangePassword;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C5090 | fefates:callgraph [tier C]
bool nn::nex::AccountManagementProtocolClient::CallCustomCreateAccount(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strPrincipalName, const nn::nex::String& strKey,
                                                                      const unsigned int& uiGroups, const nn::nex::String& strEmail, const DataHolder& authData, unsigned int* pPid)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CUSTOM_CREATE_ACCOUNT)) {
        return false;
    }
    _Type_string::Add(&message, strPrincipalName);
    _Type_string::Add(&message, strKey);
    message.AppendRaw(reinterpret_cast<const u8*>(&uiGroups), sizeof(uiGroups));
    _Type_string::Add(&message, strEmail);
    authData.StreamIn(&message);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pPid);
    }
    s_ProtoReturnFunctions[METHOD_CUSTOM_CREATE_ACCOUNT - 1] = ProtoReturn_CustomCreateAccount;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C5240 (name after the DDL strings)
bool nn::nex::AccountManagementProtocolClient::CallChangePasswordByGuest(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strPrincipalName,
                                                                        const nn::nex::String& strEmail, const nn::nex::String& strKey)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CHANGE_PASSWORD_BY_GUEST)) {
        return false;
    }
    _Type_string::Add(&message, strPrincipalName);
    _Type_string::Add(&message, strEmail);
    _Type_string::Add(&message, strKey);
    s_ProtoReturnFunctions[METHOD_CHANGE_PASSWORD_BY_GUEST - 1] = ProtoReturn_ChangePasswordByGuest;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C53A4 | fefates:callgraph [tier C]
bool nn::nex::AccountManagementProtocolClient::CallNintendoCreateAccount(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strPrincipalName, const nn::nex::String& strKey,
                                                                        const unsigned int& uiGroups, const nn::nex::String& strEmail, const DataHolder& authData, unsigned int* pPid,
                                                                        nn::nex::String* pPidHMAC)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_NINTENDO_CREATE_ACCOUNT)) {
        return false;
    }
    _Type_string::Add(&message, strPrincipalName);
    _Type_string::Add(&message, strKey);
    message.AppendRaw(reinterpret_cast<const u8*>(&uiGroups), sizeof(uiGroups));
    _Type_string::Add(&message, strEmail);
    authData.StreamIn(&message);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pPid);
        pContext->AddReturnValuePtr(pPidHMAC);
    }
    s_ProtoReturnFunctions[METHOD_NINTENDO_CREATE_ACCOUNT - 1] = ProtoReturn_NintendoCreateAccount;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C5560 | fefates:bytes [tier B]
void nn::nex::AccountManagementProtocolClient::ProtoReturn_CreateAccount(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<qResult*>(pContext->GetReturnValuePtr(0)));
}

// 0x003C55C0 (name is ours)
void nn::nex::AccountManagementProtocolClient::ProtoReturn_UpdateAccount(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C55C4 (name is ours)
void nn::nex::AccountManagementProtocolClient::ProtoReturn_ChangePassword(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<bool*>(pContext->GetReturnValuePtr(0)));
}

// 0x003C55F0 | fefates:callgraph [tier C]
bool nn::nex::AccountManagementProtocolClient::CallCreateAccountWithCustomData(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strPrincipalName,
                                                                              const nn::nex::String& strKey, const unsigned int& uiGroups, const nn::nex::String& strEmail,
                                                                              const DataHolder& publicData, const DataHolder& privateData)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CREATE_ACCOUNT_WITH_CUSTOM_DATA)) {
        return false;
    }
    _Type_string::Add(&message, strPrincipalName);
    _Type_string::Add(&message, strKey);
    message.AppendRaw(reinterpret_cast<const u8*>(&uiGroups), sizeof(uiGroups));
    _Type_string::Add(&message, strEmail);
    publicData.StreamIn(&message);
    privateData.StreamIn(&message);
    s_ProtoReturnFunctions[METHOD_CREATE_ACCOUNT_WITH_CUSTOM_DATA - 1] = ProtoReturn_CreateAccountWithCustomData;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C57A0 | fefates:bytes [tier B]
void nn::nex::AccountManagementProtocolClient::ProtoReturn_CustomCreateAccount(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<u32*>(pContext->GetReturnValuePtr(0)));
}

// 0x003C583C (name is ours)
void nn::nex::AccountManagementProtocolClient::ProtoReturn_ChangePasswordByGuest(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C5840 | fefates:bytes [tier B]
void nn::nex::AccountManagementProtocolClient::ProtoReturn_NintendoCreateAccount(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<u32*>(pContext->GetReturnValuePtr(0)));
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturnString(pMessage, static_cast<String*>(pContext->GetReturnValuePtr(1)));
}

// 0x003C5970 (name is ours)
void nn::nex::AccountManagementProtocolClient::ProtoReturn_CreateAccountWithCustomData(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

} // namespace nex
} // namespace nn
