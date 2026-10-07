#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "nn/nstd/nstd_String.h"
#include "nn/pia/common/common_Packet.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
inline void StoreU32BE(u8* p, u32 value)
{
    *reinterpret_cast<u32*>(p) = __builtin_bswap32(value);
}

// the header flags of a packet of the message
inline u8 MakeFlags(bool isDestinationKey, bool isRelayRequest, bool isRelayed, bool isOwnPacket)
{
    return (isDestinationKey ? 1 : 0) | (isRelayRequest ? 2 : 0) | (isRelayed ? 4 : 0) | (isOwnPacket ? 8 : 0);
}
} // namespace

// 0x0045A2D8 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::SetPayload(const void* pData)
{
    nnnstdMemCpy(m_Buffers[0].m_pHeader + HEADER_SIZE, pData, m_PayloadSize);
}

// 0x0045A2E8 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::SetPayload(const void* pData, unsigned int offset, unsigned int size)
{
    nnnstdMemCpy(m_Buffers[0].m_pHeader + HEADER_SIZE + offset, pData, size);
}

// 0x0045A2FC | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::AddMessageBuffer(nn::pia::common::Packet* pPacket, void* pHeader, unsigned int destination, bool isDestinationKey, bool isRelayRequest)
{
    Buffer& buffer = m_Buffers[m_BufferNum];
    buffer.m_pPacket = pPacket;
    buffer.m_pHeader = static_cast<u8*>(pHeader);
    buffer.m_Destination = destination;
    buffer.m_IsDestinationKey = isDestinationKey;
    buffer.m_IsRelayRequest = isRelayRequest;
    m_BufferNum++;
}

// 0x0045A330 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::Reset(const nn::pia::transport::ProtocolId& protocolId, unsigned int payloadSize, bool isRelayed, bool isOwnPacket)
{
    m_PayloadSize = payloadSize;
    m_ProtocolId = protocolId;
    m_IsRelayed = isRelayed;
    m_IsOwnPacket = isOwnPacket;
    m_ReservedData = 0;
    m_Ttl = 0;
    m_BufferNum = 0;
}

// 0x0045A35C | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::Commit()
{
    Buffer& first = m_Buffers[0];
    first.m_pHeader[0] = MakeFlags(first.m_IsDestinationKey, first.m_IsRelayRequest, m_IsRelayed, m_IsOwnPacket);
    first.m_pHeader[1] = m_SourceStationIndex;
    *reinterpret_cast<u16*>(first.m_pHeader + 2) = __builtin_bswap16(m_PayloadSize);
    StoreU32BE(first.m_pHeader + 4, first.m_Destination);
    StoreU32BE(first.m_pHeader + 8, m_SourceStationKey);
    StoreU32BE(first.m_pHeader + 12, m_ProtocolId.m_Id);
    StoreU32BE(first.m_pHeader + 16, m_ReservedData);
    u32 messageSize = ((m_PayloadSize + HEADER_SIZE - 1) & ~3) + 4;
    for (u32 i = 1; i < m_BufferNum; i++) {
        Buffer& buffer = m_Buffers[i];
        nnnstdMemCpy(buffer.m_pHeader, first.m_pHeader, messageSize);
        buffer.m_pHeader[0] = MakeFlags(buffer.m_IsDestinationKey, buffer.m_IsRelayRequest, m_IsRelayed, m_IsOwnPacket);
        StoreU32BE(buffer.m_pHeader + 4, buffer.m_Destination);
    }
    if (m_Ttl != 0) {
        for (u32 i = 0; i < m_BufferNum; i++) {
            common::Packet* pPacket = m_Buffers[i].m_pPacket;
            if (pPacket->m_Ttl < m_Ttl) {
                pPacket->m_Ttl = m_Ttl;
            }
        }
    }
}

// 0x0045A510 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::SetSource(nn::pia::StationIndex stationIndex, unsigned int stationKey)
{
    m_SourceStationIndex = stationIndex;
    m_SourceStationKey = stationKey;
}

// 0x0045A51C | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageWriter::ProtocolMessageWriter()
    : m_SourceStationIndex(STATION_INDEX_UNIDENTIFIED), m_SourceStationKey(0), m_ProtocolId(ProtocolId::INVALID)
{
}

} // namespace transport
} // namespace pia
} // namespace nn
