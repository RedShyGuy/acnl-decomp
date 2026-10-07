#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_String.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/inet/inet_NatPortDetecter.h"
#include "nn/pia/inet/inet_NatProbeData.h"
#include "nn/pia/inet/inet_NatRelayInterface.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_ProcessUpdateMeshJob.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
const u64 TRACE_FLAG = 0x8000ULL;
const u64 PROBE_TRACE_FLAG = 0x20000ULL;
const u64 PORT_TRACE_FLAG = 0x10000ULL;
const u64 LOCATION_TRACE_FLAG = 0x200ULL;
// the ports of the protocol: probes, replies and the dummy messages
const u16 PORT_PROBE = 1;
const u16 PORT_REPLY = 2;
const u16 PORT_DUMMY = 3;
// the keep-alive messages go to this port of the primary NAT check server
const u16 SERVER_KEEP_ALIVE_PORT = 33335;
const s32 SERVER_KEEP_ALIVE_INTERVAL_MSEC = 15000;
const s32 REQUEST_TIMEOUT_MSEC = 15000;
const s32 REQUEST_RETRY_TIMEOUT_MSEC = 5000;
const s32 SCHEDULE_DELAY_MSEC = 1000;
// probes go out twice, three times with a TTL of 4 to a public station that may not have opened
// its NAT yet
const u8 PROBE_SEND_NUM = 2;
const u8 PROBE_SEND_NUM_FIRST = 3;
const u8 PROBE_TTL_FIRST = 4;
const u32 PROBE_FIRST_SPRAY_COUNT_MAX = 10;
// the dummy messages that open the NAT for a probe request
const u8 DUMMY_TTL = 4;
const u8 DUMMY_NUM = 5;
const u8 SERVER_KEEP_ALIVE_NUM = 3;
// the debug shift of the probe port
const u16 PROBE_PORT_SHIFT = 10;
// the slots of the NAT traversals in the monitoring data
const u8 MONITORING_SLOT_NUM = 23;
// the URL type of the locations the dummy messages go to
const u8 URL_TYPE_DUMMY = 3;
// a probe with a NAT that maps endpoint dependent sprays over this port range
const u8 PORT_RANGE_EDM = 2;

// the debug flag of GetProbePortShiftFlag
// 0x00975A52
bool s_IsProbePortShifted;

// the time msec milliseconds from now (inline; name is ours)
inline common::Time GetTimeAfter(s32 msec)
{
    common::TimeSpan span(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
    common::Time now;
    now.SetNow();
    return now + span;
}

// the monitoring data is written while the mesh does not send it (inline; name is ours)
inline bool IsMonitoringWritable()
{
    return session::Mesh::s_pInstance == nullptr || !session::Mesh::s_pInstance->IsMonitoringDataSenderFlagSet();
}

// destroys all objects of the list, from the back (inline; name is ours)
template <typename T>
inline void DestroyAll(common::ObjList<T>& list)
{
    while (list.GetCount() != 0) {
        T* pObj = list.Back();
        pObj->~T();
        list.Erase(pObj);
    }
}

// the location with the debug shift of the port (inline; name is ours)
inline void ShiftProbePort(transport::StationLocation* pLocation, const transport::StationLocation& location)
{
    common::StationAddress address(location.m_StationAddress);
    common::InetAddress inetAddress(address.m_InetAddress);
    inetAddress.m_Port += PROBE_PORT_SHIFT;
    address.SetInetAddress(inetAddress);
    pLocation->SetStationAddress(address);
}
} // namespace

// the interval of updateNatTraversal (in .data; name is ours)
// 0x0097E41C
s32 g_NatTraversalUpdateIntervalMSec = 300;
// a probe of a connected station is kept this long (name is ours)
// 0x0097FA10
s32 g_NatProbeKeepTimeMSec = 90000;

// 0x003E4788 (name is ours)
nn::pia::inet::NatProbe* nn::pia::inet::NexNatTraversalProtocol::findProbe(const nn::pia::transport::StationLocation& location)
{
    return m_ProbeList.FindByConnectionId(location.m_StationKey);
}

// 0x003E70BC (name is ours)
bool* nn::pia::inet::NexNatTraversalProtocol::GetProbePortShiftFlag()
{
    return &s_IsProbePortShifted;
}

