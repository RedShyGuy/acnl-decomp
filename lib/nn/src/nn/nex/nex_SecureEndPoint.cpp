#include "nn/nex/nex_SecureEndPoint.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobConnectSecureEndPoint.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_CONNECTION_FAILED = static_cast<int>(0x80050001);
const int RESULT_TIMEOUT = static_cast<int>(0x80050008);

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}
} // namespace

// 0x00374220 slot 0x00 | slot vf_0x00 of nn::nex::EndPoint
// 0x00374204 (deleting dtor)
nn::nex::SecureEndPoint::~SecureEndPoint()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

// 0x00373D8C slot 0x08 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::ResetStatistics()
{
    // nothing to do
}

// 0x0072B2B8 slot 0x0C | virtual slot, introduced by nn::nex::EndPoint
bool nn::nex::SecureEndPoint::GetStatistics(void* pStatistics)
{
    return false;
}

// 0x0072B320 slot 0x10 | virtual slot, introduced by nn::nex::EndPoint
u32 nn::nex::SecureEndPoint::GetPendingReliablePacketCount()
{
    return 0;
}

// 0x0072B328 slot 0x14 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::GetPendingReliablePacketCounts(nn::nex::qVector<u32>* pCounts)
{
    // nothing to do
}

// 0x0072B2C0 slot 0x18 | virtual slot, introduced by nn::nex::EndPoint
u16 nn::nex::SecureEndPoint::GetMaxSubstreamId()
{
    return 0;
}

// 0x00373CA8 slot 0x1C | slot vf_0x1C of nn::nex::EndPoint
bool nn::nex::SecureEndPoint::IsNotConnected()
{
    if (m_pEndPoint == nullptr) {
        return true;
    }
    return m_pEndPoint->IsNotConnected();
}

// 0x00373C8C slot 0x20 | fefates:bytes
bool nn::nex::SecureEndPoint::IsConnecting()
{
    if (m_pEndPoint == nullptr) {
        return false;
    }
    return m_pEndPoint->IsConnecting();
}

// 0x00373CC8 slot 0x24 | slot vf_0x24 of nn::nex::EndPoint
bool nn::nex::SecureEndPoint::IsDisconnecting()
{
    if (m_pEndPoint == nullptr) {
        return false;
    }
    return m_pEndPoint->IsDisconnecting();
}

// 0x00374010 slot 0x28 | fefates:bytes
bool nn::nex::SecureEndPoint::IsFaulty()
{
    if (m_pEndPoint == nullptr) {
        return false;
    }
    return m_pEndPoint->IsFaulty();
}

// 0x00373B64 slot 0x2C | fefates:bytes
bool nn::nex::SecureEndPoint::IsConnected()
{
    if (m_pEndPoint == nullptr) {
        return false;
    }
    return m_pEndPoint->IsConnected();
}

// 0x00373CE4 slot 0x30 | fefates:bytes
bool nn::nex::SecureEndPoint::PeerIsConnected()
{
    if (m_pEndPoint == nullptr) {
        return false;
    }
    return m_pEndPoint->PeerIsConnected();
}

// 0x00373E70 slot 0x34 | virtual slot, introduced by nn::nex::EndPoint
bool nn::nex::SecureEndPoint::PeerIsNotConnected()
{
    if (m_pEndPoint == nullptr) {
        return true;
    }
    return m_pEndPoint->PeerIsNotConnected();
}

// 0x00373EC8 slot 0x3C | fefates:bytes
void nn::nex::SecureEndPoint::SetKeepAliveTimeout(unsigned int milliseconds)
{
    if (m_pEndPoint != nullptr) {
        m_pEndPoint->SetKeepAliveTimeout(milliseconds);
    }
}

// 0x00373D90 slot 0x40 | fefates:bytes
void nn::nex::SecureEndPoint::SetMaxSilenceTime(unsigned int milliseconds)
{
    if (m_pEndPoint != nullptr) {
        m_pEndPoint->SetMaxSilenceTime(milliseconds);
    }
}

