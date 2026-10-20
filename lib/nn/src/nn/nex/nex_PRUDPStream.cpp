#include "nn/nex/nex_PRUDPStream.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_EndPointEventHandler.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PRUDPEndPoint.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_PacketIn.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_Platform.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StreamSettings.h"
#include "nn/nex/nex_StreamBundling.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_TransportAdapter.h"

#include <cstring>

namespace nn {
namespace nex {
// the packets are sent with the flag of slot 0x20 of the transport (name is ours)
// 0x0096C934 (name is ours)
s8 g_Unknown0x96C934;
// the message version of the streams of the types 1, 2, 4, 6, 9 and 10 (name is ours)
// 0x0096C935 (name is ours)
u8 g_PRUDPMessageVersion = 1;

namespace {
// the results (the names are ours)
const int RESULT_INVALID_URL = static_cast<int>(0x80050003);
const int RESULT_INVALID_ARGUMENT = static_cast<int>(0x8001000A);
const int RESULT_CONNECTION_ABORTED = static_cast<int>(0x80050007);
// the type of the raw packets of SendTo (the name is ours)
const u32 TYPE_RAW_DATA = 5;
// the time between the aggregate acknowledgments (ms)
const s64 AGGREGATE_ACK_INTERVAL = 25;

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// (inline) the URL is parsed
inline void ValidateURL(const StationURL* pURL)
{
    if (!pURL->m_IsParsed) {
        const_cast<StationURL*>(pURL)->Parse();
    }
}

// the signature method of a packet (4: the connection of a peer with an old version)
inline u32 GetOldConnectSignatureMethod(const Packet* pPacket)
{
    if (pPacket->GetType() == Packet::TYPE_CONNECT && pPacket->m_MessageVersion == 1 && (pPacket->m_Unknown0x8C & 0xFF) < 2) {
        return 4;
    }
    return 0;
}

} // namespace

// 0x0035AC5C
nn::nex::PRUDPStream::PRUDPStream(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport)
    : ConnectionOrientedStream(type, pTransport), m_LastAckTime(0), m_SequenceId(0), m_Lock(0x4000)
{
    m_IsTerminating = false;
    m_LastAckTime = Time(0);
    m_Context = PseudoSingleton::GetCurrentContext();
    m_pTransport = pTransport;
    m_DefaultPort = 0;
    m_SequenceId.m_Value = static_cast<u16>(SystemClock::GetTime().m_Value + Platform::GetRandomNumber(0xFFFF));
    {
        Core* pCore = Core::GetInstance();
        ScopedCS lock(pCore->m_Lock);
        pCore->m_pInterruptCallback = InterruptCallback;
        pCore->m_pInterruptContext = this;
    }
    m_pMessageInterface = &Network::GetInstance()->m_MessageSelector;
    switch (type) {
    case 1:
    case 2:
    case 4:
    case 6:
    case 9:
    case 10:
        m_MessageVersion = g_PRUDPMessageVersion;
        break;
    default:
        m_MessageVersion = 0;
        break;
    }
}

// 0x0035AF8C slot 0x00 | slot vf_0x00 of nn::nex::Stream
// 0x0035AF5C (deleting dtor)
nn::nex::PRUDPStream::~PRUDPStream()
{
    // (not decompiled yet)
}

// 0x00185FC0 (name is ours)
void nn::nex::PRUDPStream::InterruptCallback(void* pContext)
{
    PRUDPStream* pStream = static_cast<PRUDPStream*>(pContext);
    Time deadline = Core::GetInstance()->m_InterruptTime + g_TerminationTimeout;
    pStream->m_TimeoutManager.LimitExpirationTimes(deadline);
}

// 0x00359CD0 slot 0x08
bool nn::nex::PRUDPStream::ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket)
{
    if (pPacket == nullptr) {
        return true;
    }
    PRUDPEndPoint* pEndPoint;
    u32 type = pPacket->m_TypeFlags & Packet::TYPE_MASK;
    if (type == Packet::TYPE_SYN) {
        if (pPacket->m_TypeFlags & PRUDPEndPoint::FLAG_ACK) {
            Lock();
            pEndPoint = m_EndPoints.Locate(pPacket->m_Address, VirtualPort(session));
            Unlock();
            goto process;
        }
        // a connection request: the reply has the connection signature of the address
        SignatureBytes signature;
        signature.m_Size = 0;
        m_pMessageInterface->CalcSignature(pPacket, PRUDPMessageInterface::SIGNATURE_METHOD_ADDRESS, nullptr, static_cast<Stream::Type>(pPacket->GetStreamType()),
                                           &m_SignatureGenerator, nullptr, signature);
        PacketOut* pReply = new PacketOut(nullptr, static_cast<PacketType>(Packet::TYPE_SYN), PRUDPEndPoint::FLAG_ACK, nullptr, 0);
        pReply->m_Unknown0x88 = pPacket->m_Unknown0x88;
        pReply->m_SequenceId.m_Value = pPacket->m_SequenceId.m_Value;
        pReply->m_Signature = signature;
        pReply->m_Address.m_SockAddr = pPacket->m_Address.m_SockAddr;
        VersionNegotiation(pPacket, pReply);
        Send(port, session, pReply, nullptr);
        pReply->ReleaseRef();
        pPacket->ReleaseRef();
        return true;
    }
    if (type == Packet::TYPE_CONNECT) {
        if (pPacket->m_TypeFlags & PRUDPEndPoint::FLAG_ACK) {
            int result = CheckConnectionSignature(&pPacket->m_Address, port, session, pPacket);
            if (result == SIGNATURE_DROPPED) {
                pPacket->ReleaseRef();
                return true;
            }
            if (result == SIGNATURE_INVALID) {
                return false;
            }
            Lock();
            pEndPoint = m_EndPoints.Locate(pPacket->m_Address, VirtualPort(session));
            Unlock();
            goto process;
        }
        // the connection of a peer: its signature
        SignatureBytes signature;
        signature.m_Size = 0;
        m_pMessageInterface->CalcSignature(pPacket, PRUDPMessageInterface::SIGNATURE_METHOD_ADDRESS, nullptr, static_cast<Stream::Type>(pPacket->GetStreamType()),
                                           &m_SignatureGenerator, nullptr, signature);
        u32 method = GetOldConnectSignatureMethod(pPacket);
        if (method == 0) {
            method = m_pMessageInterface->DecideSignatureMethod(pPacket, static_cast<Stream::Type>(m_Type), false);
        }
        SignatureBytes expected;
        expected.m_Size = 0;
        int result = m_pMessageInterface->CalcExpectedSignature(pPacket, static_cast<PRUDPMessageInterface::SignatureMethod>(method), nullptr, &m_SignatureGenerator, &signature, expected);
        if (result == SIGNATURE_DROPPED) {
            pPacket->ReleaseRef();
            return true;
        }
        if ((expected.m_Size != pPacket->m_PacketSignature.m_Size || memcmp(expected.m_Data, pPacket->m_PacketSignature.m_Data, expected.m_Size) != 0) &&
            pPacket->GetType() != TYPE_RAW_DATA) {
            return false;
        }
        // the end point of the peer (the event handler may refuse it)
        {
            Buffer* pPayload = pPacket->m_pPayload;
            StationURL url;
            url.SetInetAddress(&pPacket->m_Address);
            url.SetParamValue(L"sid", session);
            pEndPoint = CreateEndPoint(&url, port, false);
            if (m_pEventHandler != nullptr) {
                bool isAccepted;
                if (pPayload->m_ContentSize != 0) {
                    isAccepted = m_pEventHandler->ConnectionRequest(this, &pEndPoint->m_URL, pPayload, pEndPoint);
                } else {
                    isAccepted = m_pEventHandler->ConnectionRequest(this, &pEndPoint->m_URL, nullptr, pEndPoint);
                }
                if (!isAccepted) {
                    ReleaseEndPoint(pEndPoint);
                    pEndPoint = nullptr;
                }
            }
        }
        if (pEndPoint == nullptr) {
            pPacket->ReleaseRef();
            return true;
        }
        if (pPacket->m_Unknown0x88) {
            pEndPoint->m_Unknown0x160 = 1;
        }
        u8 version = pPacket->m_MessageVersion;
        if (version == 0 || (version == 1 && (pPacket->m_Unknown0x8C & 0xFF) < 2)) {
            pEndPoint->m_PeerSignature = signature;
        } else {
            pEndPoint->m_Signature = signature;
        }
        goto process;
    }
    if (type == TYPE_RAW_DATA) {
        // data without a connection
        Buffer* pPayload = pPacket->m_pPayload;
        if (m_pEventHandler != nullptr) {
            StationURL url;
            url.SetInetAddress(&pPacket->m_Address);
            if (pPayload->m_ContentSize != 0) {
                m_pEventHandler->Receive(this, pPayload, &url);
            } else {
                m_pEventHandler->Receive(this, nullptr, &url);
            }
        }
        pPacket->ReleaseRef();
        return true;
    }
    Lock();
    pEndPoint = m_EndPoints.Locate(pPacket->m_Address, VirtualPort(session));
    Unlock();
    if (pEndPoint == nullptr) {
        pPacket->ReleaseRef();
        return true;
    }
    if (g_Unknown0x96C92B && pEndPoint->IsDuplicateReorderingPacket(static_cast<PacketIn*>(pPacket))) {
        pPacket->ReleaseRef();
        return true;
    }
    {
        int result = CheckConnectionSignature(&pPacket->m_Address, port, session, pPacket);
        if (result == SIGNATURE_DROPPED) {
            pPacket->ReleaseRef();
            return true;
        }
        if (result == SIGNATURE_INVALID) {
            return false;
        }
    }
process:
    if (pEndPoint != nullptr && !pEndPoint->IsFaulty()) {
        pEndPoint->ProcessPacket(static_cast<PacketIn*>(pPacket));
    }
    pPacket->ReleaseRef();
    return true;
}

// 0x0035AADC slot 0x0C
void nn::nex::PRUDPStream::DoWork()
{
    Time now = SystemClock::s_LastTime;
    if (m_LastAckTime.m_Value == 0) {
        m_LastAckTime = now;
    } else if (static_cast<s64>(now.m_Value - m_LastAckTime.m_Value) > AGGREGATE_ACK_INTERVAL) {
        m_LastAckTime = now;
        m_TimeoutManager.ProcessTimeouts();
        ScopedCS lock(GetSchedulerLock());
        Lock();
        for (PRUDPEndPointMap::iterator it = m_EndPoints.begin(); it != m_EndPoints.end(); ++it) {
            it->second->SendAggregateACK();
        }
        Unlock();
    }
    DeleteReleasedEndPoints(false);
}

// 0x0035A45C slot 0x10 | fefates:bytes-fuzzy
bool nn::nex::PRUDPStream::IsDuplicateReorderingPacket(nn::nex::Packet* pPacket)
{
    VirtualPort port(pPacket->m_SourcePort & 0xF);
    Lock();
    PRUDPEndPoint* pEndPoint = m_EndPoints.Locate(pPacket->m_Address, port);
    Unlock();
    if (pEndPoint == nullptr) {
        return false;
    }
    return pEndPoint->IsDuplicateReorderingPacket(static_cast<PacketIn*>(pPacket));
}

// 0x003589C0 slot 0x18
bool nn::nex::PRUDPStream::StopListen()
{
    if (m_pTransport->m_StreamManager.UnregisterStream(static_cast<Stream::Type>(m_Type), m_Port)) {
        m_Port = 0;
    }
    ScopedCS lock(GetSchedulerLock());
    Lock();
    for (PRUDPEndPointMap::iterator it = m_EndPoints.begin(); it != m_EndPoints.end();) {
        PRUDPEndPoint* pEndPoint = it->second;
        ++it;
        pEndPoint->SetConnectionState(EndPoint::STATE_FAULTY);
    }
    Unlock();
    return true;
}

// 0x00359688 slot 0x1C | fefates:bytes
bool nn::nex::PRUDPStream::ResponsibleForURL(const nn::nex::StationURL* pURL)
{
    ValidateURL(pURL);
    return pURL->m_URLType == StationURL::URL_TYPE_PRUDP;
}

// 0x0035896C slot 0x20 | mk7dlp:bytes
const wchar_t* nn::nex::PRUDPStream::GetURLType()
{
    return L"prudp";
}

// 0x00358AC0 slot 0x24
nn::nex::EndPoint* nn::nex::PRUDPStream::OpenEndPoint(const nn::nex::StationURL* pURL)
{
    ScopedCS lock(GetSchedulerLock());
    u16 port = m_DefaultPort;
    if (port == 0) {
        port = m_pTransport->m_PortNumber;
    }
    PRUDPEndPoint* pEndPoint = CreateEndPoint(pURL, port, true);
    if (pEndPoint != nullptr) {
        pEndPoint->m_OpenCount++;
    }
    return pEndPoint;
}

// 0x00358BAC slot 0x28
nn::nex::EndPoint* nn::nex::PRUDPStream::OpenEndPoint(unsigned connectionId)
{
    EndPoint* pEndPoint = FindEndPointByConnectionID(connectionId);
    if (pEndPoint != nullptr) {
        pEndPoint->m_OpenCount++;
    }
    return pEndPoint;
}

// 0x00358B3C slot 0x2C
bool nn::nex::PRUDPStream::OpenEndPoint(nn::nex::EndPoint* pEndPoint)
{
    if (Core::s_IsThreadSafe) {
        ScopedCS lock(GetSchedulerLock());
        pEndPoint->m_OpenCount++;
    } else {
        pEndPoint->m_OpenCount++;
    }
    return true;
}

// 0x003595C8 slot 0x30
nn::nex::EndPoint* nn::nex::PRUDPStream::FindEndPointByPrincipalID(unsigned principalId)
{
    ScopedCS lock(GetSchedulerLock());
    qMultiMap<u32, EndPoint*>::iterator it = m_PrincipalIds.m_Tree.find(principalId);
    if (it == m_PrincipalIds.end()) {
        return nullptr;
    }
    return it->second;
}

// 0x00359678 slot 0x34
u32 nn::nex::PRUDPStream::GetNumberOfConnectionIDs()
{
    return m_ConnectionIdList.size();
}

// 0x00359518 slot 0x38
nn::nex::EndPoint* nn::nex::PRUDPStream::FindEndPointByConnectionID(unsigned connectionId)
{
    ScopedCS lock(GetSchedulerLock());
    qMultiMap<u32, EndPoint*>::iterator it = m_ConnectionIds.m_Tree.find(connectionId);
    if (it == m_ConnectionIds.end()) {
        return nullptr;
    }
    return it->second;
}

// 0x00358BD4 slot 0x40
void nn::nex::PRUDPStream::CloseEndPoint(nn::nex::EndPoint* pEndPoint)
{
    if (Core::s_IsThreadSafe) {
        ScopedCS lock(GetSchedulerLock());
        if (pEndPoint->m_OpenCount != 0) {
            pEndPoint->m_OpenCount--;
        }
        ReleaseEndPoint(static_cast<PRUDPEndPoint*>(pEndPoint));
    } else {
        if (pEndPoint->m_OpenCount != 0) {
            pEndPoint->m_OpenCount--;
        }
        ReleaseEndPoint(static_cast<PRUDPEndPoint*>(pEndPoint));
    }
}

// 0x00359488 slot 0x44
void nn::nex::PRUDPStream::Lock()
{
    m_Lock.CriticalSection::Enter();
}

// 0x003596B0 slot 0x48
void nn::nex::PRUDPStream::Unlock()
{
    m_Lock.CriticalSection::Leave();
}

// 0x00358980 slot 0x4C
nn::nex::qResult nn::nex::PRUDPStream::Initialize()
{
    qResult result(qResult::SUCCESS);
    if (m_pTransport == nullptr) {
        Network* pNetwork = Network::GetInstance();
        m_pTransport = pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
    }
    return result;
}

// 0x0035AC04 slot 0x50
nn::nex::qResult nn::nex::PRUDPStream::Terminate()
{
    return qResult(qResult::SUCCESS);
}

// 0x00358C60 slot 0x58
nn::nex::qResult nn::nex::PRUDPStream::SendBroadcast(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload)
{
    StationURL url;
    if (pURL != nullptr) {
        url.Copy(*pURL);
    }
    // to the broadcast address of the local network, else to this station; at the well-known port
    qList<StationURL> localURLs;
    TransportAdapter::s_pInstance->GetLocalURLs(&localURLs);
    if (localURLs.GetSize() != 0) {
        url.ValidateData();
        url.m_pAddress->SetAddress(L"255.255.255.255");
        url.m_IsFormatted = false;
    } else {
        url.ValidateData();
        url.m_pAddress->SetAddress(L"127.0.0.1");
        url.m_IsFormatted = false;
    }
    u16 port = RootTransport::s_WellKnownPortNumber.Get();
    url.ValidateData();
    url.m_pAddress->m_SockAddr.m_Port = static_cast<u16>((port << 8) | (port >> 8));
    url.m_IsFormatted = false;
    return SendTo(&url, pPayload);
}

// 0x0035A7D4 slot 0x5C
nn::nex::qResult nn::nex::PRUDPStream::SendTo(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload)
{
    ValidateURL(pURL);
    if (!pURL->m_IsValid) {
        return qResult(RESULT_INVALID_URL);
    }
    if (pPayload == nullptr) {
        return qResult(RESULT_INVALID_ARGUMENT);
    }
    PacketOut* pPacket = new PacketOut(nullptr, static_cast<PacketType>(TYPE_RAW_DATA), 0, pPayload, 0);
    ValidateURL(pURL);
    pPacket->m_Address.m_SockAddr = pURL->m_pAddress->m_SockAddr;
    u16 port = m_DefaultPort;
    if (port == 0) {
        port = m_pTransport->m_PortNumber;
    }
    bool isSent = Send(port, m_Port, pPacket, nullptr);
    pPacket->ReleaseRef();
    if (!isSent) {
        return qResult(RESULT_CONNECTION_ABORTED);
    }
    return qResult(qResult::SUCCESS);
}

// 0x0035AC20 slot 0x68
bool nn::nex::PRUDPStream::IsReliableType(unsigned int type)
{
    switch (type) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
        return true;
    default:
        return false;
    }
}

