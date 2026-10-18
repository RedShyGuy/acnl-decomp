#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class Packet;
class RootTransport;

// The streams of a transport by their port (the stream type in the high 4 bits, the port in the
// low ones), sorted by it. The names of the members and of the functions without a reference name
// are ours.
// RTTI N2nn3nex22TransportStreamManagerE @ 0x008CEC74
// vtable 0x008FE1D8 (vptr 0x008FE1E0), offset_to_top 0, 2 entries
class TransportStreamManager : public ::nn::nex::RootObject
{
public:
    // the highest port of a stream type
    static const u8 MAX_PORT = 15;

    // a listening stream
    struct Entry
    {
        u8 m_Key;           // 0x0 (stream type << 4 | port)
        Stream* m_pStream;  // 0x4
    };

    TransportStreamManager(); // 0x0039EAE0 | fefates:bytes [tier B]
    virtual ~TransportStreamManager(); // 0x0039EC70 slot 0x00 | fefates:bytes
    // 0x0039EC40 (deleting dtor)
    // the stream listens on the port (0: the highest free one; pPort: the port)
    bool RegisterStream(nn::nex::Stream* pStream, unsigned char port, unsigned char* pPort); // 0x0039E2A8 (name is ours)
    bool UnregisterStream(nn::nex::Stream::Type type, unsigned char port); // 0x0039E654
    // the packet goes to the stream of its destination port
    void DispatchPacket(nn::nex::Packet* pPacket); // 0x0039E7C4 (name is ours)
    // the stream of the key (null if none)
    nn::nex::Stream* FindStream(const unsigned char& key); // 0x0039E9B8 (name is ours)

    // (inline) the first entry whose key is not less than key
    Entry* LowerBound(u8 key) const
    {
        Entry* pFirst = m_Streams.begin();
        int count = m_Streams.end() - pFirst;
        while (count > 0) {
            int half = count / 2;
            Entry* pMiddle = pFirst + half;
            if (pMiddle->m_Key < key) {
                pFirst = pMiddle + 1;
                count -= half + 1;
            } else {
                count = half;
            }
        }
        return pFirst;
    }
    // (inline) the entry of the key (end if none)
    Entry* FindEntry(u8 key) const
    {
        Entry* pEntry = LowerBound(key);
        if (pEntry != m_Streams.end() && pEntry->m_Key > key) {
            pEntry = m_Streams.end();
        }
        return pEntry;
    }

    u32 m_Unknown0x04;          // 0x04
    qVector<Entry> m_Streams;   // 0x08
    RootTransport* m_pTransport; // 0x14
};
ASSERT_SIZE(TransportStreamManager, 0x18);
} // namespace nex
} // namespace nn
