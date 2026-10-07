#include "nn/nex/nex_Variant.h"
#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_MemoryManager.h"

namespace nn {
namespace nex {
// 0x003D3824 | fefates:bytes [tier B]
nn::nex::Variant::Variant(const nn::nex::String& value)
{
    CopyString(&m_pString, value.m_pString);
    m_Type = TYPE_STRING;
}

// 0x003D3844 | fefates:bytes [tier B]
nn::nex::Variant::Variant(const nn::nex::DateTime& value)
{
    m_DateTime = value.m_Value;
    m_Type = TYPE_DATE_TIME;
}

// 0x003D3860 | fefates:bytes [tier B]
nn::nex::Variant::Variant(const nn::nex::Variant& rhs)
{
    m_Type = TYPE_NONE;
    *this = rhs;
}

// 0x003D3880 (name is ours)
nn::nex::Variant::Variant(bool value)
{
    m_Bool = value;
    m_Type = TYPE_BOOL;
}

// 0x003D3890 (name is ours)
nn::nex::Variant::Variant(double value)
{
    m_Double = value;
    m_Type = TYPE_DOUBLE;
}

// 0x003D38A0 (name is ours)
nn::nex::Variant::Variant(u32 value)
{
    m_Type = TYPE_UINT64;
    m_UInt64 = value;
}

// 0x003D38B4 (name is ours)
nn::nex::Variant::Variant()
{
    m_Type = TYPE_NONE;
}

// 0x003D38C0 (name is ours)
nn::nex::Variant::Variant(s64 value)
{
    m_Type = TYPE_INT64;
    m_Int64 = value;
}

// 0x003D38D0 (name is ours)
nn::nex::Variant::Variant(u64 value)
{
    m_Type = TYPE_UINT64;
    m_UInt64 = value;
}

// 0x003D38E0 | fefates:bytes [tier B]
nn::nex::Variant::~Variant()
{
    // the copied string has a count in front of it
    if (m_Type == TYPE_STRING) {
        MemoryManager::Free(reinterpret_cast<u8*>(m_pString) - sizeof(u32));
    }
}

// 0x003D3AE4 | fefates:bytes-fuzzy [tier B]
void nn::nex::Variant::operator=(const nn::nex::Variant&)
{
}

// 0x0072E508 | fefates:bytes [tier B]
void nn::nex::Variant::GetBoolValue() const
{
}

// 0x0072E540 | fefates:bytes [tier B]
void nn::nex::Variant::GetDoubleValue() const
{
}

// 0x0072E560 | fefates:bytes [tier B]
void nn::nex::Variant::GetStringValue() const
{
}

// 0x0072E5CC | fefates:bytes [tier B]
void nn::nex::Variant::GetDateTimeValue() const
{
}

} // namespace nex
} // namespace nn
