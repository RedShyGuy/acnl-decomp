#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19_DDL_MatchmakeParamE @ 0x008CE820
// vtable 0x008FD730 (vptr 0x008FD738), offset_to_top 0, 2 entries
class _DDL_MatchmakeParam : public ::nn::nex::RootObject
{
public:
    _DDL_MatchmakeParam(); // ctor address unknown
    virtual ~_DDL_MatchmakeParam(); // 0x003951A0 slot 0x00 | fefates:bytes
    // 0x00395190 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_MatchmakeParam (deleting dtor)
    void Add(nn::nex::Message*, const nn::nex::_DDL_MatchmakeParam&); // 0x00394D18 | fefates:bytes [tier B]
    void Extract(nn::nex::Message*, nn::nex::_DDL_MatchmakeParam*); // 0x00394E64 | fefates:bytes [tier B]
    void operator=(const nn::nex::_DDL_MatchmakeParam&); // 0x00395248 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
