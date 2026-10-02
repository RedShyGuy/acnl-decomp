#pragma once

#include "decomp.h"
#include "Road/dRoadSearchForeachNodeCB.h"

// vtable +0x27F8C in ModuleShop.cro, offset_to_top 0, 1 entries
class RecycleAStarForeachCB : public ::RoadSearchForeachNodeCB
{
public:
    RecycleAStarForeachCB(); // ctor address unknown
};
