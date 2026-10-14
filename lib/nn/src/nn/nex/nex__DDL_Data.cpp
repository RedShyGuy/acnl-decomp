#include "nn/nex/nex__DDL_Data.h"
#include "nn/nex/nex_Data.h"

namespace nn {
namespace nex {
// 0x0072EB7C slot 0x08 | fefates:bytes
nn::nex::Data* nn::nex::_DDL_Data::Clone() const
{
    return new Data();
}

// 0x0072EB4C slot 0x0C | mk7dlp:bytes
nn::nex::String nn::nex::_DDL_Data::GetDataType() const
{
    return String(L"Data");
}

// 0x0072EB60 slot 0x10
bool nn::nex::_DDL_Data::IsA(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"Data");
}

// 0x0072EBA8 slot 0x14
bool nn::nex::_DDL_Data::IsAKindOf(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"Data");
}

// 0x003D9AF4 slot 0x18
void nn::nex::_DDL_Data::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x003D9B04 | fefates:bytes [tier B]
void nn::nex::_DDL_Data::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_Data& data)
{
    if (!pMessage->m_IsWithoutStructureHeader) {
        DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
}

// 0x003D9B68 slot 0x1C
void nn::nex::_DDL_Data::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

// 0x003D9B78 | fefates:bytes [tier B]
void nn::nex::_DDL_Data::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_Data* pData)
{
    pData->m_StructureVersion = 0;
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 size = 0;
    if (!isWithoutHeader) {
        DDLStructure::BeginExtract(pMessage, &pData->m_StructureVersion, &size);
    }
}

} // namespace nex
} // namespace nn
