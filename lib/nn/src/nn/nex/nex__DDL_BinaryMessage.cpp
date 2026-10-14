#include "nn/nex/nex__DDL_BinaryMessage.h"
#include "nn/nex/nex_BinaryMessage.h"

namespace nn {
namespace nex {
// 0x00390F84 slot 0x00
// 0x00390F44 (deleting dtor)
nn::nex::_DDL_BinaryMessage::~_DDL_BinaryMessage()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072C5B8 slot 0x08
nn::nex::Data* nn::nex::_DDL_BinaryMessage::Clone() const
{
    BinaryMessage* pCopy = new BinaryMessage();
    static_cast<_DDL_BinaryMessage&>(*pCopy) = *this;
    return pCopy;
}

// 0x0072C568 slot 0x0C
nn::nex::String nn::nex::_DDL_BinaryMessage::GetDataType() const
{
    return String(L"BinaryMessage");
}

// 0x0072C58C slot 0x10
bool nn::nex::_DDL_BinaryMessage::IsA(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"BinaryMessage");
}

// 0x0072C77C slot 0x14
bool nn::nex::_DDL_BinaryMessage::IsAKindOf(const nn::nex::String& name) const
{
    return String::IsEqual(name.m_pString, L"BinaryMessage") || UserMessage::IsAKindOf(name);
}

// 0x0072C6AC slot 0x18
void nn::nex::_DDL_BinaryMessage::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x00390E1C slot 0x1C
void nn::nex::_DDL_BinaryMessage::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

// 0x00390E2C (name is ours)
void nn::nex::_DDL_BinaryMessage::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_BinaryMessage* pData)
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
    _Type_qBuffer::Extract(pMessage, &pData->m_binaryBody);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
