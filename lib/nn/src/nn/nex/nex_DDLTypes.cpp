#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x007D6D40 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void _Type_std_list<_Type_stationurl>::Add(Message* pMessage, const Type& list)
{
    AddInline(pMessage, list);
}

// 0x007D41F4
template <>
DECOMP_NOINLINE void _Type_qlist<_Type_uint32>::Add(Message* pMessage, const Type& list)
{
    AddInline(pMessage, list);
}

// 0x007D4700 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void _Type_qvector<_Type_byte>::Extract(Message* pMessage, Type* pVector)
{
    ExtractInline(pMessage, pVector);
}

// 0x007D4C84 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void _Type_qvector<_Type_uint32>::Extract(Message* pMessage, Type* pVector)
{
    ExtractInline(pMessage, pVector);
}

// 0x007D48D4 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void _Type_qvector<_Type_string>::Extract(Message* pMessage, Type* pVector)
{
    ExtractInline(pMessage, pVector);
}

} // namespace nex
} // namespace nn
