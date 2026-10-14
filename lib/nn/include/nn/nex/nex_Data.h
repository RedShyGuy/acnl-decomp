#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_Data.h"

namespace nn {
namespace nex {
// The base of the DDL structures.
// RTTI N2nn3nex4DataE @ 0x008CF520
// vtable 0x008FF9A0 (vptr 0x008FF9A8), offset_to_top 0, 8 entries
class Data : public ::nn::nex::_DDL_Data
{
public:
    Data(); // 0x003CE054
    virtual ~Data(); // 0x003CE070 slot 0x00
    // 0x003CE06C slot 0x04 (deleting dtor)
};
ASSERT_SIZE(Data, 0x8);
} // namespace nex
} // namespace nn
