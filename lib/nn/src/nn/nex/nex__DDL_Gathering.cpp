#include "nn/nex/nex__DDL_Gathering.h"
#include "nn/nex/nex_Gathering.h"

namespace nn {
namespace nex {
// 0x00375F64 slot 0x00
// 0x00375F44 (deleting dtor)
nn::nex::_DDL_Gathering::~_DDL_Gathering()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072B42C slot 0x08 | fefates:bytes
nn::nex::Gathering* nn::nex::_DDL_Gathering::Clone() const
{
    return new Gathering();
}

// 0x0072B3EC slot 0x0C | mk7dlp:bytes
nn::nex::String nn::nex::_DDL_Gathering::GetGatheringType() const
{
    return String(L"Gathering");
}

// 0x0072B408 slot 0x10
bool nn::nex::_DDL_Gathering::IsA(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"Gathering");
}

// 0x0072B44C slot 0x14
bool nn::nex::_DDL_Gathering::IsAKindOf(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"Gathering");
}

// 0x00375F80 (name is ours)
nn::nex::_DDL_Gathering& nn::nex::_DDL_Gathering::operator=(const nn::nex::_DDL_Gathering& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_Id = other.m_Id;
    m_OwnerPrincipalId = other.m_OwnerPrincipalId;
    m_HostPrincipalId = other.m_HostPrincipalId;
    m_MinParticipants = other.m_MinParticipants;
    m_MaxParticipants = other.m_MaxParticipants;
    m_ParticipationPolicy = other.m_ParticipationPolicy;
    m_PolicyArgument = other.m_PolicyArgument;
    m_Flags = other.m_Flags;
    m_State = other.m_State;
    m_Description = other.m_Description;
    return *this;
}

// 0x00375BAC slot 0x18
void nn::nex::_DDL_Gathering::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x00375BBC | fefates:bytes [tier B]
void nn::nex::_DDL_Gathering::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_Gathering& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Id), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_OwnerPrincipalId), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_HostPrincipalId), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_MinParticipants), sizeof(u16));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_MaxParticipants), sizeof(u16));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ParticipationPolicy), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_PolicyArgument), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Flags), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_State), sizeof(u32));
    _Type_string::Add(pMessage, data.m_Description);
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

// 0x00375D10 slot 0x1C
void nn::nex::_DDL_Gathering::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

// 0x00375D20 | fefates:bytes [tier B]
void nn::nex::_DDL_Gathering::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_Gathering* pData)
{
    pData->m_StructureVersion = 0;
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 size = 0;
    if (!isWithoutHeader && !DDLStructure::BeginExtractInline(pMessage, &pData->m_StructureVersion, &size)) {
        return;
    }
    if (pMessage->m_IsError) {
        return;
    }
    u32 start = pMessage->m_Position;
    pMessage->ExtractPrimitive(&pData->m_Id);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_OwnerPrincipalId);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_HostPrincipalId);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitiveInline(&pData->m_MinParticipants);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitiveInline(&pData->m_MaxParticipants);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_ParticipationPolicy);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_PolicyArgument);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Flags);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_State);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_string::Extract(pMessage, &pData->m_Description);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
