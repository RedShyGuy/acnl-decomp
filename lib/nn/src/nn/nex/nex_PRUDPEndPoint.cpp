#include "nn/nex/nex_PRUDPEndPoint.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_EndPointEventHandler.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_PRUDPStream.h"
#include "nn/nex/nex_PacketBuffer.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_PacketBufferPacketOut.h"
#include "nn/nex/nex_PacketDispatchQueue.h"
#include "nn/nex/nex_PacketIn.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_Platform.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"
#include "nn/nex/nex_Relay.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Router.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SlidingWindow.h"
#include "nn/nex/nex_SpinTest.h"
#include "nn/nex/nex_StreamSettings.h"
#include "nn/nex/nex_Timeout.h"
#include "nn/nex/nex_TransportAdapter.h"

#include <cstring>
#include <new>

namespace nn {
namespace nex {
// the version of the PRUDP functions of the end points (name is ours)
// 0x00AE04DC (name is ours)
PseudoGlobalVariable<u8> g_PRUDPFunctionsVersion(4);
// the session id of the next end point (name is ours)
// 0x00AE04CC (name is ours)
PseudoGlobalVariable<u8> g_NextSessionId(0);
// the supported functions above the version (name is ours)
// 0x0096CA38 (name is ours)
u32 g_PRUDPSupportedFunctions = 1;
// the timeout of the retransmissions instead of the RTO and the settings (name is ours)
// 0x0096CA34 (name is ours)
u32 (*g_pRetransmissionTimeoutFunction)(u32 rto, u16 retransmissionCount);
// the time of a synchronous connection or disconnection while the core terminates (name is ours)
// 0x0096C9C0 (name is ours)
s32 g_TerminationTimeout = 2000;
// the connected end points of the main stream are not disconnected by a silent peer (name is ours)
// 0x0096C933 (name is ours)
s8 g_KeepsSilentConnections;
// duplicate unreliable data packets are not dispatched (name is ours)
// 0x0096C92A (name is ours)
bool g_IgnoresDuplicates;
// the extra header room of the packets with the setting 0x2C of the transport (defined in another
// source file; 44)
// 0x00899864
extern const u32 g_Unknown0x899864;
// the PRUDP end points of the main stream are not failed by a full window (name is ours)
// 0x0096C929 (name is ours)
u8 g_Unknown0x96C929;
// an object of unknown class: a full window fails the connection only if its slot 0x30 knows the
// node id of the peer; with it, the NAT properties are 3 (name is ours)
// 0x0096CA28 (name is ours)
void* g_pUnknown0x96CA28;

namespace {
// the results (the names are ours)
const int RESULT_NOT_CONNECTED = static_cast<int>(0x80050002);
const int RESULT_CONNECTION_ABORTED = static_cast<int>(0x80050007);
const int RESULT_DISCONNECTION_FAILED = static_cast<int>(0x80050008);
const int RESULT_INVALID_ARGUMENT = static_cast<int>(0x8001000A);
const int RESULT_NO_MEMORY = static_cast<int>(0x8005001F);
const int RESULT_WINDOW_FULL = static_cast<int>(0x8005000D);
// the flag of _Send for a reliable packet (the others are 2: 0x100, 4: 0x800, 8: m_Unknown0x130,
// 0x10: 0x200, 0x20: 0x400)
const u32 SEND_FLAG_RELIABLE = 1;
// the file of the end point (for the spin tests)
const wchar_t* const FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Transport/PRUDP/PRUDPEndPoint.cpp";
// the time of a synchronous connection without a timeout
const u32 DEFAULT_CONNECTION_TIMEOUT = 10000000;
// the retransmission interval of the disconnection
const u32 DISCONNECTION_INTERVAL = 500;
// the sleep time of the spin tests
const u32 SPIN_SLEEP_TIME = 50;

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// the size of the payload of a packet
inline u16 GetPayloadSize(const Packet* pPacket)
{
    return pPacket->m_pPayload != nullptr ? static_cast<u16>(pPacket->m_pPayload->m_ContentSize) : 0;
}

// the address of the URL
inline InetAddress* GetURLAddress(StationURL* pURL)
{
    if (!pURL->m_IsParsed) {
        pURL->Parse();
    }
    return pURL->m_pAddress;
}

// the time of a synchronous operation (shorter while the core terminates)
inline s32 GetOperationTimeout(s32 timeout)
{
    if (Core::GetInstance()->GetTerminateImmediately()) {
        return static_cast<s32>(Core::GetInstance()->m_InterruptTime.m_Value) + g_TerminationTimeout;
    }
    return timeout;
}

// the size of the buffer of an aggregate acknowledgment (new format; and old format)
const u32 ACK_BUFFER_SIZE = 128;
const u32 OLD_ACK_BUFFER_SIZE = 254;
// an aggregate acknowledgment is sent before it reaches this size (new format)
const u32 MAX_ACK_SIZE = 1000;
// the sequence ids of an aggregate acknowledgment (old format)
const int MAX_OLD_ACK_COUNT = 500;

// (inline) a buffer for an aggregate acknowledgment (from the pools if usePool); false if the pool
// has none
inline bool CreateAckBuffer(Buffer*& pBuffer, bool usePool)
{
    if (usePool) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBuffer), false);
        if (pMemory == nullptr) {
            return false;
        }
        pBuffer = ::new (pMemory) PacketBuffer(ACK_BUFFER_SIZE);
        if (pBuffer == nullptr) {
            return false;
        }
        if (pBuffer->m_pData == nullptr) {
            delete pBuffer;
            return false;
        }
        return true;
    }
    pBuffer = new Buffer(ACK_BUFFER_SIZE);
    return true;
}

// (inline) a stream for an aggregate acknowledgment of the old format (from the pools if usePool);
// false if the pool has none
inline bool CreateOldAckStream(ByteStream*& pStream, bool usePool)
{
    if (usePool) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBuffer), false);
        if (pMemory == nullptr) {
            return false;
        }
        PacketBuffer* pBuffer = ::new (pMemory) PacketBuffer(OLD_ACK_BUFFER_SIZE);
        if (pBuffer == nullptr) {
            return false;
        }
        if (pBuffer->m_pData == nullptr) {
            delete pBuffer;
            return false;
        }
        pStream = new ByteStream(pBuffer);
        pBuffer->ReleaseRef();
        return true;
    }
    pStream = new ByteStream(OLD_ACK_BUFFER_SIZE);
    return true;
}

// (inline) the functions of the peer: the ones of its version and the supported functions above
// the version (shifted by 8)
inline u64 GetPeerSupportedFunctions(u32 supportedFunctions)
{
    u64 functions = 0;
    u32 version = supportedFunctions & 0xFF;
    if (version != 0) {
        functions = 0x1000000;
        if (version >= 2) {
            functions = 0x7000000;
        }
        if (version >= 3) {
            functions |= 0x8000000;
        }
        if (version >= 4) {
            functions |= 0x10000000;
        }
    }
    return functions | (supportedFunctions >> 8);
}

// the time since the time in ms
inline s64 GetElapsedTime(const Time& now, const Time& time)
{
    return static_cast<s64>(now.m_Value - time.m_Value);
}
} // namespace

// 0x0035C67C (name is ours)
void nn::nex::PRUDPEndPointStatistics::Reset()
{
    Time now = SystemClock::GetTime();
    m_ConnectionId = 0;
    m_MinRTO = static_cast<u32>(-1);
    m_StreamId = 0;
    m_SessionId = 0;
    m_PeerSessionId = 0;
    m_FaultReason = 0;
    m_LastRTT = 0;
    m_RTO = 0;
    m_SentPackets = 0;
    m_SentUnreliablePackets = 0;
    m_SentUnreliableDataPackets = 0;
    m_SentPackets0x200 = 0;
    m_SentPackets0x400 = 0;
    m_SentPings = 0;
    m_Unknown0x38 = 0;
    m_DelayedDisconnects = 0;
    m_TooLargeSends = 0;
    m_SentBytes = 0;
    m_SentUnreliableBytes = 0;
    m_SentUnreliableDataBytes = 0;
    m_SentBytes0x200 = 0;
    m_SentBytes0x400 = 0;
    m_ReceivedPackets = 0;
    m_ReceivedUnreliablePackets = 0;
    m_ReceivedPackets0x200 = 0;
    m_ReceivedPackets0x400 = 0;
    m_ReceivedBytes = 0;
    m_ReceivedUnreliableBytes = 0;
    m_ReceivedBytes0x200 = 0;
    m_ReceivedBytes0x400 = 0;
    m_UnexpectedPongs = 0;
    m_SentAcks = 0;
    m_UnknownAcks = 0;
    m_LostUnreliablePackets = 0;
    m_Unknown0x88 = 0;
    m_DuplicatePackets = 0;
    m_SentReliablePackets = 0;
    m_SentReliableBytes = 0;
    m_FragmentedSends = 0;
    m_SendFailures = 0;
    m_Retransmissions = 0;
    m_RetransmittedBytes = 0;
    m_MaxRetransmissions = 0;
    m_FullWindows = 0;
    m_Unknown0xB0 = 0;
    m_ReceivedReliablePackets = 0;
    m_ReceivedReliableBytes = 0;
    m_MaxRTO = 0;
    m_LastPingSequenceId = 0;
    m_LastPongSequenceId = 0;
    m_ResetTime = now;
}

