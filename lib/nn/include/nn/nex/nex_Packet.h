#pragma once

#include "decomp.h"
#include "nn/nex/nex_qChain.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_LiteByteStream.h"
#include "nn/nex/nex_LogicalClockTmpl.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_SignatureBytes.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// A PRUDP packet: the fields of the header (written and read by PRUDPMessageV0/V1), the options,
// the payload and the address of the peer. The layout is from the constructor and the message
// classes; the member names are ours.
// RTTI N2nn3nex6PacketE @ 0x008CF5A0
// vtable 0x008FFA1C (vptr 0x008FFA24), offset_to_top 0, 2 entries
class Packet : public ::nn::nex::RefCountedObject
{
public:
    // the type/flags word: the type in the low 4 bits, the flags above
    static const u16 TYPE_MASK = 0xF;
    // the types (the names are ours: SYN/CONNECT carry the connection signature, DATA the fragment
    // id; a RAW packet is only its payload)
    static const u32 TYPE_SYN = 0;
    static const u32 TYPE_CONNECT = 1;
    static const u32 TYPE_DATA = 2;
    static const u32 TYPE_DISCONNECT = 3;
    static const u32 TYPE_PING = 4;
    static const u32 TYPE_HIGH_LEVEL = 5; // (the packets of the HighLevelStreams)
    static const u32 TYPE_RAW = 7;

    Packet(); // 0x003D03CC | fefates:callgraph [tier C]
    virtual ~Packet(); // 0x003D0560 slot 0x00
    // 0x003D04C0 slot 0x04 (deleting dtor)

    // the header of a V1 message after the magic (the names are ours)
    struct HeaderV1
    {
        u8 m_Version;         // 0x0
        u8 m_OptionsSize;     // 0x1
        u16 m_PayloadSize;    // 0x2
        u8 m_SourcePort;      // 0x4
        u8 m_DestinationPort; // 0x5
        u16 m_TypeFlags;      // 0x6
        u8 m_SessionId;       // 0x8
        u8 m_SubstreamId;     // 0x9
        u16 m_SequenceId;     // 0xA
    };

    // the header of a relay message (RelayMessage; the names are ours). It lies in the data of
    // m_Options (Packet +0xA0), which a relay message has none of.
    struct RelayHeader
    {
        u16 m_Unknown0x00; // 0x00 (not written)
        u16 m_TypeFlags;   // 0x02 (the RelayType in the low 4 bits)
        u32 m_Unknown0x04; // 0x04
        u32 m_Unknown0x08; // 0x08
        u16 m_Unknown0x0C; // 0x0C
        u16 m_Unknown0x0E; // 0x0E
        u32 m_Unknown0x10; // 0x10 (masked on the wire; -2 and -3 skip the time stamp check)
        u32 m_Unknown0x14; // 0x14 (masked on the wire)
        u32 m_TimeStamp;   // 0x18 (seconds of the relay clock)
        // the payload of a ping
        u8 m_Unknown0x1C;  // 0x1C
        u8 m_Unknown0x1D;  // 0x1D
        u32 m_Unknown0x20; // 0x20
        u32 m_Unknown0x24; // 0x24
    };

    // (inline)
    u32 GetType() const { return m_TypeFlags & TYPE_MASK; }

    // (inline; the name is ours)
    RelayHeader& GetRelayHeader() { return *reinterpret_cast<RelayHeader*>(m_Options.m_Buffer.m_Data); }

    // (inline) the stream type of the source port
    u32 GetStreamType() const { return m_SourcePort >> 4; }

    Packet* m_pChainNext;                     // 0x0C (PacketQueue)
    Packet* m_pChainPrev;                     // 0x10
    Packet* m_pTempNext;                      // 0x14 (temporary chains: ChainPolicyTemp)
    Packet* m_pTempPrev;                      // 0x18
    Packet* m_pHistoryNext;                   // 0x1C (the history of the bundled packets: ChainPolicyHistoryPacket)
    Packet* m_pHistoryPrev;                   // 0x20
    u8 m_MessageVersion;                      // 0x24 (0 V0, 1 V1, 0xD0 relay; selects the message class)
    u8 m_SourcePort;                          // 0x25 (the stream type in the high 4 bits)
    u8 m_DestinationPort;                     // 0x26
    SignatureBytes m_PacketSignature;         // 0x28 (V0: the signature after the session id)
    u16 m_TypeFlags;                          // 0x3C
    LogicalClockTmpl<u16, s16> m_SequenceId;  // 0x40
    LogicalClockTmpl<u16, s16> m_Unknown0x48; // 0x48 (V1 option 3)
    SignatureBytes m_Signature;               // 0x50 (V1 option 1)
    u8 m_Unknown0x64;                         // 0x64 (V1 option 2)
    u8 m_SessionId;                           // 0x65
    u8 m_Unknown0x66;                         // 0x66
    u8 m_Unknown0x67;                         // 0x67 (V1 option 4)
    HeaderV1 m_HeaderV1;                      // 0x68 (the V1 header as read / to be written)
    u8 m_Unknown0x74[0x10];                   // 0x74
    u8 m_SubstreamId;                         // 0x84
    u16 m_Unknown0x86;                        // 0x86
    u8 m_Unknown0x88;                         // 0x88
    bool m_IsHeaderV1Valid;                   // 0x89 (m_HeaderV1 is up to date)
    u32 m_Unknown0x8C;                        // 0x8C (V1 option 0)
    LiteByteStream<52> m_Options;             // 0x90
    Buffer* m_pPayload;                       // 0xD8
    InetAddress m_Address;                    // 0xDC
    Time m_Unknown0x100;                      // 0x100
    Time m_Unknown0x108;                      // 0x108
    Time m_Unknown0x110;                      // 0x110
};
ASSERT_OFFSET(Packet, m_TypeFlags, 0x3C);
ASSERT_OFFSET(Packet, m_Signature, 0x50);
ASSERT_OFFSET(Packet, m_Options, 0x90);
ASSERT_OFFSET(Packet, m_pPayload, 0xD8);
ASSERT_OFFSET(Packet, m_Unknown0x100, 0x100);
ASSERT_SIZE(Packet, 0x118);
ASSERT_SIZE(Packet::RelayHeader, 0x28);

// The links of a packet in the history of the bundled packets (Packet +0x1C/+0x20; the name is from
// the RTTI of the chain).
template <typename T>
class ChainPolicyHistoryPacket
{
public:
    static T& Next(T p) { return p->m_pHistoryNext; }
    static T& Prev(T p) { return p->m_pHistoryPrev; }
};

// The links of a packet in a temporary chain (Packet +0x14/+0x18; the name is from the RTTI of the
// chain).
template <typename T>
class ChainPolicyTemp
{
public:
    static T& Next(T p) { return p->m_pTempNext; }
    static T& Prev(T p) { return p->m_pTempPrev; }
};

// (out of line in the original; nex_Api.cpp)
template <>
DECOMP_NOINLINE qChain<Packet*, ChainPolicyHistoryPacket<Packet*> >::iterator qChain<Packet*, ChainPolicyHistoryPacket<Packet*> >::erase(iterator it);
} // namespace nex
} // namespace nn
