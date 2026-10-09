#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/cec/CTR/cec_MessageId.h"
#include "nn/cec/CTR/cec_Types.h"

namespace nn {
namespace cec {
namespace CTR {
class MessageBox;

// A StreetPass message: the header, up to 16 extended headers, the body and the HMAC (3dbrew
// "CEC Message format"). The data of the extended headers, the body and the HMAC stay where the
// caller has them. Member names are ours.
class Message
{
    friend class MessageBox;

public:
    // an extended header (the names are ours)
    struct ExHeader
    {
        u32 type;         // 0x0
        u32 size;         // 0x4
        const void* data; // 0x8
    };

    static const s32 EXHEADER_MAX = 16;
    // the size of the HMAC after the body
    static const u32 HMAC_SIZE = 32;
    // the largest message and body (values from the binary)
    static const u32 MESSAGE_SIZE_MAX = 0x19000;
    static const u32 BODY_SIZE_MAX = 0x18000;
    static const u32 EXHEADER_SIZE_MAX = 0x2000;

    Message(); // 0x003509F4 | nintendogs:bytes [tier A]
    nn::Result NewMessage(u32 titleId, u32 batchId, u8 flags, u8 sendMethod, u8 sendCount, u8 forwardCount); // 0x003504E4 | nintendogs:bytes [tier A]
    nn::Result SetExHeader(u32 type, u32 size, const void* data); // 0x00350558 | nintendogs:bytes [tier A]
    // takes a message in its binary form (the data stays where it is)
    nn::Result InputMessage(const void* data, u32 size); // 0x00350648 | nintendogs:bytes [tier A]
    nn::Result SetMessageBody(const void* body, u32 size); // 0x003507B4 | nintendogs:bytes [tier A]
    void InitializeMessage(); // 0x003508D8 | nintendogs:bytes [tier A]
    nn::Result SetExHeaderWithoutCalc(u32 type, u32 size, const void* data); // 0x0035093C | nintendogs:bytes [tier A]
    MessageId GetMessageId(MessageId* pId) const; // 0x00350A40 | nintendogs:bytes [tier A]
    nn::Result GetExHeader(u32 type, u32* pSize, void** pData) const; // 0x00729358 | nintendogs:bytes [tier A]
    // the extended header of type 4 (name is ours)
    nn::Result GetExHeaderType4(void** pData, u32* pSize) const; // 0x007293D4 (name is ours)
    u32 GetMessageBody(void* buffer, u32 size) const; // 0x00729454 | nintendogs:bytes [tier A]
    // the second message id of the header (name is ours)
    MessageId GetMessageId2(MessageId* pId) const; // 0x00729484 (name is ours)
    u32 MakeMessageBinary(void* buffer) const; // 0x007294C8 | nintendogs:bytes [tier A]
    void OutputMessageHeader(void* buffer) const; // 0x00729580 | nintendogs:bytes [tier A]
    // the body without a copy (name is ours)
    const void* GetMessageBodyPointer(const void** pBody, u32* pSize) const; // 0x00729594 (name is ours)

private:
    // the header and message sizes after a change (inline, name is ours)
    void CalcMessageSize()
    {
        u32 headerSize = sizeof(CecMessageHeader);
        for (u32 i = 0; i < static_cast<u32>(m_ExHeaderCount); i++) {
            headerSize += 8 + ((m_ExHeaders[i].size + 3) & ~3);
        }
        m_Header.totalHeaderSize = headerSize;
        m_Header.messageSize = m_BodySize + headerSize + HMAC_SIZE;
    }

    CecMessageHeader m_Header;           // 0x000
    ExHeader m_ExHeaders[EXHEADER_MAX];  // 0x070
    s8 m_ExHeaderCount;                  // 0x130
    bool m_IsInput;                      // 0x131, made with InputMessage
    u32 m_BodySize;                      // 0x134
    const void* m_Body;                  // 0x138
    const void* m_Hmac;                  // 0x13C
    u32 m_HmacSize;                      // 0x140
    u32 m_Reserved[8];                   // 0x144, only cleared
};
ASSERT_SIZE(Message, 0x164);
} // namespace CTR
} // namespace cec
} // namespace nn