// 0x0035A3F8 slot 0x70
void nn::nex::PRUDPStream::RegisterPrincipalID(unsigned principalId, nn::nex::EndPoint* pEndPoint)
{
    ScopedCS lock(GetSchedulerLock());
    m_PrincipalIds.insert(std::pair<const u32, EndPoint*>(principalId, pEndPoint));
}

// 0x0035A698 slot 0x74
bool nn::nex::PRUDPStream::UnregisterPrincipalID(unsigned principalId, nn::nex::EndPoint* pEndPoint)
{
    ScopedCS lock(GetSchedulerLock());
    for (qMultiMap<u32, EndPoint*>::iterator it = m_PrincipalIds.m_Tree.find(principalId); it != m_PrincipalIds.end() && it->first == principalId; ++it) {
        if (it->second == pEndPoint) {
            m_PrincipalIds.erase(it);
            return true;
        }
    }
    return false;
}

// 0x0035A360 slot 0x78
void nn::nex::PRUDPStream::RegisterConnectionID(unsigned connectionId, nn::nex::EndPoint* pEndPoint)
{
    ScopedCS lock(GetSchedulerLock());
    m_ConnectionIds.insert(std::pair<const u32, EndPoint*>(connectionId, pEndPoint));
    m_ConnectionIdList.push_back(connectionId);
}