// 0x00405044 | fefates:callgraph [tier C]
void nn::pia::inet::NexNatTraversalProtocol::sendProbes()
{
    if (m_LocalCid == 0) {
        return;
    }
    for (NatProbeList::Node* pNode = m_ProbeList.Begin(); pNode != m_ProbeList.End();) {
        NatProbe* pProbe = &pNode->m_Value;
        common::Time now;
        now.SetNow();
        if (pProbe->m_Deadline < now && (pProbe->m_Rtt == NatProbe::RTT_NONE || pProbe->m_Unknown0x41)) {
            // the probe timed out
            pProbe->Trace(PROBE_TRACE_FLAG);
            m_TraversalTimeList.Remove(pProbe->m_Location);
            NexFacade::s_pInstance->m_pNatTraverser->ClearMonitoringNatTraversal(pProbe->m_Location);
            NatProbeList::Node* pNext = NatProbeList::Advance(pNode);
            pProbe->~NatProbe();
            m_ProbeList.Erase(pProbe);
            pNode = pNext;
            continue;
        }
        common::Time updateTime;
        updateTime.SetNow();
        if (pProbe->UpdateIsNeeded(updateTime) && (!m_IsRequesting || pProbe->m_SprayCount != 0)) {
            u8 sendNum = PROBE_SEND_NUM;
            u8 ttl = 0;
            if (pProbe->m_SprayCount <= PROBE_FIRST_SPRAY_COUNT_MAX && pProbe->m_UpdateTime.m_Tick == 0 &&
                pProbe->m_Location.m_ProbeRequestInitiation == 1 && NexFacade::IsPublic(pProbe->m_Location) &&
                pProbe->m_Location.m_StationAddress.m_InetAddress.m_Address != m_SelfLocation.m_StationAddress.m_InetAddress.m_Address) {
                sendNum = PROBE_SEND_NUM_FIRST;
                ttl = PROBE_TTL_FIRST;
            }
            // the traversal of a request begins with its first probe
            NatProbeRequest* pRequest = findProbeRequestByConnectionId(pProbe->m_Location.m_StationKey);
            if (pRequest != nullptr && pRequest->m_pCallContext != nullptr &&
                pRequest->m_pCallContext->GetState() != common::CallContext::STATE_CALL_IN_PROGRESS) {
                pRequest->m_pCallContext->InitiateCall();
                NatTraversalTime* pTime = m_TraversalTimeList.Find(pRequest->m_Location);
                if (pTime != nullptr) {
                    pTime->m_Unknown0x28.SetNow();
                    // (traces; armlink removed the calls)
                } else {
                    NatTraversalTime time;
                    time.m_Location = pRequest->m_Location;
                    time.m_Unknown0x28.SetNow();
                    time.m_Unknown0x30.SetNow();
                    time.m_Unknown0x38 = 0;
                    time.m_PortCheckMSec = 0;
                    m_TraversalTimeList.Add(time);
                }
            }
            if (pProbe->m_UpdateTime.m_Tick == 0) {
                pProbe->SprayTargetPort();
            }
            for (u8 i = 0; i < sendNum; i++) {
                common::Time sendTime;
                sendTime.SetNow();
                sendProbe(NatProbeData::TYPE_PROBE, pProbe->m_Location, sendTime, ttl);
            }
            pProbe->m_SprayCount++;
            if (NexFacade::IsEdmMapping(m_SelfLocation)) {
                // one probe at a time
                if (pProbe->m_SprayCount >= pProbe->m_SprayCountMax) {
                    m_IsRequesting = true;
                } else {
                    m_RequestStationKey = pProbe->m_Location.m_StationKey;
                }
                return;
            }
        }
        pNode = NatProbeList::Advance(pNode);
    }
}

// 0x004054A4 | fefates:bytes [tier B]
bool nn::pia::inet::NexNatTraversalProtocol::IsTraversed(const nn::pia::transport::StationLocation& location)
{
    // (a trace of the probe list; armlink removed the call)
    NatProbe* pProbe = m_ProbeList.FindByAddressAndConnectionId(location);
    if (pProbe == nullptr || pProbe->m_Rtt == NatProbe::RTT_NONE) {
        return false;
    }
    if (pProbe->m_Unknown0x41) {
        pProbe->m_Deadline = GetTimeAfter(g_NatProbeSetting.m_TimeoutMSec);
        pProbe->m_SprayCount = 0;
        pProbe->m_Unknown0x41 = false;
        location.Trace(PROBE_TRACE_FLAG);
    }
    return true;
}

// 0x00405570 | fefates:bytes [tier B]
bool nn::pia::inet::NexNatTraversalProtocol::IsTraversed(unsigned int stationKey)
{
    // (a trace of the probe list; armlink removed the call)
    NatProbe* pProbe = m_ProbeList.FindByConnectionId(stationKey);
    return pProbe != nullptr && pProbe->m_Rtt != NatProbe::RTT_NONE;
}

// 0x004055B0 | fefates:bytes [tier B]
bool nn::pia::inet::NexNatTraversalProtocol::RegisterRelay(nn::pia::inet::NatRelayInterface* pRelay)
{
    if (m_pRelay != nullptr) {
        return false;
    }
    m_pRelay = pRelay;
    pRelay->AssociateProtocol(this);
    m_NextUpdateTime = GetTimeAfter(g_NatTraversalUpdateIntervalMSec);
    return true;
}

