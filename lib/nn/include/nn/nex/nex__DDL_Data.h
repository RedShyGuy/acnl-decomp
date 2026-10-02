#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex9_DDL_DataE @ 0x008CF7A4
class _DDL_Data : public ::nn::nex::RootObject
{
public:
    _DDL_Data(); // ctor address unknown
    void Add(nn::nex::Message*, const nn::nex::_DDL_Data&); // 0x003D9B04 | fefates:bytes [tier B]
    void Extract(nn::nex::Message*, nn::nex::_DDL_Data*); // 0x003D9B78 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