// 0x003682D4 (name is ours)
nn::nex::PRUDPEndPoint::PRUDPEndPoint(nn::nex::PRUDPStream* pStream, const nn::nex::StationURL* pURL, nn::nex::LogicalClockTmpl<unsigned short, short>* pStreamSequenceId, unsigned char messageVersion)
    : EndPoint(pStream, pURL), m_PeerPort(0), m_DispatchQueues(1), m_SlidingWindows(1), m_RTT(pStream->GetThisSettings()->m_Unknown0x84),
      m_LastSendTime(0), m_LastReceiveTime(0), m_Fragments(1), m_FragmentIds(1), m_Unknown0x160(0), m_PendingAcks(1),
      m_SendProfiling(L"Send", 1000)
{
    m_Statistics.Reset();
    m_ConnectionState = STATE_NOT_CONNECTED;
    m_IsPeerConnected = false;
    m_IgnoreNextFault = false;
    m_DispatchQueues[0] = new PacketDispatchQueue();
    StreamSettings* pSettings = m_pStream->GetThisSettings();
    m_SlidingWindows[0] = new SlidingWindow(m_pStream->GetThisSettings()->m_Unknown0x70, pSettings->m_Unknown0x74);
    m_UnreliableSequenceId.m_Value = 0;
    m_PeerUnreliableSequenceId.m_Value = 0;
    m_PeerStreamSequenceId.m_Value = 0;
    m_pStreamSequenceId = pStreamSequenceId;
    m_pPingPacket = nullptr;
    m_FaultEvent = 0;
    m_MessageVersion = messageVersion;
    m_SupportedFunctions = 0;
    m_SupportedFunctions = g_PRUDPFunctionsVersion.Get() | (g_PRUDPSupportedFunctions << 8);
    u8 type = m_pStream->m_Type;
    if ((type != 10 && type != 2) || Stream::GetSettings(static_cast<Stream::Type>(type))->m_Unknown0x99 != 2) {
        // without the functions of the bit 8
        m_SupportedFunctions = (m_SupportedFunctions & 0xFF) | (((m_SupportedFunctions >> 8) & ~1) << 8);
    }
    m_MaxSubstreamId = Stream::GetSettings(static_cast<Stream::Type>(m_pStream->m_Type))->m_Unknown0x98;
    m_pSynPacket = new PacketOut(this, static_cast<PacketType>(Packet::TYPE_SYN), FLAG_NEED_ACK, nullptr, 0);
    m_pSynPacket->m_MessageVersion = messageVersion;
    m_pSynPacket->m_Unknown0x8C = m_SupportedFunctions;
    m_pSynPacket->m_Unknown0x67 = static_cast<u8>(m_MaxSubstreamId);
    m_Fragments[0] = nullptr;
    m_FragmentIds[0] = 1;
    if (g_NextSessionId.Get() == 0) {
        g_NextSessionId.Get() = static_cast<u8>(SystemClock::GetTime().m_Value + Platform::GetRandomNumber(255));
    } else {
        g_NextSessionId.Get() = g_NextSessionId.Get() + 1;
    }
    if (g_NextSessionId.Get() == 0) {
        g_NextSessionId.Get() = g_NextSessionId.Get() + 1;
    }
    m_SessionId = g_NextSessionId.Get();
    m_PeerSessionId = 0;
    m_FaultReason = 0;
    m_PeerStreamSessionId = 0;
    m_PeerSupportedFunctions = 0;
    m_IsConnectSent = false;
    m_MaxSilenceTime = m_pStream->GetThisSettings()->m_MaxSilenceTime;
    m_KeepAliveTimeout = m_pStream->GetThisSettings()->m_KeepAliveTimeout;
    m_pMessageInterface = &Network::GetInstance()->m_MessageSelector;
    ScopedCS lock(GetSchedulerLock());
    m_Statistics.Reset();
}

// 0x003689C8 slot 0x00 | slot vf_0x00 of nn::nex::EndPoint
// 0x00368998 (deleting dtor)
nn::nex::PRUDPEndPoint::~PRUDPEndPoint()
{
    StopPing();
    if (m_pSynPacket != nullptr) {
        GetStream()->m_TimeoutManager.UnschedulePacketTimeout(m_pSynPacket);
        m_pSynPacket->ReleaseRef();
        m_pSynPacket = nullptr;
    }
    for (SlidingWindow** ppWindow = m_SlidingWindows.m_pBegin; ppWindow != m_SlidingWindows.m_pEnd; ppWindow++) {
        SlidingWindow* pWindow = *ppWindow;
        {
            ScopedCS lock(GetSchedulerLock());
            SlidingWindow::PacketMap::iterator it = pWindow->m_Packets.begin();
            PacketOut* pPacket = it != pWindow->m_Packets.end() ? it->second : nullptr;
            while (pPacket != nullptr) {
                GetStream()->m_TimeoutManager.UnschedulePacketTimeout(pPacket);
                ++it;
                pPacket = it != pWindow->m_Packets.end() ? it->second : nullptr;
            }
        }
        (*ppWindow)->Purge();
        if (*ppWindow != nullptr) {
            delete *ppWindow;
        }
    }
    for (PacketDispatchQueue** ppQueue = m_DispatchQueues.m_pBegin; ppQueue != m_DispatchQueues.m_pEnd; ppQueue++) {
        if (*ppQueue != nullptr) {
            delete *ppQueue;
        }
    }
    for (Buffer** ppFragment = m_Fragments.m_pBegin; ppFragment != m_Fragments.m_pEnd; ppFragment++) {
        if (*ppFragment != nullptr) {
            (*ppFragment)->ReleaseRef();
        }
    }
}

// 0x003636AC slot 0x08
void nn::nex::PRUDPEndPoint::ResetStatistics()
{
    ScopedCS lock(GetSchedulerLock());
    m_Statistics.Reset();
}

// 0x0072A65C slot 0x0C
bool nn::nex::PRUDPEndPoint::GetStatistics(void* pStatistics)
{
    if (pStatistics == nullptr) {
        return false;
    }
    PRUDPEndPointStatistics* pOut = static_cast<PRUDPEndPointStatistics*>(pStatistics);
    ScopedCS lock(GetSchedulerLock());
    // (all but the first words)
    memcpy(&pOut->m_ResetTime, &m_Statistics.m_ResetTime, offsetof(PRUDPEndPointStatistics, m_LastPongSequenceId) + 4 - offsetof(PRUDPEndPointStatistics, m_ResetTime));
    pOut->m_StreamId = static_cast<u8>(m_URL.GetParamValue(L"sid", 0));
    pOut->m_ConnectionId = m_ConnectionId;
    pOut->m_SessionId = m_SessionId;
    pOut->m_PeerSessionId = m_PeerSessionId;
    pOut->m_LastRTT = GetLastRTT();
    pOut->m_RTO = GetRTO();
    pOut->m_FaultReason = m_FaultReason;
    return true;
}

// 0x0072A8E0 slot 0x10
u32 nn::nex::PRUDPEndPoint::GetPendingReliablePacketCount()
{
    return m_SlidingWindows[0]->m_Packets.size();
}

// 0x0072A8F0 slot 0x14
void nn::nex::PRUDPEndPoint::GetPendingReliablePacketCounts(nn::nex::qVector<u32>* pCounts)
{
    pCounts->m_pEnd = pCounts->m_pBegin;
    for (int i = 0; i < m_MaxSubstreamId + 1; i++) {
        pCounts->push_back(m_SlidingWindows[i]->m_Packets.size());
    }
}

// 0x0072A8B0 slot 0x18
u16 nn::nex::PRUDPEndPoint::GetMaxSubstreamId()
{
    return static_cast<u8>(m_MaxSubstreamId);
}

// 0x00362EB4 slot 0x1C | fefates:bytes
bool nn::nex::PRUDPEndPoint::IsNotConnected()
{
    return m_ConnectionState == STATE_NOT_CONNECTED;
}

// 0x00362C18 slot 0x20 | slot vf_0x20 of nn::nex::EndPoint
bool nn::nex::PRUDPEndPoint::IsConnecting()
{
    return m_ConnectionState == STATE_CONNECTING;
}

// 0x00362FE0 slot 0x24 | fefates:bytes
bool nn::nex::PRUDPEndPoint::IsDisconnecting()
{
    return m_ConnectionState == STATE_DISCONNECTING;
}

// 0x00367CF0 slot 0x28 | fefates:bytes
bool nn::nex::PRUDPEndPoint::IsFaulty()
{
    return m_ConnectionState == STATE_FAULTY;
}

// 0x00362730 slot 0x2C | fefates:bytes
bool nn::nex::PRUDPEndPoint::IsConnected()
{
    return m_ConnectionState == STATE_CONNECTED;
}

// 0x00362FF4 slot 0x30 | slot vf_0x30 of nn::nex::EndPoint
bool nn::nex::PRUDPEndPoint::PeerIsConnected()
{
    return m_IsPeerConnected;
}

// 0x00363984 slot 0x34
bool nn::nex::PRUDPEndPoint::PeerIsNotConnected()
{
    return !m_IsPeerConnected;
}

// 0x003646BC slot 0x3C | slot vf_0x3C of nn::nex::EndPoint
void nn::nex::PRUDPEndPoint::SetKeepAliveTimeout(unsigned int milliseconds)
{
    ScopedCS lock(GetSchedulerLock());
    m_KeepAliveTimeout = milliseconds;
    if (m_pPingPacket != nullptr) {
        StopPing();
    }
    StartPing();
}

// 0x003636F0 slot 0x40 | slot vf_0x40 of nn::nex::EndPoint
void nn::nex::PRUDPEndPoint::SetMaxSilenceTime(unsigned int milliseconds)
{
    m_MaxSilenceTime = milliseconds;
}

// 0x00363A88 slot 0x44 | slot vf_0x44 of nn::nex::EndPoint
u32 nn::nex::PRUDPEndPoint::GetKeepAliveTimeout()
{
    return m_KeepAliveTimeout;
}

// 0x00363248 slot 0x48 | slot vf_0x48 of nn::nex::EndPoint
u32 nn::nex::PRUDPEndPoint::GetMaxSilenceTime()
{
    return m_MaxSilenceTime;
}

// 0x00363200 slot 0x4C | slot vf_0x4C of nn::nex::EndPoint
void nn::nex::PRUDPEndPoint::SetPeerConnected()
{
    m_IsPeerConnected = true;
}

// 0x00364718 slot 0x50 | slot vf_0x50 of nn::nex::EndPoint
void nn::nex::PRUDPEndPoint::SetPeerDisconnected()
{
    m_IsPeerConnected = false;
}

// 0x0072A8C0 slot 0x54 | slot vf_0x54 of nn::nex::EndPoint
u8 nn::nex::PRUDPEndPoint::GetConnectionState() const
{
    return m_ConnectionState;
}

// 0x00363998 slot 0x58 | fefates:bytes-fuzzy
bool nn::nex::PRUDPEndPoint::SetConnectionState(nn::nex::EndPoint::_ConnectionState state)
{
    u8 oldState = m_ConnectionState;
    if (state == STATE_CONNECTED) {
        m_IgnoreNextFault = false;
    }
    m_ConnectionState = state;
    if (m_IORequest.m_pCallback != nullptr) {
        // the request completes: successfully when the connection or disconnection finished
        int result = RESULT_CONNECTION_ABORTED;
        if ((oldState == STATE_CONNECTING && state == STATE_CONNECTED) || (oldState == STATE_DISCONNECTING && state == STATE_NOT_CONNECTED)) {
            result = qResult::SUCCESS;
        }
        CompletionCallback pCallback = m_IORequest.m_pCallback;
        m_IORequest.Clear();
        pCallback(this, qResult(result), &m_IORequest.m_Context);
    }
    return true;
}

// 0x00364724 slot 0x60 | fefates:bytes
bool nn::nex::PRUDPEndPoint::IsAnyPacketProcessed()
{
    for (int i = 0; i < m_MaxSubstreamId + 1; i++) {
        if (m_DispatchQueues[i]->m_NextSequenceId.m_Value != 1) {
            return true;
        }
    }
    return false;
}