// 0x0040563C (name is ours)
nn::Result nn::pia::inet::NexNatTraversalProtocol::updateNatTraversal(const nn::pia::common::Time& now)
{
    if (m_IsRequesting || m_IsInverseRequest || m_IsDirectRequest) {
        if (m_RequestDeadline < now && m_IsDirectRequest) {
            // the request timed out
            NatProbe* pProbe = m_ProbeList.FindByConnectionId(m_RequestStationKey);
            NatProbeRequest* pRequest = findProbeRequestByConnectionId(m_RequestStationKey);
            m_RequestStationKey = 0;
            m_IsDirectRequest = false;
            if (m_IsInverseRequest) {
                pProbe = m_ProbeList.FindByConnectionId(m_RequestStationKey);
                pRequest = findProbeRequestByConnectionId(m_RequestStationKey);
                m_RequestStationKey = 0;
                m_IsInverseRequest = false;
            }
            if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
                if (m_pPortDetecter != nullptr) {
                    m_pPortDetecter->CancelDetectionJob();
                }
                if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
                    m_CallContext.SignalCancel();
                }
            }
            if (pProbe != nullptr) {
                pProbe->m_Location.Trace(TRACE_FLAG);
                pProbe->m_Deadline = GetTimeAfter(0);
            }
            if (pRequest != nullptr) {
                pRequest->Trace(TRACE_FLAG);
                if (pRequest->m_pCallContext != nullptr) {
                    pRequest->m_pCallContext->SignalFailure(common::RESULT_STATION_CONNECTION_FAILED_F2);
                }
                pRequest->~NatProbeRequest();
                m_ScheduledProbeRequestList.Erase(pRequest);
            }
            m_IsRequesting = false;
        }
    }
    if (m_CallContext.IsFinished() && m_CallContext.m_Result.IsFailure()) {
        // the port check of the inverse traversal failed
        NatProbeRequest* pRequest = findProbeRequestByConnectionId(m_RequestLocation.m_StationKey);
        if (pRequest != nullptr) {
            if (pRequest->m_pCallContext != nullptr) {
                pRequest->m_pCallContext->SignalFailure(common::RESULT_NAT_CHECK_FAILED);
            }
            eraseProbeRequestByConnectionId(m_RequestLocation.m_StationKey);
        }
        NatProbe* pProbe = m_ProbeList.FindByConnectionId(m_RequestLocation.m_StationKey);
        if (pProbe != nullptr) {
            pProbe->m_Deadline = GetTimeAfter(0);
        }
        m_RequestLocation = transport::StationLocation();
        m_CallContext.Reset();
        // (traces; armlink removed the calls)
    } else if (m_CallContext.IsFinished() && m_CallContext.m_Result.IsSuccess()) {
        if (m_Unknown0x4C3) {
            // the port check succeeded: dummy messages open the NAT for the peer
            u16 port = getPeer2PeerPort(m_RequestLocation);
            if (port != 0) {
                transport::StationLocation location(m_RequestLocation);
                location.m_StationAddress.m_InetAddress.m_Port = port;
                location.m_UrlType = URL_TYPE_DUMMY;
                sendDummyPacket(location, DUMMY_TTL, DUMMY_NUM);
            }
            m_Unknown0x4C3 = false;
            u8 index = NexFacade::s_pInstance->m_pNatTraverser->FindMonitoringNatTraversalIndex(m_RequestLocation);
            if (index < MONITORING_SLOT_NUM) {
                common::g_SessionBeginMonitoringContent.m_Unknown0x485[index] = port != 0;
            }
            if (m_IsPortUnknown) {
                m_IsPortUnknown = false;
                m_RequestLocation = transport::StationLocation();
                m_CallContext.Reset();
                m_NextUpdateTime = common::Time();
                return nn::Result();
            }
        }
        m_CallContext.Reset();
    }
    while (sendProbeRequests()) {
    }
    if (m_CallContext.GetState() != common::CallContext::STATE_CALL_IN_PROGRESS) {
        sendProbes();
    }
    return nn::Result();
}

// 0x00405A64 | fefates:bytes [tier B]
bool nn::pia::inet::NexNatTraversalProtocol::UnregisterRelay()
{
    if (m_pRelay == nullptr) {
        return false;
    }
    m_pRelay->AssociateProtocol(nullptr);
    m_pRelay = nullptr;
    return true;
}

// 0x00405A98 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::addProbeRequest(const nn::pia::inet::NatProbeRequest& request)
{
    if (request.m_Location.m_StationKey == 0) {
        request.Trace(LOCATION_TRACE_FLAG);
        return;
    }
    request.Trace(TRACE_FLAG);
    NatProbeRequest* pRequest = m_ProbeRequestList.PushBackNew();
    if (pRequest != nullptr) {
        *pRequest = request;
    }
    if (request.m_IsInverse) {
        pRequest = m_ScheduledProbeRequestList.PushBackNew();
        if (pRequest != nullptr) {
            *pRequest = request;
        }
    }
    m_NextUpdateTime = common::Time();
}

// 0x00405C54 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::sendDummyPacket(const nn::pia::transport::StationLocation& location, unsigned char ttl, unsigned char num)
{
    if (!location.m_StationAddress.m_InetAddress.IsValid()) {
        location.Trace(LOCATION_TRACE_FLAG);
        return;
    }
    common::String message("Dummy");
    for (u32 i = 0; i < num; i++) {
        transport::ProtocolId protocolId(transport::PROTOCOL_TYPE_NAT, PORT_DUMMY);
        transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(protocolId, location.m_StationAddress, message.StrLen(), true);
        if (common::IsValidPointer(pWriter)) {
            if (ttl != 0) {
                pWriter->m_Ttl = ttl;
            }
            pWriter->SetPayload(message.CStr());
            m_pPacketHandler->Commit();
        }
    }
}

// 0x00405D54 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::ResetProbeStatus(const nn::pia::transport::StationLocation& location, bool isAddressOnly)
{
    NatProbe* pProbe;
    if (isAddressOnly) {
        pProbe = m_ProbeList.FindByAddressAndConnectionId(location);
        if (pProbe == nullptr || pProbe->m_Rtt == NatProbe::RTT_NONE) {
            return;
        }
    } else {
        pProbe = m_ProbeList.FindByAddressPortAndConnectionId(location);
        if (pProbe == nullptr || pProbe->m_Rtt == NatProbe::RTT_NONE) {
            pProbe = m_ProbeList.FindByAddressAndConnectionId(location);
            if (pProbe == nullptr) {
                return;
            }
        }
    }
    pProbe->m_Deadline = GetTimeAfter(g_NatProbeSetting.m_TimeoutMSec);
    pProbe->m_SprayCount = 0;
    pProbe->m_Unknown0x41 = false;
    location.Trace(PROBE_TRACE_FLAG);
}

