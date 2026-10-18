#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_RVClientCore.h"
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

// the stream types of the connections
const u16 STREAM_TYPE_SECURE = 10;
} // namespace

// 0x0036D484 slot 0x08
void nn::nex::ServiceClient::vf_0x08()
{
    // nothing to do
}

// 0x0036D488 | fefates:bytes [tier B]
void nn::nex::ServiceClient::SetDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    UpdateProtocolsDefaultCredentials(pCredentials);
    if (m_pDefaultCredentials != nullptr) {
        m_pDefaultCredentials->ReleaseRef();
        m_pDefaultCredentials = nullptr;
    }
    if (pCredentials != nullptr) {
        m_pDefaultCredentials = pCredentials;
        pCredentials->AcquireRef();
    }
}

// 0x0036D524 slot 0x1C
void nn::nex::ServiceClient::ConnectionStateHasChanged()
{
    // nothing to do
}

// 0x0036D528 slot 0x0C
bool nn::nex::ServiceClient::Bind(nn::nex::Credentials* pCredentials)
{
    ScopedCS lock(GetSchedulerLock());
    if (pCredentials == nullptr) {
        return false;
    }
    SetDefaultCredentials(pCredentials);
    ConnectionStateHasChanged();
    return true;
}

// 0x0036D5A4 slot 0x10
bool nn::nex::ServiceClient::Unbind()
{
    ScopedCS lock(GetSchedulerLock());
    ConnectionStateHasChanged();
    SetDefaultCredentials(nullptr);
    return true;
}

// 0x0036D600
nn::nex::ServiceClient::ServiceClient(unsigned int id) : m_Id(id), m_pDefaultCredentials(nullptr), m_IsLocal(false)
{
    RVClientCore::AcquireInstance();
}

// 0x0036D6F4 slot 0x00 | fefates:bytes
// 0x0036D6E4 (deleting dtor)
nn::nex::ServiceClient::~ServiceClient()
{
    if (m_pDefaultCredentials != nullptr) {
        m_pDefaultCredentials->ReleaseRef();
        m_pDefaultCredentials = nullptr;
    }
    RVClientCore::GetInstance()->ReleaseRef();
}

// 0x0072AA08 slot 0x14
bool nn::nex::ServiceClient::IsConnected() const
{
    ScopedCS lock(GetSchedulerLock());
    if (m_pDefaultCredentials == nullptr) {
        return false;
    }
    if (m_IsLocal) {
        return false;
    }
    EndPoint* pConnection = m_pDefaultCredentials->m_pConnection;
    return pConnection != nullptr && pConnection->IsConnected();
}

// 0x0072AAA4 | fefates:bytes [tier B]
nn::nex::EndPoint* nn::nex::ServiceClient::GetConnection(unsigned short streamType) const
{
    if (m_IsLocal || m_pDefaultCredentials == nullptr || streamType == 0) {
        return nullptr;
    }
    return streamType == STREAM_TYPE_SECURE ? m_pDefaultCredentials->m_pSecureConnection : m_pDefaultCredentials->m_pConnection;
}

// 0x0072AADC
nn::nex::ProtocolRegistry* nn::nex::ServiceClient::GetProtocolRegistry() const
{
    return RVClientCore::GetInstance()->m_pProtocolRegistry;
}

// 0x0072AAEC slot 0x24
bool nn::nex::ServiceClient::SetCallOwner(nn::nex::ProtocolCallContext* pContext)
{
    Credentials* pCredentials = pContext->m_pCredentials != nullptr ? pContext->m_pCredentials : m_pDefaultCredentials;
    if (pCredentials == nullptr || pCredentials->m_pConnection == nullptr) {
        return false;
    }
    EndPoint* pConnection = pCredentials->m_pConnection;
    pContext->SetOwner(pConnection, pConnection->m_ConnectionId);
    return true;
}

// 0x0072AB28 slot 0x18 | fefates:bytes
bool nn::nex::ServiceClient::IsFaulty() const
{
    if (m_pDefaultCredentials == nullptr || m_IsLocal) {
        return false;
    }
    EndPoint* pConnection = m_pDefaultCredentials->m_pConnection;
    return pConnection != nullptr && pConnection->IsFaulty();
}

} // namespace nex
} // namespace nn
