#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace appraiser {
// vtable +0x101C4 in ModuleTrain.cro, offset_to_top 0, 1 entries
class LytAppraiserHIO : public ::sead::hostio::Node
{
public:
    LytAppraiserHIO(); // ctor address unknown
};
} // namespace appraiser
