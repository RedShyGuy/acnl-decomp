#include "nn/nex/nex__DDL_DynamicGathering.h"
#include "nn/nex/nex_DynamicGathering.h"

namespace nn {
namespace nex {
// 0x0039B6EC slot 0x00
// 0x0039B6B8 (deleting dtor)
nn::nex::_DDL_DynamicGathering::~_DDL_DynamicGathering()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072CECC slot 0x08 | fefates:bytes
nn::nex::Gathering* nn::nex::_DDL_DynamicGathering::Clone() const
{
    // (a new one, nothing is copied)
    return new DynamicGathering();
}

// 0x0072CE6C slot 0x0C | mk7dlp:bytes
nn::nex::String nn::nex::_DDL_DynamicGathering::GetGatheringType() const
{
    return String(L"DynamicGathering");
}

// 0x0072CE98 slot 0x10
bool nn::nex::_DDL_DynamicGathering::IsA(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"DynamicGathering");
}

// 0x0072CF48 slot 0x14
bool nn::nex::_DDL_DynamicGathering::IsAKindOf(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"DynamicGathering") || Gathering::IsAKindOf(typeName);
}

// 0x003846C8 slot 0x18
void nn::nex::_DDL_DynamicGathering::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x0039B66C slot 0x1C | fefates:bytes
void nn::nex::_DDL_DynamicGathering::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

} // namespace nex
} // namespace nn