// 0x0035A4CC slot 0x7C
bool nn::nex::PRUDPStream::UnregisterConnectionID(unsigned connectionId, nn::nex::EndPoint* pEndPoint)
{
    ScopedCS lock(GetSchedulerLock());
    for (qMultiMap<u32, EndPoint*>::iterator it = m_ConnectionIds.m_Tree.find(connectionId); it != m_ConnectionIds.end() && it->first == connectionId; ++it) {
        if (it->second == pEndPoint) {
            u32 id = it->first;
            m_ConnectionIds.erase(it);
            // the first one of the ids
            u32* p = m_ConnectionIdList.begin();
            while (p != m_ConnectionIdList.end() && *p != id) {
                p++;
            }
            if (p != m_ConnectionIdList.end()) {
                for (u32* q = p + 1; q != m_ConnectionIdList.end(); q++) {
                    q[-1] = *q;
                }
                m_ConnectionIdList.m_pEnd--;
            }
            return true;
        }
    }
    return false;
}

// 0x003591F8 slot 0x80
void nn::nex::PRUDPStream::AbortIO()
{
    qList<PRUDPEndPoint*> endPoints;
    Lock();
    for (PRUDPEndPointMap::iterator it = m_EndPoints.begin(); it != m_EndPoints.end(); ++it) {
        endPoints.push_back(it->second);
    }
    Unlock();
    for (qList<PRUDPEndPoint*>::Node* pNode = endPoints.GetFirst(); !endPoints.IsEnd(pNode); pNode = pNode->m_pNext) {
        pNode->m_Value->CompleteIO(qResult(RESULT_CONNECTION_ABORTED));
    }
    ScopedCS lock(GetSchedulerLock());
    for (qList<PRUDPEndPoint*>::Node* pNode = m_ReleasedEndPoints.GetFirst(); !m_ReleasedEndPoints.IsEnd(pNode); pNode = pNode->m_pNext) {
        pNode->m_Value->CompleteIO(qResult(RESULT_CONNECTION_ABORTED));
    }
}

