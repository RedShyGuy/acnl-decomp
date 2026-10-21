#include "nn/nex/nex_SocketTransport.h"
#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_BerkeleySocketDriver.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PacketBuffer.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_PacketBufferPacketOut.h"
#include "nn/nex/nex_PacketIn.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_ProtectedPacketQueue.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_QueuingSocketTransportBuffer.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SocketTransport_TransportJob.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StreamBundling.h"
#include "nn/nex/nex_TransportAdapter.h"
#include "nn/nex/nex_TransportBuffer.h"
#include "nn/nex/nex_TransportBufferMultiThread.h"
#include "nn/nex/nex_TransportBufferThread.h"
#include "nn/nex/nex_VirtualFilterDevice.h"

#include <new>

namespace nn {
namespace nex {
namespace {
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_INVALID_ARGUMENT = static_cast<int>(0x8001000A);
const int RESULT_TRANSPORT_CLOSED = static_cast<int>(0x80050002);
const int RESULT_INVALID_URL = static_cast<int>(0x80050003);
const int RESULT_OUT_OF_MEMORY = static_cast<int>(0x8005001F);
// the type of a packet that is only its payload
const PacketType PACKET_TYPE_RAW = static_cast<PacketType>(Packet::TYPE_RAW);
// the type of a routed packet
const unsigned int ROUTING_PACKET_TYPE = 6;

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// slot 0x08 of the object that may take a sent packet: true if it took it
typedef bool (*HandlePacketFunction)(void* pHandler, StationURL& url, unsigned short port);
inline HandlePacketFunction GetHandlePacketFunction(void* pHandler)
{
    return reinterpret_cast<HandlePacketFunction>((*reinterpret_cast<void***>(pHandler))[2]);
}

// (inline) the station URL of the network with the address and port of the URL is removed
inline void RemoveStationURL(Network* pNetwork, const StationURL& url)
{
    ScopedCS lock(pNetwork->m_StationUrls.m_Lock);
    qList<StationURL>& urls = pNetwork->m_StationUrls.m_List;
    qList<StationURL>::Node* pNode = urls.GetFirst();
    for (; !urls.IsEnd(pNode); pNode = pNode->m_pNext) {
        bool isSame;
        if (String::IsEqual(pNode->m_Value.GetAddress().m_pString, url.GetAddress().m_pString)) {
            isSame = pNode->m_Value.GetPortNumberInline() == url.GetPortNumberInline();
        } else {
            isSame = false;
        }
        if (isSame) {
            break;
        }
    }
    urls.erase(pNode);
}

// (inline) the socket of the driver is closed and the socket is deleted
inline void DeleteSocket(QueuingSocket* pSocket)
{
    pSocket->CloseInline();
    if (pSocket != nullptr) {
        delete pSocket;
    }
}
} // namespace

// 0x0037AFD4 (name is ours)
bool nn::nex::SocketTransport::CreateSocket(unsigned short port, unsigned short* pPort, unsigned int context, bool usesPacketBuffers, bool unknown)
{
    QueuingSocket* pSocket;
    if (Core::s_IsTransportThreaded) {
        pSocket = new QueuingSocketTransportBuffer(context, this, m_MaxPacketSize, m_pTransportBuffer, g_TransportBufferCount, g_TransportReceiveCount,
                                                   g_TransportSendCount);
    } else {
        pSocket = new QueuingSocket(context, this, m_MaxPacketSize, m_pTransportBuffer);
    }
    if (!pSocket->Open(true)) {
        if (pSocket != nullptr) {
            delete pSocket;
        }
        return false;
    }
    InetAddress address(0u, port);
    pSocket->m_UsesPacketBuffers = usesPacketBuffers;
    if (!pSocket->Bind(&address, unknown)) {
        DeleteSocket(pSocket);
        *pPort = 0;
        return false;
    }
    *pPort = __builtin_bswap16(address.m_SockAddr.m_Port);
    m_Sockets.insert(std::pair<u16, QueuingSocket*>(*pPort, pSocket));
    if (Core::s_IsTransportThreaded && m_pBufferThread != nullptr && !m_pBufferThread->InsertSocketPort(*pPort, pSocket)) {
        m_Sockets.erase(*pPort);
        DeleteSocket(pSocket);
        *pPort = 0;
        return false;
    }
    return true;
}

// 0x0037B374 slot 0x08
nn::nex::qResult nn::nex::SocketTransport::Start()
{
    StartThreads();
    return qResult(RESULT_SUCCESS);
}

// 0x0037B3A0 slot 0x14
bool nn::nex::SocketTransport::StopListen(unsigned short port)
{
    ScopedCS lock(GetSchedulerLock());
    QueuingSocket* pSocket = GetSocket(port);
    if (pSocket == nullptr) {
        return false;
    }
    if (pSocket->m_UseCount != 1) {
        pSocket->m_UseCount--;
        return true;
    }
    if (m_pBufferThread != nullptr) {
        m_pBufferThread->EraseSocketPort(port);
    }
    m_Sockets.erase(port);
    pSocket->CloseInline();
    delete pSocket;
    if (Network::GetInstance() != nullptr && TransportAdapter::s_pInstance != nullptr) {
        // the URLs of the port leave the network
        qList<StationURL> urls;
        TransportAdapter::s_pInstance->GetLocalURLs(&urls);
        while (urls.GetSize() != 0) {
            StationURL& url = urls.front();
            url.ValidateData();
            url.m_pAddress->m_SockAddr.m_Port = __builtin_bswap16(port);
            url.m_IsFormatted = false;
            RemoveStationURL(Network::GetInstance(), url);
            urls.erase(urls.GetFirst());
        }
    }
    return true;
}

// 0x0037B7D8 slot 0x18
bool nn::nex::SocketTransport::StopListenAll()
{
    ScopedCS lock(GetSchedulerLock());
    while (m_Sockets.size() != 0) {
        if (!StopListen(m_Sockets.begin()->first)) {
            return false;
        }
    }
    return true;
}

// 0x0037B85C slot 0x0C | fefates:bytes
bool nn::nex::SocketTransport::StartListen(unsigned short port, unsigned short* pPort, bool isNew, unsigned int context, bool usesPacketBuffers)
{
    return StartListenImpl(port, pPort, isNew, context, usesPacketBuffers, false);
}

// 0x0037B8C8 | fefates:callgraph
bool nn::nex::SocketTransport::StartListenImpl(unsigned short port, unsigned short* pPort, bool isNew, unsigned int context, bool usesPacketBuffers, bool unknown)
{
    ScopedCS lock(GetSchedulerLock());
    bool isPortChanged = false;
    u16 actualPort = 0;
    if (port == 0 && m_Sockets.size() != 0 && m_Sockets.begin()->second->m_Context == context) {
        port = m_Sockets.begin()->first;
    }
    QueuingSocket* pSocket = GetSocket(port);
    if (pSocket != nullptr) {
        pSocket->m_UseCount++;
        actualPort = __builtin_bswap16(pSocket->m_Address.m_SockAddr.m_Port);
    }
    if (actualPort == 0) {
        if (!CreateSocket(port, &actualPort, context, usesPacketBuffers, unknown)) {
            // (for a trace that is not in this build)
            Network::GetInstance();
            GetWellKnownPortNumber();
            return false;
        }
        if (isNew) {
            // the URLs of the port join the network
            isPortChanged = true;
            qList<StationURL> urls;
            TransportAdapter::s_pInstance->GetLocalURLs(&urls);
            while (urls.GetSize() != 0) {
                StationURL& url = urls.front();
                url.ValidateData();
                url.m_pAddress->m_SockAddr.m_Port = __builtin_bswap16(actualPort);
                url.m_IsFormatted = false;
                TransportAdapter::s_pInstance->SetPortNumber(actualPort);
                Network::GetInstance()->AddStationURL(url);
                urls.erase(urls.GetFirst());
            }
        }
    }
    if (pPort != nullptr) {
        *pPort = actualPort;
    }
    if (isPortChanged) {
        m_PortNumber = actualPort;
    }
    return true;
}

// 0x0037BB6C slot 0x10
bool nn::nex::SocketTransport::StartListenUnknown(unsigned short port, unsigned short* pPort, bool isNew, unsigned int context, bool usesPacketBuffers)
{
    return StartListenImpl(port, pPort, isNew, context, usesPacketBuffers, true);
}

// 0x0037BB94 (name is ours)
void nn::nex::SocketTransport::StartThreads()
{
    if (m_pJob == nullptr) {
        Core* pCore = Core::GetInstance();
        if (pCore != nullptr && pCore->m_pScheduler != nullptr) {
            m_pJob = new TransportJob(this);
            m_pJob->AcquireRef();
            pCore = Core::GetInstance();
            (pCore != nullptr ? pCore->m_pScheduler : nullptr)->RegisterRegularJob(m_pJob, false);
        }
    }
    if (!Core::s_IsTransportThreaded) {
        return;
    }
    if (m_pBufferThread != nullptr) {
        m_pBufferThread->Stop();
        if (m_pBufferThread != nullptr) {
            delete m_pBufferThread;
        }
        m_pBufferThread = nullptr;
        if (m_pTransportBuffer != nullptr) {
            delete m_pTransportBuffer;
            m_pTransportBuffer = nullptr;
        }
    }
    if (g_UsesTransportBuffer) {
        m_pTransportBuffer = new TransportBuffer(g_TransportBufferCount, m_MaxPacketSize);
    } else {
        m_pTransportBuffer = nullptr;
    }
    if (g_TransportBufferThreadMode == 1) {
        m_pBufferThread = new TransportBufferMultiThread();
    } else {
        m_pBufferThread = new TransportBufferThread();
    }
    m_pBufferThread->Start();
}

// 0x0037BEFC (name is ours)
void nn::nex::SocketTransport::DoWork()
{
    if (m_Sockets.size() == 0) {
        return;
    }
    SystemClock::s_LastTime = SystemClock::GetTime();
    ReceivePackets();
    // the received packets go to their streams
    PacketQueue::PacketChain::iterator it = m_ReceivedPackets.m_Packets.begin();
    while (it != m_ReceivedPackets.m_Packets.end()) {
        Packet* pPacket = *it;
        pPacket->m_Unknown0x100 = SystemClock::s_LastTime;
        m_StreamManager.DispatchPacket(pPacket);
        it = m_ReceivedPackets.Dequeue(it);
    }
    if (Core::s_IsThreadSafe) {
        ScopedCS lock(GetSchedulerLock());
        for (TransportStreamManager::Entry* pEntry = m_StreamManager.m_Streams.begin(); pEntry != m_StreamManager.m_Streams.end(); pEntry++) {
            pEntry->m_pStream->DoWork();
        }
    } else {
        for (TransportStreamManager::Entry* pEntry = m_StreamManager.m_Streams.begin(); pEntry != m_StreamManager.m_Streams.end(); pEntry++) {
            pEntry->m_pStream->DoWork();
        }
    }
    CriticalSection* pLock = m_pBundleQueue->GetLock();
    if (pLock != nullptr) {
        pLock->Enter();
        SendQueuedPackets();
        pLock->Leave();
    } else {
        SendQueuedPackets();
    }
}

// 0x0037C090 (name is ours)
void nn::nex::SocketTransport::SendQueuedPackets()
{
    if (g_Unknown0x96C92B && m_pBundleQueue->m_Packets.m_Count == 0 && m_History.m_Count != 0) {
        Time now = SystemClock::GetTime();
        if (!(now < m_NextRetransmitTime)) {
            QueuingSocket::RetransmitFromHistoryPacketQueue(m_pBundleQueue, &m_History, m_History.m_pFirst->m_Unknown0x86);
            m_NextRetransmitTime = now + RETRANSMIT_INTERVAL;
        }
    }
    while (m_pBundleQueue->m_Packets.m_Count != 0) {
        Packet* pFirst = m_pBundleQueue->m_Packets.m_pFirst;
        u16 port = pFirst->m_Unknown0x86;
        QueuingSocket* pSocket = GetSocket(port);
        if (pSocket == nullptr) {
            m_pBundleQueue->Dequeue(PacketQueue::PacketChain::iterator(pFirst));
            continue;
        }
        InetAddress address;
        address.m_SockAddr = pFirst->m_Address.m_SockAddr;
        u32 maxSize = m_MaxPacketSize;
        if (m_Relay.ShouldRelay(&address)) {
            maxSize -= RELAY_HEADER_SIZE;
        } else if (m_Router.ShouldRoute(&address)) {
            maxSize -= ROUTING_HEADER_SIZE;
        }
        Buffer* pBundle = pSocket->CreateBundle(m_pBundleQueue, &m_History, maxSize);
        if (pBundle == nullptr) {
            break;
        }
        bool isHandled;
        if (__builtin_bswap16(address.m_SockAddr.m_Port) == 0) {
            isHandled = SendMulti(pBundle, port);
        } else if (m_Relay.ShouldRelay(&address)) {
            m_Relay.Send(pBundle, &address, port);
            isHandled = true;
        } else if (m_Router.ShouldRoute(&address)) {
            m_Router.Route(pBundle, &address, port);
            isHandled = true;
        } else {
            isHandled = m_pFilterDevice != nullptr && m_pFilterDevice->Match(address);
        }
        if (!isHandled) {
            m_SentCount++;
            pSocket->Send(pBundle, &address);
        }
        pBundle->ReleaseRef();
    }
    if (m_OutputDevice.m_IsEnabled) {
        // the packets that left the output emulation are sent
        for (qSortedVector<u16, QueuingSocket*>::iterator it = m_Sockets.begin(); it != m_Sockets.end(); it++) {
            QueuingSocket* pSocket = it->second;
            while (pSocket->m_OutputEmulator.m_pDevice->m_IsEnabled) {
                InetAddress address;
                Buffer* pBuffer;
                if (!pSocket->m_OutputEmulator.GetHead(&pBuffer, &address)) {
                    break;
                }
                pSocket->SendDirect(pBuffer, &address);
                pSocket->m_OutputEmulator.Dequeue(pBuffer);
            }
        }
    }
}

// 0x0037C498 slot 0x1C
u32 nn::nex::SocketTransport::GetPortCount()
{
    ScopedCS lock(GetSchedulerLock());
    return m_Sockets.size();
}

// 0x0037C4E8 (name is ours)
void nn::nex::SocketTransport::ReceivePackets()
{
    for (qSortedVector<u16, QueuingSocket*>::iterator it = m_Sockets.begin(); it != m_Sockets.end(); it++) {
        QueuingSocket* pSocket = it->second;
        if (!pSocket->m_IsAvailable) {
            // the connection of the socket was closed
            if (!m_Unknown0xFE) {
                Core::GetInstance()->SetInterrupted(true);
            }
            m_Unknown0xFE = true;
        }
        u32 bufferSize = m_MaxPacketSize;
        pSocket->ReceiveToEmulator(bufferSize);
        s32 result = Socket::RESULT_SUCCESS;
        while (pSocket->CanReceive() && result == Socket::RESULT_SUCCESS) {
            void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBuffer), false);
            if (pMemory == nullptr) {
                break;
            }
            PacketBuffer* pBuffer = ::new (pMemory) PacketBuffer(bufferSize);
            if (pBuffer == nullptr) {
                break;
            }
            if (pBuffer->m_pData == nullptr) {
                delete pBuffer;
                break;
            }
            InetAddress address;
            result = pSocket->ReceiveNonBlocking(pBuffer, &address);
            if (result == Socket::RESULT_SUCCESS) {
                FillPacketQueueFromBuffer(pSocket, pBuffer, &address);
                m_ReceivedCount++;
            }
            pBuffer->ReleaseRef();
        }
    }
}