// 0x00405E3C | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::SendProbeRequest(const nn::pia::transport::StationLocation& target, const nn::pia::transport::StationLocation& self)
{
    if (common::IsValidPointer(m_pRelay)) {
        m_pRelay->RelayProbeRequest(target, self);
    }
}

// 0x00405E64 | fefates:bytes [tier B]
u16 nn::pia::inet::NexNatTraversalProtocol::getPeer2PeerPort(const nn::pia::transport::StationLocation& location)
{
    session::ProcessUpdateMeshJob* pJob = session::Mesh::s_pInstance->m_pProcessUpdateMeshJob;
    if (pJob == nullptr) {
        return 0;
    }
    if (!pJob->m_IsProcessing) {
        // the host knows its own ports
        const transport::StationConnectionInfo& host = transport::StationConnectionInfoTable::s_pInstance->m_HostInfo;
        if (!(host.m_PublicLocation == location)) {
            return 0;
        }
        host.Trace(PORT_TRACE_FLAG);
        if (NexFacade::IsGlobal(host.m_PublicLocation)) {
            return host.m_PublicLocation.m_StationAddress.m_InetAddress.m_Port;
        }
        if (!NexFacade::IsEimMapping(host.m_PrivateLocation)) {
            return 0;
        }
        u16 port = host.m_PrivateLocation.m_StationAddress.m_InetAddress.m_Port;
        if (host.m_PublicLocation.m_StationAddress.m_InetAddress.m_Port == port) {
            return 0;
        }
        return port;
    }
    location.Trace(PORT_TRACE_FLAG);
    u32 num = pJob->m_StationNum;
    for (u32 i = 0; i < num; i++) {
        transport::StationConnectionInfo* pInfo = i < pJob->m_StationNum ? &pJob->m_pStationConnectionInfos[i] : nullptr;
        if (pInfo == nullptr) {
            continue;
        }
        if (pInfo->m_PublicLocation == location) {
            pInfo->Trace(PORT_TRACE_FLAG);
            if (NexFacade::IsEdmMapping(pInfo->m_PublicLocation)) {
                return 0;
            }
            return pInfo->m_PrivateLocation.m_StationAddress.m_InetAddress.m_Port;
        }
        if (location.m_ProbeRequestInitiation == 0) {
            continue;
        }
        if (NexFacade::IsEdmMapping(pInfo->m_PublicLocation)) {
            transport::StationLocation candidate(pInfo->m_PublicLocation);
            candidate.m_ProbeRequestInitiation = 1;
            candidate.m_StationAddress.m_InetAddress.m_Port = location.m_StationAddress.m_InetAddress.m_Port;
            if (candidate == location) {
                pInfo->Trace(PORT_TRACE_FLAG);
                return 0;
            }
        } else {
            transport::StationLocation candidate(pInfo->m_PublicLocation);
            candidate.m_ProbeRequestInitiation = 1;
            candidate.m_StationAddress.m_InetAddress.m_Port = pInfo->m_PrivateLocation.m_StationAddress.m_InetAddress.m_Port;
            if (candidate == location) {
                pInfo->Trace(PORT_TRACE_FLAG);
                return candidate.m_StationAddress.m_InetAddress.m_Port;
            }
        }
    }
    m_IsPortUnknown = true;
    return 0;
}

// 0x00406098 | fefates:callgraph [tier C]
void nn::pia::inet::NexNatTraversalProtocol::sendProbeRequest(nn::pia::transport::StationLocation self, const nn::pia::transport::StationLocation& target)
{
    bool isInverse = target.m_ProbeRequestInitiation == 0;
    // (a trace of self; armlink removed the call)
    target.Trace(TRACE_FLAG);
    self.m_StationKey = m_LocalCid;
    self.m_ProbeRequestInitiation = isInverse;
    NatTraverser* pNatTraverser;
    if (NexFacade::IsPublic(self) && NexFacade::IsEdmMapping(m_SelfLocation)) {
        // the own NAT maps endpoint dependent: the port check predicts the port first
        if (isInverse) {
            m_RequestStationKey = target.m_StationKey;
            m_IsInverseRequest = true;
            m_IsRequesting = true;
            m_RequestDeadline = GetTimeAfter(REQUEST_TIMEOUT_MSEC);
        } else {
            m_IsDirectRequest = true;
            m_RequestStationKey = target.m_StationKey;
            m_RequestDeadline = GetTimeAfter(REQUEST_TIMEOUT_MSEC);
        }
        if (NexFacade::s_pInstance->m_pNatTraverser->m_NatProperty.m_IsPortPreserved) {
            transport::StationLocation location(target);
            location.m_StationAddress.m_InetAddress.m_Port = SERVER_KEEP_ALIVE_PORT;
            location.m_UrlType = URL_TYPE_DUMMY;
            sendDummyPacket(location, DUMMY_TTL, DUMMY_NUM);
        }
        if (IsMonitoringWritable()) {
            common::g_SessionBeginMonitoringContent.m_Unknown0x272++;
        }
        common::g_SessionStateMonitoringContent.m_Unknown0x3DC++;
        m_CallContext.RegisterCallback(OnPortDetectedCallback, this);
        common::InetAddress localAddress;
        nn::Result result = m_pPortDetecter->Startup(&m_CallContext, localAddress, target, self, this, true);
        if (result.IsFailure()) {
            m_CallContext.SignalFailure(result);
        }
        m_pPortDetecter->StartDetectionJob();
        m_RequestLocation = target;
        return;
    }
    // the request goes to the relay with the port of the own NAT
    u16 port = 0;
    pNatTraverser = NexFacade::s_pInstance->m_pNatTraverser;
    if (NexFacade::IsPublic(self)) {
        port = pNatTraverser->m_NatProperty.m_PublicPort;
    } else if (common::IsValidPointer(pNatTraverser)) {
        port = pNatTraverser->m_LocalPort;
    }
    self.m_StationAddress.m_InetAddress.m_Port = port;
    if (common::IsValidPointer(m_pRelay)) {
        m_pRelay->RelayProbeRequest(target, self);
    }
}