// 0x0072A8C8 slot 0x64 | slot vf_0x64 of nn::nex::EndPoint
u32 nn::nex::PRUDPEndPoint::GetSignaledFaultError() const
{
    return m_FaultEvent;
}

// 0x0072A98C slot 0x68
u32 nn::nex::PRUDPEndPoint::GetLastRTT()
{
    return m_RTT.m_Last;
}

// 0x0072A650 slot 0x6C
u32 nn::nex::PRUDPEndPoint::GetRTO()
{
    return m_RTT.m_Smoothed >> 3;
}

// 0x0072A8D0 slot 0x7C | slot vf_0x7C of nn::nex::EndPoint
u64 nn::nex::PRUDPEndPoint::GetSupportedFunctionsFlags() const
{
    return m_PeerSupportedFunctions;
}

// 0x0036320C slot 0x88 | fefates:bytes
void nn::nex::PRUDPEndPoint::SignalFaultEvent(unsigned int error, bool isProcessed)
{
    if (m_IgnoreNextFault) {
        m_IgnoreNextFault = false;
        return;
    }
    if (isProcessed) {
        ProcessFaultEvent(error);
    } else {
        m_FaultEvent = error;
    }
    if (error != 2) {
        m_FaultReason = static_cast<u8>(error);
    }
}

// 0x0035D984 slot 0x8C | fefates:bytes
void nn::nex::PRUDPEndPoint::SetEncryptionKey(const nn::nex::Key& key)
{
    m_EncDec.m_ReliableKey = key;
    m_EncDec.m_Encryptions[0]->SetKeyInline(key);
    m_EncDec.m_Decryptions[0]->SetKeyInline(key);
    m_EncDec.ReinitUserReliableSubStreams();
}

// 0x00362FFC slot 0x90 | slot vf_0x90 of nn::nex::EndPoint
const nn::nex::Key& nn::nex::PRUDPEndPoint::GetEncryptionKey()
{
    return m_EncDec.m_ReliableKey;
}

// 0x003620B0 | fefates:bytes [tier B]
bool nn::nex::PRUDPEndPoint::CompleteIO(nn::nex::qResult result)
{
    CompletionCallback pCallback = m_IORequest.m_pCallback;
    if (pCallback == nullptr) {
        return false;
    }
    m_IORequest.Clear();
    pCallback(this, result, &m_IORequest.m_Context);
    m_IORequest.Clear();
    return true;
}

// 0x003621AC | fefates:bytes-fuzzy [tier B]
nn::nex::qResult nn::nex::PRUDPEndPoint::SendPacket(nn::nex::PacketOut* pPacket)
{
    m_Statistics.m_SentPackets++;
    m_Statistics.m_SentBytes += GetPayloadSize(pPacket);
    if (pPacket->m_TypeFlags & FLAG_UNKNOWN_0x200) {
        // to the broadcast address with the port of the peer
        InetAddress::SockAddr address = TransportAdapter::s_pInstance->m_BroadcastAddress.m_SockAddr;
        address.m_Port = GetURLAddress(&m_URL)->m_SockAddr.m_Port;
        pPacket->m_Address.m_SockAddr = address;
        m_Statistics.m_SentPackets0x200++;
        m_Statistics.m_SentBytes0x200 += GetPayloadSize(pPacket);
    } else if (pPacket->m_TypeFlags & FLAG_UNKNOWN_0x400) {
        pPacket->m_Address.m_SockAddr = TransportAdapter::s_pInstance->m_Unknown0x28.m_SockAddr;
        m_Statistics.m_SentPackets0x400++;
        m_Statistics.m_SentBytes0x400 += GetPayloadSize(pPacket);
    } else {
        pPacket->m_Address.m_SockAddr = GetURLAddress(&m_URL)->m_SockAddr;
    }
    pPacket->m_PacketSignature = m_Signature;
    pPacket->m_SessionId = m_SessionId;
    if (pPacket->m_TypeFlags & FLAG_RELIABLE) {
        m_Statistics.m_SentReliablePackets++;
        m_Statistics.m_SentReliableBytes += GetPayloadSize(pPacket);
        if (pPacket->m_Unknown0x120 == 0) {
            m_Statistics.m_FullWindows++;
            if (!m_SlidingWindows[pPacket->m_SubstreamId]->AddPacket(pPacket)) {
                return ProcessFullWindow();
            }
        }
        m_EncDec.Encode(pPacket, static_cast<Stream::Type>(m_pStream->m_Type), true);
        PacketOut* pNext = m_SlidingWindows[pPacket->m_SubstreamId]->GetNextPacketToSend();
        if (pNext != nullptr) {
            if (!m_pStream->GetThisSettings()->m_Unknown0x68) {
                m_LastSendTime = SystemClock::GetTime();
            }
            TransmitPacket(pNext);
        }
        return qResult(qResult::SUCCESS);
    }
    m_Statistics.m_SentUnreliablePackets++;
    m_Statistics.m_SentUnreliableBytes += GetPayloadSize(pPacket);
    if (!(pPacket->m_TypeFlags & (FLAG_ACK | FLAG_MULTI_ACK)) && pPacket->GetType() == Packet::TYPE_DATA) {
        m_Statistics.m_SentUnreliableDataPackets++;
        m_Statistics.m_SentUnreliableDataBytes += GetPayloadSize(pPacket);
        if (pPacket->m_TypeFlags & (FLAG_UNKNOWN_0x200 | FLAG_UNKNOWN_0x400)) {
            pPacket->m_SequenceId.m_Value = m_pStreamSequenceId->m_Value;
            m_pStreamSequenceId->m_Value++;
        } else {
            pPacket->m_SequenceId.m_Value = m_UnreliableSequenceId.m_Value;
            m_UnreliableSequenceId.m_Value++;
        }
        m_EncDec.Encode(pPacket, static_cast<Stream::Type>(m_pStream->m_Type), true);
    }
    if (m_pStream->GetThisSettings()->m_Unknown0x68) {
        m_LastSendTime = SystemClock::GetTime();
    }
    GetStream()->Send(m_PeerPort, static_cast<u8>(m_URL.GetParamValue(L"sid", 0)), pPacket, &m_EncDec.m_ReliableKey);
    return qResult(qResult::SUCCESS);
}

// 0x00362558
void nn::nex::PRUDPEndPoint::TimeToPing()
{
    Time now = SystemClock::GetTime();
    if (m_FaultEvent != 0) {
        ProcessFaultEvent(m_FaultEvent);
        return;
    }
    Network* pNetwork = Network::GetInstance();
    RootTransport* pTransport = pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
    if (pTransport->m_Unknown0xFE) {
        SignalFaultEvent(3, true);
        return;
    }
    if (static_cast<s64>(GetMaxSilenceTime()) - GetElapsedTime(now, m_LastReceiveTime) < 0) {
        if ((IsConnected() & g_KeepsSilentConnections) && m_pStream->m_pEventHandler->SwitchToRoutingConnection(this)) {
            return;
        }
        m_FaultReason = 11;
        ProcessFaultEvent(2);
        return;
    }
    StreamSettings* pSettings = m_pStream->GetThisSettings();
    if (static_cast<s64>(GetKeepAliveTimeout()) - GetElapsedTime(now, m_LastSendTime) < 0 && pSettings->m_Unknown0x95) {
        m_pPingPacket->m_SequenceId.m_Value++;
        m_pPingPacket->m_IsHeaderV1Valid = false;
        if (GetKeepAliveTimeout() != 0 && GetKeepAliveTimeout() != static_cast<u32>(-1)) {
            m_Statistics.m_SentPings++;
            m_Statistics.m_LastPingSequenceId = m_pPingPacket->m_SequenceId.m_Value;
            SendPacket(m_pPingPacket);
        }
    }
    GetStream()->m_TimeoutManager.SchedulePacketTimeout(m_pPingPacket);
}

// 0x00362744 slot 0x74 | slot vf_0x74 of nn::nex::EndPoint
nn::nex::qResult nn::nex::PRUDPEndPoint::_Disconnect(CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout)
{
    if (m_OpenCount == 0 || !IsConnected()) {
        return qResult(RESULT_NOT_CONNECTED);
    }
    SendAggregateACK();
    SetConnectionState(STATE_DISCONNECTING);
    m_IORequest.m_pConnectData = nullptr;
    m_IORequest.m_pCallback = pCallback;
    m_IORequest.m_pReplyData = nullptr;
    m_IORequest.m_Context = context;
    PacketOut* pPacket = new PacketOut(this, static_cast<PacketType>(Packet::TYPE_DISCONNECT), FLAG_RELIABLE | FLAG_NEED_ACK, nullptr, 0);
    pPacket->m_MessageVersion = m_MessageVersion;
    Timeout* pTimeout = pPacket->m_pTimeout;
    pTimeout->SetRelativeExpirationTime(GetOperationTimeout(timeout));
    pTimeout->m_Interval = DISCONNECTION_INTERVAL;
    GetStream()->m_TimeoutManager.SchedulePacketTimeout(pPacket);
    qResult result = SendPacket(pPacket);
    if (!result) {
        pPacket->ReleaseRef();
        m_IORequest.Clear();
        return result;
    }
    pPacket->ReleaseRef();
    if (pCallback != nullptr) {
        return qResult(qResult::SUCCESS);
    }
    {
        // synchronous
        SpinTest spin(SPIN_SLEEP_TIME, Core::GetInstance()->GetTerminateImmediately() ? g_TerminationTimeout : timeout);
        spin.m_Mode = 2;
        if (Core::GetInstance()->GetTerminateImmediately()) {
            while (IsDisconnecting()) {
                Scheduler::GlobalSingleThreadSpecialDispatch();
                if (!spin.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 0x25F, const_cast<wchar_t*>(L"IsDisconnecting()"))) {
                    break;
                }
            }
        } else {
            while (IsDisconnecting() && !Core::GetInstance()->GetTerminateImmediately()) {
                Scheduler::GlobalSingleThreadSpecialDispatch();
                if (!spin.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 0x266, const_cast<wchar_t*>(L"(IsDisconnecting() && !Core::GetInstance()->GetTerminateImmediately())"))) {
                    break;
                }
            }
        }
    }
    m_IORequest.Clear();
    if (IsNotConnected()) {
        return qResult(qResult::SUCCESS);
    }
    SetConnectionState(STATE_NOT_CONNECTED);
    return qResult(RESULT_DISCONNECTION_FAILED);
}