// 0x0037C718 slot 0x40
void nn::nex::SocketTransport::RequestStop()
{
    m_IsStopping = true;
}

// 0x0037C724 | mk7dlp:callgraph [tier A]
bool nn::nex::SocketTransport::FillPacketQueueFromBuffer(nn::nex::QueuingSocket* pSocket, nn::nex::Buffer* pBuffer, const nn::nex::InetAddress* pAddress)
{
    TransportAdapter* pAdapter = TransportAdapter::s_pInstance;
    if (pAdapter->m_Unknown0x04 == 1 && pAdapter->m_LocalAddress.m_SockAddr.m_Address == pAddress->m_SockAddr.m_Address &&
        pAdapter->m_LocalAddress.m_SockAddr.m_Port == pAddress->m_SockAddr.m_Port) {
        // a broadcast of this console
        return true;
    }
    bool result = true;
    bool isOutOfMemory = false;
    u32 count = 0;
    if (pSocket->m_HasLengthPrefix) {
        if (pBuffer->m_ContentSize <= sizeof(u16)) {
            goto unhandled;
        }
        u32 size = pBuffer->m_ContentSize - sizeof(u16);
        pBuffer->AppendData(pBuffer->m_pData + pBuffer->m_HeaderSize + sizeof(u16), size, 0);
        pBuffer->m_ContentSize = size;
    }
    count = 0;
    do {
        PacketIn* pPacket = pSocket->ReadPacket(&m_StreamManager, const_cast<InetAddress*>(pAddress), pBuffer, &isOutOfMemory);
        if (pPacket == nullptr) {
            break;
        }
        count++;
        pPacket->m_Unknown0x86 = __builtin_bswap16(pSocket->m_Address.m_SockAddr.m_Port);
        m_ReceivedPackets.Queue(pPacket, true);
    } while (pBuffer->m_ContentSize != 0);
unhandled:
    if (!isOutOfMemory && count == 0 && pBuffer->m_ContentSize != 0) {
        InetAddress address;
        address.m_SockAddr = pAddress->m_SockAddr;
        result = !ReceiveUnhandled(pBuffer, &address);
    }
    return result;
}

