#pragma once

#include "decomp.h"
#include "Road/dRoadSearchForeachNodeCB.h"

// vtable +0x66E4C in ModuleIndoor.cro, offset_to_top 0, 1 entries
class NpcInAStarForeachCB : public ::RoadSearchForeachNodeCB
{
public:
    NpcInAStarForeachCB(); // ctor address unknown
};