// 0x00367DA8 slot 0x70 | fefates:bytes-fuzzy
nn::nex::qResult nn::nex::PRUDPEndPoint::_Connect(nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData, CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout)
{
    if (timeout == static_cast<unsigned int>(-1)) {
        timeout = DEFAULT_CONNECTION_TIMEOUT;
    }
    if ((!PeerIsConnected() && m_ConnectionState != STATE_NOT_CONNECTED) || IsConnecting() || IsConnected()) {
        return qResult(RESULT_NOT_CONNECTED);
    }
    m_IsConnectSent = false;
    SetConnectionState(STATE_CONNECTING);
    if (pConnectData != nullptr) {
        pConnectData->AcquireRef();
    }
    m_IORequest.m_pConnectData = pConnectData;
    m_IORequest.m_pCallback = pCallback;
    m_IORequest.m_pReplyData = pReplyData;
    m_IORequest.m_Context = context;
    m_pSynPacket->m_pTimeout->SetRelativeExpirationTime(GetOperationTimeout(timeout));
    m_pSynPacket->m_Address.m_SockAddr = GetURLAddress(&m_URL)->m_SockAddr;
    TransmitPacket(m_pSynPacket);
    m_IgnoreNextFault = !PeerIsConnected();
    if (pCallback != nullptr) {
        return qResult(qResult::SUCCESS);
    }
    {
        // synchronous
        SpinTest spin(SPIN_SLEEP_TIME, Core::GetInstance()->GetTerminateImmediately() ? g_TerminationTimeout : timeout);
        spin.m_Mode = 2;
        if (Core::GetInstance()->GetTerminateImmediately()) {
            while (IsConnecting()) {
                Scheduler::GlobalSingleThreadSpecialDispatch();
                if (!spin.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 0x1F5, const_cast<wchar_t*>(L"IsConnecting()"))) {
                    break;
                }
            }
        } else {
            while (IsConnecting() && !Core::GetInstance()->GetTerminateImmediately()) {
                Scheduler::GlobalSingleThreadSpecialDispatch();
                if (!spin.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 0x1FC, const_cast<wchar_t*>(L"(IsConnecting() && !Core::GetInstance()->GetTerminateImmediately())"))) {
                    break;
                }
            }
        }
    }
    m_IORequest.Clear();
    if (IsConnected()) {
        return qResult(qResult::SUCCESS);
    }
    SetConnectionState(STATE_NOT_CONNECTED);
    return qResult(RESULT_DISCONNECTION_FAILED);
}

// 0x00362C28 (name is ours)
void nn::nex::PRUDPEndPoint::TransmitPacket(nn::nex::PacketOut* pPacket)
{
    if (pPacket->GetType() == Packet::TYPE_DISCONNECT) {
        // the disconnection waits for the reliable packets
        for (int i = 0; i < m_MaxSubstreamId + 1; i++) {
            SlidingWindow* pWindow = m_SlidingWindows[i];
            bool isPending;
            {
                ScopedCS lock(GetSchedulerLock());
                isPending = pWindow->m_DataPacketCount != 0;
            }
            if (isPending) {
                m_Statistics.m_DelayedDisconnects++;
                GetStream()->m_TimeoutManager.SchedulePacketTimeout(pPacket);
                return;
            }
        }
    }
    u16 count = ++pPacket->m_Unknown0x120;
    if (count > m_Statistics.m_MaxRetransmissions) {
        m_Statistics.m_MaxRetransmissions = count;
    }
    if (pPacket->m_Unknown0x120 > 1) {
        m_Statistics.m_Retransmissions++;
        m_Statistics.m_RetransmittedBytes += GetPayloadSize(pPacket);
    }
    StreamSettings* pSettings = m_pStream->GetThisSettings();
    u32 timeout;
    if (g_pRetransmissionTimeoutFunction != nullptr) {
        timeout = g_pRetransmissionTimeoutFunction(m_RTT.GetRTO(), pPacket->m_Unknown0x120);
    } else {
        u32 base = (pPacket->m_TypeFlags & Packet::TYPE_MASK) == Packet::TYPE_SYN ? pSettings->m_Unknown0x88 : m_RTT.GetRTO();
        f32 factor = pSettings->m_Unknown0x80 > pPacket->m_Unknown0x120 ? pSettings->m_Unknown0x78 : pSettings->m_Unknown0x7C;
        timeout = static_cast<u32>(factor * static_cast<f32>(pPacket->m_Unknown0x120 * base));
        if (timeout > pSettings->m_Unknown0x90) {
            timeout = pSettings->m_Unknown0x90;
        }
    }
    pPacket->m_pTimeout->m_Interval = timeout;
    GetStream()->m_TimeoutManager.SchedulePacketTimeout(pPacket);
    if (m_pStream->GetThisSettings()->m_Unknown0x68) {
        m_LastSendTime = SystemClock::GetTime();
    }
    if (!GetStream()->Send(m_PeerPort, static_cast<u8>(m_URL.GetParamValue(L"sid", 0)), pPacket, &m_EncDec.m_ReliableKey)) {
        m_Statistics.m_SendFailures++;
    }
}

// 0x00362EC8 (name is ours)
void nn::nex::PRUDPEndPoint::DispatchData(nn::nex::Buffer* pData, nn::nex::PacketIn* pPacket)
{
    if (pPacket->m_TypeFlags & 0x800) {
        // to the object of the kind 13 of the instance table (its class is not known; the call
        // goes through slot 0x14 of its vtable)
        typedef void (*Function)(void* pObject, EndPoint* pEndPoint, Buffer* pData, u16 typeFlags, u8 substreamId);
        ScopedCS lock(GetSchedulerLock());
        InstanceControl* pControl = InstanceTable::s_Instance.GetInstance(13, PseudoSingleton::GetCurrentContext());
        void* pObject = pControl != nullptr ? pControl->m_pOwner : nullptr;
        if (pObject != nullptr) {
            (*reinterpret_cast<Function**>(pObject))[5](pObject, this, pData, pPacket->m_TypeFlags, pPacket->m_SubstreamId);
        }
        return;
    }
    if (m_pEventHandler != nullptr) {
        ScopedCS lock(GetSchedulerLock());
        m_pEventHandler->Receive(this, pData, pPacket->m_SubstreamId);
    }
}

// 0x00363004 (name is ours)
void nn::nex::PRUDPEndPoint::SendAck(nn::nex::Buffer* pPayload, unsigned short sequenceId, bool isNewFormat)
{
    u16 flags = FLAG_ACK | FLAG_MULTI_ACK;
    PacketOut* pPacket;
    if (pPayload->m_Flags & Buffer::FLAG_USE_POOL) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketOut), false);
        if (pMemory == nullptr) {
            return;
        }
        pPacket = ::new (pMemory) PacketBufferPacketOut(this, static_cast<PacketType>(Packet::TYPE_DATA), flags, pPayload, 0);
        if ((pPacket->m_TypeFlags & FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr) {
            delete pPacket;
            return;
        }
    } else {
        pPacket = new PacketOut(this, static_cast<PacketType>(Packet::TYPE_DATA), flags, pPayload, 0);
    }
    pPacket->m_MessageVersion = m_MessageVersion;
    pPacket->m_SequenceId.m_Value = sequenceId;
    pPacket->m_SubstreamId = isNewFormat;
    SendPacket(pPacket);
    pPacket->ReleaseRef();
}

// 0x0036311C | fefates:bytes [tier B]
void nn::nex::PRUDPEndPoint::SendAggregateACK()
{
    if (m_MessageVersion == 0 || !(m_PeerSupportedFunctions & 0x1000000)) {
        return;
    }
    if (GetSupportedFunctionsFlags() & 0x4000000) {
        SendAggregateACKNew();
        return;
    }
    if (m_DispatchQueues[0] == nullptr || !m_PendingAcks.Get(0)) {
        return;
    }
    m_PendingAcks.Clear(0);
    SendAggregateACKOld();
}

// 0x00363250 | fefates:bytes [tier B]
void nn::nex::PRUDPEndPoint::InitUnreliableSeq(nn::nex::PacketOut* pPacket)
{
    if (pPacket->m_MessageVersion != 0 && m_UnreliableSequenceId.m_Value == 0) {
        m_UnreliableSequenceId.m_Value = static_cast<u16>(SystemClock::GetTime().m_Value + Platform::GetRandomNumber(0xFFFF));
        if (m_UnreliableSequenceId.m_Value == 0) {
            m_UnreliableSequenceId.m_Value = 1;
        }
        pPacket->m_Unknown0x48.m_Value = m_UnreliableSequenceId.m_Value;
    }
}

// 0x003632DC
void nn::nex::PRUDPEndPoint::ProcessFaultEvent(unsigned int error)
{
    m_FaultEvent = error;
    {
        ScopedCS lock(Network::GetInstance()->m_FaultLock);
        Network::FaultedEndPoint fault;
        fault.m_pEndPoint = this;
        fault.m_ConnectionId = m_ConnectionId;
        Network::GetInstance()->m_FaultedEndPoints.push_back(fault);
    }
    if (IsConnected() || PeerIsConnected()) {
        if (error != 2 && error != 3) {
            for (int i = 0; i < 2; i++) {
                PacketOut* pPacket = new PacketOut(this, static_cast<PacketType>(Packet::TYPE_DISCONNECT), 0, nullptr, 0);
                pPacket->m_MessageVersion = m_MessageVersion;
                SendPacket(pPacket);
                pPacket->ReleaseRef();
            }
        }
        SetConnectionState(STATE_FAULTY);
        StopPing();
        if (m_pEventHandler != nullptr) {
            m_pEventHandler->FaultDetection(this, error);
        }
        SetPeerDisconnected();
        GetStream()->ReleaseEndPoint(this);
        return;
    }
    if (m_ConnectionState == STATE_CONNECTING || m_ConnectionState == STATE_DISCONNECTING) {
        SetConnectionState(STATE_NOT_CONNECTED);
    }
}

