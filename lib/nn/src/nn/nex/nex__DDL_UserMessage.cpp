#include "nn/nex/nex__DDL_UserMessage.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_UserMessage.h"

namespace nn {
namespace nex {
// 0x0038462C slot 0x00
// 0x00384604 (deleting dtor)
nn::nex::_DDL_UserMessage::~_DDL_UserMessage()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072BBA8 slot 0x08
nn::nex::Data* nn::nex::_DDL_UserMessage::Clone() const
{
    UserMessage* pCopy = new UserMessage();
    static_cast<_DDL_UserMessage&>(*pCopy) = *this;
    return pCopy;
}

// 0x0072BB60 slot 0x0C
nn::nex::String nn::nex::_DDL_UserMessage::GetDataType() const
{
    return String(L"UserMessage");
}

// 0x0072BB80 slot 0x10
bool nn::nex::_DDL_UserMessage::IsA(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"UserMessage");
}

// 0x0072BCB0 slot 0x14
bool nn::nex::_DDL_UserMessage::IsAKindOf(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"UserMessage") || Data::IsAKindOf(name);
}

// 0x00384650 (name is ours)
nn::nex::_DDL_UserMessage& nn::nex::_DDL_UserMessage::operator=(const nn::nex::_DDL_UserMessage& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_uiID = other.m_uiID;
    m_idRecipient = other.m_idRecipient;
    m_uiRecipientType = other.m_uiRecipientType;
    m_uiParentID = other.m_uiParentID;
    m_pidSender = other.m_pidSender;
    m_receptiontime = other.m_receptiontime;
    m_uiLifeTime = other.m_uiLifeTime;
    m_uiFlags = other.m_uiFlags;
    m_strSubject = other.m_strSubject;
    m_strSender = other.m_strSender;
    return *this;
}

// 0x003842C4 slot 0x18
void nn::nex::_DDL_UserMessage::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x003842D4 (name is ours)
void nn::nex::_DDL_UserMessage::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_UserMessage& data)
{
    _DDL_Data::Add(pMessage, data);
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiID), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_idRecipient), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiRecipientType), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiParentID), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_pidSender), sizeof(u32));
    u64 receptionTime = data.m_receptiontime;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&receptionTime), sizeof(u64));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiLifeTime), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiFlags), sizeof(u32));
    _Type_string::Add(pMessage, data.m_strSubject);
    _Type_string::Add(pMessage, data.m_strSender);
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

// 0x0038443C slot 0x1C
void nn::nex::_DDL_UserMessage::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

// 0x0038444C (name is ours)
void nn::nex::_DDL_UserMessage::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_UserMessage* pData)
{
    _DDL_Data::Extract(pMessage, pData);
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
    pMessage->ExtractPrimitive(&pData->m_uiID);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_idRecipient);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_uiRecipientType);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_uiParentID);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_pidSender);
    if (pMessage->m_IsError) {
        return;
    }
    {
        u64 value;
        pMessage->ExtractPrimitive(&value);
        pData->m_receptiontime = DateTime(value);
    }
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_uiLifeTime);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_uiFlags);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_string::Extract(pMessage, &pData->m_strSubject);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_string::Extract(pMessage, &pData->m_strSender);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