// 0x0037C890 (name is ours)
bool nn::nex::SocketTransport::SendMulti(nn::nex::Buffer* pBuffer, unsigned short port)
{
    bool result = true;
    InetAddress addresses[MAX_SEND_MULTI_ADDRESSES];
    u32 count = 0;
    BerkeleySocketDriver::GetSendMultiAddresses(addresses, MAX_SEND_MULTI_ADDRESSES, &count);
    for (u32 i = 0; i < count; i++) {
        InetAddress* pAddress = &addresses[i];
        bool isRouted = m_Router.ShouldRoute(pAddress);
        bool isRelayed = m_Relay.ShouldRelay(pAddress);
        if (!isRouted && !isRelayed) {
            result = false;
            continue;
        }
        if (isRelayed) {
            m_Relay.Send(pBuffer, pAddress, port);
            continue;
        }
        InetAddress via;
        m_Router.m_pStream->m_RoutingTable.Find(*pAddress, via);
        PacketOut* pPacket = RoutingStream::CreatePacket(pBuffer, ROUTING_PACKET_TYPE, pAddress);
        if (pPacket == nullptr) {
            continue;
        }
        if ((pPacket->m_TypeFlags & PacketOut::FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr) {
            delete pPacket;
            continue;
        }
        pPacket->m_Address.m_SockAddr = via.m_SockAddr;
        RoutingStream* pStream = m_Router.m_pStream;
        pStream->m_pTransport->SendPacket(port, pStream->m_Type, Router::PORT, Router::PORT, pPacket, true);
        pPacket->ReleaseRef();
    }
    return result;
}

// 0x0037CA5C slot 0x3C
void nn::nex::SocketTransport::DropPoolPackets()
{
    Packet* pPacket = m_pBundleQueue->m_Packets.m_pFirst;
    while (pPacket != m_pBundleQueue->m_Packets.m_pEnd) {
        if (pPacket->m_pPayload != nullptr && (pPacket->m_pPayload->m_Flags & Buffer::FLAG_USE_POOL)) {
            pPacket = *m_pBundleQueue->Dequeue(PacketQueue::PacketChain::iterator(pPacket));
        } else {
            pPacket = pPacket->m_pChainNext;
        }
    }
    pPacket = m_History.m_pFirst;
    while (pPacket != m_History.m_pEnd) {
        if (pPacket->m_pPayload != nullptr && (pPacket->m_pPayload->m_Flags & Buffer::FLAG_USE_POOL)) {
            pPacket->ReleaseRef();
            pPacket = *m_History.EraseInline(QueuingSocket::HistoryChain::iterator(pPacket));
        } else {
            pPacket = pPacket->m_pHistoryNext;
        }
    }
    pPacket = m_ReceivedPackets.m_Packets.m_pFirst;
    while (pPacket != m_ReceivedPackets.m_Packets.m_pEnd) {
        if (pPacket->m_pPayload != nullptr && (pPacket->m_pPayload->m_Flags & Buffer::FLAG_USE_POOL)) {
            pPacket->ReleaseRef();
            pPacket = *m_ReceivedPackets.Dequeue(PacketQueue::PacketChain::iterator(pPacket));
        } else {
            pPacket = pPacket->m_pChainNext;
        }
    }
}

// 0x0037CC38 slot 0x38
nn::nex::qResult nn::nex::SocketTransport::SendRaw(nn::nex::StationURL* pURL, nn::nex::Buffer* pBuffer)
{
    pURL->ValidateData();
    if (pURL->m_URLType != StationURL::URL_TYPE_UDP) {
        return qResult(RESULT_INVALID_URL);
    }
    pURL->ValidateData();
    if (!pURL->m_IsValid) {
        return qResult(RESULT_INVALID_URL);
    }
    if (m_Unknown0xFE) {
        return qResult(RESULT_TRANSPORT_CLOSED);
    }
    if (pBuffer == nullptr) {
        return qResult(RESULT_INVALID_ARGUMENT);
    }
    PacketOut* pPacket;
    if (pBuffer->m_Flags & Buffer::FLAG_USE_POOL) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketOut), false);
        if (pMemory == nullptr) {
            return qResult(RESULT_OUT_OF_MEMORY);
        }
        pPacket = ::new (pMemory) PacketBufferPacketOut(nullptr, PACKET_TYPE_RAW, 0, pBuffer, 0);
        if (pPacket == nullptr) {
            return qResult(RESULT_OUT_OF_MEMORY);
        }
        if ((pPacket->m_TypeFlags & PacketOut::FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr) {
            delete pPacket;
            return qResult(RESULT_OUT_OF_MEMORY);
        }
    } else {
        pPacket = new PacketOut(nullptr, PACKET_TYPE_RAW, 0, pBuffer, 0);
    }
    pURL->ValidateData();
    pPacket->m_Address.m_SockAddr = pURL->m_pAddress->m_SockAddr;
    pPacket->m_MessageVersion = 0;
    pPacket->m_Unknown0x86 = m_PortNumber;
    m_pBundleQueue->Queue(pPacket, true);
    pPacket->ReleaseRef();
    return qResult(RESULT_SUCCESS);
}

