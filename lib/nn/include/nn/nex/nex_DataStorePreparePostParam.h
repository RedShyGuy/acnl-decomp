#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStorePreparePostParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex25DataStorePreparePostParamE @ 0x008CEE88
// vtable 0x008FE9C4 (vptr 0x008FE9CC), offset_to_top 0, 2 entries
class DataStorePreparePostParam : public ::nn::nex::_DDL_DataStorePreparePostParam
{
public:
    DataStorePreparePostParam(); // ctor candidate(s) 0x003B6E50, 0x0050FF7C (unverified)
    virtual ~DataStorePreparePostParam(); // 0x003B6F58 slot 0x00 | slot vf_0x00 of nn::nex::_DDL_DataStorePreparePostParam
    // 0x003B6F48 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_DataStorePreparePostParam (deleting dtor)
    void SetRatingSetting(const nn::nex::qMap<signed char,nn::nex::DataStoreRatingInitParam>&); // 0x003B618C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
