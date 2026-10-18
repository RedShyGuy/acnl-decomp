#include "nn/nex/nex_QueuingSocket.h"
#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_PacketBuffer.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_PacketBufferPacketIn.h"
#include "nn/nex/nex_PacketIn.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_StreamBundling.h"
#include "nn/nex/nex_StreamSettings.h"
#include "nn/nex/nex_TransportAdapter.h"
#include "nn/nex/nex_TransportStreamManager.h"

#include <new>

namespace nn {
namespace nex {
namespace {
// the flags of a packet that the bundling looks at (the names are ours)
const u16 FLAG_ACK = 0x10;
const u16 FLAG_RELIABLE = 0x20;
const u16 FLAG_TRAILING = 0x100;
const u16 FLAG_UNKNOWN_0x800 = 0x800;
// the states of a packet of the history (the names are ours)
const u8 HISTORY_STATE_WAITING = 0;
const u8 HISTORY_STATE_RETRANSMITTED = 2;
const u8 HISTORY_STATE_DONE = 3;

// (inline) a buffer of the size from the pools (null if they have none)
inline Buffer* CreatePoolBuffer(u32 size)
{
    void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBuffer), false);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return ::new (pMemory) PacketBuffer(size);
}
} // namespace

// 0x00369544 slot 0x20
s32 nn::nex::QueuingSocket::ReceiveDirect(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    return ReceiveNonBlockingInline(pBuffer, pAddress);
}

// 0x00369594 slot 0x1C
s32 nn::nex::QueuingSocket::SendDirect(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    if (pBuffer->m_ContentSize == 0) {
        return RESULT_SUCCESS;
    }
    s32 result = SendInline(pBuffer, pAddress);
    if (!m_IsAvailable) {
        m_State = STATE_CLOSED;
    }
    return result;
}

// 0x00369624 slot 0x10
s32 nn::nex::QueuingSocket::ReceiveNonBlocking(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    if (!m_InputEmulator.m_pDevice->m_IsEnabled) {
        return ReceiveDirect(pBuffer, pAddress);
    }
    Buffer* pHead;
    if (!m_InputEmulator.GetHead(&pHead, pAddress)) {
        return RESULT_ERROR;
    }
    *pBuffer = *pHead;
    m_InputEmulator.Dequeue(pHead);
    return RESULT_SUCCESS;
}

