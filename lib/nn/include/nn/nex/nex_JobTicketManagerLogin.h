#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
class Data;
class RVConnectionData;
class TicketManager;
template <typename T0, typename T1>
class AnyObjectHolder;

// The login of an AuthenticationClient: the login call to the ticket granting server, then the key
// of the password and the decryption of the ticket. The names of the members are ours.
// RTTI N2nn3nex21JobTicketManagerLoginE @ 0x008CEB0C
// vtable 0x008FDE34 (vptr 0x008FDE3C), offset_to_top 0, 13 entries
class JobTicketManagerLogin : public ::nn::nex::StepSequenceJob
{
public:
    JobTicketManagerLogin(unsigned callContextId, nn::nex::TicketManager* pManager, nn::nex::qResult* pResult, const nn::nex::String& name, const char* pPassword,
                          unsigned* pPrincipalId, nn::nex::RVConnectionData* pConnectionData, nn::nex::String* pServerName,
                          nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData); // 0x0039A320 | mk7dlp:callseq [tier A]
    virtual ~JobTicketManagerLogin(); // 0x0039A4A4 slot 0x00
    // 0x0039A448 (deleting dtor)
    // the call context of the caller gets the state and result
    void CompleteJob(nn::nex::CallContext::State state, nn::nex::qResult result); // 0x00399DA4 | fefates:callgraph [tier C]
    void ValidateKey(); // 0x00399DF4 | fefates:bytes [tier B]
    void InitiateLogin(); // 0x00399F6C | fefates:bytes [tier B]
    void ProcessLoginResult(); // 0x0039A0F0 | fefates:bytes [tier B]
    void DeriveKey(); // 0x0039A1D4 | fefates:bytes [tier B]

    u32 m_CallContextId;                          // 0x68
    TicketManager* m_pManager;                    // 0x6C
    u8 m_Unknown0x70[0x8];                        // 0x70
    CallContext m_CallContext;                    // 0x78 (of the key derivation)
    Key m_Key;                                    // 0xD8
    String m_Name;                                // 0xF0
    char* m_pPassword;                            // 0xF8 (NewArray)
    unsigned* m_pPrincipalId;                     // 0xFC
    String* m_pServerName;                        // 0x100
    RVConnectionData* m_pConnectionData;          // 0x104
    AnyObjectHolder<Data, String>* m_pData;       // 0x108 (null: a plain login)
    ProtocolCallContext m_ProtocolCallContext;    // 0x110
    Buffer m_Ticket;                              // 0x188
    qResult* m_pResult;                           // 0x1A8
};
ASSERT_OFFSET(JobTicketManagerLogin, m_CallContext, 0x78);
ASSERT_OFFSET(JobTicketManagerLogin, m_ProtocolCallContext, 0x110);
ASSERT_OFFSET(JobTicketManagerLogin, m_pResult, 0x1A8);
} // namespace nex
} // namespace nn
