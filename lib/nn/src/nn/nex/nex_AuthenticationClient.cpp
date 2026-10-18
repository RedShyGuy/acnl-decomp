#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobTicketManagerLogin.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}
} // namespace

// 0x00395390 slot 0x20
void nn::nex::AuthenticationClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    m_pProtocol->SetDefaultCredentials(pCredentials);
}

// 0x003953A0
bool nn::nex::AuthenticationClient::Login(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::String& name, const char* pPassword,
                                          nn::nex::RVConnectionData* pConnectionData, nn::nex::String* pServerName,
                                          nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData)
{
    m_CallDeadline = pContext->m_Deadline;
    TicketManager* pManager = m_pTicketManager;
    ScopedCS lock(GetSchedulerLock());
    if (!pContext->InitiateCall()) {
        return false;
    }
    JobTicketManagerLogin* pJob =
        new JobTicketManagerLogin(pContext->m_Id, pManager, pResult, name, pPassword, &m_PrincipalId, pConnectionData, pServerName, pData);
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
    return true;
}

// 0x003954BC slot 0x00 | fefates:bytes
// 0x003954AC (deleting dtor)
nn::nex::AuthenticationClient::~AuthenticationClient()
{
    if (m_pKerberos != nullptr) {
        delete m_pKerberos;
    }
    if (m_pTicketManager != nullptr) {
        delete m_pTicketManager;
    }
    if (m_pProtocol != nullptr) {
        delete m_pProtocol;
    }
    AnyDataAdapter::Unregister();
}

} // namespace nex
} // namespace nn
