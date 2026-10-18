#pragma once

#include "decomp.h"
#include "nn/nex/nex_HighLevelStream.h"

namespace nn {
namespace nex {
class NATTraversalEngine;

// The stream of the probes of the NAT traversal (the payloads go to the engine). The names of the
// members are ours.
// RTTI N2nn3nex18NATTraversalStreamE @ 0x008CE730
// vtable 0x008FD544 (vptr 0x008FD54C), offset_to_top 0, 7 entries
class NATTraversalStream : public ::nn::nex::HighLevelStream
{
public:
    // the stream type of the probes
    static const u8 TYPE = 5;
    // the port of the probes
    static const u8 PORT = 1;

    // (inline in NATTraversalEngine::RegisterRelay)
    explicit NATTraversalStream(NATTraversalEngine* pEngine) : HighLevelStream(static_cast<Stream::Type>(TYPE), nullptr), m_ReceivedCount(0), m_pEngine(pEngine)
    {
        Listen(PORT);
    }
    virtual ~NATTraversalStream(); // 0x003902A8 slot 0x00
    // 0x00390224 slot 0x04 (deleting dtor)
    virtual void DoWork(); // 0x00390210 slot 0x0C
    virtual void ProcessPayload(const nn::nex::InetAddress* pAddress, unsigned char sourceType, unsigned char sourcePort, nn::nex::Buffer* pPayload,
                                const nn::nex::Time& time, unsigned short port); // 0x00390170 slot 0x14 | mk7dlp:callseq

    u32 m_ReceivedCount;           // 0x24
    NATTraversalEngine* m_pEngine; // 0x28
};
ASSERT_SIZE(NATTraversalStream, 0x2C);
} // namespace nex
} // namespace nn
