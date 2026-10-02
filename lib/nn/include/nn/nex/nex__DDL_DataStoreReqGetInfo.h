#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24_DDL_DataStoreReqGetInfoE @ 0x008CEE50
// vtable 0x008FE978 (vptr 0x008FE980), offset_to_top 0, 2 entries
class _DDL_DataStoreReqGetInfo : public ::nn::nex::RootObject
{
public:
    _DDL_DataStoreReqGetInfo(); // ctor candidate(s) 0x003930C8 (unverified)
    virtual void vf_0x00(); // 0x003B5B90 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_DataStoreReqGetInfo
    virtual void vf_0x04(); // 0x003B5B58 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStoreReqGetInfo
    void Extract(nn::nex::Message*, nn::nex::_DDL_DataStoreReqGetInfo*); // 0x003B5A44 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
