#pragma once

#include "decomp.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// The DDL part of AuthenticationInfo, a polymorphic Data sent with the login (the binary has no DDL
// declaration of it, the member names are ours).
// RTTI N2nn3nex23_DDL_AuthenticationInfoE @ 0x008CED18
// vtable 0x008FE644 (vptr 0x008FE64C), offset_to_top 0, 8 entries
class _DDL_AuthenticationInfo : public ::nn::nex::Data
{
public:
    // (inline)
    _DDL_AuthenticationInfo() : m_StructureVersion(0) {}
    virtual ~_DDL_AuthenticationInfo(); // 0x003B18A4 slot 0x00
    // 0x003B1880 slot 0x04 (deleting dtor)
    virtual nn::nex::Data* Clone() const; // 0x0072D31C slot 0x08 | fefates:bytes
    virtual nn::nex::String GetDataType() const; // 0x0072D2B4 slot 0x0C | mk7dlp:bytes
    virtual bool IsA(const nn::nex::String& name) const; // 0x0072D2E4 slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& name) const; // 0x0072D4D0 slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x0072D3D0 slot 0x18 | fefates:bytes
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x003B1664 slot 0x1C

    // (inline)
    nn::nex::_DDL_AuthenticationInfo& operator=(const nn::nex::_DDL_AuthenticationInfo& other)
    {
        m_StructureVersion = other.m_StructureVersion;
        m_Token = other.m_Token;
        m_Version = other.m_Version;
        m_TokenType = other.m_TokenType;
        m_ServerVersion = other.m_ServerVersion;
        return *this;
    }

    // (inline) the part of Data first
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_AuthenticationInfo& data)
    {
        _DDL_Data::Add(pMessage, data);
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_string::Add(pMessage, data.m_Token);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Version), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_TokenType), sizeof(u8));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ServerVersion), sizeof(u32));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_AuthenticationInfo* pData); // 0x003B1674 | fefates:bytes [tier B]

    u8 m_StructureVersion; // 0x05
    String m_Token; // 0x08
    u32 m_Version; // 0x10
    u8 m_TokenType; // 0x14
    u32 m_ServerVersion; // 0x18
};
ASSERT_OFFSET(_DDL_AuthenticationInfo, m_ServerVersion, 0x18);
ASSERT_SIZE(_DDL_AuthenticationInfo, 0x1C);
} // namespace nex
} // namespace nn
