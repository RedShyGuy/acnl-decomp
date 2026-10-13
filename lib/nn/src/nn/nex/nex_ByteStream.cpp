#include "nn/nex/nex_ByteStream.h"

namespace nn {
namespace nex {
// 0x00354C70 | fefates:bytes-fuzzy [tier B]
bool nn::nex::ByteStream::ExtractRaw(unsigned char* pData, unsigned int size)
{
    bool isOk = true;
    u32 available = m_pBuffer->m_ContentSize - m_Position;
    if (available < size) {
        m_IsError = true;
        isOk = false;
        size = m_pBuffer->m_ContentSize - m_Position;
    }
    if (size != 0) {
        m_pBuffer->CopyContent(pData, m_Position, size);
        m_Position += size;
    }
    return isOk;
}

// 0x00354D20 | fefates:bytes [tier B]
bool nn::nex::ByteStream::Append(const nn::nex::Time* pTime)
{
    u8 bytes[sizeof(u64)];
    u64 value = pTime->m_Value;
    for (u32 i = 0; i < sizeof(u64); i++) {
        bytes[i] = static_cast<u8>(value >> (i * 8));
    }
    m_pBuffer->AppendData(bytes, sizeof(bytes), static_cast<unsigned>(-1));
    return true;
}

// 0x00354DA0 | fefates:bytes [tier B]
bool nn::nex::ByteStream::SetLength(unsigned int length)
{
    Buffer* pBuffer = m_pBuffer;
    if (pBuffer->m_Size - pBuffer->m_HeaderSize < length) {
        return false;
    }
    pBuffer->m_ContentSize = length;
    return true;
}

// 0x00354DF0 | fefates:callgraph
// 0x00354DC0 | fefates:callseq (deleting dtor)
nn::nex::ByteStream::~ByteStream()
{
    m_pBuffer->ReleaseRef();
    m_pBuffer = nullptr;
}

// 0x00354E58 | mk7dlp:bytes [tier A]
nn::nex::ByteStream& nn::nex::ByteStream::operator<<(const bool& value)
{
    u8 byte = value ? 1 : 0;
    m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
    return *this;
}

// 0x00354E8C | fefates:bytes [tier B]
nn::nex::ByteStream& nn::nex::ByteStream::operator>>(bool& value)
{
    s8 byte;
    if (m_pBuffer->m_ContentSize - m_Position < sizeof(byte)) {
        m_IsError = true;
        byte = 0;
    } else {
        byte = static_cast<s8>(m_pBuffer->m_pData[m_pBuffer->m_HeaderSize + m_Position]);
        m_Position++;
    }
    value = byte != 0;
    return *this;
}

// 0x003CF8EC | mk7dlp:callgraph [tier A]
bool nn::nex::ByteStream::AppendRaw(const unsigned char* pData, unsigned size)
{
    return m_pBuffer->AppendData(pData, size, static_cast<unsigned>(-1));
}

// 0x007D39E8 | fefates:bytes [tier B]
template bool nn::nex::ByteStream::ExtractPrimitive<unsigned char>(unsigned char*);
// 0x007D3A48 | fefates:bytes [tier B]
template bool nn::nex::ByteStream::ExtractPrimitive<unsigned int>(unsigned int*);
// 0x007D3AB8 | fefates:bytes [tier B]
template bool nn::nex::ByteStream::ExtractPrimitive<unsigned short>(unsigned short*);
// 0x007D3B1C | fefates:bytes [tier B]
template bool nn::nex::ByteStream::ExtractPrimitive<unsigned long long>(unsigned long long*);

} // namespace nex
} // namespace nn
