#include "nn/cec/CTR/cec_Message.h"
#include <string.h>
#include "nn/cec/CTR/detail/cec_Result.h"

namespace nn {
namespace cec {
namespace CTR {
namespace {
// the magic of the header ("``")
const u16 MESSAGE_MAGIC = 0x6060;
// the send methods are 0-3 (3dbrew "CEC Message format")
const u8 SEND_METHOD_NUM = 4;
// the smallest extended header in the binary form (type, size and one word)
const u32 EXHEADER_BINARY_SIZE_MIN = 12;
// the type of the extended header GetExHeaderType4 returns
const u32 EXHEADER_TYPE_4 = 4;

inline u32 Align4(u32 size)
{
    return (size + 3) & ~3;
}
} // namespace

using namespace nn::cec::CTR::detail;

// 0x003504E4 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::Message::NewMessage(u32 titleId, u32 batchId, u8 flags, u8 sendMethod, u8 sendCount, u8 forwardCount)
{
    if (titleId == 0) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    m_Header.titleId = titleId;
    m_Header.batchId = batchId;
    m_Header.flags = flags;
    if (sendMethod >= SEND_METHOD_NUM) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    m_Header.sendMethod = sendMethod;
    m_Header.sendCount = sendCount;
    m_Header.forwardCount = forwardCount;
    if (forwardCount > 1 && sendCount > 1) {
        return nn::Result(RESULT_INVALID_COMBINATION);
    }
    return nn::Result();
}

// 0x00350558 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::Message::SetExHeader(u32 type, u32 size, const void* data)
{
    MessageId id;
    GetMessageId(&id);
    if (m_IsInput || !id.IsEmpty()) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    nn::Result result = SetExHeaderWithoutCalc(type, size, data);
    if (result.IsFailure()) {
        return result;
    }
    CalcMessageSize();
    return nn::Result();
}

// 0x00350648 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::Message::InputMessage(const void* data, u32 size)
{
    InitializeMessage();
    m_IsInput = true;
    if (data == NULL || size < sizeof(CecMessageHeader) || size > MESSAGE_SIZE_MAX) {
        return nn::Result(RESULT_TOO_LARGE);
    }
    const CecMessageHeader* header = static_cast<const CecMessageHeader*>(data);
    if (header->magic != MESSAGE_MAGIC) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if (header->messageSize > size) {
        return nn::Result(RESULT_TOO_LARGE);
    }
    u32 bodySize = header->bodySize;
    u32 headerSize = header->totalHeaderSize;
    if (bodySize + headerSize + HMAC_SIZE > header->messageSize) {
        return nn::Result(RESULT_NO_DATA);
    }
    memcpy(&m_Header, data, sizeof(CecMessageHeader));

    u32 rest = headerSize - sizeof(CecMessageHeader);
    const u8* p = static_cast<const u8*>(data) + sizeof(CecMessageHeader);
    while (rest != 0) {
        if (rest < EXHEADER_BINARY_SIZE_MIN) {
            return nn::Result(RESULT_OUT_OF_RANGE);
        }
        const u32* exHeader = reinterpret_cast<const u32*>(p);
        u32 exHeaderSize = exHeader[1];
        SetExHeaderWithoutCalc(exHeader[0], exHeaderSize, p + 8);
        u32 step = Align4(exHeaderSize) + 8;
        if (rest <= step) {
            break;
        }
        rest -= step;
        p += step;
    }

    const u8* body = static_cast<const u8*>(data) + headerSize;
    m_Hmac = body + bodySize;
    m_HmacSize = HMAC_SIZE;
    if (bodySize != 0) {
        m_Body = body;
        m_BodySize = bodySize;
    } else {
        m_BodySize = 0;
        m_Body = NULL;
    }
    CalcMessageSize();
    return nn::Result();
}

// 0x003507B4 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::Message::SetMessageBody(const void* body, u32 size)
{
    if (body == NULL) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if ((size & 3) != 0) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    if (size + sizeof(CecMessageHeader) > MESSAGE_SIZE_MAX || size == 0 || size > BODY_SIZE_MAX) {
        return nn::Result(RESULT_TOO_LARGE);
    }
    MessageId id;
    GetMessageId(&id);
    if (m_IsInput || !id.IsEmpty()) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    m_BodySize = size;
    m_Body = body;
    m_Header.bodySize = size;
    CalcMessageSize();
    return nn::Result();
}

// 0x003508D8 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::InitializeMessage()
{
    for (int i = 0; i < m_ExHeaderCount; i++) {
        m_ExHeaders[i].size = 0;
        m_ExHeaders[i].type = 0;
    }
    m_ExHeaderCount = 0;
    memset(&m_Header, 0, sizeof(CecMessageHeader));
    m_Header.magic = MESSAGE_MAGIC;
    m_BodySize = 0;
    m_IsInput = false;
}

// 0x0035093C | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::Message::SetExHeaderWithoutCalc(u32 type, u32 size, const void* data)
{
    if (size > EXHEADER_SIZE_MAX) {
        return nn::Result(RESULT_TOO_LARGE);
    }
    if (data == NULL) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    for (int i = 0; i < m_ExHeaderCount; i++) {
        if (m_ExHeaders[i].type == type) {
            m_ExHeaders[i].size = size;
            m_ExHeaders[i].data = data;
            return nn::Result();
        }
    }
    m_ExHeaders[m_ExHeaderCount].type = type;
    m_ExHeaders[m_ExHeaderCount].size = size;
    m_ExHeaders[m_ExHeaderCount].data = data;
    m_ExHeaderCount++;
    return nn::Result();
}

// 0x003509F4 | nintendogs:bytes [tier A]
nn::cec::CTR::Message::Message()
{
    m_ExHeaderCount = 0;
    m_Body = NULL;
    for (int i = 0; i < 8; i++) {
        m_Reserved[i] = 0;
    }
    m_Hmac = NULL;
    m_HmacSize = 0;
    InitializeMessage();
}

// 0x00350A40 | nintendogs:bytes [tier A]
nn::cec::CTR::MessageId nn::cec::CTR::Message::GetMessageId(MessageId* pId) const
{
    if (pId != NULL) {
        *pId = MessageId(m_Header.messageId);
    }
    return MessageId(m_Header.messageId);
}

// 0x00729358 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::Message::GetExHeader(u32 type, u32* pSize, void** pData) const
{
    for (int i = 0; i < m_ExHeaderCount; i++) {
        if (m_ExHeaders[i].type == type) {
            *pSize = m_ExHeaders[i].size;
            *pData = const_cast<void*>(m_ExHeaders[i].data);
            return nn::Result();
        }
    }
    *pSize = 0;
    return nn::Result(RESULT_NO_DATA);
}

// 0x007293D4 (name is ours)
nn::Result nn::cec::CTR::Message::GetExHeaderType4(void** pData, u32* pSize) const
{
    for (int i = 0; i < m_ExHeaderCount; i++) {
        if (m_ExHeaders[i].type == EXHEADER_TYPE_4) {
            *pSize = m_ExHeaders[i].size;
            *pData = const_cast<void*>(m_ExHeaders[i].data);
            return nn::Result();
        }
    }
    *pSize = 0;
    return nn::Result(RESULT_NO_DATA);
}

// 0x00729454 | nintendogs:bytes [tier A]
u32 nn::cec::CTR::Message::GetMessageBody(void* buffer, u32 size) const
{
    if (size > m_BodySize) {
        size = m_BodySize;
    }
    if (m_Body != NULL) {
        memcpy(buffer, m_Body, size);
    }
    return m_BodySize;
}

// 0x00729484 (name is ours)
nn::cec::CTR::MessageId nn::cec::CTR::Message::GetMessageId2(MessageId* pId) const
{
    if (pId != NULL) {
        *pId = MessageId(m_Header.messageId2);
    }
    return MessageId(m_Header.messageId2);
}

// 0x007294C8 | nintendogs:bytes [tier A]
u32 nn::cec::CTR::Message::MakeMessageBinary(void* buffer) const
{
    memcpy(buffer, &m_Header, sizeof(CecMessageHeader));
    u8* p = static_cast<u8*>(buffer) + sizeof(CecMessageHeader);
    u32 count = static_cast<u32>(m_ExHeaderCount);
    for (u32 i = 0; i < count; i++) {
        memcpy(p, &m_ExHeaders[i], 8);
        p += 8;
        memcpy(p, m_ExHeaders[i].data, m_ExHeaders[i].size);
        p += Align4(m_ExHeaders[i].size);
    }
    if (m_Body != NULL) {
        memcpy(p, m_Body, m_BodySize);
        p += m_BodySize;
    }
    if (m_Hmac != NULL) {
        memcpy(p, m_Hmac, HMAC_SIZE);
    }
    return m_Header.messageSize;
}

// 0x00729580 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::OutputMessageHeader(void* buffer) const
{
    memcpy(buffer, &m_Header, sizeof(CecMessageHeader));
}

// 0x00729594 (name is ours)
const void* nn::cec::CTR::Message::GetMessageBodyPointer(const void** pBody, u32* pSize) const
{
    *pSize = m_BodySize;
    if (pBody != NULL) {
        *pBody = m_Body;
    }
    return m_Body;
}

} // namespace CTR
} // namespace cec
} // namespace nn
