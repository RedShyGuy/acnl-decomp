#pragma once

#include "decomp.h"
#include "Npc/dNpcTopsChangeModel.h"

// vtable +0x665FC in ModuleIndoor.cro, offset_to_top 0, 20 entries
class NpcYutarouModel : public ::NpcTopsChangeModel
{
public:
    NpcYutarouModel(); // ctor address unknown
    virtual ~NpcYutarouModel(); // ModuleIndoor.cro +0x02124C slot 0x1C
};
