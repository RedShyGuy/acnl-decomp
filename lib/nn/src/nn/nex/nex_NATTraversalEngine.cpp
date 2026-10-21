#include "nn/nex/nex_NATTraversalEngine.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_ConnectivityManager.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_JobCheckAndRelay.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_NATRelayInterface.h"
#include "nn/nex/nex_NATTraversalStream.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_Relay.h"
#include "nn/nex/nex_RelayStream.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
// the object of the platform that makes the transport adapter (PRUDPEndPoint)
extern void* g_pUnknown0x96CA28;

// the time to live of the "Dummy" packets and of the first probes of a probe request (names are
// ours, like the ones below)
// 0x0096C932
u8 g_DummyPacketTTL = 3;
// the port of the NAT check server
// 0x0096C940
u16 g_NATCheckPort = 33335;
// the time of a probe (ms)
// 0x0096CA0C
s32 g_ProbeTimeout = 60000;
// the time between the updates of DoWork (ms)
// 0x0096CA10
s32 g_NATTraversalUpdateInterval = 500;
// the probes to public URLs at the same time
// 0x0096CA18
u32 g_MaxProbeCount = 32;
// the time between the keep-alives of the NAT check server (ms)
// 0x0096CA1C
s32 g_ServerKeepAliveInterval = 10000;
// the time the peer has for the probes of a probe request (ms)
// 0x0096CA20
s32 g_ProbeRequestTimeout = 10000;
// the "Dummy" packets that are sent at once
// 0x0096CA24
u32 g_DummyPacketCount = 5;

namespace {
// the types of the probes
const u8 PROBE_TYPE_REQUEST = 0;
const u8 PROBE_TYPE_REPLY = 1;
// the time between the probes after a reply (ms) and the probes without a reply
const int PROBE_INTERVAL = 90000;
const u32 MAX_PROBE_SEND_COUNT = 12;
// the probes that are sent at once the first time a probe request asks for them
const u8 FIRST_PROBE_COUNT = 3;
// the NAT mapping that maps the ports per destination
const u8 NAT_MAPPING_PER_DESTINATION = 2;
// the type flag of a public URL
const u32 URL_TYPE_PUBLIC = 2;

// the results of GetNATTraversalResult (names are ours)
const NATTraversalResult NAT_TRAVERSAL_RESULT_DIRECT = static_cast<NATTraversalResult>(100);
const NATTraversalResult NAT_TRAVERSAL_RESULT_RELAY = static_cast<NATTraversalResult>(110);
const NATTraversalResult NAT_TRAVERSAL_RESULT_NOT_SENT = static_cast<NATTraversalResult>(200);
const NATTraversalResult NAT_TRAVERSAL_RESULT_NO_REPLY = static_cast<NATTraversalResult>(201);
const NATTraversalResult NAT_TRAVERSAL_RESULT_NO_REPLY_RELAY = static_cast<NATTraversalResult>(210);
const NATTraversalResult NAT_TRAVERSAL_RESULT_UNKNOWN = static_cast<NATTraversalResult>(999);

// (inline) the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    return Scheduler::GetInstanceInline()->m_Lock;
}

// (inline) the root transport of the network (null if there is none)
inline RootTransport* GetRootTransport()
{
    Network* pNetwork = Network::GetInstance();
    return pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
}

// (inline) the connectivity manager of the network (null if there is none)
inline ConnectivityManager* GetConnectivityManager()
{
    return Network::GetInstance() != nullptr ? &Network::GetInstance()->m_ConnectivityManager : nullptr;
}

// (inline; NATProperties::GetNATMapping) the mapping of the NAT
inline u8 GetNATMappingInline(const NATProperties& properties)
{
    return g_pUnknown0x96CA28 != nullptr ? 3 : properties.m_NatMapping;
}

// (inline) the probe is sent again now (without a reply: while it has probes left; with one: when
// the interval passed)
inline bool IsProbeDue(const URLProbe& probe, const Time& now)
{
    if (probe.m_Result == -1) {
        return probe.m_SendCount < probe.m_MaxSendCount;
    }
    return static_cast<s64>(now.m_Value - probe.m_LastReplyTime.m_Value) > probe.m_Interval;
}

// (inline) the same address and RVCID
inline bool IsSameStation(const StationURL& a, const StationURL& b)
{
    return a.GetAddress() == b.GetAddress() && a.GetParamValue(L"RVCID", 0) == b.GetParamValue(L"RVCID", 0);
}