// 0x0037CE24 slot 0x20
bool nn::nex::SocketTransport::SendPacket(unsigned short port, unsigned char streamType, unsigned char sourcePort, unsigned char destinationPort,
                                          nn::nex::PacketOut* pPacket, bool unknown)
{
    if (m_Unknown0xFE) {
        return false;
    }
    u8 key = sourcePort | (streamType << 4);
    if (m_StreamManager.FindStream(key) == nullptr) {
        return false;
    }
    ScopedCS lock(GetSchedulerLock());
    if (GetSocket(port) == nullptr) {
        return false;
    }
    if (m_pUnknown0xAC != nullptr) {
        StationURL url;
        url.SetInetAddress(&pPacket->m_Address);
        bool isHandled = GetHandlePacketFunction(m_pUnknown0xAC)(m_pUnknown0xAC, url, port);
        url.ValidateData();
        pPacket->m_Address.m_SockAddr = url.m_pAddress->m_SockAddr;
        if (isHandled) {
            return true;
        }
    }
    pPacket->m_Unknown0x100 = SystemClock::s_LastTime;
    pPacket->m_SourcePort = key;
    pPacket->m_DestinationPort = destinationPort | (streamType << 4);
    pPacket->m_Unknown0x86 = port;
    m_pBundleQueue->Queue(pPacket, true);
    return true;
}

