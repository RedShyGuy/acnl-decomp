#pragma once

#include "decomp.h"
#include "Road/dRoadSearchForeachNodeCB.h"

// vtable +0x43B0 in ModuleSummer.cro, offset_to_top 0, 1 entries
class AStarForeachCB : public ::RoadSearchForeachNodeCB
{
public:
    AStarForeachCB(); // ctor address unknown
};
