#pragma once

#include "decomp.h"
#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"
#include "nn/nex/nex_UserMessage.h"
#include "nn/nex/nex_qBuffer.h"

namespace nn {
namespace nex {
// The DDL part of BinaryMessage (ProtocolFoundation): a UserMessage with binary data. The member
// name is from the DDL declarations in the binary.
// RTTI N2nn3nex18_DDL_BinaryMessageE @ 0x008CE76C
// vtable 0x008FD5A4 (vptr 0x008FD5AC), offset_to_top 0, 9 entries
class _DDL_BinaryMessage : public ::nn::nex::UserMessage
{
public:
    // (inline)
    _DDL_BinaryMessage() : m_StructureVersion(0), m_binaryBody(0, 0) {}
    virtual ~_DDL_BinaryMessage(); // 0x00390F84 slot 0x00
    // 0x00390F44 slot 0x04 (deleting dtor)
    virtual nn::nex::Data* Clone() const; // 0x0072C5B8 slot 0x08
    virtual nn::nex::String GetDataType() const; // 0x0072C568 slot 0x0C
    virtual bool IsA(const nn::nex::String& name) const; // 0x0072C58C slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& name) const; // 0x0072C77C slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x0072C6AC slot 0x18
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x00390E1C slot 0x1C

    // (inline)
    nn::nex::_DDL_BinaryMessage& operator=(const nn::nex::_DDL_BinaryMessage& other)
    {
        UserMessage::operator=(other);
        m_StructureVersion = other.m_StructureVersion;
        m_binaryBody = other.m_binaryBody;
        return *this;
    }

    // (inline) the part of UserMessage first
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_BinaryMessage& data)
    {
        _DDL_UserMessage::Add(pMessage, data);
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_qBuffer::Add(pMessage, data.m_binaryBody);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_BinaryMessage* pData); // 0x00390E2C (name is ours)

    u8 m_StructureVersion; // 0x40
    qBuffer m_binaryBody; // 0x44
};
ASSERT_OFFSET(_DDL_BinaryMessage, m_binaryBody, 0x44);
ASSERT_SIZE(_DDL_BinaryMessage, 0x68);
} // namespace nex
} // namespace nn
