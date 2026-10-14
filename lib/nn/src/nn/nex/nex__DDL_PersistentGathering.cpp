#include "nn/nex/nex__DDL_PersistentGathering.h"
#include "nn/nex/nex_PersistentGathering.h"

namespace nn {
namespace nex {
// 0x003B5F70 slot 0x00 | fefates:bytes
// 0x003B5F60 (deleting dtor)
nn::nex::_DDL_PersistentGathering::~_DDL_PersistentGathering()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072D608 slot 0x08 | fefates:callseq
nn::nex::Gathering* nn::nex::_DDL_PersistentGathering::Clone() const
{
    // (a new one, nothing is copied)
    return new PersistentGathering();
}

// 0x0072D5A0 slot 0x0C | fefates:bytes
nn::nex::String nn::nex::_DDL_PersistentGathering::GetGatheringType() const
{
    return String(L"PersistentGathering");
}

// 0x0072D5D0 slot 0x10
bool nn::nex::_DDL_PersistentGathering::IsA(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"PersistentGathering");
}

// 0x0072D628 slot 0x14
bool nn::nex::_DDL_PersistentGathering::IsAKindOf(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"PersistentGathering") || Gathering::IsAKindOf(typeName);
}

// 0x003B5BC4 slot 0x18
void nn::nex::_DDL_PersistentGathering::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x003B5BD4 (name is ours)
void nn::nex::_DDL_PersistentGathering::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_PersistentGathering& data)
{
    _DDL_Gathering::Add(pMessage, data);
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x34), sizeof(u32));
    _Type_string::Add(pMessage, data.m_Unknown0x38);
    _Type_qvector<_Type_uint32>::Add(pMessage, data.m_Unknown0x40);
    _Type_qvector<_Type_byte>::Add(pMessage, data.m_Unknown0x4C);
    u64 time = data.m_Unknown0x58;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&time), sizeof(u64));
    time = data.m_Unknown0x60;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&time), sizeof(u64));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x68), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x6C), sizeof(u32));
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

// 0x003B5DAC slot 0x1C
void nn::nex::_DDL_PersistentGathering::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

// 0x003B5DBC | fefates:bytes [tier B]
void nn::nex::_DDL_PersistentGathering::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_PersistentGathering* pData)
{
    _DDL_Gathering::Extract(pMessage, pData);
    if (pMessage->m_IsError) {
        return;
    }
    pData->m_StructureVersion = 0;
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 size = 0;
    if (!isWithoutHeader && !DDLStructure::BeginExtract(pMessage, &pData->m_StructureVersion, &size)) {
        return;
    }
    if (pMessage->m_IsError) {
        return;
    }
    u32 start = pMessage->m_Position;
    pMessage->ExtractPrimitive(&pData->m_Unknown0x34);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_string::Extract(pMessage, &pData->m_Unknown0x38);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<_Type_uint32>::Extract(pMessage, &pData->m_Unknown0x40);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<_Type_byte>::Extract(pMessage, &pData->m_Unknown0x4C);
    if (pMessage->m_IsError) {
        return;
    }
    {
        u64 value;
        pMessage->ExtractPrimitive(&value);
        pData->m_Unknown0x58 = DateTime(value);
    }
    if (pMessage->m_IsError) {
        return;
    }
    {
        u64 value;
        pMessage->ExtractPrimitive(&value);
        pData->m_Unknown0x60 = DateTime(value);
    }
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Unknown0x68);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Unknown0x6C);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
