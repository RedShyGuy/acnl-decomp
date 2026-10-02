#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace qrenc {
// vtable +0x29E04 in ModuleShop.cro, offset_to_top 0, 1 entries
class MenuMainHostIO : public ::sead::hostio::Node
{
public:
    MenuMainHostIO(); // ctor address unknown
};
} // namespace qrenc
