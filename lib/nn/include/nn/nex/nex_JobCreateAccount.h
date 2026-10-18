#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
class AccountManagementClient;
class BackEndServices;
class Credentials;
class Data;

// The creation of an account with a guest login: login as guest, the account management call of
// the function type (a JobManageAccount), logout. Only the steps are in this build (no RTTI, no
// constructor); the layout is from the steps, the names of the members, of the function types
// and of the steps without a reference name are ours.
class JobCreateAccount : public ::nn::nex::StepSequenceJob
{
public:
    // the call of the job
    enum FuncType : u8
    {
        FUNC_CREATE_ACCOUNT = 0,
        FUNC_CUSTOM_CREATE_ACCOUNT = 1,
        FUNC_CHANGE_PASSWORD_BY_GUEST = 2,
        FUNC_NINTENDO_CREATE_ACCOUNT = 3,
        FUNC_CREATE_ACCOUNT_WITH_CUSTOM_DATA = 4
    };
    typedef AnyObjectHolder<Data, String> DataHolder;

    void LoginGuest(); // 0x0037FB10 | fefates:bytes [tier B]
    void CompleteJob(nn::nex::qResult result); // 0x0037FBEC | fefates:bytes [tier B]
    void LogoutGuest(); // 0x0037FC84 | fefates:bytes [tier B]
    void CallCreateAccount(); // 0x0037FD28 (name is ours)
    void CallCustomCreateAccount(); // 0x0037FF14 (name is ours)
    void CallChangePasswordByGuest(); // 0x00380100 (name is ours)
    void CallNintendoCreateAccount(); // 0x003802DC (name is ours)
    void ProcessLoginGuestResult(); // 0x003804CC (name is ours)
    void ProcessLogoutGuestResult(); // 0x00380610 | fefates:bytes [tier B]
    void ProcessCreateAccountResult(); // 0x0038063C | fefates:bytes [tier B]
    void CallCreateAccountWithCustomData(); // 0x00380734 (name is ours)

    u32 m_CallContextId;                       // 0x68
    Credentials* m_pGuestCredentials;          // 0x6C
    const char* m_pGuestPassword;              // 0x70
    u32 m_Unknown0x74;                         // 0x74
    const wchar_t* m_pAddress;                 // 0x78
    u16 m_Port;                                // 0x7C
    String m_PrincipalName;                    // 0x80
    const char* m_pPassword;                   // 0x88
    u32 m_Groups;                              // 0x8C
    String m_Email;                            // 0x90
    FuncType m_FuncType;                       // 0x98
    DataHolder m_PublicData;                   // 0x9C
    DataHolder m_PrivateData;                  // 0xA4
    DataHolder m_AuthData;                     // 0xAC
    unsigned int* m_pPid;                      // 0xB4
    String* m_pPidHMAC;                        // 0xB8
    qResult m_CreateAccountResult;             // 0xBC
    qResult m_Result;                          // 0xC8
    BackEndServices* m_pBackEndServices;       // 0xD4
    AccountManagementClient* m_pClient;        // 0xD8
    ProtocolCallContext m_LoginContext;        // 0xE0
    ProtocolCallContext m_ManageAccountContext; // 0x158
    ProtocolCallContext m_LogoutContext;       // 0x1D0
};
ASSERT_OFFSET(JobCreateAccount, m_CreateAccountResult, 0xBC);
ASSERT_OFFSET(JobCreateAccount, m_LoginContext, 0xE0);
ASSERT_OFFSET(JobCreateAccount, m_LogoutContext, 0x1D0);
} // namespace nex
} // namespace nn