// 0x00359A28 slot 0x84
void nn::nex::PRUDPStream::SetMaxSilenceTime(unsigned int milliseconds)
{
    ScopedCS lock(GetSchedulerLock());
    Lock();
    for (PRUDPEndPointMap::iterator it = m_EndPoints.begin(); it != m_EndPoints.end(); ++it) {
        PRUDPEndPoint* pEndPoint = it->second;
        if (pEndPoint->GetMaxSilenceTime() != static_cast<u32>(-1)) {
            pEndPoint->SetMaxSilenceTime(milliseconds);
        }
    }
    Unlock();
}

// 0x00358DCC (name is ours)
int nn::nex::PRUDPStream::CheckConnectionSignature(const nn::nex::InetAddress* pAddress, unsigned short port, unsigned char session, nn::nex::Packet* pPacket)
{
    Lock();
    PRUDPEndPoint* pEndPoint = m_EndPoints.Locate(*pAddress, VirtualPort(session));
    Unlock();
    const Key* pKey;
    const SignatureBytes* pSignature;
    if (pEndPoint == nullptr) {
        pKey = nullptr;
        pSignature = nullptr;
    } else {
        pKey = &pEndPoint->GetEncryptionKey();
        pSignature = &pEndPoint->m_Signature;
    }
    u32 method = GetOldConnectSignatureMethod(pPacket);
    if (method == 0) {
        method = m_pMessageInterface->DecideSignatureMethod(pPacket, static_cast<Stream::Type>(m_Type), pKey != nullptr);
    }
    SignatureBytes expected;
    expected.m_Size = 0;
    int result = m_pMessageInterface->CalcExpectedSignature(pPacket, static_cast<PRUDPMessageInterface::SignatureMethod>(method), pKey, &m_SignatureGenerator, pSignature, expected);
    if (result == SIGNATURE_DROPPED) {
        return SIGNATURE_DROPPED;
    }
    if (expected.m_Size == pPacket->m_PacketSignature.m_Size && memcmp(expected.m_Data, pPacket->m_PacketSignature.m_Data, expected.m_Size) == 0) {
        return SIGNATURE_VALID;
    }
    if (pPacket->GetType() == TYPE_RAW_DATA) {
        return SIGNATURE_VALID;
    }
    return SIGNATURE_INVALID;
}

