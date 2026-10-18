#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
class Ticket;
class TicketManager;

// The ticket of a station for the source station: from the TicketManager, else from the ticket
// granting server. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex29JobTicketManagerAcquireTicketE @ 0x008CF124
// vtable 0x008FF1D4 (vptr 0x008FF1DC), offset_to_top 0, 13 entries
class JobTicketManagerAcquireTicket : public ::nn::nex::StepSequenceJob
{
public:
    // (inline in TicketManager::AcquireTicket)
    JobTicketManagerAcquireTicket(unsigned int callContextId, TicketManager* pManager, unsigned int sourceId, unsigned int targetId, Ticket** ppTicket)
        : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(callContextId), m_pManager(pManager), m_SourceId(sourceId), m_TargetId(targetId),
          m_ppTicket(ppTicket)
    {
        *ppTicket = nullptr;
        SetStep(Step(&JobTicketManagerAcquireTicket::RequestTicket, ""));
    }
    virtual ~JobTicketManagerAcquireTicket(); // 0x003C0954 slot 0x00
    // 0x003C0928 (deleting dtor)
    // the ticket of the manager, else the request to the server
    void RequestTicket(); // 0x003C0424 (name is ours)
    void ProcessResponse(); // 0x003C06E0 | fefates:bytes [tier B]

    u32 m_CallContextId;                         // 0x68
    TicketManager* m_pManager;                   // 0x6C
    u32 m_SourceId;                              // 0x70
    u32 m_TargetId;                              // 0x74
    Ticket** m_ppTicket;                         // 0x78
    ProtocolCallContext m_ProtocolCallContext;   // 0x80
    Buffer m_Ticket;                             // 0xF8 (encrypted)
    qResult m_Result;                            // 0x118
};
ASSERT_OFFSET(JobTicketManagerAcquireTicket, m_ProtocolCallContext, 0x80);
ASSERT_SIZE(JobTicketManagerAcquireTicket, 0x128);
} // namespace nex
} // namespace nn
