#include "nn/nex/nex__DDL_TextMessage.h"
#include "nn/nex/nex_TextMessage.h"

namespace nn {
namespace nex {
// 0x00384288 slot 0x00
// 0x00384248 (deleting dtor)
nn::nex::_DDL_TextMessage::~_DDL_TextMessage()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072B950 slot 0x08
nn::nex::Data* nn::nex::_DDL_TextMessage::Clone() const
{
    TextMessage* pCopy = new TextMessage();
    static_cast<_DDL_TextMessage&>(*pCopy) = *this;
    return pCopy;
}

// 0x0072B908 slot 0x0C
nn::nex::String nn::nex::_DDL_TextMessage::GetDataType() const
{
    return String(L"TextMessage");
}

// 0x0072B928 slot 0x10
bool nn::nex::_DDL_TextMessage::IsA(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"TextMessage");
}

// 0x0072BB0C slot 0x14
bool nn::nex::_DDL_TextMessage::IsAKindOf(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"TextMessage") || UserMessage::IsAKindOf(name);
}

// 0x0072BA3C slot 0x18
void nn::nex::_DDL_TextMessage::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x00384120 slot 0x1C
void nn::nex::_DDL_TextMessage::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

// 0x00384130 (name is ours)
void nn::nex::_DDL_TextMessage::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_TextMessage* pData)
{
    _DDL_UserMessage::Extract(pMessage, pData);
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
    _Type_string::Extract(pMessage, &pData->m_strTextBody);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
