#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/common/common_Packet.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
inline u32 LoadU32BE(const u8* p)
{
    u32 value = *reinterpret_cast<const u32*>(p);
    return __builtin_bswap32(value);
}
} // namespace

// 0x0045A1A8 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageReader::Clear()
{
    m_pHeader = nullptr;
    m_SourceAddress.Clear();
    m_Unknown0x1C = 0;
    m_PayloadSize = 0;
}

// 0x0045A1CC | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageReader::Attach(const nn::pia::common::Packet& packet, unsigned int offset)
{
    u32 payloadSize = packet.m_Size - common::Packet::HEADER_SIZE;
    if (offset >= payloadSize) {
        m_pHeader = nullptr;
        m_IsTerminated = false;
        return;
    }
    const u8* pHeader = reinterpret_cast<const u8*>(&packet) + offset + common::Packet::HEADER_SIZE;
    if (pHeader[0] == TERMINATOR) {
        m_pHeader = nullptr;
        m_IsTerminated = true;
        return;
    }
    m_IsTerminated = false;
    if (offset + HEADER_SIZE > payloadSize) {
        m_pHeader = nullptr;
        return;
    }
    u16 size = __builtin_bswap16(*reinterpret_cast<const u16*>(pHeader + 2));
    m_PayloadSize = size;
    if (offset + HEADER_SIZE + size > payloadSize) {
        m_pHeader = nullptr;
        return;
    }
    m_pHeader = pHeader;
    if (pHeader[0] & FLAG_RELAYED) {
        m_SourceAddress.Clear();
        m_Unknown0x1C = 0;
        m_Ttl = 0;
    } else {
        m_SourceAddress = packet.m_SourceStationAddress;
        m_Unknown0x1C = packet.m_Unknown0x5;
        m_Ttl = packet.m_Ttl;
    }
}

// 0x0045A298 | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageReader::ProtocolMessageReader() : m_pHeader(nullptr), m_IsTerminated(false)
{
}

// 0x0045A2B8 | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageReader::~ProtocolMessageReader()
{
    // nothing to do: the members are destroyed by the compiler
}

// 0x007361B0 | fefates:bytes [tier B]
u32 nn::pia::transport::ProtocolMessageReader::GetDestination() const
{
    return LoadU32BE(m_pHeader + 4);
}

// 0x007361C4 | fefates:bytes [tier B]
u32 nn::pia::transport::ProtocolMessageReader::GetReservedData() const
{
    return LoadU32BE(m_pHeader + 16);
}

// 0x007361D8 | fefates:bytes [tier B]
u16 nn::pia::transport::ProtocolMessageReader::GetProtocolIdPort() const
{
    return LoadU32BE(m_pHeader + 12);
}

// 0x007361F0 | fefates:bytes [tier B]
u32 nn::pia::transport::ProtocolMessageReader::GetSourceStationKey() const
{
    return LoadU32BE(m_pHeader + 8);
}

} // namespace transport
} // namespace pia
} // namespace nn