// 0x003636F8 (name is ours)
void nn::nex::PRUDPEndPoint::ProcessAck(nn::nex::Time time, nn::nex::LogicalClockTmpl<unsigned short, short> sequenceId, unsigned char substreamId)
{
    PacketOut* pPacket = m_SlidingWindows[substreamId]->GetPacket(SequenceId(sequenceId.m_Value));
    if (pPacket == nullptr || !GetStream()->m_TimeoutManager.UnschedulePacketTimeout(pPacket)) {
        m_Statistics.m_UnknownAcks++;
        return;
    }
    if (m_pStream->GetThisSettings()->m_Unknown0x8C > pPacket->m_Unknown0x120) {
        UpdateRTT(time, pPacket);
    }
    u64 sendTime = pPacket->m_Unknown0x100.m_Value;
    m_SlidingWindows[substreamId]->Acknowledge(SequenceId(sequenceId.m_Value), &m_FragmentIdCounts);
    {
        // the older packets that were sent before this one are sent again (at most 2)
        ScopedCS lock(GetSchedulerLock());
        SlidingWindow* pWindow = m_SlidingWindows[substreamId];
        SlidingWindow::PacketMap::iterator it = pWindow->m_Packets.begin();
        PacketOut* pOlder = it != pWindow->m_Packets.end() ? it->second : nullptr;
        u16 limit = sequenceId.m_Value - 2;
        int count = 0;
        while (pOlder != nullptr) {
            if (static_cast<s16>(pOlder->m_SequenceId.m_Value - limit) > 0) {
                break;
            }
            if (pOlder->m_Unknown0x100.m_Value < sendTime && pOlder->m_Unknown0x120 < 5) {
                GetStream()->m_TimeoutManager.UnschedulePacketTimeout(pOlder);
                TransmitPacket(pOlder);
                count++;
            }
            ++it;
            pOlder = it != m_SlidingWindows[substreamId]->m_Packets.end() ? it->second : nullptr;
            if (count >= 2) {
                break;
            }
        }
    }
    PacketOut* pNext = m_SlidingWindows[substreamId]->GetNextPacketToSend();
    if (pNext != nullptr) {
        if (!m_pStream->GetThisSettings()->m_Unknown0x68) {
            m_LastSendTime = SystemClock::GetTime();
        }
        TransmitPacket(pNext);
    }
}

// 0x00363A90
void nn::nex::PRUDPEndPoint::SendAggregateACKNew()
{
    bool usePool = true;
    u8 type = m_pStream->m_Type;
    if (type == 10 || type == 2) {
        usePool = false;
    }
    Buffer* pBuffer;
    if (!CreateAckBuffer(pBuffer, usePool)) {
        return;
    }
    ByteStream stream(pBuffer);
    pBuffer->ReleaseRef();
    qVector<u16> sequenceIds;
    for (int i = 0; i < m_MaxSubstreamId + 1; i++) {
        if (m_DispatchQueues[i] == nullptr || !m_PendingAcks.Get(i)) {
            continue;
        }
        m_PendingAcks.Clear(i);
        PacketDispatchQueue* pQueue = m_DispatchQueues[i];
        // the last sequence id up to which all packets were received, then the ones behind it
        u16 lastSequenceId = pQueue->m_NextSequenceId.m_Value - 1;
        PacketDispatchQueue::PacketMap::iterator it = pQueue->m_Packets.begin();
        while (it != pQueue->m_Packets.end() && it->first.m_Value == lastSequenceId + 1) {
            lastSequenceId = it->first.m_Value;
            ++it;
        }
        sequenceIds.clear();
        for (; it != pQueue->m_Packets.end(); ++it) {
            sequenceIds.push_back(it->first.m_Value);
        }
        u32 size = sequenceIds.size() * sizeof(u16) + 4;
        if (!stream.m_pBuffer->AttemptExpand(size)) {
            break;
        }
        if (stream.m_pBuffer->m_ContentSize + size >= MAX_ACK_SIZE) {
            SendAck(stream.m_pBuffer, 0, true);
            Buffer* pNewBuffer;
            if (!CreateAckBuffer(pNewBuffer, usePool)) {
                return;
            }
            stream = ByteStream(pNewBuffer);
            pNewBuffer->ReleaseRef();
        }
        u8 byte = static_cast<u8>(i);
        stream.m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
        byte = static_cast<u8>(sequenceIds.size());
        stream.m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
        stream.m_pBuffer->AppendData(&lastSequenceId, sizeof(lastSequenceId), static_cast<unsigned>(-1));
        for (u16* p = sequenceIds.begin(); p != sequenceIds.end(); p++) {
            stream.m_pBuffer->AppendData(p, sizeof(u16), static_cast<unsigned>(-1));
        }
    }
    if (stream.m_pBuffer->m_ContentSize != 0) {
        SendAck(stream.m_pBuffer, 0, true);
    }
}

// 0x003641EC
void nn::nex::PRUDPEndPoint::SendAggregateACKOld()
{
    // the last sequence id up to which all packets of the substream 0 were received
    PacketDispatchQueue* pQueue = m_DispatchQueues[0];
    u16 lastSequenceId = pQueue->m_NextSequenceId.m_Value - 1;
    PacketDispatchQueue::PacketMap::iterator it = pQueue->m_Packets.begin();
    while (it != pQueue->m_Packets.end() && it->first.m_Value == lastSequenceId + 1) {
        lastSequenceId = it->first.m_Value;
        ++it;
    }
    bool usePool = true;
    u8 type = m_pStream->m_Type;
    if (type == 10 || type == 2) {
        usePool = false;
    }
    ByteStream* pStream;
    if (!CreateOldAckStream(pStream, usePool)) {
        return;
    }
    // the packets behind it
    int count = 0;
    int sentCount = 0;
    for (; it != pQueue->m_Packets.end(); ++it) {
        if (!pStream->m_pBuffer->AttemptExpand(sizeof(u16))) {
            break;
        }
        u16 sequenceId = it->first.m_Value;
        pStream->m_pBuffer->AppendData(&sequenceId, sizeof(sequenceId), static_cast<unsigned>(-1));
        if (++count >= MAX_OLD_ACK_COUNT) {
            SendAck(pStream->m_pBuffer, lastSequenceId, false);
            count = 0;
            delete pStream;
            if (!CreateOldAckStream(pStream, usePool)) {
                return;
            }
            sentCount++;
        }
    }
    if (count > 0 || sentCount == 0) {
        SendAck(pStream->m_pBuffer, lastSequenceId, false);
    }
    delete pStream;
}

