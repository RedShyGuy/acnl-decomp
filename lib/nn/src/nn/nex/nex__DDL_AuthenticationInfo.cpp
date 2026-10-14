#include "nn/nex/nex__DDL_AuthenticationInfo.h"
#include "nn/nex/nex_AuthenticationInfo.h"

namespace nn {
namespace nex {
// 0x003B18A4 slot 0x00
// 0x003B1880 (deleting dtor)
nn::nex::_DDL_AuthenticationInfo::~_DDL_AuthenticationInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072D31C slot 0x08 | fefates:bytes
nn::nex::Data* nn::nex::_DDL_AuthenticationInfo::Clone() const
{
    AuthenticationInfo* pCopy = new AuthenticationInfo();
    static_cast<_DDL_AuthenticationInfo&>(*pCopy) = *this;
    return pCopy;
}

// 0x0072D2B4 slot 0x0C | mk7dlp:bytes
nn::nex::String nn::nex::_DDL_AuthenticationInfo::GetDataType() const
{
    return String(L"AuthenticationInfo");
}

// 0x0072D2E4 slot 0x10
bool nn::nex::_DDL_AuthenticationInfo::IsA(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"AuthenticationInfo");
}

// 0x0072D4D0 slot 0x14
bool nn::nex::_DDL_AuthenticationInfo::IsAKindOf(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"AuthenticationInfo") || Data::IsAKindOf(name);
}

// 0x0072D3D0 slot 0x18 | fefates:bytes
void nn::nex::_DDL_AuthenticationInfo::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x003B1664 slot 0x1C
void nn::nex::_DDL_AuthenticationInfo::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

// 0x003B1674 | fefates:bytes [tier B]
void nn::nex::_DDL_AuthenticationInfo::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_AuthenticationInfo* pData)
{
    _DDL_Data::Extract(pMessage, pData);
    if (pMessage->m_IsError) {
        return;
    }
    pData->m_StructureVersion = 0;
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 size = 0;
    if (!isWithoutHeader && !DDLStructure::BeginExtractFullyInline(pMessage, &pData->m_StructureVersion, &size)) {
        return;
    }
    if (pMessage->m_IsError) {
        return;
    }
    u32 start = pMessage->m_Position;
    _Type_string::Extract(pMessage, &pData->m_Token);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitiveInline(&pData->m_Version);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitiveInline(&pData->m_TokenType);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitiveInline(&pData->m_ServerVersion);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
