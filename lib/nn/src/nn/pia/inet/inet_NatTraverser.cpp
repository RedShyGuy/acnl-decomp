#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_Globals.h"
#include "nn/nex/nex_MatchMakingClient.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_Time.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"
#include "nn/pia/inet/inet_SocketAddress.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationLocation.h"
#include "nn/pia/transport/transport_Transport.h"
#include "pead/peadHeapMgr.h"
#include <new>

namespace nn {
namespace pia {
namespace inet {
// the host names of the two NAT check servers (in .data, the functions load them; name is ours)
// 0x0097E408
const char* g_NatServerHostNames[2] = {"nncs1.app.nintendowifi.net", "nncs2.app.nintendowifi.net"};

namespace {
const u64 TRACE_FLAG = 0x8000ULL;
const u64 LOCATION_TRACE_FLAG = 0x200ULL;
// the slots of the NAT traversals in the monitoring data
const u8 MONITORING_SLOT_NUM = 23;
const u8 INVALID_MONITORING_SLOT = 255;
const u32 INVALID_STATION_KEY = 0xFFFFFFFF;
// stopNatSession waits for the resolution in steps of this time, at most this many steps
const u32 STOP_WAIT_MSEC = 5;
const s32 STOP_WAIT_NUM_MAX = 15000;
// the timeout of the nex call that replaces the station URL
const u32 REPLACE_URL_TIMEOUT_MSEC = 10000;
// the values of the NAT properties
const u8 NAT_MAPPING_EIM = 1;
const u8 NAT_MAPPING_EDM = 2;
// the bit of the station URL type in the monitoring data for a NAT that preserves ports
const u8 MONITORING_PORT_PRESERVED = 0x80;

// the counters of the monitoring data wrap around to 1 (inline; name is ours)
inline void IncrementCounter(u16& counter)
{
    counter = counter == 0xFFFF ? 1 : counter + 1;
}

// the monitoring data is written while the mesh does not send it (inline; name is ours)
inline bool IsMonitoringWritable()
{
    return session::Mesh::s_pInstance == nullptr || !session::Mesh::s_pInstance->IsMonitoringDataSenderFlagSet();
}

// their addresses (resolved once)
// 0x00AE74E0
SocketAddress s_NatServerAddress0;
// 0x00AE7504
SocketAddress s_NatServerAddress1;

// adds a server address; RESULT_BUFFER_IS_FULL if there are 4 (inline; name is ours)
inline nn::Result AddServerAddress(common::SimpleContainer<common::InetAddress, 4>& servers, const common::InetAddress& address)
{
    if (servers.IsFull()) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    servers.PushBack(address);
    return nn::Result();
}

// the root transport of nex (inline; name is ours)
inline nex::RootTransport* GetNexRootTransport()
{
    nex::Network* pNetwork = nex::Network::GetInstance();
    return pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
}
} // namespace

// 0x003E5124 | fefates:callgraph [tier C]
void nn::pia::inet::NatTraverser::stopNatSession()
{
    if (m_State == STATE_NONE) {
        return;
    }
    if (!NexFacade::s_IsNatSessionSkipped) {
        if (m_State == STATE_RESOLVING_SERVER_ADDRESS) {
            m_ServerAddressResolveJob.m_IsCancelRequested = true;
            // the resolution stops at the latest at its deadline
            common::Time now;
            now.SetNow();
            s64 remainingMSec = (m_ServerAddressResolveJob.m_Deadline - now).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
            s32 waitNum = (remainingMSec + 1) / STOP_WAIT_MSEC;
            if (waitNum < 1) {
                waitNum = 1;
            } else if (waitNum > STOP_WAIT_NUM_MAX) {
                waitNum = STOP_WAIT_NUM_MAX;
            }
            for (s32 i = 0; i < waitNum; i++) {
                m_ServerAddressResolveJob.WaitForCompletion(STOP_WAIT_MSEC);
                if (!m_ServerAddressResolveJob.IsRunning()) {
                    break;
                }
            }
        }
        if (m_State == STATE_DETECTING_PROPERTY) {
            m_PropertyDetecter.CancelDetectionJob();
        }
        if (m_State == STATE_DETECTING_PORT && m_pProtocol != nullptr) {
            m_PortDetecter.CancelDetectionJob();
        }
        if (m_pProtocol != nullptr) {
            m_pProtocol->StopServerKeepAlive();
        }
        if (common::IsValidPointer(m_pCallContext)) {
            m_pCallContext->SignalCancel();
            m_pCallContext = nullptr;
        }
    }
    m_State = STATE_NONE;
}

// 0x003E526C | fefates:bytes [tier B]
nn::Result nn::pia::inet::NatTraverser::CreateProtocols()
{
    transport::Transport* pTransport = transport::Transport::s_pInstance;
    if (!common::IsValidPointer(pTransport)) {
        return common::RESULT_INVALID_STATE;
    }
    m_ProtocolId = pTransport->m_ProtocolManager.CreateProtocol<NexNatTraversalProtocol>(transport::PROTOCOL_TYPE_NAT, 0);
    m_pProtocol = pTransport->m_ProtocolManager.GetProtocol<NexNatTraversalProtocol>(m_ProtocolId, transport::PROTOCOL_TYPE_NAT);
    return nn::Result();
}

// 0x003E5310 (name is ours)
nn::Result nn::pia::inet::NatTraverser::startNatServerAddressResolve()
{
    if (m_State != STATE_NONE) {
        return common::RESULT_INVALID_STATE;
    }
    nex::Network* pNetwork = nex::Network::GetInstance();
    if (pNetwork == nullptr || pNetwork->m_pRootTransport == nullptr || !common::IsValidPointer(m_pCallContext) ||
        m_ServerAddressResolveJob.IsRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    m_State = STATE_RESOLVING_SERVER_ADDRESS;
    m_ServerAddressCallContext.RegisterCallback(OnServerAddressResolvedCallback, this);
    nn::Result result = m_ServerAddressResolveJob.Startup(&m_ServerAddressCallContext, this);
    if (result.IsFailure()) {
        return result;
    }
    m_ServerAddressResolveJob.Ready(true);
    return nn::Result();
}

// 0x003E53C0 | fefates:callgraph [tier C]
void nn::pia::inet::NatTraverser::ClearNatTraversal(const nn::pia::transport::StationLocation& location, bool isConnected)
{
    if (location.m_StationKey != 0) {
        m_pProtocol->ClearNatTraversal(location, isConnected);
        return;
    }
    location.Trace(1);
}

// 0x003E53F8 | fefates:bytes [tier B]
bool nn::pia::inet::NatTraverser::StartNatTraversal()
{
    if (common::IsValidPointer(m_pProtocol)) {
        return m_pProtocol->StartupNatTraversal(&m_PortDetecter).IsSuccess();
    }
    return false;
}

// 0x00406A00 (name is ours)
bool nn::pia::inet::NatTraverser::RequestNatTraversal(const nn::pia::transport::StationLocation& location, const nn::pia::transport::StationConnectionInfo& info, nn::pia::common::CallContext* pCallContext, bool isCheckOnly)
{
    if (isCheckOnly) {
        pCallContext->InitiateCall();
        return !m_pProtocol->IsTraversed(location);
    }
    return m_pProtocol->startNatTraversal(location, info, pCallContext);
}

// 0x003E5438 | fefates:bytes [tier B]
bool nn::pia::inet::NatTraverser::IsStartupCancelled()
{
    if (m_pCallContext != nullptr) {
        return m_pCallContext->IsCancelRequested();
    }
    return false;
}

// 0x003E544C | fefates:callgraph [tier C]
nn::Result nn::pia::inet::NatTraverser::startNatPortDetection()
{
    if (IsMonitoringWritable()) {
        IncrementCounter(common::g_SessionBeginMonitoringContent.m_Unknown0x272);
    }
    IncrementCounter(common::g_SessionStateMonitoringContent.m_Unknown0x3DC);
    m_LocalPort = NatDetecter::GetDifferentPortNumber(nex::RootTransport::GetCurrentPortNumber());
    if (m_NatProperty.m_NatMapping == NAT_MAPPING_EDM) {
        // the port of the next mapping cannot be predicted
        m_State = STATE_DETECTING_PORT;
        OnPortDetected(false);
        return nn::Result();
    }
    if (m_pProtocol == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    transport::StationLocation location;
    location.m_StationAddress.Clear();
    m_PortCallContext.RegisterCallback(OnPortDetectedCallback, this);
    common::InetAddress localAddress(0, m_LocalPort);
    nn::Result result = m_PortDetecter.Startup(&m_PortCallContext, localAddress, location, location, m_pProtocol, false);
    if (result.IsFailure()) {
        m_PortCallContext.SignalFailure(result);
    } else {
        m_State = STATE_DETECTING_PORT;
        m_PortDetecter.StartDetectionJob();
    }
    return nn::Result();
}

// 0x003E55DC (name is ours)
void nn::pia::inet::NatTraverser::ClearMonitoringNatTraversal(const nn::pia::transport::StationLocation& location)
{
    u8 index = FindMonitoringNatTraversalIndex(location);
    if (index >= MONITORING_SLOT_NUM) {
        return;
    }
    if (session::Mesh::s_pInstance != nullptr &&
        (session::Mesh::s_pInstance->IsMonitoringDataSenderFlagSet() || session::Mesh::s_pInstance->IsLeaving())) {
        return;
    }
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_Unknown0x274[index] = 0xFFFFFFFF;
    content.m_Unknown0x2D0[index] = 0xFFFFFFFF;
    content.m_Unknown0x32C[index] = 0xFFFFFFFF;
    content.m_Unknown0x388[index] = 0xFFFFFFFF;
    content.m_Unknown0x3E4[index] = 0xFFFFFFFF;
    content.m_Unknown0x440[index] = 0xFF;
    content.m_Unknown0x457[index] = 0xFF;
    content.m_Unknown0x46E[index] = 0xFF;
    content.m_Unknown0x485[index] = 0xFF;
}

// 0x003E56D4 | fefates:bytes [tier B]
nn::Result nn::pia::inet::NatTraverser::UpdateNatServerAddress()
{
    common::Time start;
    start.SetNow();
    if (m_ServerAddresses.GetNum() != 4) {
        common::InetAddress address;
        m_ServerAddresses.m_Num = 0;
        s_NatServerAddress0.GetInetAddress(&address);
        if (!address.IsValidAddress() && !s_NatServerAddress0.GetAddressInfo(g_NatServerHostNames[0], nullptr, nullptr)) {
            return common::RESULT_NAT_SERVER_NOT_FOUND;
        }
        s_NatServerAddress1.GetInetAddress(&address);
        if (!address.IsValidAddress() && !s_NatServerAddress1.GetAddressInfo(g_NatServerHostNames[1], nullptr, nullptr)) {
            return common::RESULT_NAT_SERVER_NOT_FOUND;
        }
        // both ports of the first server, then both ports of the second one
        s_NatServerAddress0.GetInetAddress(&address);
        address.m_Port = NAT_SERVER_PRIMARY_PORT;
        if (AddServerAddress(m_ServerAddresses, address).IsFailure()) {
            return common::RESULT_NAT_SERVER_NOT_FOUND;
        }
        address.m_Port = NAT_SERVER_SECONDARY_PORT;
        if (AddServerAddress(m_ServerAddresses, address).IsFailure()) {
            return common::RESULT_NAT_SERVER_NOT_FOUND;
        }
        s_NatServerAddress1.GetInetAddress(&address);
        address.m_Port = NAT_SERVER_PRIMARY_PORT;
        if (AddServerAddress(m_ServerAddresses, address).IsFailure()) {
            return common::RESULT_NAT_SERVER_NOT_FOUND;
        }
        address.m_Port = NAT_SERVER_SECONDARY_PORT;
        if (AddServerAddress(m_ServerAddresses, address).IsFailure()) {
            return common::RESULT_NAT_SERVER_NOT_FOUND;
        }
    }
    common::Time now;
    now.SetNow();
    common::g_SessionBeginMonitoringContent.m_Unknown0x4A = (now - start).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
    return nn::Result();
}

// 0x003E5948 (name is ours)
nn::Result nn::pia::inet::NatTraverser::updateStationUrls()
{
    // (a trace of the NAT property; armlink removed the call)
    bool isPrivateUrlUpdated = false;
    nex::qList<nex::StationURL>& urls = nex::Network::GetInstance()->m_StationUrls;
    transport::StationConnectionInfo info;
    for (nex::qList<nex::StationURL>::Node* pNode = urls.GetFirst(); !urls.IsEnd(pNode); pNode = pNode->m_pNext) {
        nex::StationURL& url = pNode->m_Value;
        if (url.GetType() & 2) {
            // the public URL
            NexFacade::ConvertNexStationUrlToStationLocation(url, &info.m_PublicLocation);
            if (!info.m_PublicLocation.m_StationAddress.m_InetAddress.IsValid()) {
                transport::StationLocation location;
                NexFacade::ConvertNexStationUrlToStationLocation(url, &location);
                // (traces of the location and the URL; armlink removed the calls)
                return common::RESULT_NAT_CHECK_FAILED;
            }
            info.m_PublicLocation.m_NatMapping = m_NatProperty.m_NatMapping;
            info.m_PublicLocation.m_NatFiltering = m_NatProperty.m_NatFiltering;
            if (NexFacade::IsGlobal(info.m_PublicLocation)) {
                if (NexFacade::s_IsNatSessionSkipped) {
                    info.m_PrivateLocation.SetStationLocation(info.m_PublicLocation);
                    info.m_PublicLocation.m_Type = 3;
                    info.m_PrivateLocation.m_Type = 0;
                }
                info.m_PrivateLocation.m_StationAddress.m_InetAddress.m_Port = m_LocalPort;
                info.m_PrivateLocation.Trace(LOCATION_TRACE_FLAG);
            }
            info.m_PublicLocation.Trace(LOCATION_TRACE_FLAG);
        } else {
            // the private URL gets the NAT and the port of the session
            *m_pOldStationUrl = url;
            *m_pNewStationUrl = url;
            nex::StationURL* pUrl = m_pNewStationUrl;
            pUrl->SetNATMapping(m_NatProperty.m_NatMapping);
            pUrl->SetNATFiltering(m_NatProperty.m_NatFiltering);
            if (m_NatProperty.m_NatMapping == NAT_MAPPING_EIM) {
                pUrl->SetPortNumber(m_NatProperty.m_PublicPort);
                // (traces; armlink removed the calls)
            } else {
                pUrl->SetPortNumber(m_LocalPort);
            }
            NexFacade::ConvertNexStationUrlToStationLocation(*pUrl, &info.m_PrivateLocation);
            if (!info.m_PrivateLocation.m_StationAddress.m_InetAddress.IsValid()) {
                transport::StationLocation location;
                NexFacade::ConvertNexStationUrlToStationLocation(*pUrl, &location);
                // (traces of the location and the URL; armlink removed the calls)
                return common::RESULT_NAT_CHECK_FAILED;
            }
            info.m_PrivateLocation.m_StationAddress.m_InetAddress.m_Port =
                m_NatProperty.m_NatMapping == NAT_MAPPING_EIM ? m_NatProperty.m_PublicPort : m_LocalPort;
            info.m_PrivateLocation.Trace(LOCATION_TRACE_FLAG);
            isPrivateUrlUpdated = true;
        }
    }
    // nex replaces the private URL on the server
    nex::NgsBridgeInterface* pBridge = NexFacade::s_pInstance->m_pNgsBridge;
    if (common::IsValidPointer(pBridge) && m_ProtocolCallContext.m_State != 1 && isPrivateUrlUpdated) {
        m_ProtocolCallContext.Reset();
        m_ProtocolCallContext.m_Deadline = nex::Time::ConvertTimeoutToDeadline(REPLACE_URL_TIMEOUT_MSEC);
        nex::UserContext userContext;
        userContext._unknown = 0;
        m_ProtocolCallContext.RegisterCompletionCallback(OnReplaceUrlCompleted, userContext, true);
        pBridge->ReplaceURL(&m_ProtocolCallContext, *m_pOldStationUrl, *m_pNewStationUrl);
    }
    info.m_PublicLocation.m_PrincipalId = NexFacade::s_pInstance->m_pMatchMakingClient->m_pDefaultCredentials->m_PrincipalId;
    info.m_PrivateLocation.m_PrincipalId = NexFacade::s_pInstance->m_pMatchMakingClient->m_pDefaultCredentials->m_PrincipalId;
    info.m_PublicLocation.m_ConnectionId = static_cast<u32>(common::Scheduler::s_pInstance->m_DispatchTime.m_Tick);
    info.m_PrivateLocation.m_ConnectionId = static_cast<u32>(common::Scheduler::s_pInstance->m_DispatchTime.m_Tick);
    transport::StationConnectionInfoTable::s_pInstance->m_LocalInfo = info;
    if (!NexFacade::s_IsNatSessionSkipped) {
        m_pProtocol->m_SelfLocation = info.m_PublicLocation;
    }
    return nn::Result();
}

// 0x003E5D0C (name is ours)
void nn::pia::inet::NatTraverser::completeNatSession()
{
    if (!common::IsValidPointer(m_pCallContext)) {
        return;
    }
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        return;
    }
    if (!common::IsValidPointer(GetNexRootTransport())) {
        return;
    }
    m_NatProperty.SetNexNatProperties(GetNexRootTransport()->GetNATProperties());
    if (NexFacade::s_IsNatSessionSkipped) {
        m_LocalPort = nex::RootTransport::GetCurrentPortNumber() + 1;
    } else if (m_NatProperty.m_PrivatePort != 0 && m_NatProperty.m_NatMapping != NAT_MAPPING_EDM) {
        // the port the NAT check used
        m_LocalPort = m_NatProperty.m_PrivatePort;
    }
    nn::Result result = NexFacade::s_pInstance->CompleteStartNatSession(m_LocalPort);
    if (result.IsSuccess()) {
        result = updateStationUrls();
    }
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return;
    }
    if (!NexFacade::s_IsNatSessionSkipped) {
        if (m_pProtocol == nullptr) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
            m_pCallContext = nullptr;
            return;
        }
        m_pProtocol->StartServerKeepAlive();
    }
    m_State = STATE_STARTED;
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    // the monitoring data of the NAT and the station
    const transport::StationConnectionInfo& self = transport::StationConnectionInfoTable::s_pInstance->m_LocalInfo;
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    u8 type = self.m_PublicLocation.m_Type;
    if (m_NatProperty.m_IsPortPreserved) {
        type |= MONITORING_PORT_PRESERVED;
    }
    content.m_Unknown0x44 = m_NatProperty.m_NatMapping;
    content.m_Unknown0x45 = m_NatProperty.m_NatFiltering;
    content.m_Unknown0x46 = m_NatProperty.m_PortIncrement;
    content.m_Unknown0x160 = m_NatProperty.m_PublicPort;
    content.m_Unknown0x162 = m_NatProperty.m_PrivatePort;
    content.m_Unknown0x47 = type;
    content.m_Unknown0x3C = self.m_PublicLocation.m_StationAddress.m_InetAddress.m_Address;
    content.m_Unknown0x40 = self.m_PrivateLocation.m_StationAddress.m_InetAddress.m_Address;
    content.m_Unknown0x54 = common::hashWithMd5(self.m_PublicLocation.m_PrincipalId);
    content.m_Unknown0x5C = NexFacade::s_pInstance->m_Unknown0x4;
    content.m_Unknown0x60 = nex::g_Unknown0x96C91E;
    if (common::IsValidPointer(GetNexRootTransport())) {
        content.m_Unknown0x64 = nex::g_Unknown0x96C9C8;
        content.m_Unknown0x68 = nex::g_Unknown0x96C9D4;
        content.m_Unknown0x6C = nex::g_Unknown0x96C9D8;
        content.m_Unknown0x70 = nex::g_Unknown0x96C91F;
        content.m_Unknown0x71 = nex::g_Unknown0x96C92B;
    }
}

// 0x003E5F98 | fefates:callgraph [tier C]
void nn::pia::inet::NatTraverser::ReportNatTraversalResult(const nn::pia::transport::StationLocation& location, nn::Result result, bool isReport)
{
    if (location.m_StationKey == 0) {
        location.Trace(1);
        return;
    }
    if (!(NexFacade::IsBehindNat(location) && NexFacade::IsPublic(location)) && !isReport) {
        // the traversal starts now
        NatTraversalTime time;
        time.m_Location = location;
        time.m_Unknown0x28.SetNow();
        time.m_Unknown0x30.SetNow();
        time.m_Unknown0x38 = 0;
        time.m_PortCheckMSec = 0;
        m_pProtocol->m_TraversalTimeList.Add(time);
    }
    if (result.IsSuccess()) {
        if (IsMonitoringWritable()) {
            IncrementCounter(common::g_SessionBeginMonitoringContent.m_Unknown0x4A0);
        }
    } else {
        if (IsMonitoringWritable()) {
            IncrementCounter(common::g_SessionBeginMonitoringContent.m_Unknown0x4A2);
        }
    }
    u8 index = FindMonitoringNatTraversalIndex(location);
    if (index < MONITORING_SLOT_NUM) {
        u8 sprayCount = 0;
        u32 portSprayCount = 0;
        u32 elapsedMSec = 0xFFFFFFFF;
        u32 unknown0x388 = 0xFFFFFFFF;
        u32 kind = 0;
        NatProbe* pProbe = m_pProtocol->findProbe(location);
        if (pProbe != nullptr) {
            sprayCount = pProbe->m_SprayCount;
            portSprayCount = static_cast<u8>(pProbe->GetPortSprayCount());
            kind = pProbe->m_Kind;
        }
        NatTraversalTime* pTime = m_pProtocol->m_TraversalTimeList.Find(location);
        if (pTime != nullptr) {
            if (sprayCount != 0) {
                common::Time now;
                now.SetNow();
                elapsedMSec = (now - pTime->m_Unknown0x28).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
            }
            unknown0x388 = pTime->m_Unknown0x38;
            m_pProtocol->m_TraversalTimeList.Remove(location);
        }
        common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
        content.m_Unknown0x274[index] = location.m_StationKey;
        content.m_Unknown0x2D0[index] = common::hashWithMd5(location.m_PrincipalId);
        content.m_Unknown0x32C[index] = elapsedMSec;
        content.m_Unknown0x388[index] = unknown0x388;
        content.m_Unknown0x3E4[index] = result.GetPrintableBits();
        content.m_Unknown0x440[index] = sprayCount;
        content.m_Unknown0x457[index] = portSprayCount;
        content.m_Unknown0x46E[index] = kind;
    }
    if (isReport) {
        m_pProtocol->ReportNatTraversalResult(location, result.IsSuccess());
    }
}

// 0x003E62BC (name is ours)
void nn::pia::inet::NatTraverser::OnPortDetectedCallback(nn::Result, void* pArg)
{
    static_cast<NatTraverser*>(pArg)->OnPortDetected(true);
}

// 0x003E62C8 (name is ours)
void nn::pia::inet::NatTraverser::OnPortDetected(bool isDetected)
{
    if (m_State == STATE_NONE || !common::IsValidPointer(m_pCallContext)) {
        return;
    }
    if (isDetected) {
        if (m_PortCallContext.GetState() == common::CallContext::STATE_CALL_CANCEL) {
            m_PortCallContext.Reset();
            return;
        }
        nn::Result result = m_PortCallContext.m_Result;
        if (result.IsFailure()) {
            if (result == common::RESULT_CANCELED) {
                m_pCallContext->SignalCancel();
            } else {
                m_pCallContext->SignalFailure(result);
            }
            m_pCallContext = nullptr;
            m_PortCallContext.Reset();
            return;
        }
    }
    completeNatSession();
}

// 0x003E636C (name is ours)
u8 nn::pia::inet::NatTraverser::FindMonitoringNatTraversalIndex(const nn::pia::transport::StationLocation& location)
{
    u8 index = INVALID_MONITORING_SLOT;
    u32 stationKey = location.m_StationKey;
    if (!IsMonitoringWritable()) {
        return index;
    }
    const common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    for (u8 i = 0; i < MONITORING_SLOT_NUM; i++) {
        if (content.m_Unknown0x274[i] == stationKey) {
            index = i;
            break;
        }
    }
    if (index == INVALID_MONITORING_SLOT) {
        // a free slot
        for (u8 i = 0; i < MONITORING_SLOT_NUM; i++) {
            if (content.m_Unknown0x274[i] == INVALID_STATION_KEY) {
                return i;
            }
        }
    }
    return index;
}

// 0x003E6408 (name is ours)
nn::Result nn::pia::inet::NatTraverser::startNatPropertyDetection()
{
    if (IsMonitoringWritable()) {
        IncrementCounter(common::g_SessionBeginMonitoringContent.m_Unknown0x270);
    }
    IncrementCounter(common::g_SessionStateMonitoringContent.m_Unknown0x3DA);
    common::InetAddress localAddress;
    m_LocalPort = NatDetecter::GetDifferentPortNumber(nex::RootTransport::GetCurrentPortNumber());
    localAddress.m_Port = m_LocalPort;
    m_PropertyCallContext.RegisterCallback(OnPropertyDetectedCallback, this);
    nn::Result result = m_PropertyDetecter.Startup(&m_PropertyCallContext, localAddress);
    if (result.IsFailure()) {
        return result;
    }
    m_State = STATE_DETECTING_PROPERTY;
    m_PropertyDetecter.StartDetectionJob();
    return nn::Result();
}

// 0x003E651C | fefates:bytes [tier B]
bool nn::pia::inet::NatTraverser::CheckLatestStationLocation(nn::pia::transport::StationLocation& location)
{
    transport::StationLocation latest;
    if (m_pProtocol->GetLatestStationLocation(location, &latest)) {
        location.Trace(TRACE_FLAG);
        location = latest;
        location.Trace(TRACE_FLAG);
        return true;
    }
    return false;
}

// 0x003E65B0 (name is ours)
void nn::pia::inet::NatTraverser::OnServerAddressResolvedCallback(nn::Result, void* pArg)
{
    static_cast<NatTraverser*>(pArg)->OnServerAddressResolved();
}

// 0x003E65B8 (name is ours)
void nn::pia::inet::NatTraverser::OnServerAddressResolved()
{
    if (m_State == STATE_NONE || !common::IsValidPointer(m_pCallContext)) {
        return;
    }
    if (m_ServerAddressCallContext.GetState() == common::CallContext::STATE_CALL_CANCEL) {
        m_ServerAddressCallContext.Reset();
        return;
    }
    nn::Result result = m_ServerAddressCallContext.m_Result;
    if (result.IsFailure()) {
        if (result == common::RESULT_CANCELED) {
            m_pCallContext->SignalCancel();
        } else {
            m_pCallContext->SignalFailure(result);
        }
        m_pCallContext = nullptr;
        m_ServerAddressCallContext.Reset();
        return;
    }
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        m_ServerAddressCallContext.Reset();
        return;
    }
    // the NAT is known from an earlier session: only the port is checked
    if (m_NatProperty.m_NatMapping != 0) {
        result = startNatPortDetection();
    } else {
        result = startNatPropertyDetection();
    }
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
    }
}