// 0x00364780 (name is ours)
void nn::nex::PRUDPEndPoint::ProcessPacket(nn::nex::PacketIn* pPacket)
{
    bool isDispatched = false;
    u16 flags = pPacket->m_TypeFlags;
    u32 type = flags & Packet::TYPE_MASK;
    u8 streamType = m_pStream->m_Type;
    if (streamType == 10 || streamType == 2) {
        // the peer refuses the connection
        if (type == Packet::TYPE_SYN && (flags & FLAG_ACK) && IsConnecting() && !m_IsConnectSent && (pPacket->m_Unknown0x8C & 0x100)) {
            ProcessFaultEvent(8);
            return;
        }
    } else if ((flags & ~(FLAG_ACK | FLAG_MULTI_ACK)) == 0 && type == Packet::TYPE_DATA) {
        // (as in the original: this condition is never true) a packet is dropped while the pools
        // are short of buffers
        if (!(flags & 0x800) && PacketBufferManager::GetInstance() != nullptr &&
            !PacketBufferManager::GetInstance()->HasFreeBuffers(PacketBufferManager::MAX_POOL_SIZE, PacketBufferManager::GetInstance()->m_Unknown0x28)) {
            return;
        }
    }
    if (m_pStream->GetThisSettings()->m_Unknown0x68) {
        m_LastReceiveTime = SystemClock::s_LastTime;
    }
    flags = pPacket->m_TypeFlags;
    if ((flags & (FLAG_UNKNOWN_0x200 | FLAG_UNKNOWN_0x400)) && m_Unknown0x84 != 2) {
        if (m_PeerSessionId == 0 || m_ConnectionState != STATE_CONNECTED || m_Unknown0x84 == 1) {
            return;
        }
    }
    if (!(flags & FLAG_MULTI_ACK) && pPacket->m_SubstreamId > m_MaxSubstreamId) {
        return;
    }
    if (!(flags & (FLAG_UNKNOWN_0x200 | FLAG_UNKNOWN_0x400))) {
        if (((flags & Packet::TYPE_MASK) == Packet::TYPE_SYN && (flags & FLAG_ACK)) || (flags & (Packet::TYPE_MASK | FLAG_ACK)) == Packet::TYPE_CONNECT) {
            // the connection signature
            u8 version = pPacket->m_MessageVersion;
            if (version == 0 || (version == 1 && (pPacket->m_Unknown0x8C & 0xFF) < 2)) {
                m_Signature = pPacket->m_Signature;
            } else {
                m_PeerSignature = pPacket->m_Signature;
            }
        }
        if (type != Packet::TYPE_SYN) {
            if (m_PeerSessionId != 0) {
                if (pPacket->m_SessionId != m_PeerSessionId) {
                    if (type == Packet::TYPE_CONNECT && !(pPacket->m_TypeFlags & FLAG_ACK)) {
                        ProcessFaultEvent(1);
                    }
                    return;
                }
            } else if ((pPacket->m_TypeFlags & Packet::TYPE_MASK) == Packet::TYPE_CONNECT) {
                m_PeerSessionId = pPacket->m_SessionId;
                if (pPacket->m_MessageVersion != 0 && pPacket->m_Unknown0x48.m_Value != 0) {
                    m_PeerUnreliableSequenceId.m_Value = pPacket->m_Unknown0x48.m_Value;
                }
                // the keys of the unreliable packets
                u32 peerType = pPacket->GetStreamType();
                if ((peerType == 1 || peerType == 9) && Stream::GetSettings(static_cast<Stream::Type>(peerType))->m_Unknown0xA0) {
                    LiteBuffer<32> keyData(Stream::GetSettings(static_cast<Stream::Type>(pPacket->GetStreamType()))->m_Unknown0xA4);
                    Key key(keyData.m_Data, keyData.m_Size);
                    if (key.GetLength() != 0) {
                        m_EncDec.m_ReliableKey = key;
                        m_EncDec.GenerateKey(m_EncDec.m_ReliableKey, s_UnreliableKeySalts, s_UnreliableKeySalts + 8, &m_EncDec.m_UnreliableKey);
                        m_EncDec.GenerateKey(m_EncDec.m_ReliableKey, s_UnreliableKeySalts + 16, s_UnreliableKeySalts + 24, &m_EncDec.m_UnreliableKeyHigh);
                        m_EncDec.SetReliableEncryptionKey(key, m_PeerSessionId, m_SessionId);
                    }
                }
            } else if (pPacket->m_SessionId != 0) {
                return;
            }
        }
    }
    flags = pPacket->m_TypeFlags;
    if (flags & FLAG_NEED_ACK) {
        if ((flags & Packet::TYPE_MASK) == Packet::TYPE_DATA && m_MessageVersion != 0 && (m_PeerSupportedFunctions & 0x1000000)) {
            // acknowledged with the next aggregate acknowledgment
            m_PendingAcks.Set(pPacket->m_SubstreamId);
        } else {
            AcknowledgePacket(pPacket);
        }
    }
    if (!m_pStream->GetThisSettings()->m_Unknown0x68) {
        m_LastReceiveTime = SystemClock::s_LastTime;
    }
    if (pPacket->m_TypeFlags & FLAG_RELIABLE) {
        m_Statistics.m_ReceivedReliablePackets++;
        m_Statistics.m_ReceivedReliableBytes += GetPayloadSize(pPacket);
        m_DispatchQueues[pPacket->m_SubstreamId]->Queue(pPacket);
    }
    switch (type) {
    case Packet::TYPE_SYN:
        if (IsConnecting() && (pPacket->m_TypeFlags & FLAG_ACK) && !m_IsConnectSent) {
            // the connection request
            PacketOut* pConnect = new PacketOut(this, static_cast<PacketType>(Packet::TYPE_CONNECT), FLAG_RELIABLE | FLAG_NEED_ACK, m_IORequest.m_pConnectData, 0);
            m_MessageVersion = pPacket->m_MessageVersion;
            pConnect->m_MessageVersion = pPacket->m_MessageVersion;
            m_SupportedFunctions = pPacket->m_Unknown0x8C;
            DecideMaxUserReliableSubStreams(pPacket->m_Unknown0x67);
            m_PeerSupportedFunctions = GetPeerSupportedFunctions(m_SupportedFunctions);
            pConnect->m_Unknown0x8C = m_SupportedFunctions;
            pConnect->m_Unknown0x67 = static_cast<u8>(m_MaxSubstreamId);
            InitUnreliableSeq(pConnect);
            SignatureBytes signature;
            signature.m_Size = 0;
            m_pMessageInterface->CalcSignature(pPacket, PRUDPMessageInterface::SIGNATURE_METHOD_ADDRESS, nullptr, static_cast<Stream::Type>(pPacket->GetStreamType()),
                                               &GetStream()->m_SignatureGenerator, nullptr, signature);
            pConnect->m_Signature = signature;
            if (GetSupportedFunctionsFlags() & 0x2000000) {
                m_Signature = signature;
            } else {
                m_PeerSignature = signature;
            }
            pConnect->m_pTimeout->m_ExpirationTime = m_pSynPacket->m_pTimeout->m_ExpirationTime;
            SendPacket(pConnect);
            pConnect->ReleaseRef();
            GetStream()->m_TimeoutManager.UnschedulePacketTimeout(m_pSynPacket);
            m_IsConnectSent = true;
        }
        break;
    case Packet::TYPE_CONNECT:
        if (IsConnecting() && (pPacket->m_TypeFlags & FLAG_ACK)) {
            // the reply to the connection request
            if (m_IORequest.m_pReplyData != nullptr) {
                *m_IORequest.m_pReplyData = *pPacket->m_pPayload;
                m_IORequest.m_pReplyData = nullptr;
            }
            ProcessAck(pPacket->m_Unknown0x100, SequenceId(pPacket->m_SequenceId.m_Value), 0);
            SetConnectionState(STATE_CONNECTED);
        } else if (!(pPacket->m_TypeFlags & FLAG_ACK)) {
            SetPeerConnected();
        }
        StartPing();
        break;
    case Packet::TYPE_DATA:
        if (IsConnecting()) {
            break;
        }
        flags = pPacket->m_TypeFlags;
        if (flags & FLAG_ACK) {
            ProcessAck(pPacket->m_Unknown0x100, SequenceId(pPacket->m_SequenceId.m_Value), pPacket->m_SubstreamId);
            break;
        }
        if (flags & FLAG_MULTI_ACK) {
            if (GetSupportedFunctionsFlags() & 0x4000000) {
                ProcessAggregateAckNew(pPacket);
            } else {
                ProcessAggregateAckOld(pPacket);
            }
            break;
        }
        isDispatched = true;
        if (flags & FLAG_RELIABLE) {
            goto dispatch;
        }
        {
            // the lost and the duplicate unreliable packets
            s16 difference;
            if (flags & (FLAG_UNKNOWN_0x200 | FLAG_UNKNOWN_0x400)) {
                if (m_PeerStreamSessionId == 0) {
                    m_PeerStreamSequenceId.m_Value = pPacket->m_SequenceId.m_Value;
                    m_PeerStreamSessionId = pPacket->m_SessionId;
                }
                if (m_PeerStreamSessionId != pPacket->m_SessionId) {
                    m_PeerStreamSessionId = pPacket->m_SessionId;
                }
                difference = static_cast<s16>(pPacket->m_SequenceId.m_Value - m_PeerStreamSequenceId.m_Value);
            } else {
                difference = static_cast<s16>(pPacket->m_SequenceId.m_Value - m_PeerUnreliableSequenceId.m_Value);
            }
            if (difference >= 0) {
                m_Statistics.m_LostUnreliablePackets += difference;
                if (pPacket->m_TypeFlags & (FLAG_UNKNOWN_0x200 | FLAG_UNKNOWN_0x400)) {
                    m_PeerStreamSequenceId.m_Value = pPacket->m_SequenceId.m_Value + 1;
                } else {
                    m_PeerUnreliableSequenceId.m_Value = pPacket->m_SequenceId.m_Value + 1;
                }
            } else {
                m_Statistics.m_DuplicatePackets++;
                if (g_IgnoresDuplicates) {
                    isDispatched = false;
                }
            }
        }
        break;
    case Packet::TYPE_DISCONNECT:
        if (pPacket->m_TypeFlags & FLAG_ACK) {
            if (IsDisconnecting()) {
                SetConnectionState(STATE_NOT_CONNECTED);
            }
            ProcessAck(pPacket->m_Unknown0x100, SequenceId(pPacket->m_SequenceId.m_Value), 0);
        } else {
            SendAggregateACK();
            bool wasConnected = PeerIsConnected();
            SetPeerDisconnected();
            if (wasConnected && m_pEventHandler != nullptr) {
                m_pEventHandler->PeerDisconnected(this);
            }
            if (m_pStream != nullptr && (m_pStream->m_Type == 10 || m_pStream->m_Type == 2) && IsConnected() && !(pPacket->m_TypeFlags & FLAG_NEED_ACK)) {
                SetConnectionState(STATE_FAULTY);
                StopPing();
            }
            GetStream()->ReleaseEndPoint(this);
        }
        break;
    case Packet::TYPE_PING:
        if ((pPacket->m_TypeFlags & FLAG_ACK) && m_pPingPacket != nullptr) {
            if (m_pPingPacket->m_SequenceId.m_Value == pPacket->m_SequenceId.m_Value) {
                UpdateRTT(pPacket->m_Unknown0x100, m_pPingPacket);
            }
        } else {
            m_Statistics.m_UnexpectedPongs++;
            m_Statistics.m_LastPongSequenceId = pPacket->m_SequenceId.m_Value;
        }
        break;
    }
    if (!(pPacket->m_TypeFlags & FLAG_RELIABLE)) {
        m_Statistics.m_ReceivedPackets++;
        m_Statistics.m_ReceivedBytes += GetPayloadSize(pPacket);
        if (pPacket->m_TypeFlags & FLAG_UNKNOWN_0x200) {
            m_Statistics.m_ReceivedPackets0x200++;
            m_Statistics.m_ReceivedBytes0x200 += GetPayloadSize(pPacket);
        } else if (pPacket->m_TypeFlags & FLAG_UNKNOWN_0x400) {
            m_Statistics.m_ReceivedPackets0x400++;
            m_Statistics.m_ReceivedBytes0x400 += GetPayloadSize(pPacket);
        }
        if (isDispatched) {
            m_Statistics.m_ReceivedUnreliablePackets++;
            m_Statistics.m_ReceivedUnreliableBytes += GetPayloadSize(pPacket);
            DispatchUnreliablePacket(pPacket, pPacket->m_Unknown0x100);
        }
    }
dispatch:
    // the reliable packets of the substream that are next in order
    PacketIn* pNext = m_DispatchQueues[pPacket->m_SubstreamId]->GetNextToDispatch();
    while (pNext != nullptr) {
        if (pNext->GetType() == Packet::TYPE_DATA) {
            m_EncDec.Decode(pNext);
            if (pNext->m_TypeFlags & FLAG_RELIABLE) {
                if (Defrag(pNext)) {
                    DispatchData(m_Fragments[pNext->m_SubstreamId], pNext);
                    m_FragmentIds[pNext->m_SubstreamId] = 1;
                    m_Fragments[pNext->m_SubstreamId]->ReleaseRef();
                    m_Fragments[pNext->m_SubstreamId] = nullptr;
                }
            } else {
                DispatchData(pNext->m_pPayload, pNext);
            }
        }
        m_DispatchQueues[pPacket->m_SubstreamId]->m_Packets.EraseInline(SequenceId(pNext->m_SequenceId.m_Value));
        pNext->ReleaseRef();
        pNext = m_DispatchQueues[pPacket->m_SubstreamId]->GetNextToDispatch();
    }
}

// 0x003656A8 | fefates:bytes [tier B]
bool nn::nex::PRUDPEndPoint::IsDuplicateReorderingPacket(nn::nex::PacketIn* pPacket)
{
    u16 flags = pPacket->m_TypeFlags;
    if ((flags & Packet::TYPE_MASK) == Packet::TYPE_DATA && (flags & FLAG_RELIABLE)) {
        return false;
    }
    if (flags & FLAG_ACK) {
        if (pPacket->m_SubstreamId > m_MaxSubstreamId) {
            return false;
        }
        return static_cast<s16>(m_SlidingWindows[pPacket->m_SubstreamId]->m_FirstUnacked.m_Value - pPacket->m_SequenceId.m_Value) > 0;
    }
    if (flags & FLAG_MULTI_ACK) {
        return false;
    }
    bool isOld;
    if (flags & (FLAG_UNKNOWN_0x200 | FLAG_UNKNOWN_0x400)) {
        if (m_PeerStreamSessionId == 0) {
            return false;
        }
        isOld = static_cast<s16>(m_PeerStreamSequenceId.m_Value - pPacket->m_SequenceId.m_Value) > 0;
    } else {
        isOld = static_cast<s16>(m_PeerUnreliableSequenceId.m_Value - pPacket->m_SequenceId.m_Value) > 0;
    }
    if (!isOld) {
        return false;
    }
    m_Statistics.m_DuplicatePackets++;
    return true;
}

// 0x00365794 (name is ours)
nn::nex::qResult nn::nex::PRUDPEndPoint::ProcessFullWindow()
{
    qResult result(true);
    m_Statistics.m_Unknown0xB0++;
    m_FaultReason = 10;
    SignalFaultEvent(2, true);
    u8 type = m_pStream->m_Type;
    if (type == 10) {
        return result;
    }
    if (type == 1) {
        if (g_Unknown0x96C929 == 1) {
            return result;
        }
        if (g_pUnknown0x96CA28 != nullptr) {
            // slot 0x30 of the object (its class is not known)
            typedef bool (*Function)(void* pObject, u16 nodeId);
            if (!(*reinterpret_cast<Function**>(g_pUnknown0x96CA28))[12](g_pUnknown0x96CA28, static_cast<u16>(m_URL.GetParamValue(L"NodeId", 0)))) {
                return result;
            }
        }
    }
    result = qResult(RESULT_WINDOW_FULL);
    if (m_pFaultResultHolder != nullptr && m_pFaultResultHolder->IsSuccess()) {
        m_pFaultResultHolder->SetResult(result);
    }
    return result;
}