// 0x00358F1C (name is ours)
nn::nex::PRUDPEndPoint* nn::nex::PRUDPStream::CreateEndPoint(const nn::nex::StationURL* pURL, unsigned short port, bool isOpened)
{
    PRUDPEndPoint* pEndPoint = nullptr;
    ScopedCS lock(GetSchedulerLock());
    ValidateURL(pURL);
    if (pURL->m_IsValid) {
        VirtualPort virtualPort(static_cast<u8>(pURL->GetParamValue(L"sid", 0)));
        ValidateURL(pURL);
        pEndPoint = m_EndPoints.Locate(*pURL->m_pAddress, virtualPort);
        if (pEndPoint == nullptr) {
            pEndPoint = new PRUDPEndPoint(this, pURL, &m_SequenceId, m_MessageVersion);
            pEndPoint->m_PeerPort = port;
            u8 session = static_cast<u8>(pURL->GetParamValue(L"sid", 0));
            ValidateURL(&pEndPoint->m_URL);
            m_EndPoints.m_Tree.InsertInline(std::pair<const EndPointUniqueID, PRUDPEndPoint*>(EndPointUniqueID(*pEndPoint->m_URL.m_pAddress, VirtualPort(session)), pEndPoint));
        }
    }
    return pEndPoint;
}

