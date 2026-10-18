#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"

#include <cstring>

namespace nn {
namespace nex {
class Data;
class RVConnectionData;
class String;
class TicketManager;
template <typename T0, typename T1>
class AnyObjectHolder;

// The login with data of an AuthenticationClient (like JobTicketManagerLogin with the login call
// with context). The names of the members are ours.
// RTTI N2nn3nex29JobTicketManagerLoginWithDataE @ 0x008CF130
// vtable 0x008FF210 (vptr 0x008FF218), offset_to_top 0, 13 entries
class JobTicketManagerLoginWithData : public ::nn::nex::StepSequenceJob
{
public:
    // (inline in TicketManager::LoginWithData)
    JobTicketManagerLoginWithData(unsigned int callContextId, TicketManager* pManager, qResult* pResult, const char* pPassword, unsigned int* pPrincipalId,
                                  RVConnectionData* pConnectionData, const AnyObjectHolder<Data, String>* pData)
        : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_pData(pData)
    {
        m_CallContextId = callContextId;
        m_pManager = pManager;
        m_pPrincipalId = pPrincipalId;
        m_pConnectionData = pConnectionData;
        m_pResult = pResult;
        m_pPassword = NewArray<char>(strlen(pPassword) + 1);
        strcpy(m_pPassword, pPassword);
        SetStep(Step(&JobTicketManagerLoginWithData::InitiateLogin, ""));
    }
    virtual ~JobTicketManagerLoginWithData(); // 0x003C0EBC slot 0x00
    // 0x003C0E68 (deleting dtor)
    void CompleteJob(nn::nex::CallContext::State state, nn::nex::qResult result); // 0x003C097C | fefates:callgraph [tier C]
    void ValidateKey(); // 0x003C09CC | fefates:bytes [tier B]
    void InitiateLogin(); // 0x003C0B24 | fefates:bytes [tier B]
    void ProcessLoginResult(); // 0x003C0C54 | fefates:bytes [tier B]
    void DeriveKey(); // 0x003C0D14 | fefates:bytes [tier B]

    u32 m_CallContextId;                          // 0x68
    TicketManager* m_pManager;                    // 0x6C
    u8 m_Unknown0x70[0x8];                        // 0x70
    CallContext m_CallContext;                    // 0x78 (of the key derivation)
    Key m_Key;                                    // 0xD8
    char* m_pPassword;                            // 0xF0 (NewArray)
    unsigned int* m_pPrincipalId;                 // 0xF4
    RVConnectionData* m_pConnectionData;          // 0xF8
    const AnyObjectHolder<Data, String>* m_pData; // 0xFC
    ProtocolCallContext m_ProtocolCallContext;    // 0x100
    Buffer m_Ticket;                              // 0x178
    qResult* m_pResult;                           // 0x198
};
ASSERT_OFFSET(JobTicketManagerLoginWithData, m_ProtocolCallContext, 0x100);
ASSERT_SIZE(JobTicketManagerLoginWithData, 0x1A0);
} // namespace nex
} // namespace nn
