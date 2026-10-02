#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex32_DDL_DataStorePrepareUpdateParamE @ 0x008CF2C8
// vtable 0x008FF668 (vptr 0x008FF670), offset_to_top 0, 2 entries
class _DDL_DataStorePrepareUpdateParam : public ::nn::nex::RootObject
{
public:
    _DDL_DataStorePrepareUpdateParam(); // ctor address unknown
    virtual void vf_0x00(); // 0x003CADB8 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_DataStorePrepareUpdateParam
    virtual void vf_0x04(); // 0x003CAD68 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStorePrepareUpdateParam
    void Add(nn::nex::Message*, const nn::nex::_DDL_DataStorePrepareUpdateParam&); // 0x003CAC3C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