// 0x00373EAC slot 0x44 | slot vf_0x44 of nn::nex::EndPoint
u32 nn::nex::SecureEndPoint::GetKeepAliveTimeout()
{
    if (m_pEndPoint == nullptr) {
        return 0;
    }
    return m_pEndPoint->GetKeepAliveTimeout();
}

// 0x00373D70 slot 0x48 | slot vf_0x48 of nn::nex::EndPoint
u32 nn::nex::SecureEndPoint::GetMaxSilenceTime()
{
    if (m_pEndPoint == nullptr) {
        return 0;
    }
    return m_pEndPoint->GetMaxSilenceTime();
}

// 0x00373D38 slot 0x4C | slot vf_0x4C of nn::nex::EndPoint
void nn::nex::SecureEndPoint::SetPeerConnected()
{
    if (m_pEndPoint != nullptr) {
        m_pEndPoint->SetPeerConnected();
    }
}

// 0x00373EE4 slot 0x50 | slot vf_0x50 of nn::nex::EndPoint
void nn::nex::SecureEndPoint::SetPeerDisconnected()
{
    if (m_pEndPoint != nullptr) {
        m_pEndPoint->SetPeerDisconnected();
    }
}

// 0x0072B2C8 slot 0x54 | slot vf_0x54 of nn::nex::EndPoint
u8 nn::nex::SecureEndPoint::GetConnectionState() const
{
    if (m_pEndPoint == nullptr) {
        return STATE_NOT_CONNECTED;
    }
    return m_pEndPoint->GetConnectionState();
}

// 0x00373E90 slot 0x58 | fefates:bytes
bool nn::nex::SecureEndPoint::SetConnectionState(nn::nex::EndPoint::_ConnectionState state)
{
    if (m_pEndPoint == nullptr) {
        return false;
    }
    return m_pEndPoint->SetConnectionState(state);
}

// 0x00373F1C slot 0x5C | mk7dlp:callseq
nn::nex::qResult nn::nex::SecureEndPoint::RegisterEventHandler(nn::nex::EndPointEventHandler* pHandler)
{
    qResult result = EndPoint::RegisterEventHandler(pHandler);
    if (!result.Equals(false) && m_pEndPoint != nullptr) {
        // the end point below gets the same event handler
        result = m_pEndPoint->RegisterEventHandler(m_pEventHandler);
        // (a check of the original whose use was compiled out)
        result.Equals(false);
    }
    return result;
}

// 0x00373F00 slot 0x60 | fefates:bytes
bool nn::nex::SecureEndPoint::IsAnyPacketProcessed()
{
    if (m_pEndPoint == nullptr) {
        return false;
    }
    return m_pEndPoint->IsAnyPacketProcessed();
}

// 0x0072B2E4 slot 0x64 | fefates:bytes
u32 nn::nex::SecureEndPoint::GetSignaledFaultError() const
{
    if (m_pEndPoint == nullptr) {
        return 0;
    }
    return m_pEndPoint->GetSignaledFaultError();
}

// 0x0072B32C slot 0x68 | virtual slot, introduced by nn::nex::EndPoint
u32 nn::nex::SecureEndPoint::GetLastRTT()
{
    if (m_pEndPoint == nullptr) {
        return 0;
    }
    return m_pEndPoint->GetLastRTT();
}

// 0x0072B29C slot 0x6C | virtual slot, introduced by nn::nex::EndPoint
u32 nn::nex::SecureEndPoint::GetRTO()
{
    if (m_pEndPoint == nullptr) {
        return 0;
    }
    return m_pEndPoint->GetRTO();
}

