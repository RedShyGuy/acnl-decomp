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
    // (the copied string has a count in front of it)
    ReleaseString();
}

// 0x003D3AE4 | fefates:bytes-fuzzy [tier B]
nn::nex::Variant& nn::nex::Variant::operator=(const nn::nex::Variant& other)
{
    ReleaseString();
    m_Type = other.m_Type;
    switch (other.m_Type) {
    case TYPE_STRING:
        CopyString(&m_pString, other.GetStringValue().m_pString);
        break;
    case TYPE_INT64:
    case TYPE_UINT64:
        m_Int64 = other.m_Int64;
        break;
    case TYPE_DOUBLE:
        m_Double = other.m_Double;
        break;
    case TYPE_BOOL:
        m_Bool = other.m_Bool;
        break;
    case TYPE_DATE_TIME:
        m_DateTime = other.m_DateTime;
        break;
    default:
        break;
    }
    return *this;
}

// 0x0072E508 | fefates:bytes [tier B]
bool nn::nex::Variant::GetBoolValue() const
{
    return m_Type == TYPE_BOOL ? m_Bool : false;
}

// 0x0072E51C
s64 nn::nex::Variant::GetInt64Value() const
{
    return m_Type == TYPE_INT64 || m_Type == TYPE_UINT64 ? m_Int64 : 0;
}

// 0x0072E540 | fefates:bytes [tier B]
double nn::nex::Variant::GetDoubleValue() const
{
    return m_Type == TYPE_DOUBLE ? m_Double : 0.0;
}

// 0x0072E560 | fefates:bytes [tier B]
nn::nex::String nn::nex::Variant::GetStringValue() const
{
    if (m_Type == TYPE_STRING) {
        return String(m_pString);
    }
    return String(L"");
}

// 0x0072E590
u32 nn::nex::Variant::GetUInt32Value() const
{
    return m_Type == TYPE_INT64 || m_Type == TYPE_UINT64 ? static_cast<u32>(m_UInt64) : 0;
}

// 0x0072E5A8
u64 nn::nex::Variant::GetUInt64Value() const
{
    return m_Type == TYPE_INT64 || m_Type == TYPE_UINT64 ? m_UInt64 : 0;
}

// 0x0072E5CC | fefates:bytes [tier B]
nn::nex::DateTime nn::nex::Variant::GetDateTimeValue() const
{
    return m_Type == TYPE_DATE_TIME ? DateTime(m_DateTime) : DateTime(0ULL);
}

} // namespace nex
} // namespace nn
