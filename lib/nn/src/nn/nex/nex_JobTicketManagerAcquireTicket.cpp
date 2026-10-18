#include "nn/nex/nex_JobTicketManagerAcquireTicket.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_BitStream.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolRequestBroker.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_Ticket.h"
#include "nn/nex/nex_TicketGrantingProtocolClient.h"
#include "nn/nex/nex_TicketManager.h"
#include "nn/nex/nex_UserContext.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// (inline) the call context of the caller succeeds
inline void SignalSuccess(u32 callContextId)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(callContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(qResult::SUCCESS), true);
    }
}

// the size of the session key in the ticket (bits)
const u32 SESSION_KEY_BITS = 256;
} // namespace

// 0x003C0424 (name is ours)
void nn::nex::JobTicketManagerAcquireTicket::RequestTicket()
{
    qMap<u32, Ticket*>::iterator it = m_pManager->m_Tickets.find(m_TargetId);
    Ticket* pTicket = it != m_pManager->m_Tickets.end() ? it->second : nullptr;
    if (pTicket != nullptr) {
        *m_ppTicket = pTicket;
        pTicket->AcquireRef();
        SetToComplete();
        SignalSuccess(m_CallContextId);
        return;
    }
    AuthenticationClient* pClient = m_pManager->m_pClient;
    if (pClient->m_pDefaultCredentials == nullptr) {
        SetToComplete();
        SignalSuccess(m_CallContextId);
        return;
    }
    Message message;
    ProtocolRequestBroker::InitMessage(&message, AuthenticationClient::PROTOCOL_ID, Protocol::TYPE_SERVER);
    m_ProtocolCallContext.m_Deadline = m_pManager->m_pClient->m_CallDeadline;
    TicketGrantingProtocolClient::RequestTicket_PrepareRequest(&m_ProtocolCallContext, &message, &m_Result, m_SourceId, m_TargetId, &m_Ticket);
    UserContext context;
    context.m_pPointer = this;
    m_ProtocolCallContext.RegisterCompletionCallback(Job::ReadyOnCompletion, context, false);
    EndPoint* pEndPoint = m_pManager->m_pClient->m_pDefaultCredentials->m_pSecureConnection;
    m_ProtocolCallContext.SetOwner(pEndPoint, pEndPoint->m_ConnectionId);
    qResult result;
    {
        ScopedCS lock(GetSchedulerLock());
        result = pEndPoint->_Send(message.GetBuffer(), 1, false, 0, 0);
    }
    static_cast<bool>(result);
    SetStep(Step(&JobTicketManagerAcquireTicket::ProcessResponse, ""));
    SetToSuspended();
}

// 0x003C06E0 | fefates:bytes [tier B]
void nn::nex::JobTicketManagerAcquireTicket::ProcessResponse()
{
    if (m_ProtocolCallContext.m_State == CallContext::STATE_SUCCESS) {
        Buffer ticket;
        if (!m_pManager->m_pClient->m_pKerberos->Decrypt(m_Ticket, &ticket)) {
            SetToComplete();
            SignalSuccess(m_CallContextId);
            return;
        }
        Key sessionKey;
        Buffer data;
        BitStream stream(&ticket);
        stream.ExtractRaw(sessionKey.PrepareContentPtr(SESSION_KEY_BITS / 8), SESSION_KEY_BITS);
        u32 targetId;
        stream.Extract(reinterpret_cast<u8*>(&targetId), sizeof(targetId), 1);
        stream >> data;
        DateTime now;
        DateTime::GetSystemTime(now);
        Ticket* pTicket = new Ticket(data, m_SourceId, targetId, sessionKey, DateTime(now));
        m_pManager->InsertTicket(targetId, pTicket);
        *m_ppTicket = pTicket;
        pTicket->AcquireRef();
    }
    SetToComplete();
    SignalSuccess(m_CallContextId);
}

// 0x003C0954 slot 0x00
// 0x003C0928 (deleting dtor)
nn::nex::JobTicketManagerAcquireTicket::~JobTicketManagerAcquireTicket()
{
    // nothing to do: the members are destroyed by the compiler
}

} // namespace nex
} // namespace nn