// (inline) the characters of the string go to the stream
inline void AppendString(ByteStream& stream, const String& string)
{
    Buffer data(string);
    stream.m_pBuffer->AppendData(data.m_pData + data.m_HeaderSize, data.m_ContentSize, static_cast<u32>(-1));
}

// (inline) the value goes to the stream
template <typename T>
inline void AppendPrimitive(ByteStream& stream, const T& value)
{
    stream.m_pBuffer->AppendData(&value, sizeof(T), static_cast<u32>(-1));
}

// (inline) the parameter of the string parameters of the URL
inline void SetStringParam(StationURL& url, const wchar_t* pKey, const String& value)
{
    url.m_StringParams.erase(String(pKey));
    url.m_StringParams[String(pKey)] = value;
}
} // namespace

// 0x0038C5E8 (name is ours)
void nn::nex::NATTraversalEngine::ResendProbes()
{
    ScopedCS lock(GetSchedulerLock());
    for (qList<URLProbe>::Node* pNode = m_Probes.GetFirst(); !m_Probes.IsEnd(pNode); pNode = pNode->m_pNext) {
        URLProbe& probe = pNode->m_Value;
        if (!IsProbeDue(probe, SystemClock::GetTime())) {
            continue;
        }
        // the first probes of a probe request open the NAT of this side
        bool isRepeated = true;
        u8 ttl = 0;
        if (probe.m_LastReplyTime.m_Value == 0 && probe.m_SendCount < 8 && probe.m_URL.GetParamValue(L"probeinit", 0) != 0 &&
            probe.m_URL.GetParamValue(L"R", 0) == 0) {
            ttl = g_DummyPacketTTL;
            isRepeated = false;
        }
        u8 count = (probe.m_SendCount == 0 && !isRepeated) ? FIRST_PROBE_COUNT : 1;
        for (u8 i = 0; i < count; i++) {
            Time now = SystemClock::GetTime();
            SendProbe(PROBE_TYPE_REQUEST, &probe.m_URL, &now, ttl);
        }
        probe.m_SendCount++;
    }
}

