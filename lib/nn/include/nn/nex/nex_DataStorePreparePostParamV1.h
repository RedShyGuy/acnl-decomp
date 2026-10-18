#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStorePreparePostParamV1.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex27DataStorePreparePostParamV1E @ 0x008CF00C
// vtable 0x008FEF2C (vptr 0x008FEF34), offset_to_top 0, 2 entries
class DataStorePreparePostParamV1 : public ::nn::nex::_DDL_DataStorePreparePostParamV1
{
public:
    // the type of the data IDs of this version
    typedef u32 TDataId;

    DataStorePreparePostParamV1(); // 0x003BCD14
    virtual ~DataStorePreparePostParamV1(); // 0x003BCF1C slot 0x00
    // 0x003BCF0C slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStorePreparePostParamV1, 0x84);
} // namespace nex
} // namespace nn
