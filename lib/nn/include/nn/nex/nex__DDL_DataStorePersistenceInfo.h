#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex29_DDL_DataStorePersistenceInfoE @ 0x008CF160
// vtable 0x008FF344 (vptr 0x008FF34C), offset_to_top 0, 2 entries
class _DDL_DataStorePersistenceInfo : public ::nn::nex::RootObject
{
public:
    _DDL_DataStorePersistenceInfo(); // ctor address unknown
    virtual void vf_0x00(); // 0x003C1808 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_DataStorePersistenceInfo
    virtual void vf_0x04(); // 0x003C1804 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStorePersistenceInfo
    void Extract(nn::nex::Message*, nn::nex::_DDL_DataStorePersistenceInfo*); // 0x003C1720 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