// 0x0038C770 (name is ours)
void nn::nex::NATTraversalEngine::ProcessFinishedProbes()
{
    ScopedCS lock(GetSchedulerLock());
    qList<u32> rvcids;
    qList<StationURL> relayURLs;
    qList<URLProbe>::Node* pNode = m_Probes.GetFirst();
    while (!m_Probes.IsEnd(pNode)) {
        URLProbe& probe = pNode->m_Value;
        if (IsProbeDue(probe, SystemClock::GetTime()) && probe.m_Result == -1) {
            pNode = pNode->m_pNext;
            continue;
        }
        if (probe.m_Result != -1) {
            // the probe got a reply: the other URLs of the station are not needed
            u32 rvcid = probe.m_URL.GetParamValue(L"RVCID", 0);
            qList<StationURL>::Node* pURL = m_PendingURLs.GetFirst();
            while (!m_PendingURLs.IsEnd(pURL)) {
                if (pURL->m_Value.GetParamValue(L"RVCID", 0) != rvcid) {
                    pURL = pURL->m_pNext;
                } else {
                    pURL = m_PendingURLs.erase(pURL);
                }
            }
            rvcids.PushBackInline(rvcid);
            m_FinishedProbes.PushBackInline(probe);
        } else {
            // no reply: a public station without a relay waits for the relay
            if (GetRootTransport()->GetRelay()->m_IsEnabled && !String::IsEqual(m_RelayHost.m_pString, L"") && m_RelayPort != 0 &&
                probe.m_URL.GetParamValue(L"R", 0) == 0 && (probe.m_URL.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC) &&
                probe.m_URL.GetParamValue(L"probeinit", 0) == 0) {
                relayURLs.PushBackInline(probe.m_URL);
            } else {
                m_FinishedProbes.PushBackInline(probe);
            }
            bool usesRelay = GetNATMappingInline(GetRootTransport()->GetNATProperties()) == NAT_MAPPING_PER_DESTINATION &&
                             GetRootTransport()->GetRelay()->m_IsEnabled && !(m_RelayHost == L"") && m_RelayPort != 0;
            if (usesRelay) {
                // the NAT maps the ports per destination: the public URLs go through the relay
                m_UsesRelay = true;
                for (qList<StationURL>::Node* pURL = m_PendingURLs.GetFirst(); !m_PendingURLs.IsEnd(pURL); pURL = pURL->m_pNext) {
                    StationURL& url = pURL->m_Value;
                    if (url.GetParamValue(L"R", 0) == 0 && (url.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC)) {
                        url.SetParamValue(L"R", 1);
                        SetStringParam(url, L"Rsa", m_RelayHost);
                        url.SetParamValue(L"Rsp", m_RelayPort);
                    }
                }
            }
        }
        pNode = m_Probes.erase(pNode);
    }
    // the other probes of the stations with a reply are done too
    pNode = m_Probes.GetFirst();
    while (rvcids.GetSize() != 0 && !m_Probes.IsEnd(pNode)) {
        u32 rvcid = pNode->m_Value.m_URL.GetParamValue(L"RVCID", 0);
        qList<u32>::Node* pId = rvcids.GetFirst();
        while (!rvcids.IsEnd(pId) && pId->m_Value != rvcid) {
            pId = pId->m_pNext;
        }
        if (rvcids.IsEnd(pId)) {
            pNode = pNode->m_pNext;
            continue;
        }
        m_FinishedProbes.PushBackInline(pNode->m_Value);
        pNode = m_Probes.erase(pNode);
    }
    // the finished probes are kept until their deadline
    pNode = m_FinishedProbes.GetFirst();
    while (!m_FinishedProbes.IsEnd(pNode)) {
        if (pNode->m_Value.m_Deadline < SystemClock::GetTime()) {
            pNode = m_FinishedProbes.erase(pNode);
        } else {
            pNode = pNode->m_pNext;
        }
    }
    // the public stations without a reply wait for a traversal through the relay
    for (qList<StationURL>::Node* pURL = relayURLs.GetFirst(); !relayURLs.IsEnd(pURL); pURL = pURL->m_pNext) {
        pURL->m_Value.SetParamValue(L"R", 1);
        SetStringParam(pURL->m_Value, L"Rsa", m_RelayHost);
        pURL->m_Value.SetParamValue(L"Rsp", m_RelayPort);
        m_PendingURLs.PushBackInline(pURL->m_Value);
    }
}

// 0x0038D484 slot 0x10 (name is ours)
bool nn::nex::NATTraversalEngine::RegisterUnknown(void* pUnknown)
{
    ScopedCS lock(GetSchedulerLock());
    if (m_pUnknown0x0C != nullptr) {
        return false;
    }
    m_pUnknown0x0C = pUnknown;
    return true;
}

// 0x0038D4F0 | fefates:bytes [tier B]
bool nn::nex::NATTraversalEngine::GetUpdatedURL(const nn::nex::StationURL& url, nn::nex::StationURL* pUpdated)
{
    ScopedCS lock(GetSchedulerLock());
    URLProbe* pProbe = m_FinishedProbes.FindProbe(url);
    if (pProbe != nullptr && pProbe->m_URL != url) {
        pUpdated->Copy(pProbe->m_URL);
        return true;
    }
    return false;
}

// 0x0038D5B0 slot 0x08 (name after pia::inet::NexNatTraversalProtocol::RegisterRelay)
bool nn::nex::NATTraversalEngine::RegisterRelay(nn::nex::NATRelayInterface* pInterface)
{
    ScopedCS lock(GetSchedulerLock());
    if (m_pRelayInterface != nullptr) {
        return false;
    }
    m_pRelayInterface = pInterface;
    m_NextUpdateTime = SystemClock::GetTime() + g_NATTraversalUpdateInterval;
    m_pStream = new NATTraversalStream(this);
    return true;
}

// 0x0038D6C8 (name is ours)
bool nn::nex::NATTraversalEngine::IsProbeFailed(const nn::nex::StationURL& url)
{
    ScopedCS lock(GetSchedulerLock());
    URLProbe* pProbe = m_FinishedProbes.FindProbe(url);
    if (pProbe != nullptr) {
        return pProbe->m_Result == -1;
    }
    if (m_Probes.FindProbe(url) != nullptr) {
        return false;
    }
    for (qList<StationURL>::Node* pNode = m_PendingURLs.GetFirst(); !m_PendingURLs.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (IsSameStation(pNode->m_Value, url)) {
            return false;
        }
    }
    for (ProbeRequestMap::iterator it = m_ProbeRequests.begin(); it != m_ProbeRequests.end(); ++it) {
        if (IsSameStation(it->second.second, url)) {
            return false;
        }
    }
    return true;
}