// 0x003E6694 | fefates:bytes
void nn::pia::inet::NatTraverser::Cleanup()
{
    stopNatSession();
    if (m_pProtocol != nullptr) {
        m_pProtocol->CleanupNatTraversal();
    }
    if (m_ServerAddressCallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_ServerAddressCallContext.SignalCancel();
    }
    m_ServerAddressCallContext.Reset();
    if (m_PropertyCallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_PropertyCallContext.SignalCancel();
    }
    m_PropertyCallContext.Reset();
    if (m_PortCallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_PortCallContext.SignalCancel();
    }
    m_PortCallContext.Reset();
    m_ServerAddresses.m_Num = 0;
    m_pCallContext = nullptr;
}

// 0x003E6714 | slot vf_0x08 of nn::pia::inet::NatTraverser
nn::Result nn::pia::inet::NatTraverser::Startup(nn::pia::common::CallContext* pCallContext)
{
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (m_pProtocol == nullptr && !NexFacade::s_IsNatSessionSkipped) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_ServerAddressCallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS ||
        m_PropertyCallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS ||
        m_PortCallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (!pCallContext->InitiateCall()) {
        m_pCallContext = nullptr;
        return common::RESULT_INVALID_STATE;
    }
    if (NexFacade::s_IsNatSessionSkipped) {
        completeNatSession();
        return nn::Result();
    }
    nn::Result result = startNatServerAddressResolve();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
    }
    return nn::Result();
}

