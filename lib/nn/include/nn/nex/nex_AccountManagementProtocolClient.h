#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
class Data;
class ProtocolCallContext;
class String;

// The calls of the account management protocol (used by JobManageAccount). The class has no RTTI
// and no ExtractCallSpecificResults in this build: the results are extracted by the protocol
// client of the job. The method and parameter names are from the DDL strings and the reference
// symbols.
class AccountManagementProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol that are called
    enum Method
    {
        METHOD_CREATE_ACCOUNT = 1,
        METHOD_CHANGE_PASSWORD = 4,
        METHOD_CREATE_ACCOUNT_WITH_CUSTOM_DATA = 21,
        METHOD_UPDATE_ACCOUNT = 23,
        METHOD_CHANGE_PASSWORD_BY_GUEST = 24,
        METHOD_CUSTOM_CREATE_ACCOUNT = 26,
        METHOD_NINTENDO_CREATE_ACCOUNT = 27,
        // (the highest id that is called; the size of the table is not known)
        METHOD_COUNT = 27
    };
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);
    typedef AnyObjectHolder<Data, String> DataHolder;

    // (the return types are ours)
    bool CallCreateAccount(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pRetval, const nn::nex::String& strPrincipalName, const nn::nex::String& strKey,
                           const unsigned int& uiGroups, const nn::nex::String& strEmail); // 0x003C4C10 | fefates:callgraph [tier C]
    bool CallUpdateAccount(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strKey, const nn::nex::String& strEmail, const DataHolder& publicData,
                           const DataHolder& privateData); // 0x003C4D9C | fefates:callgraph [tier C]
    bool CallChangePassword(nn::nex::ProtocolCallContext* pContext, bool* pRetval, const nn::nex::String& strNewKey); // 0x003C4F30
    bool CallCustomCreateAccount(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strPrincipalName, const nn::nex::String& strKey, const unsigned int& uiGroups,
                                 const nn::nex::String& strEmail, const DataHolder& authData, unsigned int* pPid); // 0x003C5090 | fefates:callgraph [tier C]
    bool CallChangePasswordByGuest(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strPrincipalName, const nn::nex::String& strEmail,
                                   const nn::nex::String& strKey); // 0x003C5240
    bool CallNintendoCreateAccount(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strPrincipalName, const nn::nex::String& strKey, const unsigned int& uiGroups,
                                   const nn::nex::String& strEmail, const DataHolder& authData, unsigned int* pPid,
                                   nn::nex::String* pPidHMAC); // 0x003C53A4 | fefates:callgraph [tier C]
    bool CallCreateAccountWithCustomData(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& strPrincipalName, const nn::nex::String& strKey,
                                         const unsigned int& uiGroups, const nn::nex::String& strEmail, const DataHolder& publicData,
                                         const DataHolder& privateData); // 0x003C55F0 | fefates:callgraph [tier C]

    static void ProtoReturn_CreateAccount(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C5560 | fefates:bytes [tier B]
    static void ProtoReturn_UpdateAccount(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C55C0
    static void ProtoReturn_ChangePassword(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C55C4
    static void ProtoReturn_CustomCreateAccount(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C57A0 | fefates:bytes [tier B]
    static void ProtoReturn_ChangePasswordByGuest(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C583C
    static void ProtoReturn_NintendoCreateAccount(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C5840 | fefates:bytes [tier B]
    static void ProtoReturn_CreateAccountWithCustomData(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C5970

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
} // namespace nex
} // namespace nn
