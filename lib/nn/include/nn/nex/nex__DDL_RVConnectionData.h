#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21_DDL_RVConnectionDataE @ 0x008CEB98
// vtable 0x008FDFF4 (vptr 0x008FDFFC), offset_to_top 0, 2 entries
class _DDL_RVConnectionData : public ::nn::nex::RootObject
{
public:
    _DDL_RVConnectionData(); // ctor address unknown
    virtual ~_DDL_RVConnectionData(); // 0x0039BFF4 slot 0x00 | fefates:bytes
    // 0x0039BFE4 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_RVConnectionData (deleting dtor)
    void Extract(nn::nex::Message*, nn::nex::_DDL_RVConnectionData*); // 0x0039BD24 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