// 0x003E67D8 | fefates:bytes [tier B]
nn::pia::inet::NatTraverser::NatTraverser()
    : m_pCallContext(nullptr), m_pProtocol(nullptr), m_ProtocolId(0, 0), m_State(STATE_NONE), m_LocalPort(0)
{
    void* pBuffer = pead::AllocMemory(sizeof(nex::StationURL), common::HeapManager::GetHeap());
    m_pOldStationUrl = ::new (pBuffer) nex::StationURL();
    pBuffer = pead::AllocMemory(sizeof(nex::StationURL), common::HeapManager::GetHeap());
    m_pNewStationUrl = ::new (pBuffer) nex::StationURL();
}

// 0x003E69A8
// 0x003E68B4 (deleting dtor)
nn::pia::inet::NatTraverser::~NatTraverser()
{
    m_pProtocol = nullptr;
    if (m_pOldStationUrl != nullptr) {
        m_pOldStationUrl->~StationURL();
        pead::FreeMemory(m_pOldStationUrl);
        m_pOldStationUrl = nullptr;
    }
    if (m_pNewStationUrl != nullptr) {
        m_pNewStationUrl->~StationURL();
        pead::FreeMemory(m_pNewStationUrl);
        m_pNewStationUrl = nullptr;
    }
}

