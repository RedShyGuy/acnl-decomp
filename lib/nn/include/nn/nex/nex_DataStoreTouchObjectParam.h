#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreTouchObjectParam.h"

namespace nn {
namespace nex {
// (this build has no RTTI or vtable of it)
class DataStoreTouchObjectParam : public ::nn::nex::_DDL_DataStoreTouchObjectParam
{
public:
    // (inline)
    DataStoreTouchObjectParam() {}
    virtual ~DataStoreTouchObjectParam() {}
};
ASSERT_SIZE(DataStoreTouchObjectParam, 0x20);
} // namespace nex
} // namespace nn
