#pragma once

#include "decomp.h"
#include "Other/dMacroRoadSearch.h"
#include "Other/dMacroRoadSearch_DefaultMicroForeachCB.h"

// vtable +0x97E9C in ModuleOutdoor.cro, offset_to_top 0, 1 entries
class NpcOutMacroRoadSearchCB : public ::MacroRoadSearch::DefaultMicroForeachCB
{
public:
    NpcOutMacroRoadSearchCB(); // ctor address unknown
};
