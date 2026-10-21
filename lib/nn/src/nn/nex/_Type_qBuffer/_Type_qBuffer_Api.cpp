#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_qBuffer.h"

namespace nn {
namespace nex {
// 0x0036FAE8 | fefates:bytes [tier B]
void nn::nex::_Type_qBuffer::Add(nn::nex::Message* pMessage, const nn::nex::qBuffer& buffer)
{
    u16 size = buffer.size();
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
    if (buffer.size() != 0) {
        pMessage->AppendRaw(buffer.m_pBegin, buffer.size());
    }
}

// 0x0036FB38 | fefates:bytes [tier B]
bool nn::nex::_Type_qBuffer::Extract(nn::nex::Message* pMessage, nn::nex::qBuffer* pBuffer)
{
    u16 size;
    pMessage->ExtractPrimitiveInline(&size);
    if (pMessage->m_pBuffer->m_ContentSize - pMessage->m_Position < size) {
        pMessage->m_IsError = true;
        return false;
    }
    pBuffer->clear();
    if (size != 0) {
        Buffer* pData = pMessage->GetBuffer();
        pBuffer->push_back(pData->m_pData + pData->m_HeaderSize + pMessage->m_Position, size);
        if (pMessage->m_Position + size <= pMessage->m_pBuffer->m_ContentSize) {
            pMessage->m_Position += size;
        }
    }
    return true;
}

} // namespace nex
} // namespace nn