// 0x00406378 (name is ours)
void nn::pia::inet::NexNatTraversalProtocol::ClearNatTraversal(const nn::pia::transport::StationLocation& location, bool isConnected)
{
    if (eraseProbeRequestByConnectionId(location.m_StationKey) && m_RequestStationKey == location.m_StationKey) {
        if (m_IsInverseRequest) {
            m_RequestStationKey = 0;
            m_IsInverseRequest = false;
            m_IsRequesting = false;
        }
        if (m_IsDirectRequest) {
            m_RequestStationKey = 0;
            m_IsDirectRequest = false;
            m_IsRequesting = false;
        }
        if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            if (m_pPortDetecter != nullptr) {
                m_pPortDetecter->CancelDetectionJob();
            }
            if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
                m_CallContext.SignalCancel();
            }
        }
        m_IsPortUnknown = false;
    }
    if (isConnected) {
        m_ProbeList.RemoveSameAddressPortAndConnectionIdProbes(location);
        m_TraversalTimeList.Remove(location);
        return;
    }
    NatProbe* pProbe = m_ProbeList.FindByConnectionId(location.m_StationKey);
    if (pProbe != nullptr && pProbe->m_Rtt != NatProbe::RTT_NONE) {
        // the probe stays for a while
        pProbe->m_Unknown0x41 = true;
        pProbe->m_SprayCount = 1;
        pProbe->m_Deadline = GetTimeAfter(g_NatProbeKeepTimeMSec);
        pProbe->Trace(TRACE_FLAG);
        return;
    }
    m_ProbeList.RemoveProbesByConnectionId(location.m_StationKey);
    m_TraversalTimeList.Remove(location);
}

// 0x00406508 | fefates:bytes [tier B]
bool nn::pia::inet::NexNatTraversalProtocol::sendProbeRequests()
{
    if (NexFacade::IsEdmMapping(m_SelfLocation) &&
        (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS || m_IsInverseRequest || m_IsDirectRequest)) {
        return false;
    }
    if (m_ProbeRequestList.GetCount() == 0) {
        return false;
    }
    NatProbeRequest request(*m_ProbeRequestList.Front());
    common::Time now;
    now.SetNow();
    if (request.m_Unknown0x88.m_Tick != 0 && now < request.m_Unknown0x88) {
        // (a trace of the request; armlink removed the call)
        return false;
    }
    bool isPrivateSent = false;
    const transport::StationConnectionInfo& local = transport::StationConnectionInfoTable::s_pInstance->m_LocalInfo;
    if (request.m_IsInverse ? request.m_ConnectionInfo.m_PublicLocation.m_StationAddress.m_InetAddress.m_Address ==
                                  local.m_PublicLocation.m_StationAddress.m_InetAddress.m_Address
                            : !NexFacade::IsPublic(request.m_Location)) {
        // behind the same NAT (or the target is not public): the private location
        isPrivateSent = true;
        sendProbeRequest(local.m_PrivateLocation, request.m_Location);
        if (request.m_IsInverse && !NexFacade::IsEdmMapping(m_SelfLocation) && !NexFacade::IsEdmMapping(request.m_Location)) {
            sendProbeRequest(local.m_PublicLocation, request.m_Location);
        }
    } else {
        sendProbeRequest(local.m_PublicLocation, request.m_Location);
    }
    NatProbeRequest* pFront = m_ProbeRequestList.Front();
    pFront->~NatProbeRequest();
    m_ProbeRequestList.Erase(pFront);
    if (!isPrivateSent && NexFacade::IsEdmMapping(request.m_Location) && !NexFacade::IsEdmMapping(m_SelfLocation)) {
        // the peer predicts its port first
        scheduleProbeRequest(request);
    }
    return m_ProbeRequestList.GetCount() != 0 && !m_IsRequesting;
}

// 0x004067F0 | fefates:bytes-fuzzy [tier B]
void nn::pia::inet::NexNatTraversalProtocol::CleanupNatTraversal()
{
    DestroyAll(m_ProbeList);
    DestroyAll(m_ProbeRequestList);
    DestroyAll(m_ScheduledProbeRequestList);
    DestroyAll(m_TraversalTimeList);
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        if (m_pPortDetecter != nullptr) {
            m_pPortDetecter->CancelDetectionJob();
        }
        if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_CallContext.SignalCancel();
        }
    }
    m_CallContext.Reset();
    m_IsStarted = false;
    m_IsRequesting = false;
    m_IsInverseRequest = false;
    m_IsDirectRequest = false;
    m_Unknown0x4C3 = false;
    m_IsPortUnknown = false;
    m_Unknown0x4C5 = false;
    m_RequestStationKey = 0;
    m_IsServerKeepAlive = false;
    m_pPortDetecter = nullptr;
}

