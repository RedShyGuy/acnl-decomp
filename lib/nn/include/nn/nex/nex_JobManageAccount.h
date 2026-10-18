#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
class AccountManagementProtocolClient;
class Data;

// A call of the account management protocol: the password is hashed into the key first, then the
// call of the function type (or of the custom command) runs. The names of the members, of the
// function types and of the steps without a reference name are ours.
// RTTI N2nn3nex16JobManageAccountE @ 0x008CE59C
// vtable 0x008FD12C (vptr 0x008FD134), offset_to_top 0, 13 entries
class JobManageAccount : public ::nn::nex::StepSequenceJob
{
public:
    // the call of the job
    enum FuncType : u8
    {
        FUNC_CREATE_ACCOUNT = 0,
        FUNC_CREATE_ACCOUNT_WITH_CUSTOM_DATA = 1,
        FUNC_CHANGE_PASSWORD = 2,
        FUNC_UPDATE_ACCOUNT = 3,
        FUNC_CHANGE_PASSWORD_BY_GUEST = 4,
        FUNC_CUSTOM_CREATE_ACCOUNT = 5,
        FUNC_NINTENDO_CREATE_ACCOUNT = 6
    };
    // a call that replaces the one of the function type (set by the creator of the job; it gets
    // the hashed key)
    class Command : public RootObject
    {
    public:
        virtual ~Command() {}
        virtual bool Call(nn::nex::ProtocolCallContext* pContext) = 0; // slot 0x08

        u8 m_Unknown0x04[0xC]; // 0x04
        String m_Key;          // 0x10
    };
    typedef AnyObjectHolder<Data, String> DataHolder;

    JobManageAccount(unsigned int callContextId, nn::nex::AccountManagementProtocolClient* pProtocol, const nn::nex::String& principalName, const nn::nex::String& password,
                     unsigned int groups, const nn::nex::String& email, nn::nex::qResult* pResult, FuncType funcType, DataHolder* pData1, DataHolder* pData2,
                     DataHolder* pData3, unsigned int* pPid, nn::nex::String* pPidHMAC); // 0x00381ECC | fefates:callgraph [tier C]
    virtual ~JobManageAccount(); // 0x003820F8 slot 0x00 | fefates:bytes
    // 0x003820E8 (deleting dtor)
    void CompleteJob(); // 0x00380FD4 | fefates:bytes [tier B]
    void CompleteJobImpl(nn::nex::CallContext::State state, nn::nex::qResult result); // 0x0038105C
    void CallCreateAccount(); // 0x003810AC | fefates:bytes [tier B]
    void CallUpdateAccount(); // 0x003811FC | fefates:bytes [tier B]
    void CallChangePassword(); // 0x00381340 (name is ours)
    void PasswordInitialHash(); // 0x00381484 | fefates:bytes [tier B]
    void CallCustomCreateAccount(); // 0x0038159C | fefates:bytes [tier B]
    void CallChangePasswordByGuest(); // 0x003816E8 (name is ours)
    void CallNintendoCreateAccount(); // 0x0038182C | fefates:bytes [tier B]
    void CallAccountManagementCommand(); // 0x00381988 | fefates:bytes [tier B]
    // the call after the hash of the password (name is ours)
    void SelectCall(); // 0x00381AA4
    void CallCreateAccountWithCustomData(); // 0x00381D04 | fefates:bytes [tier B]
    void Cancel(); // 0x00381E54 | fefates:bytes [tier B]

    u32 m_CallContextId;                          // 0x68
    AccountManagementProtocolClient* m_pProtocol; // 0x6C
    String m_PrincipalName;                       // 0x70
    char* m_pPassword;                            // 0x78 (String::CreateCopy)
    u32 m_Groups;                                 // 0x7C
    String m_Email;                               // 0x80
    FuncType m_FuncType;                          // 0x88
    DataHolder m_Data1;                           // 0x8C (public data / auth data)
    DataHolder m_Data2;                           // 0x94 (private data)
    DataHolder m_Data3;                           // 0x9C (auth data of the custom calls; a Data if none)
    unsigned int* m_pPid;                         // 0xA4
    String* m_pPidHMAC;                           // 0xA8
    Key m_Key;                                    // 0xAC (the hashed password)
    qResult* m_pResult;                           // 0xC4
    bool* m_pIsSuccess;                           // 0xC8
    CallContext m_CallContext;                    // 0xD0 (of the hash)
    ProtocolCallContext m_ProtocolCallContext;    // 0x130
    Command* m_pCommand;                          // 0x1A8
};
ASSERT_OFFSET(JobManageAccount, m_Key, 0xAC);
ASSERT_OFFSET(JobManageAccount, m_CallContext, 0xD0);
ASSERT_OFFSET(JobManageAccount, m_ProtocolCallContext, 0x130);
ASSERT_SIZE(JobManageAccount, 0x1B0);
} // namespace nex
} // namespace nn
