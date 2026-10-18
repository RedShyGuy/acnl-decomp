#include "nn/nex/nex_TransportStreamManager.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StationURL.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// the key of a stream type and port
inline u8 MakeKey(u32 type, u8 port)
{
    return static_cast<u8>(port | (type << 4));
}

// slot 0x0C of the object of the transport that may take a received packet (its class is not
// known): true if it took the packet; it may change the address in the URL
typedef bool (*TakePacketFunction)(void* pObject, StationURL* pURL, u16 port);
inline TakePacketFunction GetTakePacketFunction(void* pObject)
{
    return reinterpret_cast<TakePacketFunction>((*reinterpret_cast<void***>(pObject))[3]);
}

// the initial capacity of the streams
const u32 INITIAL_CAPACITY = 16;
} // namespace

// 0x0039E2A8 (name is ours)
bool nn::nex::TransportStreamManager::RegisterStream(nn::nex::Stream* pStream, unsigned char port, unsigned char* pPort)
{
    ScopedCS lock(GetSchedulerLock());
    u8 key;
    if (port == 0) {
        // the highest free port
        port = MAX_PORT;
        for (;;) {
            key = MakeKey(pStream->m_Type, port);
            if (FindStream(key) == nullptr) {
                break;
            }
            if (--port == 0) {
                return false;
            }
        }
    } else {
        key = MakeKey(pStream->m_Type, port);
    }
    if (FindStream(key) != nullptr) {
        return false;
    }
    Entry entry;
    entry.m_Key = key;
    entry.m_pStream = pStream;
    Entry* pEntry = LowerBound(key);
    if (pEntry == m_Streams.end() || key < pEntry->m_Key) {
        m_Streams.insert(pEntry, entry);
    }
    if (pPort != nullptr) {
        *pPort = port;
    }
    return true;
}

// 0x0039E654
bool nn::nex::TransportStreamManager::UnregisterStream(nn::nex::Stream::Type type, unsigned char port)
{
    ScopedCS lock(GetSchedulerLock());
    u8 key = MakeKey(type, port);
    if (FindStream(key) == nullptr) {
        return false;
    }
    Entry* pEntry = FindEntry(key);
    if (pEntry != m_Streams.end() && m_Streams.begin() != m_Streams.end()) {
        m_Streams.erase(pEntry);
    }
    return true;
}

// 0x0039E7C4 (name is ours)
void nn::nex::TransportStreamManager::DispatchPacket(nn::nex::Packet* pPacket)
{
    u8 key = pPacket->m_DestinationPort;
    u16 port = pPacket->m_Unknown0x86;
    Stream* pStream = FindStream(key);
    if (pStream == nullptr) {
        // (the streams were logged)
        {
            ScopedCS lock(GetSchedulerLock());
            for (Entry* pEntry = m_Streams.begin(); pEntry != m_Streams.end(); pEntry++) {
                // nothing to do
            }
        }
        pPacket->ReleaseRef();
        return;
    }
    void* pObject = m_pTransport->m_pUnknown0xAC;
    if (pObject != nullptr) {
        bool isTaken;
        {
            StationURL url;
            url.SetInetAddress(&pPacket->m_Address);
            isTaken = GetTakePacketFunction(pObject)(pObject, &url, port);
            pPacket->m_Address.m_SockAddr = url.GetInetAddress()->m_SockAddr;
        }
        if (isTaken) {
            pPacket->ReleaseRef();
            return;
        }
    }
    if (!pStream->ReceiveIncomingPacket(port, pPacket->m_SourcePort & 0xF, pPacket)) {
        m_pTransport->ReceiveUnhandled(pPacket->m_pPayload, &pPacket->m_Address);
        pPacket->ReleaseRef();
    }
}

// 0x0039E9B8 (name is ours)
nn::nex::Stream* nn::nex::TransportStreamManager::FindStream(const unsigned char& key)
{
    if (Core::s_IsThreadSafe) {
        ScopedCS lock(GetSchedulerLock());
        Entry* pEntry = FindEntry(key);
        return pEntry != m_Streams.end() ? pEntry->m_pStream : nullptr;
    }
    Entry* pEntry = FindEntry(key);
    return pEntry != m_Streams.end() ? pEntry->m_pStream : nullptr;
}

// 0x0039EAE0 | fefates:bytes [tier B]
nn::nex::TransportStreamManager::TransportStreamManager()
{
    m_Streams.reserve(INITIAL_CAPACITY);
}

// 0x0039EC70 slot 0x00 | fefates:bytes
// 0x0039EC40 (deleting dtor)
nn::nex::TransportStreamManager::~TransportStreamManager()
{
    // nothing to do: the streams are destroyed by the compiler
}

} // namespace nex
} // namespace nn