// 0x00406A40 (name is ours)
bool nn::pia::inet::NexNatTraversalProtocol::startNatTraversal(const nn::pia::transport::StationLocation& location, const nn::pia::transport::StationConnectionInfo& info, nn::pia::common::CallContext* pCallContext)
{
    if (m_pRelay == nullptr || location.m_StationKey == 0) {
        return false;
    }
    transport::StationLocation target(location);
    if (*GetProbePortShiftFlag()) {
        ShiftProbePort(&target, location);
    }
    NatProbeRequest request(target, info, true, pCallContext);
    // (traces of the request and the probe list; armlink removed the calls)
    bool isPortKnown = false;
    NatProbe* pProbe = m_ProbeList.FindByAddressPortAndConnectionId(location);
    if (pProbe != nullptr && pProbe->m_Rtt != NatProbe::RTT_NONE) {
        isPortKnown = true;
    }
    pProbe = m_ProbeList.FindByAddressAndConnectionId(location);
    if ((pProbe == nullptr || !pProbe->m_Unknown0x41) && !isPortKnown) {
        addProbeRequest(request);
        return true;
    }
    // a probe reached the station before: it starts again
    pProbe->m_Deadline = GetTimeAfter(g_NatProbeSetting.m_TimeoutMSec);
    pProbe->m_SprayCount = 0;
    pProbe->m_Unknown0x41 = false;
    location.Trace(PROBE_TRACE_FLAG);
    return false;
}

// 0x00406C44 (name is ours)
void nn::pia::inet::NexNatTraversalProtocol::StartProbe(const nn::pia::transport::StationLocation& location)
{
    if (!m_IsStarted || m_Unknown0x4C5) {
        return;
    }
    if (!location.m_StationAddress.m_InetAddress.IsValid()) {
        location.Trace(1);
        return;
    }
    bool isRequested = false;
    if (location.m_ProbeRequestInitiation != 0) {
        // the peer asks for a probe request back
        NatProbeRequest request(location);
        addProbeRequest(request);
        NatTraversalTime* pTime = m_TraversalTimeList.Find(location);
        if (pTime != nullptr) {
            pTime->m_Unknown0x28.SetNow();
            pTime->m_Unknown0x30.SetNow();
            pTime->m_Unknown0x38 = 0;
        } else {
            NatTraversalTime time;
            time.m_Location = location;
            time.m_Unknown0x28.SetNow();
            time.m_Unknown0x30.SetNow();
            time.m_Unknown0x38 = 0;
            time.m_PortCheckMSec = 0;
            m_TraversalTimeList.Add(time);
        }
        // (traces; armlink removed the calls)
    } else {
        if (NexFacade::IsEdmMapping(m_SelfLocation) && m_IsRequesting && m_RequestStationKey == location.m_StationKey) {
            isRequested = true;
            m_IsRequesting = false;
        }
        NatTraversalTime* pTime = m_TraversalTimeList.Find(location);
        if (pTime != nullptr) {
            common::Time now;
            now.SetNow();
            pTime->m_Unknown0x38 = (now - pTime->m_Unknown0x30).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
        }
    }
    u8 portRange = NexFacade::IsEdmMapping(location) ? PORT_RANGE_EDM : 0;
    common::TimeSpan timeout(common::TimeSpan::GetTicksPerMSec().GetTick() * g_NatProbeSetting.m_TimeoutMSec);
    common::Time time;
    NatProbe probe(location, time, timeout, 0, g_NatProbeSetting.m_SprayCountMax, portRange);
    if (isRequested) {
        m_RequestDeadline = GetTimeAfter(REQUEST_RETRY_TIMEOUT_MSEC);
        probe.m_Unknown0x40 = true;
        // (a trace of the probe; armlink removed the call)
    }
    m_ProbeList.AddProbe(probe);
}

// 0x00406F28 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::ReportNatProperties(const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt)
{
    if (m_pRelay != nullptr) {
        m_pRelay->ReportNatProperties(mapping, filtering, rtt);
    }
}

// 0x00406F44 | fefates:callgraph [tier C]
nn::Result nn::pia::inet::NexNatTraversalProtocol::StartupNatTraversal(nn::pia::inet::NatPortDetecter* pPortDetecter)
{
    common::SimpleContainer<common::InetAddress, 4>& servers = NexFacade::s_pInstance->m_pNatTraverser->m_ServerAddresses;
    if (servers.Begin() == servers.End()) {
        return common::RESULT_INVALID_STATE;
    }
    common::InetAddress address(*servers.Begin());
    address.m_Port = SERVER_KEEP_ALIVE_PORT;
    m_ServerLocation.m_StationAddress.SetInetAddress(address);
    m_ServerLocation.m_UrlType = URL_TYPE_DUMMY;
    // (a trace of the location; armlink removed the call)
    if (!m_ServerLocation.m_StationAddress.m_InetAddress.IsValid()) {
        // (a trace of the location; armlink removed the call)
        return common::RESULT_INVALID_STATE;
    }
    m_IsStarted = true;
    m_IsRequesting = false;
    m_IsInverseRequest = false;
    m_IsDirectRequest = false;
    m_Unknown0x4C3 = false;
    m_IsPortUnknown = false;
    m_Unknown0x4C5 = false;
    m_RequestStationKey = 0;
    m_IsServerKeepAlive = false;
    m_pPortDetecter = pPortDetecter;
    return nn::Result();
}

// 0x00407044 (name is ours)
void nn::pia::inet::NexNatTraversalProtocol::StopServerKeepAlive()
{
    m_IsServerKeepAlive = false;
}

