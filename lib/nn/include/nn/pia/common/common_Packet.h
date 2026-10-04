#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Crypto.h"
#include "nn/pia/common/common_StationAddress.h"

namespace nn {
namespace pia {
namespace common {
// A packet of the current pia protocol: a 12 byte header (magic 0x6498AB32) and the payload in
// one buffer, and where it comes from / goes to. Layout from the constructor and Reset; the member
// names are ours.
class Packet
{
public:
    static const u32 MAGIC = 0x6498AB32;
    static const unsigned int HEADER_SIZE = 12;
    static const unsigned int BUFFER_SIZE_MAX = 0x5B6;
    static const unsigned int PAYLOAD_SIZE_MAX = BUFFER_SIZE_MAX - HEADER_SIZE;

    // m_State
    enum State : u8
    {
        STATE_PLAIN = 1,
        STATE_ENCRYPTED = 2,
    };

    Packet(); // 0x004292D0 | fefates:bytes [tier B]
    ~Packet(); // 0x00429358 | fefates:bytes [tier B]
    // the whole object (name after C++)
    Packet& operator=(const Packet& rhs); // 0x004292C4

    void Reset(); // 0x0042902C | fefates:bytes [tier B]
    // reserves size bytes after the used part; null if they do not fit
    u8* AssignPayload(unsigned int size); // 0x00429000 | fefates:bytes [tier B]
    nn::Result Decrypt(const nn::pia::common::Crypto::Setting& setting); // 0x00429098 | fefates:bytes [tier B]
    nn::Result Encrypt(const nn::pia::common::Crypto::Setting& setting); // 0x0042919C | fefates:bytes [tier B]
    // the stations the packet goes to (at least 1)
    int GetPacketNumInNetwork() const; // 0x00733374 | fefates:bytes [tier B]
    bool IsValid() const; // 0x0073338C | fefates:bytes [tier B]

    u8* GetPayload() { return reinterpret_cast<u8*>(this) + HEADER_SIZE; }

    // the header
    u32 m_Magic;                          // 0x000
    State m_State;                        // 0x004
    u8 m_Unknown0x5;                      // 0x005
    u16 m_Unknown0x6;                     // 0x006
    u16 m_Unknown0x8;                     // 0x008
    u16 m_Unknown0xA;                     // 0x00A
    u8 m_Payload[0x5AC];                  // 0x00C
    u32 m_Size;                           // 0x5B8, the used part of the buffer (with the header)
    u8 m_SourceStationIndex;              // 0x5BC
    u32 m_DestinationBitmap;              // 0x5C0
    StationAddress m_SourceStationAddress; // 0x5C4
    u8 m_Unknown0x5D4;                    // 0x5D4
    u8 m_Unknown0x5D5;                    // 0x5D5
    StationAddress m_DestinationStationAddress; // 0x5D8
    u8 m_Unknown0x5E8;                    // 0x5E8
};
ASSERT_SIZE(Packet, 0x5EC);
ASSERT_OFFSET(Packet, m_Size, 0x5B8);
} // namespace common
} // namespace pia
} // namespace nn
