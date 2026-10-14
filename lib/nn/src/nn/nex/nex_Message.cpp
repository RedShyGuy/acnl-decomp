#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"
#include "nn/nex/nex_EndPoint.h"

namespace nn {
namespace nex {
namespace {
const int RESULT_SUCCESS = qResult::SUCCESS;
} // namespace

// 0x003D2324 | fefates:bytes [tier B]
void nn::nex::Message::AddDataFromMessage(const nn::nex::Message& other)
{
    const Buffer* pOther = other.m_pBuffer;
    m_pBuffer->AppendData(pOther->m_pData + pOther->m_HeaderSize + SIZE_LENGTH, pOther->m_ContentSize - SIZE_LENGTH, static_cast<unsigned>(-1));
}

// 0x003D2348 | fefates:bytes [tier B]
void nn::nex::Message::SetSize()
{
    u32 size = m_pBuffer->m_ContentSize;
    m_pBuffer->m_ContentSize = 0;
    u32 messageSize = size - SIZE_LENGTH;
    u8 data[SIZE_LENGTH];
    data[0] = messageSize;
    data[1] = messageSize >> 8;
    data[2] = messageSize >> 16;
    data[3] = messageSize >> 24;
    m_pBuffer->AppendData(data, SIZE_LENGTH, static_cast<unsigned>(-1));
    if (m_pBuffer->m_Size - m_pBuffer->m_HeaderSize >= size) {
        m_pBuffer->m_ContentSize = size;
    }
}

// 0x003D23B8 | fefates:bytes [tier B]
nn::nex::Buffer* nn::nex::Message::GetBuffer()
{
    SetSize();
    return m_pBuffer;
}

// 0x003D23CC | fefates:bytes-fuzzy [tier B]
nn::nex::Message::Message(nn::nex::Buffer* pBuffer)
    : ByteStream(pBuffer), m_Unknown0x20(0), m_Unknown0x28(0), m_Unknown0x2C(0), m_Unknown0x30(0), m_pEndPoint(nullptr), m_Unknown0x38(0),
      m_Unknown0x39(0), m_Unknown0x3A(0), m_IsWithoutStructureHeader(0)
{
    m_Result = qResult(RESULT_SUCCESS);
    Rewind();
}

// 0x003D2490 (name from the reference symbols)
nn::nex::Message::Message()
    : m_Unknown0x20(0), m_Unknown0x28(0), m_Unknown0x2C(0), m_Unknown0x30(0), m_pEndPoint(nullptr), m_Unknown0x38(0), m_Unknown0x39(0),
      m_Unknown0x3A(0), m_IsWithoutStructureHeader(0)
{
    m_pBuffer->m_ContentSize = 0;
    m_pBuffer->m_HeaderSize = 0;
    m_IsError = false;
    m_Position = 0;
    m_Unknown0x20 = 0;
    m_Unknown0x38 = 0;
    SetLength(SIZE_LENGTH);
    SetSize();
    Rewind();
    m_Result = qResult(RESULT_SUCCESS);
    Rewind();
}

// 0x003D2600 | fefates:bytes-fuzzy [tier B]
// 0x003D25AC (deleting dtor)
nn::nex::Message::~Message()
{
    if (m_pEndPoint != nullptr) {
        m_pEndPoint->Close();
    }
}

} // namespace nex
} // namespace nn
