#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_Message.h"

namespace nn {
namespace nex {
// The DDL type of a Buffer: a u32 size and the bytes. ARMCC inlines it everywhere (the class name is
// ours, after the other _Type_ classes).
class _Type_buffer
{
public:
    // (inline)
    static void Add(nn::nex::Message* pMessage, const nn::nex::Buffer& buffer)
    {
        u32 size = buffer.m_ContentSize;
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
        pMessage->AppendRaw(buffer.m_pData + buffer.m_HeaderSize, size);
    }
    // (inline) the message has an error if it is too short
    static void Extract(nn::nex::Message* pMessage, nn::nex::Buffer* pBuffer)
    {
        u32 size;
        if (!pMessage->ExtractPrimitiveInline(&size)) {
            return;
        }
        if (pMessage->m_pBuffer->m_ContentSize - pMessage->m_Position < size) {
            pMessage->m_IsError = true;
            return;
        }
        Buffer* pData = pMessage->GetBuffer();
        pBuffer->AppendData(pData->m_pData + pData->m_HeaderSize + pMessage->m_Position, size, static_cast<unsigned>(-1));
        if (pMessage->m_Position + size <= pMessage->m_pBuffer->m_ContentSize) {
            pMessage->m_Position += size;
        }
    }
};
} // namespace nex
} // namespace nn
