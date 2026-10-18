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
    // the type of the data IDs of this version
    typedef u64 TDataId;

    DataStorePreparePostParam(); // 0x003B6E50 | fefates:callgraph [tier C]
    DataStorePreparePostParam(const nn::nex::DataStorePreparePostParam& other); // 0x003B6C3C
    virtual ~DataStorePreparePostParam(); // 0x003B6F58 slot 0x00
    // 0x003B6F48 slot 0x04 (deleting dtor)
    // the defaults of a post (the name is ours)
    void Reset(); // 0x003B674C
    // the rating slots and their settings
    void SetRatingSetting(const nn::nex::qMap<signed char, nn::nex::DataStoreRatingInitParam>& settings); // 0x003B618C | fefates:bytes [tier B]
};
ASSERT_SIZE(DataStorePreparePostParam, 0x9C);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStorePreparePostParam, nn::nex::MemAllocator<nn::nex::DataStorePreparePostParam> >::_C_insert_aux(nn::nex::DataStorePreparePostParam* pos, const nn::nex::DataStorePreparePostParam& value);
} // namespace std

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE vector<nn::nex::DataStorePreparePostParam, nn::nex::MemAllocator<nn::nex::DataStorePreparePostParam> >::~vector();
} // namespace std