// 0x0036968C (name is ours)
nn::nex::PacketIn* nn::nex::QueuingSocket::ReadPacket(nn::nex::TransportStreamManager* pStreams, nn::nex::InetAddress* pAddress, nn::nex::Buffer* pBuffer,
                                                       bool* pIsOutOfMemory)
{
    PacketIn* pPacket;
    if (m_UsesPacketBuffers) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketIn), false);
        if (pMemory == nullptr) {
            *pIsOutOfMemory = true;
            return nullptr;
        }
        pPacket = ::new (pMemory) PacketBufferPacketIn();
        Buffer* pPayload = CreatePoolBuffer(pBuffer->m_ContentSize);
        if (pPacket->m_pPayload != nullptr) {
            pPacket->m_pPayload->ReleaseRef();
        }
        pPacket->m_pPayload = pPayload;
        if (pPayload == nullptr || pPayload->m_pData == nullptr) {
            delete pPacket;
            *pIsOutOfMemory = true;
            return nullptr;
        }
    } else {
        pPacket = new PacketIn();
        Buffer* pPayload = new Buffer();
        if (pPacket->m_pPayload != nullptr) {
            pPacket->m_pPayload->ReleaseRef();
        }
        pPacket->m_pPayload = pPayload;
    }
    pPacket->m_Address.m_SockAddr = pAddress->m_SockAddr;
    ByteStream stream(pBuffer);
    u32 size;
    if (g_Unknown0x96C92B) {
        // the duplicates of the bundles that the streams already have are skipped
        u32 skippedSize = 0;
        PRUDPMessageInterface* pMessage = &Network::GetInstance()->m_MessageSelector;
        while (pMessage->FastUnpack(&stream, pPacket, &size)) {
            u16 flags = pPacket->m_TypeFlags;
            if ((flags & Packet::TYPE_MASK) != Packet::TYPE_DATA || pPacket->GetStreamType() != 1) {
                break;
            }
            if (!g_Unknown0x96C92D && !(flags & (FLAG_ACK | FLAG_RELIABLE))) {
                break;
            }
            if ((!g_Unknown0x96C92C || (flags & FLAG_ACK)) && (flags & (FLAG_ACK | FLAG_RELIABLE))) {
                break;
            }
            if (!g_Unknown0x96C92E && !(flags & FLAG_UNKNOWN_0x800)) {
                break;
            }
            u8 key = pPacket->m_DestinationPort;
            Stream* pStream = pStreams->FindStream(key);
            if (pStream == nullptr || !pStream->IsDuplicateReorderingPacket(pPacket)) {
                break;
            }
            skippedSize += size;
        }
        if (skippedSize != 0) {
            u32 restSize = pBuffer->m_ContentSize - skippedSize;
            if (restSize == 0) {
                pBuffer->m_ContentSize = 0;
                pBuffer->m_HeaderSize = 0;
                pPacket->ReleaseRef();
                return nullptr;
            }
            pBuffer->AppendData(pBuffer->m_pData + pBuffer->m_HeaderSize + skippedSize, restSize, 0);
            pBuffer->m_ContentSize = restSize;
        }
        stream.m_Position = 0;
    }
    if (!m_pMessage->Unpack(&stream, pPacket, &size)) {
        pPacket->ReleaseRef();
        return nullptr;
    }
    u32 headerSize = pBuffer->m_HeaderSize;
    pBuffer->m_HeaderSize = size + headerSize;
    pBuffer->m_ContentSize += headerSize - pBuffer->m_HeaderSize;
    return pPacket;
}

// 0x00369A90 (name is ours)
void nn::nex::QueuingSocket::ReceiveToEmulator(u32 bufferSize)
{
    if (!m_InputEmulator.m_pDevice->m_IsEnabled) {
        return;
    }
    s32 result = RESULT_SUCCESS;
    while (CanReceive() && result == RESULT_SUCCESS) {
        Buffer* pBuffer = new Buffer(bufferSize);
        InetAddress address;
        result = ReceiveDirect(pBuffer, &address);
        if (result == RESULT_SUCCESS) {
            m_InputEmulator.Queue(pBuffer, address);
        }
        pBuffer->ReleaseRef();
    }
}