// 0x0037402C slot 0x70 | fefates:bytes
nn::nex::qResult nn::nex::SecureEndPoint::_Connect(nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData, CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout)
{
    JobConnectSecureEndPoint* pJob = new JobConnectSecureEndPoint(this, &m_URL, pConnectData, pReplyData, pCallback, context, timeout);
    pJob->AcquireRef();
    Core::GetInstance()->m_pScheduler->Queue(pJob, false);
    if (pCallback != nullptr) {
        pJob->ReleaseRef();
        return qResult(qResult::SUCCESS);
    }
    // synchronous
    qResult result(RESULT_CONNECTION_FAILED);
    pJob->WaitForCompletion(timeout);
    if (pJob->m_State != Job::STATE_COMPLETED) {
        result = qResult(RESULT_TIMEOUT);
    } else {
        result = pJob->m_Result;
    }
    pJob->ReleaseRef();
    return result;
}

// 0x00373B80 slot 0x74 | fefates:bytes
nn::nex::qResult nn::nex::SecureEndPoint::_Disconnect(CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout)
{
    qResult result(qResult::SUCCESS);
    if (pCallback != nullptr) {
        m_pCallback = pCallback;
        m_Context = context;
        UserContext disconnectContext;
        disconnectContext.m_pPointer = this;
        result = m_pEndPoint->_Disconnect(DisconnectCallback, disconnectContext, timeout);
        return result;
    }
    result = m_pEndPoint->_Disconnect(pCallback, context, timeout);
    ReleaseEndPoint();
    return result;
}

// 0x00373DAC (name is ours)
void nn::nex::SecureEndPoint::DisconnectCallback(nn::nex::EndPoint* pEndPoint, nn::nex::qResult result, const nn::nex::UserContext* pContext)
{
    ScopedCS lock(GetSchedulerLock());
    SecureEndPoint* pSecureEndPoint = static_cast<SecureEndPoint*>(pEndPoint->m_pOwner);
    if (pSecureEndPoint != nullptr) {
        pSecureEndPoint->ReleaseEndPoint();
        CompletionCallback pCallback = pSecureEndPoint->m_pCallback;
        pSecureEndPoint->m_pCallback = nullptr;
        pCallback(pSecureEndPoint, result, &pSecureEndPoint->m_Context);
    }
}

// 0x00373F90 slot 0x78 | slot vf_0x78 of nn::nex::EndPoint
nn::nex::qResult nn::nex::SecureEndPoint::_Send(nn::nex::Buffer* pBuffer, unsigned int flags, bool copy, unsigned char substreamId, unsigned int id)
{
    EndPoint* pEndPoint = m_pEndPoint;
    ScopedCS lock(GetSchedulerLock());
    return pEndPoint->_Send(pBuffer, flags, copy, substreamId, id);
}

// 0x0072B300 slot 0x7C | fefates:bytes
u64 nn::nex::SecureEndPoint::GetSupportedFunctionsFlags() const
{
    if (m_pEndPoint == nullptr) {
        return 0;
    }
    return m_pEndPoint->GetSupportedFunctionsFlags();
}

// 0x00373D54 slot 0x88 | fefates:bytes
void nn::nex::SecureEndPoint::SignalFaultEvent(unsigned int error, bool isProcessed)
{
    if (m_pEndPoint != nullptr) {
        m_pEndPoint->SignalFaultEvent(error, isProcessed);
    }
}

// 0x00373D1C slot 0x8C | fefates:bytes
void nn::nex::SecureEndPoint::SetEncryptionKey(const nn::nex::Key& key)
{
    if (m_pEndPoint != nullptr) {
        m_pEndPoint->SetEncryptionKey(key);
    }
}

// 0x00373D00 slot 0x90 | slot vf_0x90 of nn::nex::EndPoint
const nn::nex::Key& nn::nex::SecureEndPoint::GetEncryptionKey()
{
    // (as in the original: without an end point below, the reference is null)
    if (m_pEndPoint == nullptr) {
        return *reinterpret_cast<const Key*>(m_pEndPoint);
    }
    return m_pEndPoint->GetEncryptionKey();
}

} // namespace nex
} // namespace nn