// 0x0038D998 (name is ours)
void nn::nex::NATTraversalEngine::ProcessProbe(const nn::nex::StationURL& url, const u8* pData, unsigned int size)
{
    ScopedCS lock(GetSchedulerLock());
    Buffer buffer;
    buffer.AppendData(pData, size, static_cast<u32>(-1));
    ByteStream stream(&buffer);
    u8 type;
    u32 rvcid;
    Time sendTime;
    u16 port;
    stream.ExtractPrimitive(&type);
    stream.ExtractPrimitive(&rvcid);
    stream.ExtractPrimitiveInline(&sendTime.m_Value);
    if (type == PROBE_TYPE_REPLY) {
        stream.ExtractPrimitive(&port);
    }
    if (stream.m_IsError) {
        return;
    }
    StationURL source(url);
    source.SetParamValue(L"RVCID", rvcid);
    URLProbe* pProbe = m_Probes.FindProbe(source);
    if (pProbe == nullptr) {
        pProbe = m_FinishedProbes.FindProbe(source);
    }
    if (pProbe == nullptr) {
        // a probe of the station from another URL: this side probes it too
        URLProbe* pStation = nullptr;
        for (qList<URLProbe>::Node* pNode = m_Probes.GetFirst(); !m_Probes.IsEnd(pNode); pNode = pNode->m_pNext) {
            if (pNode->m_Value.m_URL.GetParamValue(L"RVCID", 0) == rvcid) {
                pStation = &pNode->m_Value;
                break;
            }
        }
        if (pStation != nullptr && !(source.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC)) {
            StartSendProbe(source);
        }
    }
    switch (type) {
    case PROBE_TYPE_REQUEST:
        if (pProbe != nullptr) {
            pProbe->ProcessReply(source, Time(0));
        }
        SendProbe(PROBE_TYPE_REPLY, &source, &sendTime, 0);
        break;
    case PROBE_TYPE_REPLY:
        if (pProbe != nullptr) {
            bool isPortChecked = GetRootTransport() != nullptr &&
                                 GetNATMappingInline(GetRootTransport()->GetNATProperties()) == NAT_MAPPING_PER_DESTINATION &&
                                 (pProbe->m_URL.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC) && pProbe->m_URL.GetParamValue(L"probeinit", 0) == 0;
            if (isPortChecked) {
                // the peer saw another public port of this side
                u16 publicPort = GetRootTransport()->GetNATProperties().m_PublicPort;
                if (publicPort != port) {
                    NATProperties properties = GetRootTransport()->GetNATProperties();
                    properties.m_PublicPort = port;
                    GetRootTransport()->m_NATProperties = properties;
                }
            }
            pProbe->ProcessReply(source, sendTime);
        }
        break;
    }
}

// 0x0038DE30 | fefates:bytes-fuzzy [tier B]
void nn::nex::NATTraversalEngine::StartSendProbe(const nn::nex::StationURL& url)
{
    URLProbe* pProbe = m_Probes.FindProbe(url);
    if (pProbe != nullptr) {
        pProbe->m_Deadline = SystemClock::GetTime() + g_ProbeTimeout;
        return;
    }
    StationURL target(url);
    if (GetRootTransport()->GetRelay()->m_IsEnabled && target.GetParamValue(L"R", 0) != 0) {
        // the probes go through the relay server of the station
        bool isIncomplete = String::IsEqual(target.GetRelayAddress().m_pString, L"") || static_cast<u16>(target.GetParamValue(L"Rp", 0)) == 0 ||
                            String::IsEqual(target.GetRelayServerAddress().m_pString, L"") || static_cast<u16>(target.GetParamValue(L"Rsp", 0)) == 0;
        if (isIncomplete) {
            m_FinishedProbes.push_back(URLProbe(target, g_ProbeTimeout, PROBE_INTERVAL, MAX_PROBE_SEND_COUNT));
            return;
        }
        target.SetAddressInline(target.GetRelayAddress().m_pString);
        target.SetPortNumberInline(static_cast<u16>(target.GetParamValue(L"Rp", 0)));
        InetAddress relayAddress(target.GetRelayAddress().m_pString, static_cast<u16>(target.GetParamValue(L"Rp", 0)));
        InetAddress serverAddress(target.GetRelayServerAddress().m_pString, static_cast<u16>(target.GetParamValue(L"Rsp", 0)));
        GetRootTransport()->GetRelay()->m_pStream->m_RoutingTable.Add(relayAddress, serverAddress, true);
        SendRelayProbe(&serverAddress);
    }
    m_Probes.push_back(URLProbe(target, g_ProbeTimeout, PROBE_INTERVAL, MAX_PROBE_SEND_COUNT));
}