// 0x00369C78 (name is ours)
nn::nex::Buffer* nn::nex::QueuingSocket::CreateBundle(nn::nex::PacketQueue* pQueue, HistoryChain* pHistory, u32 maxSize)
{
    PacketQueue::PacketChain& packets = pQueue->m_Packets;
    PacketOut* pPacket = static_cast<PacketOut*>(packets.m_pFirst);
    u32 count = 0;
    PacketOut* pLastHistoryPacket = nullptr;
    if (pPacket == nullptr || !pPacket->IsBundlingTimeReached()) {
        return nullptr;
    }
    u16 maxReliableCount = Stream::GetSettings(static_cast<Stream::Type>(pPacket->GetStreamType()))->m_Bundling.m_Unknown0x1A;
    u16 maxUnreliableCount = Stream::GetSettings(static_cast<Stream::Type>(pPacket->GetStreamType()))->m_Bundling.m_Unknown0x18;
    u32 maxCount = maxReliableCount;
    ByteStream* pTrailingStream = nullptr;
    if (maxUnreliableCount > maxCount) {
        maxCount = maxUnreliableCount;
    }
    u8 ttl = pPacket->m_Unknown0x66;

    // the buffer of one packet and the buffer of the bundle
    Buffer* pPacketBuffer;
    Buffer* pBundleBuffer;
    if ((pPacket->GetType() == Packet::TYPE_PING || pPacket->m_pPayload == nullptr || (pPacket->m_pPayload->m_Flags & Buffer::FLAG_USE_POOL)) &&
        m_UsesPacketBuffers) {
        pPacketBuffer = CreatePoolBuffer(maxSize);
        pBundleBuffer = CreatePoolBuffer(maxSize);
        if (pPacketBuffer == nullptr || pPacketBuffer->m_pData == nullptr || pBundleBuffer == nullptr || pBundleBuffer->m_pData == nullptr) {
            if (pPacketBuffer != nullptr) {
                delete pPacketBuffer;
            }
            if (pBundleBuffer != nullptr) {
                delete pBundleBuffer;
            }
            pPacketBuffer = new Buffer(maxSize);
            pBundleBuffer = new Buffer(maxSize);
        }
    } else {
        pPacketBuffer = new Buffer(maxSize);
        pBundleBuffer = new Buffer(maxSize);
    }
    ByteStream packetStream(pPacketBuffer);
    pPacketBuffer->ReleaseRef();
    ByteStream bundleStream(pBundleBuffer);
    pBundleBuffer->ReleaseRef();
    ByteStream* pResultStream = &bundleStream;
    if (m_HasLengthPrefix) {
        u16 length = 0;
        bundleStream.m_pBuffer->AppendData(&length, sizeof(length), static_cast<unsigned>(-1));
    }

    PRUDPMessageInterface* pMessage = &Network::GetInstance()->m_MessageSelector;
    Packet* pNext = pPacket;
    while (pNext != packets.m_pEnd) {
        packetStream.m_pBuffer->m_ContentSize = 0;
        packetStream.m_pBuffer->m_HeaderSize = 0;
        packetStream.m_IsError = false;
        packetStream.m_Position = 0;
        pMessage->Pack(pPacket, &packetStream);
        u32 trailingSize = pTrailingStream != nullptr ? pTrailingStream->m_pBuffer->m_ContentSize : 0;
        if (trailingSize + bundleStream.m_pBuffer->m_ContentSize + packetStream.m_pBuffer->m_ContentSize > maxSize) {
            break;
        }
        Buffer* pData = packetStream.m_pBuffer;
        if (m_HasLengthPrefix && (pPacket->m_TypeFlags & FLAG_TRAILING)) {
            if (pTrailingStream == nullptr) {
                pTrailingStream = new ByteStream();
            }
            pTrailingStream->m_pBuffer->AppendData(pData->m_pData + pData->m_HeaderSize, pData->m_ContentSize, static_cast<unsigned>(-1));
        } else {
            bundleStream.m_pBuffer->AppendData(pData->m_pData + pData->m_HeaderSize, pData->m_ContentSize, static_cast<unsigned>(-1));
        }
        pPacket->AcquireRef();
        pNext = *pQueue->Dequeue(PacketQueue::PacketChain::iterator(pPacket));
        if (pPacket->m_Unknown0x134 != 0 && IsBundlingHistoryPacket(pPacket) && !pPacket->m_Unknown0x131) {
            // for the history
            if (pPacket->m_pChainPrev == nullptr && pPacket->m_pChainNext == nullptr && m_HistoryQueue.m_Packets.m_pFirst != pPacket) {
                pPacket->AcquireRef();
                m_HistoryQueue.m_Packets.push_back(pPacket);
            }
            pLastHistoryPacket = pPacket;
        }
        count++;
        if (count > maxCount) {
            pNext = packets.m_pEnd;
        } else {
            // the next packet that can go into the same bundle
            for (; pNext != packets.m_pEnd; pNext = pNext->m_pChainNext) {
                u32 type = pPacket->GetType();
                if (type == Packet::TYPE_RAW || type == Packet::TYPE_DISCONNECT) {
                    continue;
                }
                if (!Stream::GetSettings(static_cast<Stream::Type>(pPacket->GetStreamType()))->m_Bundling.m_IsEnabled) {
                    continue;
                }
                if (!CanBundleWith(pNext, pPacket)) {
                    continue;
                }
                if (pNext->m_TypeFlags & (FLAG_ACK | FLAG_RELIABLE)) {
                    if (maxReliableCount >= count) {
                        break;
                    }
                } else if (maxUnreliableCount >= count) {
                    break;
                }
            }
        }
        pPacket->ReleaseRef();
        pPacket = static_cast<PacketOut*>(pNext);
    }

    if (g_Unknown0x96C92B && pLastHistoryPacket != nullptr) {
        // the packets of the history that fit go in front of the bundle
        u32 bundleSize = g_MaxBundleSize;
        if (bundleSize > maxSize) {
            bundleSize = maxSize;
        }
        s32 room = bundleSize - ((pTrailingStream != nullptr ? pTrailingStream->m_pBuffer->m_ContentSize : 0) + bundleStream.m_pBuffer->m_ContentSize);
        u32 historyCount = 0;
        if (room > 0) {
            packetStream.m_pBuffer->m_ContentSize = 0;
            packetStream.m_pBuffer->m_HeaderSize = 0;
            packetStream.m_IsError = false;
            packetStream.m_Position = 0;
            u32 prefixSize = 0;
            if (m_HasLengthPrefix) {
                u16 length = 0;
                packetStream.m_pBuffer->AppendData(&length, sizeof(length), static_cast<unsigned>(-1));
                prefixSize = sizeof(length);
            }
            Time now = SystemClock::GetTime();
            Time time = !(Time(static_cast<s64>(g_BundlingHistoryDelay)) < now) ? Time(0) : now + (-g_BundlingHistoryDelay);
            historyCount = CreateBunldingStreamFromHistoryPacketQueue(pHistory, pLastHistoryPacket, time, room, &packetStream);
            if (packetStream.m_pBuffer->m_ContentSize > prefixSize) {
                Buffer* pBundle = bundleStream.m_pBuffer;
                packetStream.m_pBuffer->AppendData(pBundle->m_pData + pBundle->m_HeaderSize + prefixSize, pBundle->m_ContentSize - prefixSize,
                                                    static_cast<unsigned>(-1));
                pResultStream = &packetStream;
            }
        }
        InsertToHistoryPacketQueue(pHistory, &m_HistoryQueue);
        if (pLastHistoryPacket->m_Unknown0x138 != HISTORY_STATE_RETRANSMITTED) {
            PacketOut* pFirst = static_cast<PacketOut*>(packets.m_pFirst);
            if (pFirst == nullptr || !pFirst->IsBundlingTimeReached()) {
                RetransmitFromHistoryPacketQueue(pQueue, pHistory, pLastHistoryPacket->m_Unknown0x86);
            }
        }
        m_HistoryQueue.Purge();
        if (historyCount == 0) {
            g_BundleStatistics.m_EmptyHistoryCount++;
        } else {
            g_BundleStatistics.m_HistoryCount++;
        }
        if (historyCount >= g_MaxBundlingHistoryCount / 2u) {
            g_BundleStatistics.m_HalfFullHistoryCount++;
        }
        if (historyCount >= g_MaxBundlingHistoryCount) {
            g_BundleStatistics.m_FullHistoryCount++;
        }
    } else {
        g_BundleStatistics.m_WithoutHistoryCount++;
    }

    if (m_HasLengthPrefix) {
        u16 length = pResultStream->m_pBuffer->m_ContentSize - sizeof(length);
        pResultStream->m_pBuffer->AppendData(&length, sizeof(length), 0);
    }
    if (pTrailingStream != nullptr) {
        Buffer* pTrailing = pTrailingStream->m_pBuffer;
        pResultStream->m_pBuffer->AppendData(pTrailing->m_pData + pTrailing->m_HeaderSize, pTrailing->m_ContentSize, static_cast<unsigned>(-1));
        delete pTrailingStream;
    }
    Buffer* pBuffer = nullptr;
    if (pResultStream->m_pBuffer != nullptr) {
        g_BundleStatistics.m_BundleCount++;
        pBuffer = pResultStream->m_pBuffer;
        pBuffer->m_TTL = ttl;
        pBuffer->AcquireRef();
    }
    return pBuffer;
}