// 0x00407050 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::StartServerKeepAlive()
{
    m_IsServerKeepAlive = true;
    common::Time now;
    now.SetNow();
    m_ServerKeepAliveTime = now;
}

// 0x00407090 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::scheduleProbeRequest(const nn::pia::inet::NatProbeRequest& request)
{
    for (NatProbeRequestList::Node* pNode = m_ProbeRequestList.Begin(); pNode != m_ProbeRequestList.End(); pNode = NatProbeRequestList::Advance(pNode)) {
        NatProbeRequest& scheduled = pNode->m_Value;
        if (NexFacade::IsEdmMapping(scheduled.m_Location) &&
            scheduled.m_Location.m_StationAddress.m_InetAddress.m_Address == request.m_Location.m_StationAddress.m_InetAddress.m_Address) {
            // a request to the same NAT waits a moment
            scheduled.m_Unknown0x88 = GetTimeAfter(SCHEDULE_DELAY_MSEC);
            request.Trace(TRACE_FLAG);
            scheduled.Trace(TRACE_FLAG);
            return;
        }
    }
}

// 0x0040716C | fefates:bytes [tier B]
bool nn::pia::inet::NexNatTraversalProtocol::GetLatestStationLocation(const nn::pia::transport::StationLocation& location, nn::pia::transport::StationLocation* pLatest)
{
    NatProbe* pProbe = m_ProbeList.FindByConnectionId(location.m_StationKey);
    if (pProbe == nullptr || pProbe->m_Location == location) {
        return false;
    }
    *pLatest = pProbe->m_Location;
    return true;
}

// 0x004071BC | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::ReportNatTraversalResult(const nn::pia::transport::StationLocation& location, bool isSucceeded)
{
    u32 stationKey = location.m_StationKey;
    if (m_pRelay != nullptr) {
        u32 rtt = 0xFFFFFFFF;
        NatProbe* pProbe = m_ProbeList.FindByConnectionId(stationKey);
        if (pProbe != nullptr) {
            rtt = pProbe->m_Rtt;
        }
        m_pRelay->ReportNatTraversalResult(stationKey, isSucceeded, rtt);
    }
    m_NextUpdateTime = common::Time();
}

// 0x0040722C (name is ours)
void nn::pia::inet::NexNatTraversalProtocol::SetLocalCid(unsigned int cid)
{
    m_LocalCid = cid;
    common::g_SessionBeginMonitoringContent.m_Unknown0x50 = cid;
}

// 0x00407240 | fefates:bytes [tier B]
nn::pia::inet::NatProbeRequest* nn::pia::inet::NexNatTraversalProtocol::findProbeRequestByConnectionId(unsigned int stationKey)
{
    for (NatProbeRequestList::Node* pNode = m_ScheduledProbeRequestList.Begin(); pNode != m_ScheduledProbeRequestList.End();
         pNode = NatProbeRequestList::Advance(pNode)) {
        if (pNode->m_Value.m_Location.m_StationKey == stationKey) {
            return &pNode->m_Value;
        }
    }
    return nullptr;
}

// 0x0040727C | fefates:bytes [tier B]
bool nn::pia::inet::NexNatTraversalProtocol::eraseProbeRequestByConnectionId(unsigned int stationKey)
{
    bool isErased = false;
    for (NatProbeRequestList::Node* pNode = m_ScheduledProbeRequestList.Begin(); pNode != m_ScheduledProbeRequestList.End();) {
        NatProbeRequestList::Node* pNext = NatProbeRequestList::Advance(pNode);
        NatProbeRequest* pRequest = &pNode->m_Value;
        if (pRequest->m_Location.m_StationKey == stationKey) {
            pRequest->~NatProbeRequest();
            m_ScheduledProbeRequestList.Erase(pRequest);
            isErased = true;
        }
        pNode = pNext;
    }
    for (NatProbeRequestList::Node* pNode = m_ProbeRequestList.Begin(); pNode != m_ProbeRequestList.End();) {
        NatProbeRequestList::Node* pNext = NatProbeRequestList::Advance(pNode);
        NatProbeRequest* pRequest = &pNode->m_Value;
        if (pRequest->m_Location.m_StationKey == stationKey) {
            pRequest->~NatProbeRequest();
            m_ProbeRequestList.Erase(pRequest);
            isErased = true;
        }
        pNode = pNext;
    }
    return isErased;
}