// 0x0038E1B0 slot 0x14 (name is ours)
bool nn::nex::NATTraversalEngine::UnregisterUnknown()
{
    ScopedCS lock(GetSchedulerLock());
    if (m_pUnknown0x0C != nullptr) {
        m_pUnknown0x0C = nullptr;
        return true;
    }
    return false;
}

// 0x0038E220 (name after pia::inet::NexNatTraversalProtocol::sendDummyPacket)
void nn::nex::NATTraversalEngine::SendDummyPacket(const nn::nex::StationURL& url, unsigned char ttl)
{
    StationURL target(url);
    target.SetPortNumberInline(g_NATCheckPort);
    target.SetURLTypeInline(StationURL::URL_TYPE_UDP);
    Buffer* pBuffer = new Buffer();
    ByteStream stream;
    AppendString(stream, String(L"Dummy"));
    // (sic: the data is the buffer object of the stream)
    pBuffer->AppendData(stream.m_pBuffer, stream.m_pBuffer->m_ContentSize, static_cast<u32>(-1));
    for (u32 i = 0; i < g_DummyPacketCount; i++) {
        if (m_pSender != nullptr) {
            m_pSender->SendTo(&target, pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_ContentSize, ttl);
        } else if (m_pStream != nullptr) {
            m_pStream->Send(target.GetInetAddressInline(), m_pStream->m_Type, m_pStream->m_Port, pBuffer, ttl, 0);
        }
    }
    pBuffer->ReleaseRef();
}

// 0x0038E420 (name is ours)
void nn::nex::NATTraversalEngine::SendRelayProbe(const nn::nex::InetAddress* pAddress)
{
    StationURL target;
    target.SetInetAddress(pAddress);
    target.SetURLTypeInline(StationURL::URL_TYPE_UDP);
    ByteStream stream;
    u8 type = PROBE_TYPE_REQUEST;
    AppendPrimitive(stream, type);
    AppendPrimitive(stream, m_LocalCid);
    Time now = SystemClock::GetTime();
    stream.Append(&now);
    Buffer* pBuffer = stream.m_pBuffer;
    for (u32 i = 0; i < g_DummyPacketCount; i++) {
        if (m_pSender != nullptr) {
            m_pSender->SendTo(&target, pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_ContentSize, 0);
        } else if (m_pStream != nullptr) {
            m_pStream->Send(target.GetInetAddressInline(), m_pStream->m_Type, m_pStream->m_Port, pBuffer, 0, 0);
        }
    }
}

// 0x0038E584 (name is ours)
void nn::nex::NATTraversalEngine::SendServerKeepAlive(bool unknown)
{
    StationURL target(String(GetRootTransport()->m_NATCheckURLs.m_pBegin[0]->CStr()).m_pString);
    target.SetPortNumberInline(g_NATCheckPort);
    target.SetURLTypeInline(StationURL::URL_TYPE_UDP);
    Buffer* pBuffer = new Buffer();
    ByteStream stream;
    AppendString(stream, String(L"Dummy"));
    // (sic: the data is the buffer object of the stream)
    pBuffer->AppendData(stream.m_pBuffer, stream.m_pBuffer->m_ContentSize, static_cast<u32>(-1));
    for (u32 i = 0; i < g_DummyPacketCount; i++) {
        GetRootTransport()->SendRaw(&target, pBuffer);
    }
    pBuffer->ReleaseRef();
}

// 0x0038E7F4 slot 0x0C (name after pia::inet::NexNatTraversalProtocol::UnregisterRelay)
bool nn::nex::NATTraversalEngine::UnregisterRelay()
{
    ScopedCS lock(GetSchedulerLock());
    if (m_pRelayInterface == nullptr) {
        return false;
    }
    if (m_pStream != nullptr) {
        delete m_pStream;
    }
    m_pStream = nullptr;
    m_pRelayInterface = nullptr;
    return true;
}