// 0x0036A628 | fefates:bytes [tier B]
void nn::nex::QueuingSocket::RetransmitFromHistoryPacketQueue(nn::nex::PacketQueue* pQueue, HistoryChain* pHistory, unsigned short id)
{
    Time now = SystemClock::GetTime();
    Packet* pNext = pHistory->m_pFirst;
    while (pNext != pHistory->m_pEnd) {
        PacketOut* pPacket = static_cast<PacketOut*>(pNext);
        if (pPacket->m_Unknown0x131) {
            pNext = *PacketHistoryChainDequeue(pHistory, HistoryChain::iterator(pPacket));
            continue;
        }
        if (pPacket->m_Unknown0x86 != id) {
            pNext = pPacket->m_pHistoryNext;
            continue;
        }
        switch (pPacket->m_Unknown0x138) {
        case HISTORY_STATE_WAITING:
            if (now < pPacket->m_Unknown0x140) {
                pNext = pPacket->m_pHistoryNext;
                break;
            }
            pPacket->m_Unknown0x138 = HISTORY_STATE_RETRANSMITTED;
            pPacket->m_Unknown0x130 = 1;
            pPacket->m_Unknown0x122++;
            pPacket->AcquireRef();
            pNext = *PacketHistoryChainDequeue(pHistory, HistoryChain::iterator(pPacket));
            pQueue->QueueFront(pPacket, false);
            pPacket->ReleaseRef();
            break;
        case 1:
        case HISTORY_STATE_RETRANSMITTED:
            if (!(now < pPacket->m_Unknown0x140)) {
                pPacket->m_Unknown0x138 = HISTORY_STATE_DONE;
            }
            pNext = pPacket->m_pHistoryNext;
            break;
        case HISTORY_STATE_DONE:
            pNext = pHistory->m_pEnd;
            break;
        }
    }
}

