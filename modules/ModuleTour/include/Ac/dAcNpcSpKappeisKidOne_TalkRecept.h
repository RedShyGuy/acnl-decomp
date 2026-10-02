#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpKappeisKidOne.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x125EC in ModuleTour.cro, offset_to_top 0, 91 entries
// vtable +0x12760 in ModuleTour.cro, offset_to_top -124, 14 entries
class AcNpcSpKappeisKidOne::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleTour.cro +0x008E38 slot 0x00
};
