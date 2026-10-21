#pragma once

#include "decomp.h"
#include "nn/nex/nex_AuthenticationInfo.h"
#include "nn/nex/nex_CustomDataHolder.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
class NgsFacade;
class Credentials;

// The custom data that the login jobs send to the secure server (name is ours; defined in
// nex_JobLogin.cpp)
extern AnyObjectHolder<Data, String> g_LoginCustomData;

// A login of BackEndServices: the steps of the platform login and of the login to the server
// (RendezVous::Login with m_CallContext). It completes the call context of the caller with its
// result. The names of the members are ours.
// RTTI N2nn3nex8JobLoginE @ 0x008CF6DC
// vtable 0x008FFC68 (vptr 0x008FFC70), offset_to_top 0, 14 entries
class JobLogin : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    // the time between the checks of a request (ms)
    static const int WAIT_TIME = 250;

    // (inline)
    explicit JobLogin(const DebugString& debugString)
        : StepSequenceJob(static_cast<JobType>(2), debugString), m_CallContextId(0), m_Unknown0xE4(0), m_pBackEndServices(nullptr),
          m_pCredentials(nullptr), m_pKeyHash(nullptr), m_pServiceToken(nullptr), m_Timeout(0), m_StartTime(0)
    {
    }
    virtual ~JobLogin(); // 0x003D5104 slot 0x00 | fefates:bytes
    // 0x003D50F4 slot 0x04 (deleting dtor)
    virtual void StepFirst() = 0; // slot 0x34
    // completes the job and the call of the caller with the result
    void Complete(const nn::nex::qResult& result); // 0x003D5064 | mk7dlp:callseq [tier A]

    // (inline; the name is ours) the timeout passed
    bool IsTimedOut()
    {
        return m_Timeout != -1 && static_cast<s32>(Time::GetTime().m_Value - m_StartTime.m_Value) >= m_Timeout;
    }

    ProtocolCallContext m_CallContext;             // 0x068 (of the login to the server)
    u32 m_CallContextId;                           // 0x0E0 (of the caller)
    u32 m_Unknown0xE4;                             // 0x0E4 (the server id of the friends requests)
    NgsFacade* m_pBackEndServices;                 // 0x0E8
    Credentials* m_pCredentials;                   // 0x0EC (the result)
    const char* m_pKeyHash;                        // 0x0F0
    char* m_pServiceToken;                         // 0x0F4 (256 bytes)
    s32 m_Timeout;                                 // 0x0F8 (ms; -1: none)
    Time m_StartTime;                              // 0x100
    CustomDataHolder<AuthenticationInfo> m_CustomData; // 0x108
    String m_PrincipalName;                        // 0x110 (the result)
};
ASSERT_OFFSET(JobLogin, m_CallContext, 0x68);
ASSERT_OFFSET(JobLogin, m_StartTime, 0x100);
ASSERT_SIZE(JobLogin, 0x118);
} // namespace nex
} // namespace nn
