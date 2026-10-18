#include "nn/nex/nex_ClientStreamManager.h"
#include "nn/nex/nex_ClientProtocolRequestBroker.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_JobProcessProtocolMessage.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// 0x003929DC slot 0x0C
void nn::nex::ClientStreamManager::FaultDetection(nn::nex::EndPoint* pEndPoint, unsigned int error)
{
    static_cast<ClientProtocolRequestBroker*>(m_pBroker)->FaultDetection(pEndPoint, error);
}

// 0x003929EC slot 0x14 | fefates:bytes
void nn::nex::ClientStreamManager::SetCredentials(nn::nex::Credentials* pCredentials)
{
    if (m_pCredentials != nullptr) {
        m_pCredentials->ReleaseRef();
        m_pCredentials = nullptr;
    }
    if (pCredentials != nullptr) {
        m_pCredentials = pCredentials;
        pCredentials->AcquireRef();
    }
}

// 0x00392A78 slot 0x10
void nn::nex::ClientStreamManager::PeerDisconnected(nn::nex::EndPoint* pEndPoint)
{
    static_cast<ClientProtocolRequestBroker*>(m_pBroker)->PeerDisconnected(pEndPoint);
}

// 0x00392B04 slot 0x00
// 0x00392A88 (deleting dtor)
nn::nex::ClientStreamManager::~ClientStreamManager()
{
    if (m_pCredentials != nullptr) {
        m_pCredentials->ReleaseRef();
        m_pCredentials = nullptr;
    }
}

// 0x003D912C slot 0x08 | fefates:bytes
void nn::nex::ClientStreamManager::Receive(nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pData, unsigned char substreamId)
{
    JobProcessProtocolMessage* pJob = new JobProcessProtocolMessage(m_pBroker, pEndPoint, pData);
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
}

} // namespace nex
} // namespace nn