// 0x00407374 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
nn::Result nn::pia::inet::NexNatTraversalProtocol::Dispatch()
{
    if (!NexFacade::s_pInstance->m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    transport::PacketHandler::Iterator* pIterator = m_pPacketHandler->GetIterator(transport::PROTOCOL_TYPE_NAT);
    pIterator->m_pPacketHandler->BeginIteration();
    while (!pIterator->m_pPacketHandler->IsEndIteration()) {
        const transport::ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        pReader->GetProtocolIdPort();
        if (pReader->m_PayloadSize == NatProbeData::SERIALIZED_SIZE) {
            NatProbeData data;
            if (data.Deserialize(pReader->GetPayload(), NatProbeData::SERIALIZED_SIZE).IsSuccess()) {
                // (a trace of the data; armlink removed the call)
                if (data.m_StationKey != 0) {
                    transport::StationLocation location;
                    location.SetStationAddress(pReader->m_SourceAddress);
                    location.m_StationKey = data.m_StationKey;
                    // (a trace of the location; armlink removed the call)
                    if (!location.m_StationAddress.m_InetAddress.IsValid()) {
                        // (traces; armlink removed the calls)
                    } else if (data.m_Type == NatProbeData::TYPE_PROBE) {
                        if (m_ProbeList.ProcessProbe(location, pReader->m_Ttl) && m_LocalCid != 0) {
                            common::Time sendTime(data.m_SendTime);
                            sendProbe(NatProbeData::TYPE_REPLY, location, sendTime, 0);
                            // (traces; armlink removed the calls)
                        }
                    } else if (data.m_Type == NatProbeData::TYPE_REPLY) {
                        m_ProbeList.ProcessProbeReply(location, common::Time(data.m_SendTime), pReader->m_Ttl);
                        if (m_RequestStationKey == data.m_StationKey) {
                            // the request reached the station
                            eraseProbeRequestByConnectionId(data.m_StationKey);
                            m_IsInverseRequest = false;
                            m_IsDirectRequest = false;
                            m_IsRequesting = false;
                            m_RequestStationKey = 0;
                        }
                    }
                }
            }
        }
        pIterator->m_pPacketHandler->NextIteration();
    }
    common::Time now;
    now.SetNow();
    if (m_NextUpdateTime >= now) {
        return nn::Result();
    }
    m_NextUpdateTime = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * g_NatTraversalUpdateIntervalMSec);
    if (m_IsServerKeepAlive && m_ServerKeepAliveTime < now) {
        sendDummyPacket(m_ServerLocation, 0, SERVER_KEEP_ALIVE_NUM);
        m_ServerKeepAliveTime = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * SERVER_KEEP_ALIVE_INTERVAL_MSEC);
    }
    return updateNatTraversal(now);
}

// 0x00407680 (name is ours)
void nn::pia::inet::NexNatTraversalProtocol::sendProbe(u8 type, const nn::pia::transport::StationLocation& location, const nn::pia::common::Time& sendTime, u8 ttl)
{
    NatProbeData data;
    data.m_StationKey = m_LocalCid;
    data.m_Type = type;
    data.m_SendTime = sendTime.m_Tick;
    // (a trace of the data; armlink removed the call)
    location.Trace(TRACE_FLAG);
    u8 buffer[NatProbeData::SERIALIZED_SIZE];
    u32 size;
    u16 port = data.m_Type == NatProbeData::TYPE_REPLY ? PORT_REPLY : PORT_PROBE;
    if (data.Serialize(buffer, &size, sizeof(buffer)).IsFailure()) {
        return;
    }
    transport::StationLocation target(location);
    if (*GetProbePortShiftFlag()) {
        ShiftProbePort(&target, location);
    }
    transport::ProtocolId protocolId(transport::PROTOCOL_TYPE_NAT, port);
    transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(protocolId, target.m_StationAddress, NatProbeData::SERIALIZED_SIZE, true);
    if (common::IsValidPointer(pWriter)) {
        if (ttl != 0) {
            pWriter->m_Ttl = ttl;
        }
        pWriter->SetPayload(buffer);
        m_pPacketHandler->Commit();
    }
}

// 0x00407808 | fefates:bytes [tier B]
nn::pia::inet::NexNatTraversalProtocol::NexNatTraversalProtocol()
    : m_IsStarted(false), m_LocalCid(0), m_pRelay(nullptr), m_NextUpdateTime(), m_RequestDeadline(), m_IsRequesting(false),
      m_IsInverseRequest(false), m_IsDirectRequest(false), m_Unknown0x4C3(false), m_IsPortUnknown(false), m_Unknown0x4C5(false),
      m_RequestStationKey(0), m_IsServerKeepAlive(false), m_ServerKeepAliveTime(), m_pPortDetecter(nullptr)
{
    m_ProbeList.ClearNodes();
    m_ProbeRequestList.ClearNodes();
    m_ScheduledProbeRequestList.ClearNodes();
    m_TraversalTimeList.ClearNodes();
}

// 0x00407AF0 | fefates:bytes
// 0x00407AE0 (deleting dtor)
nn::pia::inet::NexNatTraversalProtocol::~NexNatTraversalProtocol()
{
    DestroyAll(m_ProbeList);
    DestroyAll(m_ProbeRequestList);
    DestroyAll(m_ScheduledProbeRequestList);
    DestroyAll(m_TraversalTimeList);
    if (NexFacade::s_pInstance != nullptr) {
        NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol = nullptr;
    }
}

// 0x0072EFE4 slot 0x08
void nn::pia::inet::NexNatTraversalProtocol::Trace(u64 flag) const
{
    m_ProbeList.Trace(flag);
}

// 0x0072F530 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
u16 nn::pia::inet::NexNatTraversalProtocol::GetProtocolType() const
{
    return transport::PROTOCOL_TYPE_NAT;
}

// 0x0072F538 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol
bool nn::pia::inet::NexNatTraversalProtocol::IsEnableProtocolFiltering() const
{
    return false;
}

// 0x00412318 (name is ours)
void nn::pia::inet::NexNatTraversalProtocol::OnPortDetectedCallback(nn::Result, void* pArg)
{
    NexNatTraversalProtocol* pProtocol = static_cast<NexNatTraversalProtocol*>(pArg);
    pProtocol->m_Unknown0x4C3 = true;
    pProtocol->m_NextUpdateTime = common::Time();
    if (pProtocol->m_IsDirectRequest) {
        pProtocol->m_RequestDeadline = GetTimeAfter(REQUEST_RETRY_TIMEOUT_MSEC);
    }
}

} // namespace inet
} // namespace pia
} // namespace nn
