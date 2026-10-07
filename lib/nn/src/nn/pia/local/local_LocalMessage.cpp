#include "nn/pia/local/local_LocalMessage.h"
#include "pead/peadHashCrc16.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x00414940 | fefates:callseq-callee
bool nn::pia::local::LocalMessage::ParseMessageHeader()
{
    Header* pHeader = reinterpret_cast<Header*>(m_pBuffer);
    if (pHeader->m_Checksum != pead::HashCrc16::calcHash(pHeader, CHECKSUM_SIZE)) {
        return false;
    }
    m_Type = pHeader->m_Type;
    m_DataSize = pHeader->m_DataSize;
    return true;
}

// 0x00414980 | fefates:bytes
void nn::pia::local::LocalMessage::UpdateMessageHeader()
{
    Header* pHeader = reinterpret_cast<Header*>(m_pBuffer);
    pHeader->m_Version = VERSION;
    pHeader->m_Type = m_Type;
    pHeader->m_DataSize = m_DataSize;
    std::memset(pHeader->m_Reserved, 0, sizeof(pHeader->m_Reserved));
    pHeader->m_Checksum = pead::HashCrc16::calcHash(m_pBuffer, CHECKSUM_SIZE);
}

// 0x004149C0 | fefates:bytes [tier B]
void nn::pia::local::LocalMessage::SetData(const void* pData, int offset, u16 size)
{
    m_DataSize = offset + size;
    std::memcpy(m_pBuffer + m_HeaderSize + offset, pData, size);
    UpdateMessageHeader();
}

// 0x00414A2C | fefates:bytes [tier B]
void nn::pia::local::LocalMessage::SetData(const void* pData, u16 size)
{
    std::memcpy(m_pBuffer + m_HeaderSize + m_DataSize, pData, size);
    m_DataSize += size;
    UpdateMessageHeader();
}

// 0x00414A74 | fefates:bytes [tier B]
nn::pia::local::LocalMessage::LocalMessage(u8* pBuffer, u32 bufferSize)
    : m_pBuffer(pBuffer), m_BufferSize(bufferSize), m_Type(1), m_HeaderSize(HEADER_SIZE), m_DataSize(0)
{
}

// 0x0072FBB0 | fefates:bytes [tier B]
void nn::pia::local::LocalMessage::GetData(void* pData, int offset, u16 size) const
{
    std::memcpy(pData, m_pBuffer + m_HeaderSize + offset, size);
}

// 0x00414AB0
// 0x00414AAC (deleting dtor)
nn::pia::local::LocalMessage::~LocalMessage()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