// 0x0038E880 (name is ours)
void nn::nex::NATTraversalEngine::ProcessPendingURLs()
{
    ScopedCS lock(GetSchedulerLock());
    while (m_PendingURLs.GetSize() != 0) {
        if (!m_UsesRelay) {
            // the probes to public URLs are limited
            u32 publicCount = 0;
            for (qList<URLProbe>::Node* pNode = m_Probes.GetFirst(); !m_Probes.IsEnd(pNode); pNode = pNode->m_pNext) {
                if (pNode->m_Value.m_URL.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC) {
                    publicCount++;
                }
            }
            u8 mapping = GetNATMappingInline(GetRootTransport()->GetNATProperties());
            if (publicCount >= (mapping == NAT_MAPPING_PER_DESTINATION ? 1 : g_MaxProbeCount)) {
                break;
            }
            if (m_pCheckAndRelayJob != nullptr) {
                break;
            }
        }
        StationURL& url = m_PendingURLs.front();
        String publicAddress(GetRootTransport()->GetNATProperties().m_PublicAddress);
        bool isSamePublicAddress = url.GetInetAddressInline()->GetAddressStr() == publicAddress;
        // the station of the URL starts the probes (it asked for the probe request)
        bool isInitiator = url.GetParamValue(L"probeinit", 0) == 0;
        qList<StationURL> ownURLs;
        {
            Network* pNetwork = Network::GetInstanceInline();
            ScopedCS urlLock(pNetwork->m_StationUrls.m_Lock);
            ownURLs = pNetwork->m_StationUrls.m_List;
        }
        for (qList<StationURL>::Node* pNode = ownURLs.GetFirst(); !ownURLs.IsEnd(pNode); pNode = pNode->m_pNext) {
            StationURL& own = pNode->m_Value;
            if (!(own.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC)) {
                // a private URL only for a station behind the same NAT
                if (!isSamePublicAddress) {
                    continue;
                }
                if (isInitiator && url.GetParamValue(L"R", 0) != 0) {
                    continue;
                }
            }
            StationURL source(own);
            source.SetParamValue(L"RVCID", m_LocalCid);
            source.SetParamValue(L"probeinit", isInitiator);
            source.SetParamValue(L"PRID", url.GetParamValue(L"PRID", 0));
            if (Network::GetInstance()->m_pRootTransport->GetRelay()->m_IsEnabled) {
                InetAddress relayAddress(Network::GetInstance()->m_pRootTransport->GetRelay()->m_Address);
                SetStringParam(source, L"Ra", relayAddress.GetAddressStr());
                source.SetParamValue(L"Rp", __builtin_bswap16(relayAddress.m_SockAddr.m_Port));
            }
            qList<StationURL> targets;
            targets.PushBackInline(url);
            bool isCheckAndRelay = (own.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC) &&
                                   GetNATMappingInline(GetRootTransport()->GetNATProperties()) == NAT_MAPPING_PER_DESTINATION && url.GetParamValue(L"R", 0) == 0;
            if (isCheckAndRelay) {
                // the NAT maps the ports per destination: the job checks the next port first
                bool isPortPreserved = GetRootTransport()->GetNATProperties().m_IsPortPreserved;
                if (isPortPreserved) {
                    SendDummyPacket(url, g_DummyPacketTTL);
                }
                if (m_CallContext.m_State != CallContext::STATE_AVAILABLE) {
                    m_CallContext.Reset();
                }
                m_pCheckAndRelayJob = new JobCheckAndRelay(&m_CallContext, targets, source);
                Scheduler::GetInstanceInline()->Queue(m_pCheckAndRelayJob, false);
            } else {
                if (url.GetParamValue(L"R", 0) != 0 && (own.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC)) {
                    // the station of the URL is behind its relay server
                    SetStringParam(source, L"Rsa", url.GetRelayServerAddress());
                    source.SetParamValue(L"Rsp", static_cast<u16>(url.GetParamValue(L"Rsp", 0)));
                    source.SetParamValue(L"R", 1);
                } else if (own.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC) {
                    source.SetPortNumberInline(GetRootTransport()->GetNATProperties().m_PublicPort);
                } else {
                    source.SetPortNumberInline(RootTransport::s_WellKnownPortNumber.Get());
                }
                m_pRelayInterface->RelayProbeRequest(targets, source);
                if (!isInitiator) {
                    StartSendProbe(url);
                }
            }
        }
        if (isInitiator) {
            // the peer has until the deadline for the probes
            std::pair<Time, StationURL>& request = m_ProbeRequests.IndexInline(url.GetParamValue(L"PRID", 0));
            request = std::pair<Time, StationURL>(SystemClock::GetTime() + g_ProbeRequestTimeout, url);
        }
        m_PendingURLs.erase(m_PendingURLs.GetFirst());
    }
}

