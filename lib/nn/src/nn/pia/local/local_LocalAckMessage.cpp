#include "nn/pia/local/local_LocalAckMessage.h"
#include "pead/peadHashCrc16.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x00416954 | fefates:bytes
bool nn::pia::local::LocalAckMessage::ParseMessageHeader()
{
    Header* pHeader = reinterpret_cast<Header*>(m_pBuffer);
    if (pHeader->m_Checksum != pead::HashCrc16::calcHash(pHeader, CHECKSUM_SIZE)) {
        return false;
    }
    m_Type = pHeader->m_Type;
    m_DataSize = pHeader->m_DataSize;
    m_AckValue = reinterpret_cast<AckHeader*>(m_pBuffer)->m_AckValue;
    return true;
}

// 0x004169A0 | fefates:bytes
void nn::pia::local::LocalAckMessage::UpdateMessageHeader()
{
    Header* pHeader = reinterpret_cast<Header*>(m_pBuffer);
    pHeader->m_Version = VERSION;
    pHeader->m_Type = m_Type;
    pHeader->m_DataSize = m_DataSize;
    std::memset(pHeader->m_Reserved, 0, sizeof(pHeader->m_Reserved));
    pHeader->m_Checksum = pead::HashCrc16::calcHash(m_pBuffer, CHECKSUM_SIZE);
    AckHeader* pAckHeader = reinterpret_cast<AckHeader*>(m_pBuffer);
    pAckHeader->m_AckValue = m_AckValue;
    pAckHeader->m_Reserved = 0;
}

// 0x004169F4
nn::pia::local::LocalAckMessage::LocalAckMessage(u8* pBuffer, u32 bufferSize, u8 type, u32 ackValue) : LocalMessage(pBuffer, bufferSize)
{
    m_Type = type;
    m_HeaderSize = ACK_HEADER_SIZE;
    m_AckValue = ackValue;
}

// 0x00416A28
// 0x00416A24 (deleting dtor)
nn::pia::local::LocalAckMessage::~LocalAckMessage()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
