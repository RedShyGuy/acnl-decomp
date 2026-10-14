#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_StringConverter.h"

namespace nn {
namespace nex {
// 0x00ADE934 (name is ours)
SystemSetting _Type_string::s_ConnectionEncoding(L"ConnectionEncoding", ENCODING_UTF8);
// 0x0096C954 (name is ours)
u32 _Type_string::s_ExtractMode = 2;

// 0x003618AC | mk7dlp:bytes [tier A]
void nn::nex::_Type_string::Add(nn::nex::Message* pMessage, const nn::nex::String& string)
{
    const wchar_t* pString = string.m_pString;
    if (pString == nullptr) {
        u16 size = 0;
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
        return;
    }
    if (s_ConnectionEncoding.m_Value == ENCODING_UTF8) {
        StringConverter converter(0);
        converter.Encode(pString, false);
        u16 size = converter.GetSize();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(converter.GetBuffer()), converter.GetSize());
    } else {
        u16 size = string.GetLength() + 1;
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(pString), size * sizeof(wchar_t));
    }
}

// 0x003619AC | mk7dlp:callgraph [tier A]
bool nn::nex::_Type_string::Extract(nn::nex::Message* pMessage, nn::nex::String* pString)
{
    u16 size;
    pMessage->ExtractPrimitiveInline(&size);
    if (pMessage->m_pBuffer->m_ContentSize - pMessage->m_Position < size) {
        pMessage->m_IsError = true;
        return false;
    }
    if (size == 0) {
        return true;
    }
    pString->Reserve(size + 1);
    u32 encoding = s_ConnectionEncoding.m_Value;
    if (encoding == ENCODING_UTF8) {
        StringConverter converter(size + 1);
        pMessage->ExtractRaw(reinterpret_cast<u8*>(converter.GetBuffer()), size);
        converter.Decode(pString->m_pString, size, false);
    } else if (encoding == ENCODING_UTF16 && s_ExtractMode == 1) {
        StringConverter converter(0);
        u32* pBuffer = static_cast<u32*>(MemoryManager::Allocate(sizeof(u32) + size * sizeof(wchar_t)));
        pBuffer[0] = size;
        wchar_t* pCharacters = reinterpret_cast<wchar_t*>(pBuffer + 1);
        pMessage->ExtractRaw(reinterpret_cast<u8*>(pCharacters), size * sizeof(wchar_t));
        converter.Encode(pCharacters, false);
        *pString = converter.GetBuffer();
        MemoryManager::Free(reinterpret_cast<u32*>(pCharacters) - 1);
    } else {
        pMessage->ExtractRaw(reinterpret_cast<u8*>(pString->m_pString), size * sizeof(wchar_t));
    }
    return true;
}

} // namespace nex
} // namespace nn
