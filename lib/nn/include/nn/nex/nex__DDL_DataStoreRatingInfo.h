#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24_DDL_DataStoreRatingInfoE @ 0x008CEE44
// vtable 0x008FE968 (vptr 0x008FE970), offset_to_top 0, 2 entries
class _DDL_DataStoreRatingInfo : public ::nn::nex::RootObject
{
public:
    _DDL_DataStoreRatingInfo(); // ctor address unknown
    virtual void vf_0x00(); // 0x003B5A40 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_DataStoreRatingInfo
    virtual void vf_0x04(); // 0x003B5A3C slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStoreRatingInfo
    void Extract(nn::nex::Message*, nn::nex::_DDL_DataStoreRatingInfo*); // 0x003B57F8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
