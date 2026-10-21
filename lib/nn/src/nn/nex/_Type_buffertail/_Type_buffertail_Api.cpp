#include "nn/nex/_Type_buffertail/_Type_buffertail_Api.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_Message.h"

namespace nn {
namespace nex {
// 0x003846F0 | fefates:bytes [tier B]
void nn::nex::_Type_buffertail::Add(nn::nex::Message* pMessage, const nn::nex::Buffer& buffer)
{
    if (buffer.m_ContentSize != 0) {
        pMessage->GetBuffer()->AppendData(buffer.m_pData + buffer.m_HeaderSize, buffer.m_ContentSize, static_cast<unsigned>(-1));
    }
}

// 0x00384728 | fefates:bytes [tier B]
void nn::nex::_Type_buffertail::Extract(nn::nex::Message* pMessage, nn::nex::Buffer* pBuffer)
{
    u32 position = pMessage->m_Position;
    u32 size = pMessage->GetBuffer()->m_ContentSize;
    pBuffer->m_ContentSize = 0;
    pBuffer->m_HeaderSize = 0;
    if (size > position) {
        u32 rest = size - position;
        Buffer* pData = pMessage->GetBuffer();
        pBuffer->AppendData(pData->m_pData + pData->m_HeaderSize + pMessage->m_Position, rest, static_cast<unsigned>(-1));
        if (pMessage->m_Position + rest <= pMessage->m_pBuffer->m_ContentSize) {
            pMessage->m_Position += rest;
        }
    }
}

} // namespace nex
} // namespace nn
