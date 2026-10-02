#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex22_DDL_DataStoreMetaInfoE @ 0x008CEC98
// vtable 0x008FE20C (vptr 0x008FE214), offset_to_top 0, 2 entries
class _DDL_DataStoreMetaInfo : public ::nn::nex::RootObject
{
public:
    virtual ~_DDL_DataStoreMetaInfo(); // 0x0039F164 slot 0x00 | fefates:bytes
    // 0x0039F154 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_DataStoreMetaInfo (deleting dtor)
    void Extract(nn::nex::Message*, nn::nex::_DDL_DataStoreMetaInfo*); // 0x0039EDA4 | fefates:bytes [tier B]
    _DDL_DataStoreMetaInfo(); // 0x0039F070 | fefates:bytes [tier B]
    void operator=(const nn::nex::_DDL_DataStoreMetaInfo&); // 0x0039F1F0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