// 0x00365900 (name is ours)
void nn::nex::PRUDPEndPoint::ProcessAggregateAckNew(nn::nex::PacketIn* pPacket)
{
    // per substream: the substream id, the number of sequence ids, the last sequence id up to which
    // all packets were received, then the sequence ids behind it
    ByteStream stream(pPacket->m_pPayload);
    while (stream.m_Position < stream.m_pBuffer->m_ContentSize) {
        u8 substreamId;
        u8 count;
        u16 lastSequenceId;
        stream.ExtractPrimitive(&substreamId);
        stream.ExtractPrimitive(&count);
        stream.ExtractPrimitive(&lastSequenceId);
        if (stream.m_IsError) {
            break;
        }
        if (stream.m_pBuffer->m_ContentSize - stream.m_Position < static_cast<u32>(count * sizeof(u16))) {
            stream.m_IsError = true;
            break;
        }
        if (substreamId > m_MaxSubstreamId) {
            u32 position = count * sizeof(u16) + stream.m_Position;
            if (stream.m_pBuffer->m_ContentSize >= position) {
                stream.m_Position = position;
            }
            continue;
        }
        // the packets up to the last sequence id
        qVector<SequenceId> sequenceIds;
        {
            SlidingWindow* pWindow = m_SlidingWindows[substreamId];
            ScopedCS lock(GetSchedulerLock());
            SlidingWindow::PacketMap::iterator it = pWindow->m_Packets.begin();
            for (;;) {
                PacketOut* pOut = it != m_SlidingWindows[substreamId]->m_Packets.end() ? it->second : nullptr;
                if (pOut == nullptr) {
                    break;
                }
                u16 sequenceId = pOut->m_SequenceId.m_Value;
                if (lastSequenceId != sequenceId && static_cast<s16>(lastSequenceId - sequenceId) <= 0) {
                    break;
                }
                sequenceIds.push_back(it->first);
                ++it;
            }
        }
        for (u32 i = 0; i < sequenceIds.size(); i++) {
            ProcessAck(pPacket->m_Unknown0x100, sequenceIds[i], substreamId);
        }
        for (int i = 0; i < count; i++) {
            u16 sequenceId;
            if (!stream.ExtractPrimitiveInline(&sequenceId)) {
                break;
            }
            ProcessAck(pPacket->m_Unknown0x100, SequenceId(sequenceId), substreamId);
        }
    }
}

// 0x00365E24 (name is ours)
void nn::nex::PRUDPEndPoint::ProcessAggregateAckOld(nn::nex::PacketIn* pPacket)
{
    // the packets of the substream 0 up to the sequence id of the packet
    qVector<SequenceId> sequenceIds;
    {
        SlidingWindow* pWindow = m_SlidingWindows[0];
        ScopedCS lock(GetSchedulerLock());
        u16 lastSequenceId = pPacket->m_SequenceId.m_Value;
        SlidingWindow::PacketMap::iterator it = pWindow->m_Packets.begin();
        for (;;) {
            PacketOut* pOut = it != m_SlidingWindows[0]->m_Packets.end() ? it->second : nullptr;
            if (pOut == nullptr) {
                break;
            }
            u16 sequenceId = pOut->m_SequenceId.m_Value;
            if (lastSequenceId != sequenceId && static_cast<s16>(lastSequenceId - sequenceId) <= 0) {
                break;
            }
            sequenceIds.push_back(it->first);
            ++it;
        }
    }
    for (u32 i = 0; i < sequenceIds.size(); i++) {
        ProcessAck(pPacket->m_Unknown0x100, sequenceIds[i], 0);
    }
    // the sequence ids of the payload
    ByteStream stream(pPacket->m_pPayload);
    while (stream.m_Position < stream.m_pBuffer->m_ContentSize) {
        u16 sequenceId = 0;
        if (!stream.ExtractPrimitive(&sequenceId)) {
            break;
        }
        ProcessAck(pPacket->m_Unknown0x100, SequenceId(sequenceId), 0);
    }
}

// 0x00366288
void nn::nex::PRUDPEndPoint::DecideMaxUserReliableSubStreams(unsigned short maxSubstreamId)
{
    m_MaxSubstreamId = maxSubstreamId;
    m_DispatchQueues.reserve(maxSubstreamId + 1);
    m_SlidingWindows.reserve(maxSubstreamId + 1);
    m_PendingAcks.resize(maxSubstreamId + 1, false);
    m_Fragments.ResizeInline(maxSubstreamId + 1, nullptr);
    m_FragmentIds.resize(maxSubstreamId + 1, 1);
    m_EncDec.DecideMaxUserReliableSubStreams(maxSubstreamId);
    for (int i = 0; i < maxSubstreamId + 1; i++) {
        m_DispatchQueues.push_back(new PacketDispatchQueue());
        StreamSettings* pSettings = m_pStream->GetThisSettings();
        m_SlidingWindows.push_back(new SlidingWindow(static_cast<u16>(m_pStream->GetThisSettings()->m_Unknown0x70), pSettings->m_Unknown0x74));
    }
}

// 0x00366C08 (name is ours)
nn::nex::qResult nn::nex::PRUDPEndPoint::SendFragmented(nn::nex::Buffer* pPayload, unsigned int bufferSize, unsigned int fragmentSize, unsigned int type, unsigned short flags,
                                                        unsigned char unknown0x130, unsigned int id, unsigned char substreamId)
{
    qVector<PacketOut*> packets;
    u32 offset = 0;
    u32 count = 0;
    u8 fragmentId = 1;
    while (pPayload->m_ContentSize > offset) {
        // the fragment of the payload
        Buffer* pFragment;
        if (pPayload->m_Flags & Buffer::FLAG_USE_POOL) {
            void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBuffer), false);
            if (pMemory == nullptr) {
                break;
            }
            pFragment = ::new (pMemory) PacketBuffer(bufferSize);
            if (pFragment == nullptr) {
                break;
            }
            if (pFragment->m_pData == nullptr) {
                delete pFragment;
                break;
            }
        } else {
            pFragment = new Buffer(bufferSize);
        }
        u32 size = pPayload->m_ContentSize - offset;
        if (size > fragmentSize) {
            size = fragmentSize;
        }
        pFragment->AppendData(pPayload->m_pData + pPayload->m_HeaderSize + offset, size, static_cast<unsigned>(-1));
        PacketOut* pPacket = nullptr;
        if (pPayload->m_Flags & Buffer::FLAG_USE_POOL) {
            void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketOut), false);
            if (pMemory != nullptr) {
                pPacket = ::new (pMemory) PacketBufferPacketOut(this, static_cast<PacketType>(type), flags, pFragment, id);
            }
            if (pPacket == nullptr || ((pPacket->m_TypeFlags & FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr)) {
                pFragment->ReleaseRef();
                if (pPacket != nullptr) {
                    delete pPacket;
                }
                break;
            }
        } else {
            pPacket = new PacketOut(this, static_cast<PacketType>(type), flags, pFragment, id);
        }
        pPacket->m_Unknown0x130 = unknown0x130;
        pPacket->m_MessageVersion = m_MessageVersion;
        pPacket->m_SubstreamId = substreamId;
        offset += fragmentSize;
        if (pPayload->m_ContentSize <= offset) {
            // the last fragment
            pPacket->m_Unknown0x64 = 0;
        } else {
            pPacket->m_Unknown0x64 = fragmentId;
            fragmentId++;
            if (fragmentId == 0) {
                fragmentId = 1;
            }
        }
        count++;
        packets.push_back(pPacket);
    }
    for (PacketOut** it = packets.begin(); it != packets.end(); it++) {
        PacketOut* pPacket = *it;
        Buffer* pFragment = pPacket->m_pPayload;
        qResult result = SendPacket(pPacket);
        if (!result) {
            // the packets that were not sent yet are released
            for (; it != packets.end(); it++) {
                PacketOut* pRest = *it;
                Buffer* pRestFragment = pRest->m_pPayload;
                pRest->ReleaseRef();
                pRestFragment->ReleaseRef();
            }
            return result;
        }
        pPacket->ReleaseRef();
        pFragment->ReleaseRef();
    }
    if ((flags & FLAG_RELIABLE) && id != 0) {
        m_FragmentIdCounts[id] = count;
    }
    return qResult(qResult::SUCCESS);
}

