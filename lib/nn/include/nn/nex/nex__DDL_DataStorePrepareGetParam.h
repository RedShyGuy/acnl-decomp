#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex29_DDL_DataStorePrepareGetParamE @ 0x008CF16C
// vtable 0x008FF354 (vptr 0x008FF35C), offset_to_top 0, 2 entries
class _DDL_DataStorePrepareGetParam : public ::nn::nex::RootObject
{
public:
    _DDL_DataStorePrepareGetParam(); // ctor address unknown
    virtual void vf_0x00(); // 0x003C1A48 slot 0x00 | fefates:callgraph
    virtual void vf_0x04(); // 0x003C19F8 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStorePrepareGetParam
    void Add(nn::nex::Message*, const nn::nex::_DDL_DataStorePrepareGetParam&); // 0x003C180C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
