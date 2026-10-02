#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex29_DDL_DataStoreRatingInitParamE @ 0x008CF184
// vtable 0x008FF374 (vptr 0x008FF37C), offset_to_top 0, 2 entries
class _DDL_DataStoreRatingInitParam : public ::nn::nex::RootObject
{
public:
    _DDL_DataStoreRatingInitParam(); // ctor address unknown
    virtual void vf_0x00(); // 0x003C1CB0 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_DataStoreRatingInitParam
    virtual void vf_0x04(); // 0x003C1CAC slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStoreRatingInitParam
    void Add(nn::nex::Message*, const nn::nex::_DDL_DataStoreRatingInitParam&); // 0x003C1B74 | fefates:bytes [tier B]
    void operator=(const nn::nex::_DDL_DataStoreRatingInitParam&); // 0x003C1CB4 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