// 0x003670DC slot 0x78 | slot vf_0x78 of nn::nex::EndPoint
nn::nex::qResult nn::nex::PRUDPEndPoint::_Send(nn::nex::Buffer* pBuffer, unsigned int flags, bool copy, unsigned char substreamId, unsigned int id)
{
    if (!IsConnected() && !PeerIsConnected()) {
        return qResult(RESULT_NOT_CONNECTED);
    }
    if (Network::GetInstance()->m_pRootTransport->m_Unknown0xFE) {
        ScopedCS lock(GetSchedulerLock());
        ProcessFaultEvent(3);
        return qResult(RESULT_NOT_CONNECTED);
    }
    if ((pBuffer->m_Flags & Buffer::FLAG_USE_POOL) && PacketBufferManager::GetInstance() != nullptr &&
        !PacketBufferManager::GetInstance()->HasFreeBuffers(PacketBufferManager::MAX_POOL_SIZE, PacketBufferManager::GetInstance()->m_Unknown0x2C)) {
        return qResult(RESULT_NO_MEMORY);
    }
    // the room for the payload in a packet
    u32 bufferSize = Network::GetInstance()->m_pRootTransport->m_MaxPacketSize;
    u32 maxPayloadSize = Network::GetInstance()->m_pRootTransport->m_MaxPacketSize - 64;
    if (Network::GetInstance()->m_pRootTransport->GetRelay()->m_IsEnabled) {
        maxPayloadSize -= g_Unknown0x899864;
    } else if (Network::GetInstance()->m_pRootTransport->GetRouter()->m_IsEnabled || m_MessageVersion == 0 || (m_MessageVersion == 1 && (m_SupportedFunctions & 0xFF) == 0)) {
        maxPayloadSize -= 36;
    }
    u16 packetFlags = 0;
    if (flags & SEND_FLAG_RELIABLE) {
        packetFlags = FLAG_RELIABLE | FLAG_NEED_ACK;
    }
    if (flags & 2) {
        packetFlags |= 0x100;
    }
    if (flags & 0x10) {
        packetFlags |= FLAG_UNKNOWN_0x200;
    }
    if (flags & 0x20) {
        packetFlags |= FLAG_UNKNOWN_0x400;
    }
    if (flags & 4) {
        packetFlags |= 0x800;
    }
    u8 unknown0x130 = (flags & 8) >> 3;
    if ((flags & SEND_FLAG_RELIABLE) && pBuffer->m_ContentSize > maxPayloadSize) {
        m_Statistics.m_FragmentedSends++;
        return SendFragmented(pBuffer, bufferSize, maxPayloadSize, Packet::TYPE_DATA, packetFlags, unknown0x130, id, substreamId);
    }
    if (pBuffer->m_ContentSize > maxPayloadSize) {
        m_Statistics.m_TooLargeSends++;
        return qResult(RESULT_INVALID_ARGUMENT);
    }
    PacketOut* pPacket = nullptr;
    if (pBuffer->m_Flags & Buffer::FLAG_USE_POOL) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketOut), false);
        if (pMemory != nullptr) {
            pPacket = ::new (pMemory) PacketBufferPacketOut(this, static_cast<PacketType>(Packet::TYPE_DATA), packetFlags, pBuffer, id);
        }
        if (pPacket == nullptr || ((pPacket->m_TypeFlags & FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr)) {
            if (pPacket != nullptr) {
                delete pPacket;
            }
            return qResult(RESULT_NO_MEMORY);
        }
    } else {
        pPacket = new PacketOut(this, static_cast<PacketType>(Packet::TYPE_DATA), packetFlags, pBuffer, id);
    }
    if (copy) {
        // the packet gets a copy of the buffer
        Buffer* pCopy;
        if (pBuffer->m_Flags & Buffer::FLAG_USE_POOL) {
            void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBuffer), false);
            pCopy = nullptr;
            if (pMemory != nullptr) {
                pCopy = ::new (pMemory) PacketBuffer(pBuffer->m_ContentSize + pBuffer->m_HeaderSize);
                if (pCopy != nullptr && pCopy->m_pData == nullptr) {
                    delete pCopy;
                    pCopy = nullptr;
                }
            }
            if (pCopy == nullptr) {
                pPacket->ReleaseRef();
                return qResult(RESULT_NO_MEMORY);
            }
        } else {
            pCopy = new Buffer(pBuffer->m_Size);
        }
        *pCopy = *pBuffer;
        pPacket->m_pPayload->ReleaseRef();
        pPacket->m_pPayload = pCopy;
    }
    pPacket->m_Unknown0x130 = unknown0x130;
    pPacket->m_Unknown0x64 = 0;
    pPacket->m_MessageVersion = m_MessageVersion;
    pPacket->m_SubstreamId = substreamId;
    qResult result = SendPacket(pPacket);
    if (!result) {
        pPacket->ReleaseRef();
        return result;
    }
    if ((pPacket->m_TypeFlags & FLAG_RELIABLE) && id != 0) {
        m_FragmentIdCounts[id] = 1;
    }
    pPacket->ReleaseRef();
    return qResult(qResult::SUCCESS);
}

// 0x00367718 | fefates:bytes [tier B]
bool nn::nex::PRUDPEndPoint::Defrag(nn::nex::PacketIn* pPacket)
{
    u8 substreamId = pPacket->m_SubstreamId;
    Buffer* pPayload = pPacket->m_pPayload;
    if (m_Fragments[substreamId] == nullptr) {
        m_Fragments[substreamId] = pPayload;
        pPayload->AcquireRef();
        pPayload = nullptr;
    }
    bool isComplete;
    u8 fragmentId = pPacket->m_Unknown0x64;
    if (fragmentId == 0) {
        isComplete = true;
    } else {
        isComplete = false;
        if (m_FragmentIds[substreamId] != fragmentId) {
            // a fragment is missing: the fragments so far are dropped
            if (m_Fragments[substreamId] != nullptr) {
                m_Fragments[substreamId]->ReleaseRef();
                m_Fragments[substreamId] = nullptr;
            }
            return isComplete;
        }
    }
    if (pPayload != nullptr) {
        Buffer* pFragments = m_Fragments[substreamId];
        if (pFragments->m_Flags & Buffer::FLAG_USE_POOL) {
            u32 size = pFragments->m_ContentSize + pPayload->m_ContentSize;
            if (size > PacketBufferManager::MAX_POOL_SIZE) {
                Buffer* pBuffer = new Buffer(size);
                *pBuffer = *m_Fragments[substreamId];
                m_Fragments[substreamId]->ReleaseRef();
                m_Fragments[substreamId] = pBuffer;
            }
        } else if (!pFragments->AttemptExpand(pFragments->m_ContentSize + pPayload->m_ContentSize)) {
            return false;
        }
        m_Fragments[substreamId]->AppendData(pPayload->m_pData + pPayload->m_HeaderSize, pPayload->m_ContentSize, static_cast<unsigned>(-1));
    }
    if (!isComplete) {
        m_FragmentIds[substreamId]++;
        if (m_FragmentIds[substreamId] == 0) {
            m_FragmentIds[substreamId] = 1;
        }
    }
    return isComplete;
}

// 0x00367904 (name is ours)
void nn::nex::PRUDPEndPoint::AcknowledgePacket(nn::nex::PacketIn* pPacket)
{
    PacketOut* pAck;
    u32 type = pPacket->GetType();
    if (type == Packet::TYPE_CONNECT) {
        // the reply to the connection request
        pAck = new PacketOut(this, static_cast<PacketType>(type), FLAG_ACK, pPacket->m_pPayload, 0);
        m_MessageVersion = pPacket->m_MessageVersion;
        pAck->m_MessageVersion = pPacket->m_MessageVersion;
        m_SupportedFunctions = pPacket->m_Unknown0x8C;
        m_PeerSupportedFunctions = GetPeerSupportedFunctions(m_SupportedFunctions);
        DecideMaxUserReliableSubStreams(pPacket->m_Unknown0x67);
        pAck->m_Unknown0x8C = m_SupportedFunctions;
        pAck->m_Unknown0x67 = static_cast<u8>(m_MaxSubstreamId);
        InitUnreliableSeq(pAck);
    } else {
        if (type == Packet::TYPE_DATA && pPacket->m_pPayload != nullptr && (pPacket->m_pPayload->m_Flags & Buffer::FLAG_USE_POOL)) {
            void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketOut), false);
            if (pMemory == nullptr) {
                return;
            }
            pAck = ::new (pMemory) PacketBufferPacketOut(this, static_cast<PacketType>(type), FLAG_ACK, nullptr, 0);
            if ((pAck->m_TypeFlags & FLAG_NEED_ACK) && pAck->m_pTimeout == nullptr) {
                delete pAck;
                return;
            }
        } else {
            pAck = new PacketOut(this, static_cast<PacketType>(type), FLAG_ACK, nullptr, 0);
        }
        pAck->m_MessageVersion = m_MessageVersion;
    }
    pAck->m_SequenceId.m_Value = pPacket->m_SequenceId.m_Value;
    pAck->m_Unknown0x64 = pPacket->m_Unknown0x64;
    m_Statistics.m_SentAcks++;
    SendPacket(pAck);
    if (pAck->GetType() == Packet::TYPE_DISCONNECT) {
        // the disconnection is acknowledged three times
        for (int i = 0; i < 2; i++) {
            PacketOut* pAgain = new PacketOut(this, static_cast<PacketType>(Packet::TYPE_DISCONNECT), FLAG_ACK, nullptr, 0);
            pAgain->m_SequenceId.m_Value = pPacket->m_SequenceId.m_Value;
            pAgain->m_Unknown0x64 = pPacket->m_Unknown0x64;
            pAgain->m_MessageVersion = m_MessageVersion;
            SendPacket(pAgain);
            pAgain->ReleaseRef();
        }
    }
    pAck->ReleaseRef();
}

// 0x00367C1C (name is ours)
void nn::nex::PRUDPEndPoint::DispatchUnreliablePacket(nn::nex::PacketIn* pPacket, nn::nex::Time time)
{
    m_EncDec.Decode(pPacket);
    if (!(pPacket->m_TypeFlags & FLAG_RELIABLE)) {
        DispatchData(pPacket->m_pPayload, pPacket);
        return;
    }
    if (!Defrag(pPacket)) {
        return;
    }
    u8 substreamId = pPacket->m_SubstreamId;
    DispatchData(m_Fragments[substreamId], pPacket);
    m_FragmentIds[pPacket->m_SubstreamId] = 1;
    m_Fragments[pPacket->m_SubstreamId]->ReleaseRef();
    m_Fragments[pPacket->m_SubstreamId] = nullptr;
}

// 0x00367D04 (name is ours)
void nn::nex::PRUDPEndPoint::StopPing()
{
    ScopedCS lock(GetSchedulerLock());
    if (m_pPingPacket != nullptr) {
        GetStream()->m_TimeoutManager.UnschedulePacketTimeout(m_pPingPacket);
        m_pPingPacket->ReleaseRef();
        m_pPingPacket = nullptr;
    }
}

// 0x0036813C (name is ours)
void nn::nex::PRUDPEndPoint::UpdateRTT(nn::nex::Time time, nn::nex::PacketOut* pPacket)
{
    m_RTT.Adjust(static_cast<u32>(time.m_Value) - static_cast<u32>(pPacket->m_Unknown0x100.m_Value));
    u32 rto = m_RTT.GetRTO();
    if (m_Statistics.m_MaxRTO < rto) {
        m_Statistics.m_MaxRTO = rto;
    }
    if (m_Statistics.m_MinRTO > rto) {
        m_Statistics.m_MinRTO = rto;
    }
}

// 0x00368204 | fefates:bytes [tier B]
void nn::nex::PRUDPEndPoint::StartPing()
{
    if (m_pPingPacket != nullptr) {
        return;
    }
    m_pPingPacket = new PacketOut(this, static_cast<PacketType>(Packet::TYPE_PING), FLAG_NEED_ACK, nullptr, 0);
    m_pPingPacket->m_MessageVersion = m_MessageVersion;
    if (GetKeepAliveTimeout() != 0 && GetKeepAliveTimeout() != static_cast<u32>(-1)) {
        m_pPingPacket->m_pTimeout->m_Interval = GetKeepAliveTimeout();
    } else {
        m_pPingPacket->m_pTimeout->m_Interval = 1000;
    }
    GetStream()->m_TimeoutManager.SchedulePacketTimeout(m_pPingPacket);
}

// 0x003CE418 (name is ours)
template <>
DECOMP_NOINLINE qMap<u32, u32>::qMap()
{
    // nothing to do: the tree is constructed by the compiler
}

} // namespace nex
} // namespace nn