// 0x003594AC (name is ours)
void nn::nex::PRUDPStream::ReleaseEndPoint(nn::nex::PRUDPEndPoint* pEndPoint)
{
    if (Core::s_IsThreadSafe) {
        ScopedCS lock(GetSchedulerLock());
        ReleaseEndPointImpl(pEndPoint);
    } else {
        ReleaseEndPointImpl(pEndPoint);
    }
}

// 0x003596D4 | fefates:bytes-fuzzy [tier B]
void nn::nex::PRUDPStream::VersionNegotiation(nn::nex::Packet* pReceived, nn::nex::Packet* pReply)
{
    if (pReceived->m_MessageVersion != m_MessageVersion) {
        pReply->m_MessageVersion = pReceived->m_MessageVersion;
    } else {
        pReply->m_MessageVersion = m_MessageVersion;
    }
    if (pReply->m_MessageVersion == 0) {
        pReply->m_Unknown0x8C = 0;
    } else {
        // the lower version and the functions both support
        u8 peerVersion = static_cast<u8>(pReceived->m_Unknown0x8C);
        u8& version = g_PRUDPFunctionsVersion.Get();
        u8 common = version >= peerVersion ? peerVersion : version;
        u32 functions = common | (((pReceived->m_Unknown0x8C >> 8) & g_PRUDPSupportedFunctions) << 8);
        if ((m_Type != 10 && m_Type != 2) || Stream::GetSettings(static_cast<Stream::Type>(m_Type))->m_Unknown0x99 != 1) {
            functions = (functions & 0xFF) | (((functions >> 8) & ~1u) << 8);
        }
        pReply->m_Unknown0x8C = functions;
    }
    u8 maxSubstreamId = pReceived->m_Unknown0x67;
    u8 ownMaxSubstreamId = Stream::GetSettings(static_cast<Stream::Type>(m_Type))->m_Unknown0x98;
    pReply->m_Unknown0x67 = ownMaxSubstreamId < maxSubstreamId ? ownMaxSubstreamId : maxSubstreamId;
}

