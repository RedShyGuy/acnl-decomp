#pragma once

#include "decomp.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// A value of one of several types (the parameters of a matchmake session, MatchmakeParam). The
// layout and the type values are from the constructors; the names of the members, of the types
// and of the constructors without symbol are ours.
class Variant
{
public:
    enum Type : u8
    {
        TYPE_NONE = 0,
        TYPE_INT64 = 1,
        TYPE_DOUBLE = 2,
        TYPE_BOOL = 3,
        TYPE_STRING = 4,
        TYPE_DATE_TIME = 5,
        TYPE_UINT64 = 6,
    };

    Variant(); // 0x003D38B4
    Variant(bool value); // 0x003D3880
    Variant(double value); // 0x003D3890
    Variant(u32 value); // 0x003D38A0
    Variant(s64 value); // 0x003D38C0
    Variant(u64 value); // 0x003D38D0
    Variant(const nn::nex::String&); // 0x003D3824 | fefates:bytes [tier B]
    Variant(const nn::nex::DateTime&); // 0x003D3844 | fefates:bytes [tier B]
    Variant(const nn::nex::Variant&); // 0x003D3860 | fefates:bytes [tier B]
    ~Variant(); // 0x003D38E0 | fefates:bytes [tier B]
    void operator=(const nn::nex::Variant&); // 0x003D3AE4 | fefates:bytes-fuzzy [tier B]
    void GetBoolValue() const; // 0x0072E508 | fefates:bytes [tier B]
    void GetDoubleValue() const; // 0x0072E540 | fefates:bytes [tier B]
    void GetStringValue() const; // 0x0072E560 | fefates:bytes [tier B]
    void GetDateTimeValue() const; // 0x0072E5CC | fefates:bytes [tier B]

    union
    {
        s64 m_Int64;
        u64 m_UInt64;
        double m_Double;
        bool m_Bool;
        wchar_t* m_pString; // nex::CopyString
        u64 m_DateTime;
    };                // 0x0
    Type m_Type;      // 0x8
};
ASSERT_SIZE(Variant, 0x10);
} // namespace nex
} // namespace nn