// 0x0072EFF0 slot 0x10
void nn::pia::inet::NatTraverser::Trace(u64) const
{
    // empty (in the original too)
}

// 0x004110BC (name is ours)
void nn::pia::inet::NatTraverser::OnReplaceUrlCompleted(nn::nex::CallContext*, const nn::nex::UserContext*)
{
    // empty (in the original too)
}

// 0x004123A0 (name is ours)
void nn::pia::inet::NatTraverser::OnPropertyDetectedCallback(nn::Result, void* pArg)
{
    NatTraverser* pNatTraverser = static_cast<NatTraverser*>(pArg);
    if (pNatTraverser->m_State == STATE_NONE || !common::IsValidPointer(pNatTraverser->m_pCallContext)) {
        return;
    }
    if (pNatTraverser->m_PropertyCallContext.GetState() == common::CallContext::STATE_CALL_CANCEL) {
        pNatTraverser->m_PropertyCallContext.Reset();
        return;
    }
    nn::Result result = pNatTraverser->m_PropertyCallContext.m_Result;
    if (result.IsFailure()) {
        if (result == common::RESULT_CANCELED) {
            pNatTraverser->m_pCallContext->SignalCancel();
        } else {
            pNatTraverser->m_pCallContext->SignalFailure(result);
        }
        pNatTraverser->m_pCallContext = nullptr;
        pNatTraverser->m_PropertyCallContext.Reset();
        return;
    }
    pNatTraverser->completeNatSession();
}

} // namespace inet
} // namespace pia
} // namespace nn

// the container of the NAT server addresses (out of line in the original)

// 0x007E503C
// 0x007E5008 (deleting dtor)
template nn::pia::common::SimpleContainer<nn::pia::common::InetAddress, 4u>::~SimpleContainer();
// 0x00827FE0
template void nn::pia::common::SimpleContainer<nn::pia::common::InetAddress, 4u>::Trace(u64) const;