// 0x0037CF98 slot 0x24 | mk7dlp:bytes
bool nn::nex::SocketTransport::Receive(unsigned short port, nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress)
{
    bool result = false;
    QueuingSocket* pSocket = GetSocket(port);
    if (pSocket != nullptr) {
        result = FillPacketQueueFromBuffer(pSocket, pData, pAddress);
    }
    return result;
}

// 0x0037CFD4 slot 0x2C
nn::nex::Relay* nn::nex::SocketTransport::GetRelay()
{
    return &m_Relay;
}

// 0x0037CFDC slot 0x28
nn::nex::Router* nn::nex::SocketTransport::GetRouter()
{
    return &m_Router;
}

// 0x0037CFE4 | mk7dlp:callgraph [tier A]
nn::nex::QueuingSocket* nn::nex::SocketTransport::GetSocket(unsigned short port)
{
    if (Core::s_IsThreadSafe) {
        ScopedCS lock(GetSchedulerLock());
        qSortedVector<u16, QueuingSocket*>::iterator it = m_Sockets.find(port);
        if (it == m_Sockets.end()) {
            return nullptr;
        }
        return it->second;
    }
    qSortedVector<u16, QueuingSocket*>::iterator it = m_Sockets.find(port);
    return it != m_Sockets.end() ? it->second : nullptr;
}