// 0x0038F5C0 | fefates:callgraph [tier C]
bool nn::nex::NATTraversalEngine::PrepareNATTraversal(nn::nex::StationURL url)
{
    ScopedCS lock(GetSchedulerLock());
    if (m_pRelayInterface == nullptr) {
        return false;
    }
    if (url.GetParamValue(L"RVCID", 0) == 0) {
        return false;
    }
    url.SetParamValue(L"PRID", m_NextPRID++);
    if (ShouldUseRelay(url, false)) {
        url.SetParamValue(L"R", 1);
        url.SetRelayServerAddress(m_RelayHost);
        url.SetParamValue(L"Rsp", m_RelayPort);
    }
    qList<StationURL>::Node* pNode = m_PendingURLs.GetFirst();
    for (; !m_PendingURLs.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (IsSameStation(pNode->m_Value, url)) {
            break;
        }
    }
    if (!m_PendingURLs.IsEnd(pNode)) {
        return false;
    }
    if (m_Probes.FindProbe(url) != nullptr || m_FinishedProbes.FindProbe(url) != nullptr) {
        return false;
    }
    m_PendingURLs.push_back(url);
    return true;
}

// 0x0038F7EC (name is ours)
void nn::nex::NATTraversalEngine::ProcessExpiredProbeRequests(const nn::nex::Time& now)
{
    ScopedCS lock(GetSchedulerLock());
    ProbeRequestMap::iterator it = m_ProbeRequests.begin();
    while (it != m_ProbeRequests.end()) {
        if (it->second.first < now) {
            m_FinishedProbes.PushBackInline(URLProbe(it->second.second, g_ProbeTimeout, PROBE_INTERVAL, MAX_PROBE_SEND_COUNT));
            ProbeRequestMap::iterator current = it;
            ++it;
            m_ProbeRequests.erase(current);
        } else {
            ++it;
        }
    }
}

// 0x0038FA28 | fefates:callgraph [tier C]
nn::nex::NATTraversalResult nn::nex::NATTraversalEngine::GetNATTraversalResult(unsigned int rvcid)
{
    ScopedCS lock(GetSchedulerLock());
    URLProbe* pProbe = nullptr;
    for (qList<URLProbe>::Node* pNode = m_FinishedProbes.GetFirst(); !m_FinishedProbes.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value.m_URL.GetParamValue(L"RVCID", 0) == rvcid) {
            pProbe = &pNode->m_Value;
            break;
        }
    }
    if (pProbe == nullptr) {
        return NAT_TRAVERSAL_RESULT_UNKNOWN;
    }
    if (pProbe->m_Result != -1) {
        if (pProbe->m_URL.GetParamValue(L"R", 0) != 0) {
            return NAT_TRAVERSAL_RESULT_RELAY;
        }
        return NAT_TRAVERSAL_RESULT_DIRECT;
    }
    if (pProbe->m_SendCount == 0) {
        return NAT_TRAVERSAL_RESULT_NOT_SENT;
    }
    if (pProbe->m_URL.GetParamValue(L"R", 0) != 0) {
        return NAT_TRAVERSAL_RESULT_NO_REPLY_RELAY;
    }
    return NAT_TRAVERSAL_RESULT_NO_REPLY;
}

// 0x0038FB78 slot 0x18 (name is ours)
bool nn::nex::NATTraversalEngine::SetConnectivityUnknown(unsigned int value)
{
    if (GetConnectivityManager() == nullptr) {
        return false;
    }
    ConnectivityManager* pManager = &Network::GetInstance()->m_ConnectivityManager;
    if (pManager->m_Unknown0x18 != 0) {
        return false;
    }
    pManager->m_Unknown0x18 = value;
    return true;
}

