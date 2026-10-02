#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex26_DDL_DataStoreSearchResultE @ 0x008CEFDC
// vtable 0x008FEEA0 (vptr 0x008FEEA8), offset_to_top 0, 2 entries
class _DDL_DataStoreSearchResult : public ::nn::nex::RootObject
{
public:
    _DDL_DataStoreSearchResult(); // ctor candidate(s) 0x0050E9DC, 0x005121D8 (unverified)
    virtual void vf_0x00(); // 0x003BC24C slot 0x00 | virtual slot, introduced by nn::nex::_DDL_DataStoreSearchResult
    virtual void vf_0x04(); // 0x003BC1FC slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStoreSearchResult
    void Extract(nn::nex::Message*, nn::nex::_DDL_DataStoreSearchResult*); // 0x003BBE30 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
