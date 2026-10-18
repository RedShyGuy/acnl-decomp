#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_TransportAdapter.h"

namespace nn {
namespace nex {
// The bundling of the packets of a stream (part of StreamSettings). The layout is from the
// constructor of StreamSettings; the member names are ours.
// RTTI N2nn3nex14StreamBundlingE @ 0x008CE3DC
// vtable 0x008FCD68 (vptr 0x008FCD70), offset_to_top 0, 2 entries
class StreamBundling : public ::nn::nex::RootObject
{
public:
    // (inline)
    StreamBundling()
        : m_IsEnabled(0), m_Interval(0), m_LastTime(0), m_Unknown0x18(0xFFFF), m_Unknown0x1A(0xFFFF), m_Lock(0x20000000)
    {
    }
    virtual ~StreamBundling(); // 0x003745A4 slot 0x00
    // 0x00374568 slot 0x04 (deleting dtor)

    // (inline) the values without the lock
    StreamBundling& operator=(const StreamBundling& other)
    {
        m_IsEnabled = other.m_IsEnabled;
        m_Interval = other.m_Interval;
        m_LastTime = other.m_LastTime;
        m_Unknown0x18 = other.m_Unknown0x18;
        m_Unknown0x1A = other.m_Unknown0x1A;
        return *this;
    }

    u8 m_IsEnabled;          // 0x04
    s32 m_Interval;          // 0x08 (milliseconds between the sendings of the bundles)
    u64 m_LastTime;          // 0x10 (the time of the last sending of the bundles)
    u16 m_Unknown0x18;       // 0x18
    u16 m_Unknown0x1A;       // 0x1A
    CriticalSection m_Lock;  // 0x1C
};
ASSERT_OFFSET(StreamBundling, m_Lock, 0x1C);
ASSERT_SIZE(StreamBundling, 0x48);

// switches of the bundling of the packets (read by PRUDPMessageV0::Pack and the history queue;
// the names are ours)
extern bool g_Unknown0x96C92B;
extern bool g_Unknown0x96C92C;
extern bool g_Unknown0x96C92D;
extern bool g_Unknown0x96C92E;

// the statistics of the bundling (QueuingSocket::CreateBundle; the names are ours)
struct BundleStatistics
{
    u32 m_WithoutHistoryCount;  // 0x00 (no packet for the history)
    u32 m_EmptyHistoryCount;    // 0x04 (no packet of the history in the bundle)
    u32 m_HistoryCount;         // 0x08
    u32 m_HalfFullHistoryCount; // 0x0C (at least half of the maximum)
    u32 m_FullHistoryCount;     // 0x10
    u32 m_BundleCount;          // 0x14
};
extern BundleStatistics g_BundleStatistics;
// the packets of the history that are bundled again were sent this many milliseconds ago
extern s32 g_BundlingHistoryDelay;
// the packets of the history are due again after this many milliseconds
extern s32 g_BundlingRetransmissionDelay;
// the maximum size of a bundle
extern u32 g_MaxBundleSize;
// the maximum number of packets in the history
extern u16 g_MaxBundlingHistoryCount;
// the maximum number of packets of the history in one bundle
extern u16 g_MaxBundledHistoryPacketCount;
// a packet of the history goes into this many bundles at most
extern u16 g_MaxHistoryPacketBundleCount;

// (inline) a reliable data packet of the PRUDP stream that goes into the bundling history (the name
// is ours)
inline bool IsBundlingHistoryPacket(const Packet* pPacket)
{
    if (!g_Unknown0x96C92B) {
        return false;
    }
    u16 flags = pPacket->m_TypeFlags;
    if ((flags & Packet::TYPE_MASK) != Packet::TYPE_DATA || pPacket->GetStreamType() != 1) {
        return false;
    }
    if (!g_Unknown0x96C92D && !(flags & 0x30)) {
        return false;
    }
    if ((!g_Unknown0x96C92C || (flags & 0x10)) && (flags & 0x30)) {
        return false;
    }
    return true;
}

// (inline) the packet can go into the bundle of the packet that was just bundled (the name is ours)
inline bool CanBundleWith(const Packet* pPacket, const Packet* pBundled)
{
    if (pBundled->m_Unknown0x66 != pPacket->m_Unknown0x66 || pBundled->m_Unknown0x86 != pPacket->m_Unknown0x86) {
        return false;
    }
    // (0x200: broadcast)
    if (TransportAdapter::s_pInstance->m_Unknown0x04 == 1 && (pPacket->m_TypeFlags & 0x200) && (pBundled->m_TypeFlags & 0x200)) {
        return true;
    }
    return pPacket->m_Address.m_SockAddr.m_Address == pBundled->m_Address.m_SockAddr.m_Address &&
           pPacket->m_Address.m_SockAddr.m_Port == pBundled->m_Address.m_SockAddr.m_Port;
}
} // namespace nex
} // namespace nn
