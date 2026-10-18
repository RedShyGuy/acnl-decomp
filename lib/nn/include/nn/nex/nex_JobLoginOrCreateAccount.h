#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_JobManageAccount.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
class BackEndServices;
class Credentials;
class Data;

// A login that creates the account (with the call of its command) when the login fails because
// the account does not exist, then logs in again. Only the steps are in this build (no RTTI, no
// constructor); the layout is from the steps, the names of the members and of the steps without a
// reference name are ours.
class JobLoginOrCreateAccount : public ::nn::nex::StepSequenceJob
{
public:
    // the job is completed with the result (introduced by the class or its base)
    virtual void CompleteJob(nn::nex::qResult result) = 0; // slot 0x34

    void CreateAccount(); // 0x003B0D74 | fefates:bytes [tier B]
    void ProcessRVLoginResult(); // 0x003B0E28 | fefates:bytes [tier B]
    void ProcessCreateAccountResult(); // 0x003B0F0C | fefates:bytes [tier B]
    // the login to the back end
    void LoginRV(); // 0x003B0F90 (name is ours)

    u8 m_Unknown0x68[0x8];                       // 0x68
    const wchar_t* m_pName;                      // 0x70
    String m_Password;                           // 0x74
    u8 m_Unknown0x7C[0xC];                       // 0x7C
    Credentials** m_ppCredentials;               // 0x88
    String* m_pServerName;                       // 0x8C
    AnyObjectHolder<Data, String>* m_pData;      // 0x90
    u32 m_Unknown0x94;                           // 0x94
    CallContext m_LoginContext;                  // 0x98
    ProtocolCallContext m_CreateAccountContext;  // 0xF8
    JobManageAccount::Command* m_pCreateAccount; // 0x170
    bool m_IsAccountCreated;                     // 0x174
    u8 m_Unknown0x178[0x40];                     // 0x178
    const wchar_t* m_pAddress;                   // 0x1B8
    u16 m_Port;                                  // 0x1BC
    u8 m_Unknown0x1C0[0x10];                     // 0x1C0
    u32 m_Deadline;                              // 0x1D0 (the low word of the time)
    u32 m_Unknown0x1D4;                          // 0x1D4
    BackEndServices* m_pBackEndServices;         // 0x1D8
};
ASSERT_OFFSET(JobLoginOrCreateAccount, m_LoginContext, 0x98);
ASSERT_OFFSET(JobLoginOrCreateAccount, m_CreateAccountContext, 0xF8);
ASSERT_OFFSET(JobLoginOrCreateAccount, m_pAddress, 0x1B8);
ASSERT_OFFSET(JobLoginOrCreateAccount, m_pBackEndServices, 0x1D8);
} // namespace nex
} // namespace nn
