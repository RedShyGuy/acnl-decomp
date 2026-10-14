#pragma once

#include "decomp.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/_Type_variant/_Type_variant_Api.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Variant.h"
#include "nn/nex/nex_qMap.h"

namespace __rw {
// (the original calls this instance out of line; defined in nex__DDL_MatchmakeParam.cpp)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::iterator __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::erase(iterator first, iterator last);
} // namespace __rw

namespace std {
// (inline with the body of IndexInline)
template <>
inline nn::nex::Variant& map<nn::nex::String, nn::nex::Variant, less<nn::nex::String>, nn::nex::MemAllocator<pair<const nn::nex::String, nn::nex::Variant> > >::operator[](const nn::nex::String& key)
{
    return IndexInline(key);
}
} // namespace std

namespace nn {
namespace nex {
// The DDL part of MatchmakeParam (MatchmakeExtension): parameters of a matchmake session by their
// keys (@SI, @DR, ...). The binary has no DDL declaration of it; the names are ours.
// RTTI N2nn3nex19_DDL_MatchmakeParamE @ 0x008CE820
// vtable 0x008FD730 (vptr 0x008FD738), offset_to_top 0, 2 entries
class _DDL_MatchmakeParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_MatchmakeParam() : m_StructureVersion(0) {}
    virtual ~_DDL_MatchmakeParam(); // 0x003951A0 slot 0x00 | fefates:bytes
    // 0x00395190 slot 0x04 (deleting dtor)

    static void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_MatchmakeParam& data); // 0x00394D18 | fefates:bytes [tier B]
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_MatchmakeParam* pData); // 0x00394E64 | fefates:bytes [tier B]
    nn::nex::_DDL_MatchmakeParam& operator=(const nn::nex::_DDL_MatchmakeParam& other); // 0x00395248 | fefates:bytes [tier B]

    u8 m_StructureVersion;                    // 0x04
    qMap<String, Variant> m_Params;           // 0x08, the parameters by their keys (@SI, @DR, ...)
};
ASSERT_SIZE(_DDL_MatchmakeParam, 0x24);
} // namespace nex
} // namespace nn
