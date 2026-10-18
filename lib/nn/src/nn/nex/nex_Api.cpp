#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_PacketQueue.h"
#include "nn/nex/nex_StreamBundling.h"
#include "nn/nex/nex_CallbackRoot.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/nex/nex_UserContext.h"

namespace nn {
namespace nex {
// 0x003B8554 | fefates:bytes [tier B]
nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >::iterator
PacketHistoryChainDequeue(nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >* pHistory,
                          nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >::iterator it)
{
    Packet* pPacket = *it;
    it = pHistory->erase(it);
    pPacket->ReleaseRef();
    return it;
}

// 0x003B9818 | fefates:bytes-fuzzy [tier B]
void InsertToHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >* pHistory, nn::nex::PacketQueue* pQueue)
{
    PacketQueue::PacketChain::iterator it = pQueue->m_Packets.begin();
    while (it != pQueue->m_Packets.end()) {
        PacketOut* pPacket = static_cast<PacketOut*>(*it);
        if (!IsBundlingHistoryPacket(pPacket) || (!g_Unknown0x96C92E && !(pPacket->m_TypeFlags & 0x800)) || pPacket->m_Unknown0x131) {
            it = pQueue->Dequeue(it);
            continue;
        }
        pPacket->AcquireRef();
        it = pQueue->Dequeue(it);
        if (pPacket->m_Unknown0x138 != 2) {
            pPacket->m_Unknown0x138 = 0;
        }
        pPacket->m_Unknown0x140 = SystemClock::GetTime() + g_BundlingRetransmissionDelay;
        pPacket->AcquireRef();
        if (pPacket->m_pHistoryPrev != nullptr || pPacket->m_pHistoryNext != nullptr || pHistory->m_pFirst == pPacket) {
            PacketHistoryChainDequeue(pHistory, pPacket);
        }
        pHistory->push_front(pPacket);
        pPacket->m_Unknown0x110 = SystemClock::GetTime();
        if (pHistory->m_Count > g_MaxBundlingHistoryCount) {
            PacketHistoryChainDequeue(pHistory, pHistory->m_pLast);
        }
        pPacket->ReleaseRef();
    }
}

// 0x007E4774 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE qChain<Packet*, ChainPolicyHistoryPacket<Packet*> >::iterator qChain<Packet*, ChainPolicyHistoryPacket<Packet*> >::erase(iterator it)
{
    return EraseInline(it);
}

// 0x007E481C
// 0x007E47EC (deleting dtor)
template qChain<Packet*, ChainPolicyHistoryPacket<Packet*> >::~qChain();

// 0x003B9A9C | mk7dlp:bytes [tier A]
void InvokeCallbackOnCompletion(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext)
{
    // the user context is the callback to make
    CallbackRoot* pCallback = static_cast<CallbackRoot*>(pUserContext->m_pPointer);
    pCallback->Invoke();
    if (pCallback != nullptr) {
        delete pCallback;
    }
}

// 0x003CD054 | mk7dlp:bytes [tier B]
nn::nex::StringStream& hex(nn::nex::StringStream& stream)
{
    stream.m_IsHex = true;
    stream.m_IsZeroPadded = false;
    return stream;
}

// 0x0035697C | fefates:callgraph [tier C]
nn::nex::StringStream& noshowbase(nn::nex::StringStream& stream)
{
    stream.m_ShowsBase = false;
    return stream;
}

// 0x003D47E8 | fefates:callgraph [tier C]
nn::nex::StringStream& zerohex(nn::nex::StringStream& stream)
{
    stream.m_IsHex = true;
    stream.m_IsZeroPadded = true;
    return stream;
}

// 0x003CD224 | fefates:bytes-fuzzy [tier B]
u32 CreateBunldingStreamFromHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >* pHistory, nn::nex::PacketOut* pPacket, nn::nex::Time time, unsigned int size, nn::nex::ByteStream* pStream)
{
    typedef qChain<Packet*, ChainPolicyHistoryPacket<Packet*> > HistoryChain;
    typedef qChain<Packet*, ChainPolicyTemp<Packet*> > TempChain;
    if (pHistory->m_Count == 0) {
        return 0;
    }
    u32 initialSize = pStream->m_pBuffer->m_ContentSize;
    u32 totalSize = initialSize;
    // the packets of the history (newest first) that go into the bundle
    TempChain bundle;
    HistoryChain::iterator it = pHistory->begin();
    while (it != pHistory->end()) {
        if (bundle.m_Count >= g_MaxBundledHistoryPacketCount) {
            break;
        }
        PacketOut* pHistoryPacket = static_cast<PacketOut*>(*it);
        if (pHistoryPacket->m_Unknown0x122 >= g_MaxHistoryPacketBundleCount || pHistoryPacket->m_Unknown0x131) {
            it = PacketHistoryChainDequeue(pHistory, it);
            continue;
        }
        if (pHistoryPacket->m_Unknown0x110 < time) {
            break;
        }
        if (!IsBundlingHistoryPacket(pPacket) || pPacket->m_Unknown0x131 || !CanBundleWith(pHistoryPacket, pPacket)) {
            it = HistoryChain::iterator(pHistoryPacket->m_pHistoryNext);
            continue;
        }
        if (totalSize + pHistoryPacket->m_Unknown0x134 > size) {
            break;
        }
        bundle.push_front(pHistoryPacket);
        pHistoryPacket->m_Unknown0x122++;
        totalSize += pHistoryPacket->m_Unknown0x134;
        if (pHistoryPacket->m_Unknown0x122 >= g_MaxHistoryPacketBundleCount) {
            // (kept until it is packed below)
            pHistoryPacket->AcquireRef();
            it = PacketHistoryChainDequeue(pHistory, it);
        } else {
            it = HistoryChain::iterator(pHistoryPacket->m_pHistoryNext);
        }
    }
    // the older packets leave the history
    while (it != pHistory->end()) {
        if ((*it)->m_Unknown0x110 < time) {
            it = PacketHistoryChainDequeue(pHistory, it);
        } else {
            it = HistoryChain::iterator((*it)->m_pHistoryNext);
        }
    }
    if (totalSize == initialSize) {
        return 0;
    }

    u32 count = bundle.m_Count;
    ByteStream* pPacketStream = new ByteStream();
    PRUDPMessageSelector* pSelector = &Network::GetInstance()->m_MessageSelector;
    TempChain::iterator bundled = bundle.begin();
    while (bundled != bundle.end()) {
        PacketOut* pHistoryPacket = static_cast<PacketOut*>(*bundled);
        pPacketStream->m_pBuffer->m_ContentSize = 0;
        pPacketStream->m_pBuffer->m_HeaderSize = 0;
        pPacketStream->m_IsError = false;
        pPacketStream->m_Position = 0;
        pSelector->Pack(pHistoryPacket, pPacketStream);
        if (!pHistoryPacket->m_Unknown0x138) {
            pHistoryPacket->m_Unknown0x138 = 1;
        }
        if (pHistoryPacket->m_Unknown0x122 < g_MaxHistoryPacketBundleCount) {
            bundled = TempChain::iterator(pHistoryPacket->m_pTempNext);
        } else {
            bundled = bundle.erase(bundled);
            pHistoryPacket->ReleaseRef();
        }
        Buffer* pData = pPacketStream->m_pBuffer;
        pStream->m_pBuffer->AppendData(pData->m_pData + pData->m_HeaderSize, pData->m_ContentSize, static_cast<unsigned>(-1));
    }
    if (pPacketStream != nullptr) {
        delete pPacketStream;
    }
    return count;
}

// 0x007E4654
// 0x007E4624 (deleting dtor)
template qChain<Packet*, ChainPolicyTemp<Packet*> >::~qChain();

} // namespace nex
} // namespace nn