// 0x0036A7BC slot 0x08
bool nn::nex::QueuingSocket::Bind(nn::nex::InetAddress* pAddress, bool unknown)
{
    return BindInline(pAddress);
}

// 0x0036A840 slot 0x0C
s32 nn::nex::QueuingSocket::Receive(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    return RESULT_ERROR;
}

// 0x0036A848 slot 0x14
s32 nn::nex::QueuingSocket::Send(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    if (!m_OutputEmulator.m_pDevice->m_IsEnabled) {
        return SendDirect(pBuffer, pAddress);
    }
    Buffer* pCopy = new Buffer(*pBuffer);
    m_OutputEmulator.Queue(pCopy, *pAddress);
    pCopy->ReleaseRef();
    return RESULT_SUCCESS;
}

// 0x0036A8F0 | fefates:callgraph [tier C]
nn::nex::QueuingSocket::QueuingSocket(u32 context, nn::nex::RootTransport* pTransport, u32 maxPacketSize, nn::nex::TransportBuffer* pTransportBuffer)
    : Socket(context, maxPacketSize), m_UseCount(1), m_OutputEmulator(&pTransport->m_OutputDevice), m_InputEmulator(&pTransport->m_InputDevice),
      m_Unknown0xF8(1), m_pTransportBuffer(pTransportBuffer), m_UsesPacketBuffers(false)
{
    m_pMessage = &Network::GetInstance()->m_MessageSelector;
}

// 0x0036AA1C slot 0x00 | fefates:callseq
// 0x0036A9EC (deleting dtor)
nn::nex::QueuingSocket::~QueuingSocket()
{
    CloseInline();
}

// 0x0072A994 slot 0x18
bool nn::nex::QueuingSocket::CanReceive()
{
    return true;
}

} // namespace nex
} // namespace nn
