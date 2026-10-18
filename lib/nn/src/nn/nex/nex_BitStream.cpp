#include "nn/nex/nex_BitStream.h"
#include "nn/nex/nex_Buffer.h"

namespace nn {
namespace nex {
// 0x003D620C | fefates:callgraph [tier C]
void nn::nex::BitStream::Extract(unsigned char* pData, unsigned int count, unsigned int size)
{
    ExtractRaw(pData, count * size * 8);
}

// 0x003D6218 | fefates:bytes-fuzzy [tier B]
void nn::nex::BitStream::ExtractRaw(unsigned char* pData, unsigned int bits)
{
    if (GetPosition() + bits > m_BitLength) {
        m_IsError = true;
        return;
    }
    u32 index = 0;
    if (m_BitIndex == 7) {
        // whole bytes
        u32 bytes = bits >> 3;
        index = bytes;
        m_pBuffer->CopyContent(pData, m_ByteIndex, bytes);
        m_ByteIndex += bytes;
    } else {
        for (u32 count = 0; count + 8 <= bits; count += 8, index++) {
            if (GetPosition() + 8 > m_BitLength) {
                m_IsError = true;
                continue;
            }
            u8 shift = m_BitIndex + 1;
            u8 high = (*m_pBuffer)[m_ByteIndex] << (8 - shift);
            pData[index] = high | ((*m_pBuffer)[m_ByteIndex + 1] >> shift);
            m_ByteIndex++;
        }
    }
    u8 rest = bits & 7;
    if (rest == 0) {
        return;
    }
    u8 value = 0;
    u8 mask = 0x80;
    do {
        bool isSet = false;
        if (GetPosition() + 1 > m_BitLength) {
            m_IsError = true;
        } else {
            isSet = ((*m_pBuffer)[m_ByteIndex] & static_cast<u8>(1 << m_BitIndex)) != 0;
            if (m_BitIndex == 0) {
                m_BitIndex = 8;
                m_ByteIndex++;
            }
            m_BitIndex--;
        }
        if (isSet) {
            value |= mask;
        }
        mask >>= 1;
    } while (--rest != 0);
    pData[index] = value;
}

// 0x003D63FC | fefates:callgraph [tier C]
void nn::nex::BitStream::AdjustLength()
{
    m_BitLength = m_pBuffer->m_ContentSize << 3;
}

// 0x003D6410 | fefates:callgraph [tier C]
void nn::nex::BitStream::Append(const unsigned char* pData, unsigned int count, unsigned int size)
{
    AppendRaw(pData, count * size * 8);
}

// 0x003D641C | mk7dlp:callseq [tier A]
void nn::nex::BitStream::AppendRaw(const unsigned char* pData, unsigned bits)
{
    m_BitIndex = 7 - (m_BitLength & 7);
    m_ByteIndex = m_BitLength >> 3;
    u32 needed = m_ByteIndex + 1 + ((7 - m_BitIndex + bits) >> 3);
    u32 room = m_pBuffer->m_Size - m_pBuffer->m_HeaderSize;
    if (room < needed) {
        u32 grown = room + (room >> 1);
        m_pBuffer->ResizeByRealSize(grown > needed ? m_pBuffer->m_HeaderSize + grown : needed + m_pBuffer->m_HeaderSize);
    }
    u32 index = 0;
    if (m_BitIndex == 7) {
        // whole bytes
        u32 bytes = bits >> 3;
        index = bytes;
        m_pBuffer->AppendData(pData, bytes, m_ByteIndex);
        m_ByteIndex += bytes;
        m_BitLength += bytes * 8;
    } else {
        for (u32 count = 0; count + 8 <= bits; count += 8, index++) {
            u8 value = pData[index];
            if (m_BitIndex != 7) {
                u8 shift = m_BitIndex + 1;
                u8 low = value << shift;
                u8 high = value >> (8 - shift);
                (*m_pBuffer)[m_ByteIndex] = high | (*m_pBuffer)[m_ByteIndex];
                (*m_pBuffer)[m_ByteIndex + 1] = low;
            } else {
                (*m_pBuffer)[m_ByteIndex] = value;
            }
            m_BitLength += 8;
            m_ByteIndex++;
        }
    }
    u8 rest = bits & 7;
    if (rest == 0) {
        return;
    }
    u8 value = pData[index];
    u8 mask = 0x80;
    do {
        bool isSet = (value & mask) != 0;
        if (m_BitIndex == 7) {
            (*m_pBuffer)[m_ByteIndex] = isSet ? 0x80 : 0;
        } else if (isSet) {
            (*m_pBuffer)[m_ByteIndex] = (*m_pBuffer)[m_ByteIndex] | (1 << m_BitIndex);
        }
        if (m_BitIndex == 0) {
            m_ByteIndex++;
            m_BitIndex = 8;
        }
        mask >>= 1;
        m_BitIndex--;
        m_BitLength++;
    } while (--rest != 0);
}

// 0x003D6618 | fefates:bytes [tier B]
nn::nex::BitStream::BitStream(nn::nex::Buffer* pBuffer) : m_pBuffer(pBuffer)
{
    pBuffer->AcquireRef();
    m_IsError = false;
    m_BitLength = 0;
    m_ByteIndex = 0;
    m_BitIndex = 7;
    AdjustLength();
}

// 0x003D6670 | fefates:bytes [tier B]
nn::nex::BitStream::BitStream() : m_pBuffer(new Buffer())
{
    m_IsError = false;
    m_BitLength = 0;
    m_ByteIndex = 0;
    m_BitIndex = 7;
}

// 0x003D6734 slot 0x00
// 0x003D66B4 (deleting dtor)
nn::nex::BitStream::~BitStream()
{
    m_pBuffer->ReleaseRef();
    m_pBuffer = nullptr;
}

// 0x003D679C | mk7dlp:bytes [tier A]
nn::nex::BitStream& nn::nex::BitStream::operator<<(const nn::nex::Buffer& buffer)
{
    u32 size = buffer.m_ContentSize;
    AppendRaw(reinterpret_cast<const u8*>(&size), 32);
    if (buffer.m_ContentSize != 0) {
        AppendRaw(buffer.m_pData + buffer.m_HeaderSize, buffer.m_ContentSize * 8);
    }
    return *this;
}

// 0x003D67EC | fefates:bytes [tier B]
nn::nex::BitStream& nn::nex::BitStream::operator>>(nn::nex::Buffer& buffer)
{
    u32 size;
    ExtractRaw(reinterpret_cast<u8*>(&size), 32);
    if (GetPosition() + size * 8 > m_BitLength) {
        m_IsError = true;
        return *this;
    }
    if (m_IsError) {
        return *this;
    }
    if (size != 0) {
        buffer.ResizeByRealSize(buffer.m_HeaderSize + size);
        ExtractRaw(buffer.m_pData + buffer.m_HeaderSize, size * 8);
    }
    buffer.m_ContentSize = size;
    return *this;
}

// 0x003D687C | fefates:bytes [tier B]
nn::nex::BitStream& nn::nex::BitStream::operator>>(nn::nex::BitStream& stream)
{
    Buffer* pBuffer = stream.m_pBuffer;
    u32 size;
    ExtractRaw(reinterpret_cast<u8*>(&size), 32);
    if (GetPosition() + size * 8 > m_BitLength) {
        m_IsError = true;
    } else if (!m_IsError) {
        if (size != 0) {
            pBuffer->ResizeByRealSize(pBuffer->m_HeaderSize + size);
            ExtractRaw(pBuffer->m_pData + pBuffer->m_HeaderSize, size * 8);
        }
        pBuffer->m_ContentSize = size;
    }
    stream.AdjustLength();
    return *this;
}

} // namespace nex
} // namespace nn