// 0x0038FBC0 slot 0x1C (name is ours)
bool nn::nex::NATTraversalEngine::ClearConnectivityUnknown()
{
    if (GetConnectivityManager() == nullptr) {
        return false;
    }
    ConnectivityManager* pManager = &Network::GetInstance()->m_ConnectivityManager;
    if (pManager->m_Unknown0x18 == 0) {
        return false;
    }
    pManager->m_Unknown0x18 = 0;
    return true;
}

// 0x0038FC08 (name is ours)
void nn::nex::NATTraversalEngine::DoWork()
{
    Time now = SystemClock::s_LastTime;
    if (m_IsServerKeepAlive && m_ServerKeepAliveTime < now) {
        SendServerKeepAlive(false);
        m_ServerKeepAliveTime = now + g_ServerKeepAliveInterval;
    }
    if (m_NextUpdateTime < now) {
        ProcessPendingURLs();
        ResendProbes();
        ProcessFinishedProbes();
        m_NextUpdateTime = now + g_NATTraversalUpdateInterval;
        if (m_CallContext.m_State == CallContext::STATE_FAILURE || m_CallContext.m_State == CallContext::STATE_SUCCESS) {
            // the JobCheckAndRelay is done
            m_pCheckAndRelayJob = nullptr;
            m_CallContext.Reset();
        }
        ProcessExpiredProbeRequests(now);
    }
}

// 0x0038FD30 (name after pia::inet::NexNatTraversalProtocol::sendProbe)
void nn::nex::NATTraversalEngine::SendProbe(unsigned char type, const nn::nex::StationURL* pURL, const nn::nex::Time* pSendTime, unsigned char ttl)
{
    ByteStream stream;
    AppendPrimitive(stream, type);
    AppendPrimitive(stream, m_LocalCid);
    stream.Append(pSendTime);
    if (type == PROBE_TYPE_REPLY) {
        // the port the probe came from
        u16 port = pURL->GetPortNumberInline();
        AppendPrimitive(stream, port);
    }
    if (m_pSender != nullptr) {
        Buffer* pBuffer = stream.m_pBuffer;
        m_pSender->SendTo(pURL, pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_ContentSize, ttl);
    } else {
        m_pStream->Send(pURL->GetInetAddressInline(), m_pStream->m_Type, m_pStream->m_Port, stream.m_pBuffer, ttl, 0);
    }
}

// 0x0038FE80 | fefates:bytes [tier B]
nn::nex::NATTraversalEngine::NATTraversalEngine()
    : m_Unknown0x04(0), m_pRelayInterface(nullptr), m_pUnknown0x0C(nullptr), m_pStream(nullptr), m_pSender(nullptr), m_NextUpdateTime(0),
      m_pCheckAndRelayJob(nullptr), m_IsServerKeepAlive(false), m_ServerKeepAliveTime(0), m_NextPRID(0), m_RelayHost(L""), m_RelayPort(0), m_UsesRelay(false)
{
    // nothing to do
}

// 0x0039006C slot 0x00 | fefates:bytes
// 0x0039003C (deleting dtor)
nn::nex::NATTraversalEngine::~NATTraversalEngine()
{
    // nothing to do
}

// 0x0072C200 | fefates:callgraph [tier C]
bool nn::nex::NATTraversalEngine::ShouldUseRelay(const nn::nex::StationURL& url, bool checkURL) const
{
    if (!GetRootTransport()->GetRelay()->m_IsEnabled) {
        return false;
    }
    if (String::IsEqual(m_RelayHost.m_pString, L"")) {
        return false;
    }
    if (m_RelayPort == 0) {
        return false;
    }
    if (checkURL && (url.GetParamValue(L"R", 0) != 0 || !(url.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC))) {
        return false;
    }
    return m_UsesRelay;
}

} // namespace nex
} // namespace nn

namespace __rw {
// 0x00809524 | fefates:bytes [tier B]
template __rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
                   __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
                   nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::iterator
__rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
          __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
          nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::erase(iterator it);

// 0x00809B8C | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE __rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
                          __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
                          nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::iterator
__rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
          __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
          nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}

// 0x00809CE0 | fefates:bytes [tier B]
template void __rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
                        __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
                        nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::_C_erase(Node* pNode);

// 0x00809D2C (name is ours)
template __rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
                   __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
                   nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::iterator
__rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
          __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
          nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::_C_insert(Node* pNode, Node* pParent,
                                                                                                                          const std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >& value);
} // namespace __rw
