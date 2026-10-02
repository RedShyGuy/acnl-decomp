#pragma once

#include "decomp.h"
#include "Npc/dNpcSimpleModel.h"

// vtable +0xDB24 in ModuleAutoCamp.cro, offset_to_top 0, 20 entries
class NpcPanielModel : public ::NpcSimpleModel
{
public:
    NpcPanielModel(); // ctor address unknown
    virtual ~NpcPanielModel(); // ModuleAutoCamp.cro +0x004168 slot 0x1C
};
