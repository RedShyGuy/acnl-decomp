#pragma once

#include "decomp.h"
#include "Search/dSearchCand.h"

namespace evdmcafearbeit {
// vtable +0xDD2C in ModuleCafe.cro, offset_to_top 0, 1 entries
class mySpecialCand : public ::SearchCand<17>
{
public:
    mySpecialCand(); // ctor address unknown
};
} // namespace evdmcafearbeit
