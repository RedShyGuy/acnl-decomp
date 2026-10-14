#include "nn/nex/nex_Key.h"

namespace nn {
namespace nex {
namespace {
// the value of a hexadecimal digit (lowercase)
inline u8 DigitToValue(wchar_t digit)
{
    return static_cast<u32>(digit - L'0') <= 9 ? digit - L'0' : digit - (L'a' - 10);
}
// the hexadecimal digit of a value below 16 (lowercase)
inline wchar_t ValueToDigit(u32 value)
{
    return value <= 9 ? value + L'0' : value + (L'a' - 10);
}
} // namespace

// 0x003CC824 | fefates:bytes [tier B]
unsigned char* nn::nex::Key::PrepareContentPtr(unsigned int size)
{
    m_Data.resize(size, 0);
    return m_Data.m_pBegin;
}

// 0x003CC840 | mk7dlp:callseq-callee [tier A]
nn::nex::Key::Key(const unsigned char* pData, unsigned size) : m_Data(size)
{
    for (u32 i = 0; i < size; i++) {
        m_Data[i] = pData[i];
    }
}

// 0x003CC8A4 | fefates:bytes [tier B]
nn::nex::Key::Key(const nn::nex::String& string)
{
    u32 size = string.GetLengthInline() / 2;
    m_Data.resize(size, 0);
    for (u32 i = 0; i < size; i++) {
        u8 high = DigitToValue(string.m_pString[i * 2]);
        u8 low = DigitToValue(string.m_pString[i * 2 + 1]);
        m_Data[i] = low | (high << 4);
    }
}

// 0x003CC95C | fefates:bytes [tier B]
nn::nex::Key::Key(const nn::nex::Key& other) : m_Data(other.m_Data)
{
}

// 0x003CCA2C | fefates:bytes [tier B]
nn::nex::Key::Key()
{
}

// 0x003CCA8C slot 0x00 | fefates:bytes
// 0x003CCA5C (deleting dtor)
nn::nex::Key::~Key()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CCB14 | fefates:bytes [tier B]
nn::nex::Key& nn::nex::Key::operator=(const nn::nex::Key& other)
{
    m_Data = other.m_Data;
    return *this;
}

// 0x0072DF4C | fefates:bytes [tier B]
void nn::nex::Key::ExtractToString(nn::nex::String* pString) const
{
    pString->Reserve(GetLength() * 2 + 1);
    u32 i;
    for (i = 0; i < GetLength(); i++) {
        u8 value = m_Data[i];
        pString->m_pString[i * 2] = ValueToDigit((value & 0xF0) >> 4);
        pString->m_pString[i * 2 + 1] = ValueToDigit(value & 0x0F);
    }
    pString->m_pString[i * 2] = L'\0';
}

// 0x0072E040 | fefates:bytes [tier B]
nn::nex::String nn::nex::Key::ToString() const
{
    String string;
    ExtractToString(&string);
    return string;
}

} // namespace nex
} // namespace nn