// 0x003597F8 | fefates:bytes-fuzzy [tier B]
void nn::nex::PRUDPStream::ReleaseEndPointImpl(nn::nex::PRUDPEndPoint* pEndPoint)
{
    if (pEndPoint->m_OpenCount != 0 || pEndPoint->PeerIsConnected()) {
        return;
    }
    pEndPoint->SetConnectionID(0);
    pEndPoint->SetPrincipalID(0);
    VirtualPort virtualPort(static_cast<u8>(pEndPoint->m_URL.GetParamValue(L"sid", 0)));
    ValidateURL(&pEndPoint->m_URL);
    if (m_EndPoints.Locate(*pEndPoint->m_URL.m_pAddress, virtualPort) == nullptr) {
        return;
    }
    // it moves to the released end points
    ValidateURL(&pEndPoint->m_URL);
    InetAddress* pAddress = pEndPoint->m_URL.m_pAddress;
    u8 session = static_cast<u8>(pEndPoint->m_URL.GetParamValue(L"sid", 0));
    PRUDPEndPointMap::iterator it = m_EndPoints.find(EndPointUniqueID(*pAddress, VirtualPort(session)));
    if (it != m_EndPoints.end()) {
        m_EndPoints.erase(it);
    }
    m_ReleasedEndPoints.push_back(pEndPoint);
}