// 0x0037D124
nn::nex::SocketTransport::SocketTransport() : m_ProfilingUnit(L"Transport Job", 1000)
{
    Core::AcquireInstance();
    if (Core::s_IsThreadSafe) {
        m_pBundleQueue = new ProtectedPacketQueue(&GetSchedulerLock());
    } else {
        m_pBundleQueue = new PacketQueue();
    }
    m_IsStopping = false;
    m_pBufferThread = nullptr;
    m_pJob = nullptr;
    m_Context = PseudoSingleton::GetCurrentContext();
    m_pTransportBuffer = nullptr;
    m_Router.SetTransport(this);
    m_Relay.SetTransport(this);
    m_SentCount = 0;
    m_ReceivedCount = 0;
    BerkeleySocketDriver::s_Counters.m_SendToCount = 0;
    BerkeleySocketDriver::s_Counters.m_SendMultiCount = 0;
    BerkeleySocketDriver::s_Counters.m_ReceivedCount = 0;
    BerkeleySocketDriver::s_Counters.m_RecvFromCount = 0;
}

// 0x0037D534 slot 0x00
// 0x0037D500 (deleting dtor)
nn::nex::SocketTransport::~SocketTransport()
{
    m_IsStopping = true;
    if (Core::s_IsTransportThreaded && m_pBufferThread != nullptr) {
        m_pBufferThread->Stop();
        if (m_pBufferThread != nullptr) {
            delete m_pBufferThread;
        }
        m_pBufferThread = nullptr;
    }
    if (m_pJob != nullptr) {
        Core* pCore = Core::GetInstance();
        if (pCore != nullptr && pCore->m_pScheduler != nullptr) {
            pCore = Core::GetInstance();
            (pCore != nullptr ? pCore->m_pScheduler : nullptr)->UnregisterRegularJob(m_pJob);
            m_pJob->ReleaseRef();
            m_pJob = nullptr;
        }
    }
    if (m_pUnknown0xAC != nullptr) {
        ScopedCS lock(GetSchedulerLock());
        m_pUnknown0xAC = nullptr;
    }
    {
        ScopedCS lock(GetSchedulerLock());
        while (m_Sockets.size() != 0) {
            if (!StopListen(m_Sockets.begin()->first)) {
                break;
            }
        }
    }
    if (m_pTransportBuffer != nullptr) {
        delete m_pTransportBuffer;
        m_pTransportBuffer = nullptr;
    }
    m_pBundleQueue->Purge();
    if (m_pBundleQueue != nullptr) {
        delete m_pBundleQueue;
    }
    while (m_History.m_pFirst != m_History.m_pEnd) {
        PacketHistoryChainDequeue(&m_History, m_History.m_pFirst);
    }
    m_ReceivedPackets.Purge();
    Core::ReleaseInstance();
}

// 0x0072B6D0 slot 0x30
void* nn::nex::SocketTransport::vf_0x30()
{
    return nullptr;
}

// 0x0072B6D8 slot 0x34
void* nn::nex::SocketTransport::vf_0x34()
{
    return nullptr;
}

// 0x007D5BFC
template bool qSortedVector<u16, QueuingSocket*>::erase(const u16& key);

} // namespace nex
} // namespace nn

namespace std {
// 0x00834C68 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void vector<std::pair<u16, nn::nex::QueuingSocket*>, nn::nex::MemAllocator<std::pair<u16, nn::nex::QueuingSocket*> > >::_C_insert_aux(
    std::pair<u16, nn::nex::QueuingSocket*>* pos, const std::pair<u16, nn::nex::QueuingSocket*>& value)
{
    InsertAuxInline(pos, value);
}

// 0x00834E68 | fefates:bytes [tier B]
template void vector<std::pair<u16, nn::nex::QueuingSocket*>, nn::nex::MemAllocator<std::pair<u16, nn::nex::QueuingSocket*> > >::reserve(u32 num);
} // namespace std
