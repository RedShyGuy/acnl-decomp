#pragma once

#include "decomp.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_UserMessage.h"

namespace nn {
namespace nex {
// The DDL part of TextMessage (ProtocolFoundation): a UserMessage with a text. The member name is
// from the DDL declarations in the binary.
// RTTI N2nn3nex16_DDL_TextMessageE @ 0x008CE650
// vtable 0x008FD2D8 (vptr 0x008FD2E0), offset_to_top 0, 9 entries
class _DDL_TextMessage : public ::nn::nex::UserMessage
{
public:
    // (inline)
    _DDL_TextMessage() : m_StructureVersion(0) {}
    virtual ~_DDL_TextMessage(); // 0x00384288 slot 0x00
    // 0x00384248 slot 0x04 (deleting dtor)
    virtual nn::nex::Data* Clone() const; // 0x0072B950 slot 0x08
    virtual nn::nex::String GetDataType() const; // 0x0072B908 slot 0x0C
    virtual bool IsA(const nn::nex::String& name) const; // 0x0072B928 slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& name) const; // 0x0072BB0C slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x0072BA3C slot 0x18
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x00384120 slot 0x1C

    // (inline)
    nn::nex::_DDL_TextMessage& operator=(const nn::nex::_DDL_TextMessage& other)
    {
        UserMessage::operator=(other);
        m_StructureVersion = other.m_StructureVersion;
        m_strTextBody = other.m_strTextBody;
        return *this;
    }

    // (inline) the part of UserMessage first
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_TextMessage& data)
    {
        _DDL_UserMessage::Add(pMessage, data);
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_string::Add(pMessage, data.m_strTextBody);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_TextMessage* pData); // 0x00384130 (name is ours)

    u8 m_StructureVersion; // 0x40
    String m_strTextBody; // 0x44
};
ASSERT_OFFSET(_DDL_TextMessage, m_strTextBody, 0x44);
ASSERT_SIZE(_DDL_TextMessage, 0x50);
} // namespace nex
} // namespace nn
