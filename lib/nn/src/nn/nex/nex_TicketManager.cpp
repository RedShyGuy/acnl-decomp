#include "nn/nex/nex_TicketManager.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobTicketManagerAcquireTicket.h"
#include "nn/nex/nex_JobTicketManagerLoginWithData.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_Ticket.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// (inline) the job is queued
inline void QueueJob(Job* pJob)
{
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
}
} // namespace

// 0x0036EC1C | mk7dlp:bytes [tier A]
void nn::nex::TicketManager::InsertTicket(unsigned principalId, nn::nex::Ticket* pTicket)
{
    m_Tickets.IndexInline(principalId) = pTicket;
}

// 0x0036ED64 | fefates:callgraph [tier C]
bool nn::nex::TicketManager::AcquireTicket(nn::nex::CallContext* pContext, unsigned int sourceId, unsigned int targetId, nn::nex::Ticket** ppTicket)
{
    ScopedCS lock(GetSchedulerLock());
    if (!pContext->InitiateCall()) {
        return false;
    }
    pContext->m_Deadline = m_pClient->m_CallDeadline;
    QueueJob(new JobTicketManagerAcquireTicket(pContext->m_Id, this, sourceId, targetId, ppTicket));
    return true;
}

// 0x0036EEE4
bool nn::nex::TicketManager::LoginWithData(nn::nex::CallContext* pContext, nn::nex::qResult* pResult, const char* pPassword, unsigned int* pPrincipalId,
                                           nn::nex::RVConnectionData* pConnectionData, const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData)
{
    ScopedCS lock(GetSchedulerLock());
    if (!pContext->InitiateCall()) {
        return false;
    }
    QueueJob(new JobTicketManagerLoginWithData(pContext->m_Id, this, pResult, pPassword, pPrincipalId, pConnectionData, pData));
    return true;
}

// 0x0036F098 | fefates:bytes [tier B]
void nn::nex::TicketManager::ReleaseTicket(nn::nex::Ticket* pTicket)
{
    pTicket->ReleaseRef();
    if (pTicket->m_RefCount != 1) {
        return;
    }
    // only the manager holds it
    m_Tickets.EraseInline(pTicket->m_Unknown0x30);
    pTicket->ReleaseRef();
}

// 0x0036F244 | mk7dlp:bytes [tier A]
nn::nex::TicketManager::TicketManager(nn::nex::AuthenticationClient* pClient) : m_pClient(pClient)
{
    // nothing to do
}

} // namespace nex
} // namespace nn

namespace __rw {
// the functions of the tree of the tickets
// 0x00807FFC (name is ours)
template <>
DECOMP_NOINLINE __rb_tree<u32, std::pair<const u32, nn::nex::Ticket*>, __select1st<std::pair<const u32, nn::nex::Ticket*>, u32>, std::less<u32>,
                          nn::nex::MemAllocator<std::pair<const u32, nn::nex::Ticket*> > >::iterator
__rb_tree<u32, std::pair<const u32, nn::nex::Ticket*>, __select1st<std::pair<const u32, nn::nex::Ticket*>, u32>, std::less<u32>,
          nn::nex::MemAllocator<std::pair<const u32, nn::nex::Ticket*> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
// 0x00808794 (name is ours)
template __rb_tree<u32, std::pair<const u32, nn::nex::Ticket*>, __select1st<std::pair<const u32, nn::nex::Ticket*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::Ticket*> > >::iterator __rb_tree<u32, std::pair<const u32, nn::nex::Ticket*>, __select1st<std::pair<const u32, nn::nex::Ticket*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::Ticket*> > >::_C_insert(Node* pNode, Node* pParent, const std::pair<const u32, nn::nex::Ticket*>& value);
} // namespace __rw
