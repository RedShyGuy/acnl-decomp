#include "nn/nex/nex_DateTime.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::DateTime::DateTime()
{
}

// 0x003D494C | fefates:bytes [tier B]
void nn::nex::DateTime::GetSystemTime(nn::nex::DateTime&)
{
}

// 0x003D4B4C | mk7dlp:bytes-fuzzy [tier A]
nn::nex::DateTime::DateTime(unsigned short, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char)
{
}

// 0x0072E644 | fefates:bytes [tier B]
void nn::nex::DateTime::ToEpochTime() const
{
}

// 0x0072E9AC | fefates:bytes [tier B]
void nn::nex::DateTime::operator-(const nn::nex::DateTime&) const
{
}

// 0x0072E904 (name is ours)
u8 nn::nex::DateTime::GetDay() const
{
    return (m_Value >> 17) & 0x1F;
}

// 0x0072E920 (name is ours)
u8 nn::nex::DateTime::GetHour() const
{
    return (m_Value >> 12) & 0x1F;
}

// 0x0072E93C (name is ours)
u16 nn::nex::DateTime::GetYear() const
{
    return m_Value >> 26;
}

// 0x0072E958 (name is ours)
u8 nn::nex::DateTime::GetMonth() const
{
    return (m_Value >> 22) & 0xF;
}

// 0x0072E974 (name is ours)
u8 nn::nex::DateTime::GetMinute() const
{
    return (m_Value >> 6) & 0x3F;
}

// 0x0072E990 (name is ours)
u8 nn::nex::DateTime::GetSecond() const
{
    return m_Value & 0x3F;
}

} // namespace nex
} // namespace nn
