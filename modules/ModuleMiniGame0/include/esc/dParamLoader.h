#pragma once

#include "decomp.h"
#include "ssys/ma/dLoadSplit.h"

namespace esc {
// vtable +0xDE9BC in ModuleMiniGame0.cro, offset_to_top 0, 4 entries
class ParamLoader : public ::ssys::ma::LoadSplit
{
public:
    ParamLoader(); // ctor address unknown
    virtual ~ParamLoader(); // ModuleMiniGame0.cro +0x00F300 slot 0x00
};
} // namespace esc
