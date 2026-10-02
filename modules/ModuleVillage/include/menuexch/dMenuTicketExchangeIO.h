#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace menuexch {
// vtable +0x21568 in ModuleVillage.cro, offset_to_top 0, 1 entries
class MenuTicketExchangeIO : public ::sead::hostio::Node
{
public:
    MenuTicketExchangeIO(); // ctor address unknown
};
} // namespace menuexch
