#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StationAddress.h"

namespace nn {
namespace pia {
namespace common {
// The packet format of the older pia protocol (magic 0x32AB9864, version 1): a 24 byte header,
// the payload and the signature. The class name and the static setters are from the fefates
// symbols; the layout is the one of Deserialize, the rest of the object (after the source
// address) is not known yet. The member names and the names marked so are ours.
class PacketOld
{
public:
    static const u32 MAGIC = 0x32AB9864;
    static const u8 VERSION = 1;
    static const unsigned int HEADER_SIZE = 24;
    static const unsigned int DEFAULT_PAYLOAD_SIZE_MAX = 0x59E;

    static nn::Result SetSignatureSize(unsigned int size); // 0x00429A94 | fefates:bytes [tier B]
    static bool IsValidSignatureSize(unsigned int size); // 0x00429AB8 | fefates:bytes [tier B]
    static nn::Result SetDefaultPayloadSize(unsigned int size); // 0x00429AD4 | fefates:bytes [tier B]
    static bool IsValidDefaultPayloadSize(unsigned int size); // 0x00429B14
    // whether the data starts with the magic
    static bool IsPacketOld(const unsigned char* pBuffer); // 0x00429A74

    // checks the header and the signature
    nn::Result Deserialize(const unsigned char* pBuffer, unsigned int size); // 0x004298D0
    // with the signature
    nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00733420
    nn::Result SetSourceStationAddress(const StationAddress& address); // 0x00429AFC

    u8 m_Version;            // 0x000
    u8 m_Unknown0x1;         // 0x001
    u16 m_PacketSize;        // 0x002
    u32 m_Unknown0x4;        // 0x004
    u16 m_PayloadSize;       // 0x008
    u8 m_Unknown0xA;         // 0x00A
    u8 m_Unknown0xB;         // 0x00B
    u16 m_Unknown0xC;        // 0x00C
    u8 m_Unknown0xE;         // 0x00E
    u8 m_Unknown0xF;         // 0x00F
    u16 m_Unknown0x10;       // 0x010
    u16 m_Unknown0x12;       // 0x012
    u8 m_Payload[0x5B0];     // 0x014
    StationAddress m_SourceStationAddress; // 0x5C4

    static u16 s_DefaultPayloadSize; // 0x0097F9F0
    static unsigned int s_SignatureSize; // 0x0097F9F4
    static u32 s_SerializeCount; // 0x0097FA00
    static u32 s_DeserializeCount; // 0x0097FA04
};
ASSERT_OFFSET(PacketOld, m_SourceStationAddress, 0x5C4);
} // namespace common
} // namespace pia
} // namespace nn
