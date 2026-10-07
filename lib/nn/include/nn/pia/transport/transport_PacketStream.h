#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Packet.h"

namespace nn {
namespace pia {
namespace transport {
// A ring of packets between a writer and a reader (the send and receive threads). The writer
// assigns packets and pushes them, the reader pulls them and releases them. Layout from the
// constructor and Initialize; the member names are ours.
class PacketStream
{
public:
    // a position in the ring (base of Reader and Writer)
    class Accessor
    {
    public:
        explicit Accessor(PacketStream* pStream) : m_pStream(pStream), m_Head(-1) {}

        // the packet index entries after the head, null outside of the ring
        common::Packet* Get(int index); // 0x0044DD70 | fefates:bytes [tier B]

        PacketStream* m_pStream; // 0x0
        s32 m_Head;              // 0x4, pushed (writer) / released (reader) position
    };

    class Reader : public Accessor
    {
    public:
        explicit Reader(PacketStream* pStream) : Accessor(pStream), m_Position(-1), m_Count(0) {}

        // takes all pushed packets / the next one
        void PullAll(); // 0x0044DB78 | fefates:bytes [tier B]
        common::Packet* PullOne(); // 0x0044DBA8 | fefates:bytes [tier B]
        void Release(); // 0x0044DC18 | fefates:bytes [tier B]

        s32 m_Position; // 0x8
        s32 m_Count;    // 0xC
    };

    class Writer : public Accessor
    {
    public:
        explicit Writer(PacketStream* pStream) : Accessor(pStream), m_Position(-1), m_Count(0) {}

        // the next free packet, null if the ring is full
        common::Packet* Assign(); // 0x0044DC40 | fefates:bytes [tier B]
        void Push(); // 0x0044DC2C | fefates:bytes [tier B]

        s32 m_Position; // 0x8
        s32 m_Count;    // 0xC
    };

    PacketStream(); // 0x0044DE40 | fefates:bytes [tier B]

    // packetNum packets (at least 2) on the current heap; watermarkIndex is the watermark of the
    // use of the ring (parameter names are ours)
    nn::Result Initialize(unsigned int packetNum, unsigned int watermarkIndex); // 0x0044DAAC | fefates:bytes [tier B]
    void Finalize(); // 0x0044DDAC | fefates:bytes [tier B]
    nn::Result Startup(); // 0x0044DD38 | fefates:bytes [tier B]
    void Cleanup(); // 0x0044DD14 | fefates:bytes [tier B]

    // the packet at index, null outside of the ring
    common::Packet* GetPacket(u32 index) const
    {
        return index < m_PacketNum ? &m_pPackets[index] : nullptr;
    }

    u32 m_PacketNum;           // 0x00
    common::Packet* m_pPackets; // 0x04
    Writer m_Writer;           // 0x08
    Reader m_Reader;           // 0x18
    u32 m_WatermarkIndex;      // 0x28
};
ASSERT_SIZE(PacketStream, 0x2C);
} // namespace transport
} // namespace pia
} // namespace nn
