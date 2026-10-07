#include "nn/pia/inet/inet_NexNatRelay.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_NATTraversalRelayClient.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_qList.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"
#include "nn/pia/transport/transport_StationLocation.h"
#include "pead/peadHeapMgr.h"
#include <new>

namespace nn {
namespace pia {
namespace inet {
namespace {
const u64 TRACE_FLAG = 0x8000ULL;

// a running call is cancelled before the next one (inline; name is ours)
inline void ResetCallContext(nex::ProtocolCallContext* pContext)
{
    if (pContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        pContext->Cancel(nex::CallContext::STATE_CANCELLED);
    }
    pContext->Reset();
}

// the location as a station URL (inline; name is ours)
inline void SetStationUrl(nex::StationURL* pUrl, nex::InetAddress* pNexAddress, const transport::StationLocation& location)
{
    pNexAddress->SetAddress(location.m_StationAddress.m_InetAddress.m_Address);
    pNexAddress->SetPortNumber(location.m_StationAddress.m_InetAddress.m_Port);
    pUrl->SetInetAddress(pNexAddress);
    pUrl->SetPrincipalID(location.m_PrincipalId);
    pUrl->SetConnectionID(location.m_ConnectionId);
    pUrl->SetRVConnectionID(location.m_StationKey);
    pUrl->SetURLType(static_cast<nex::StationURL::URLType>(location.m_UrlType));
    pUrl->SetStreamID(location.m_StreamId);
    pUrl->SetStreamType(location.m_StreamType);
    pUrl->SetNATMapping(location.m_NatMapping);
    pUrl->SetNATFiltering(location.m_NatFiltering);
    pUrl->SetType(location.m_Type);
    pUrl->SetProbeRequestInitiation(location.m_ProbeRequestInitiation != 0);
}
} // namespace

// 0x003E3B3C | slot vf_0x18 of nn::pia::inet::NexNatRelay
void nn::pia::inet::NexNatRelay::SetLocalCID(unsigned int cid)
{
    if (m_pProtocol != nullptr) {
        m_pProtocol->SetLocalCid(cid);
    }
}

// 0x003E3B50 | slot vf_0x08 of nn::pia::inet::NexNatRelay
void nn::pia::inet::NexNatRelay::RequestProbe(const nn::pia::transport::StationLocation& location)
{
    if (m_pProtocol != nullptr) {
        m_pProtocol->StartProbe(location);
    }
}

// 0x003E3B64 | slot vf_0x1C of nn::pia::inet::NexNatRelay
void nn::pia::inet::NexNatRelay::AssociateProtocol(nn::pia::inet::NexNatTraversalProtocol* pProtocol)
{
    m_pProtocol = pProtocol;
}

// 0x003E3B6C | slot vf_0x0C of nn::pia::inet::NexNatRelay
void nn::pia::inet::NexNatRelay::RelayProbeRequest(const nn::pia::transport::StationLocation& target, const nn::pia::transport::StationLocation& self)
{
    if (!NexFacade::s_pInstance->m_IsStarted || !common::IsValidPointer(m_pNexAddress) || !common::IsValidPointer(m_pTargetUrls)) {
        return;
    }
    target.Trace(TRACE_FLAG);
    self.Trace(TRACE_FLAG);
    SetStationUrl(&m_pTargetUrls->front(), m_pNexAddress, target);
    SetStationUrl(m_pSourceUrl, m_pNexAddress, self);
    // the traversal to the target begins
    NatTraversalTimeList& times = NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol->m_TraversalTimeList;
    NatTraversalTime* pTime = times.Find(target);
    if (pTime != nullptr) {
        pTime->m_Unknown0x30.SetNow();
        pTime->m_Unknown0x38 = 0;
    } else {
        NatTraversalTime time;
        time.m_Location = target;
        time.m_Unknown0x28.SetNow();
        time.m_Unknown0x30.SetNow();
        time.m_Unknown0x38 = 0;
        time.m_PortCheckMSec = 0;
        NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol->m_TraversalTimeList.Add(time);
    }
    nex::qList<nex::StationURL>* pTargetUrls = m_pTargetUrls;
    nex::StationURL* pSourceUrl = m_pSourceUrl;
    ResetCallContext(m_RelayInterface.m_pCallContext);
    m_RelayInterface.m_pRelayClient->CallRequestProbeInitiationExt(m_RelayInterface.m_pCallContext, *pTargetUrls, *pSourceUrl);
}

// 0x003E3E48 | fefates:bytes
void nn::pia::inet::NexNatRelay::ReportNatProperties(const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt)
{
    if (!NexFacade::s_pInstance->m_IsStarted) {
        return;
    }
    u32 mappingValue = mapping;
    u32 filteringValue = filtering;
    u32 rttValue = rtt;
    ResetCallContext(m_RelayInterface.m_pCallContext);
    m_RelayInterface.m_pRelayClient->CallReportNATProperties(m_RelayInterface.m_pCallContext, mappingValue, filteringValue, rttValue);
}

// 0x003E3EC4 | fefates:bytes
void nn::pia::inet::NexNatRelay::ReportNatTraversalResult(const unsigned int& stationKey, const bool& isSucceeded, const unsigned int& rtt)
{
    if (!NexFacade::s_pInstance->m_IsStarted) {
        return;
    }
    ResetCallContext(m_RelayInterface.m_pCallContext);
    m_RelayInterface.m_pRelayClient->CallReportNATTraversalResult(m_RelayInterface.m_pCallContext, stationKey, isSucceeded, rtt);
}

// 0x003E3F28 | fefates:bytes [tier B]
nn::pia::inet::NexNatRelay::NexNatRelay()
{
    void* pBuffer = pead::AllocMemory(sizeof(nex::qList<nex::StationURL>), common::HeapManager::GetHeap());
    m_pTargetUrls = ::new (pBuffer) nex::qList<nex::StationURL>();
    m_pTargetUrls->push_back(nex::StationURL());
    pBuffer = pead::AllocMemory(sizeof(nex::StationURL), common::HeapManager::GetHeap());
    m_pSourceUrl = ::new (pBuffer) nex::StationURL();
    pBuffer = pead::AllocMemory(sizeof(nex::InetAddress), common::HeapManager::GetHeap());
    m_pNexAddress = ::new (pBuffer) nex::InetAddress();
    pBuffer = pead::AllocMemory(sizeof(nex::ProtocolCallContext), common::HeapManager::GetHeap());
    m_pCallContext = ::new (pBuffer) nex::ProtocolCallContext();
    m_RelayInterface.m_pCallContext = m_pCallContext;
    m_RelayInterface.m_pNatRelay = this;
}

// 0x003E4154 | fefates:bytes
// 0x003E4144 (deleting dtor)
nn::pia::inet::NexNatRelay::~NexNatRelay()
{
    m_RelayInterface.m_pNatRelay = nullptr;
    m_RelayInterface.m_pCallContext = nullptr;
    if (m_pTargetUrls != nullptr) {
        m_pTargetUrls->~qList();
        pead::FreeMemory(m_pTargetUrls);
        m_pTargetUrls = nullptr;
    }
    if (m_pSourceUrl != nullptr) {
        m_pSourceUrl->~StationURL();
        pead::FreeMemory(m_pSourceUrl);
        m_pSourceUrl = nullptr;
    }
    if (m_pNexAddress != nullptr) {
        m_pNexAddress->~InetAddress();
        pead::FreeMemory(m_pNexAddress);
        m_pNexAddress = nullptr;
    }
    if (m_pCallContext != nullptr) {
        m_pCallContext->~ProtocolCallContext();
        pead::FreeMemory(m_pCallContext);
        m_pCallContext = nullptr;
    }
}

} // namespace inet
} // namespace pia
} // namespace nn