// 0x00359B24 (name is ours)
void nn::nex::PRUDPStream::DeleteReleasedEndPoints(bool isAll)
{
    if (Core::s_IsThreadSafe) {
        ScopedCS lock(GetSchedulerLock());
        for (qList<PRUDPEndPoint*>::Node* pNode = m_ReleasedEndPoints.GetFirst(); !m_ReleasedEndPoints.IsEnd(pNode);) {
            PRUDPEndPoint* pEndPoint = pNode->m_Value;
            if (!isAll && pEndPoint->m_Unknown0x8C) {
                pEndPoint->CompleteIO(qResult(RESULT_CONNECTION_ABORTED));
                pNode = pNode->m_pNext;
            } else {
                if (pEndPoint != nullptr) {
                    delete pEndPoint;
                }
                pNode = m_ReleasedEndPoints.erase(pNode);
            }
        }
    } else {
        for (qList<PRUDPEndPoint*>::Node* pNode = m_ReleasedEndPoints.GetFirst(); !m_ReleasedEndPoints.IsEnd(pNode);) {
            PRUDPEndPoint* pEndPoint = pNode->m_Value;
            if (!isAll && pEndPoint->m_Unknown0x8C) {
                pEndPoint->CompleteIO(qResult(RESULT_CONNECTION_ABORTED));
                pNode = pNode->m_pNext;
            } else {
                if (pEndPoint != nullptr) {
                    delete pEndPoint;
                }
                pNode = m_ReleasedEndPoints.erase(pNode);
            }
        }
    }
}

// 0x0035A918 | fefates:bytes-fuzzy [tier B]
bool nn::nex::PRUDPStream::Send(unsigned short port, unsigned char session, nn::nex::PacketOut* pPacket, nn::nex::Key* pKey)
{
    pPacket->m_SourcePort = static_cast<u8>((m_Type << 4) | m_Port);
    pPacket->m_DestinationPort = static_cast<u8>((m_Type << 4) | session);
    Lock();
    PRUDPEndPoint* pEndPoint = m_EndPoints.Locate(pPacket->m_Address, VirtualPort(session));
    Unlock();
    u32 method;
    if (pEndPoint != nullptr) {
        pPacket->m_MessageVersion = pEndPoint->m_MessageVersion;
        if (pPacket->GetType() == Packet::TYPE_CONNECT && pEndPoint->m_MessageVersion == 1 && !(pEndPoint->GetSupportedFunctionsFlags() & 0x2000000)) {
            method = 4;
        } else {
            method = pEndPoint->m_pMessageInterface->DecideSignatureMethod(pPacket, static_cast<Stream::Type>(m_Type), pKey != nullptr);
        }
    } else {
        method = m_pMessageInterface->DecideSignatureMethod(pPacket, static_cast<Stream::Type>(m_Type), pKey != nullptr);
    }
    if (method != PRUDPMessageInterface::SIGNATURE_METHOD_ADDRESS) {
        SignatureBytes signature;
        signature.m_Size = 0;
        const SignatureBytes* pPeerSignature = pEndPoint != nullptr ? &pEndPoint->m_PeerSignature : nullptr;
        if (!m_pMessageInterface->CalcSignature(pPacket, static_cast<PRUDPMessageInterface::SignatureMethod>(method), pKey, static_cast<Stream::Type>(m_Type), &m_SignatureGenerator,
                                                pPeerSignature, signature)) {
            return false;
        }
        pPacket->m_PacketSignature = signature;
    }
    return m_pTransport->SendPacket(port, m_Type, m_Port, session, pPacket, g_Unknown0x96C934);
}

} // namespace nex
} // namespace nn

namespace nn {
namespace nex {
// 0x0072A298 slot 0x3C
nn::nex::qVector<u32> nn::nex::PRUDPStream::GetConnectionIDs()
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    return m_ConnectionIdList;
}

// 0x003D9BC0 (name is ours)
template <>
DECOMP_NOINLINE qMultiMap<u32, EndPoint*>::qMultiMap()
{
    // nothing to do: the tree is constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x0083766C (name is ours)
template <>
DECOMP_NOINLINE multimap<u32, nn::nex::EndPoint*>::iterator multimap<u32, nn::nex::EndPoint*>::insert(const value_type& value)
{
    return m_Tree.InsertInline(value).first;
}
} // namespace std

namespace std {
// (the original calls it out of line)
// 0x0083041C (name is ours)
template <>
DECOMP_NOINLINE list<nn::nex::PRUDPEndPoint*, nn::nex::MemAllocator<nn::nex::PRUDPEndPoint*> >::~list()
{
    DestroyInline();
}
} // namespace std
