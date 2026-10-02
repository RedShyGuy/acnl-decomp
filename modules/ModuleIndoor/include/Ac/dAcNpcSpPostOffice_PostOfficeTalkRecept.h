#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPostOffice.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x68A04 in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x68B78 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpPostOffice::PostOfficeTalkRecept : public ::NpcSpTalkRecept
{
public:
    PostOfficeTalkRecept(); // ctor address unknown
    virtual ~PostOfficeTalkRecept(); // ModuleIndoor.cro +0x023BCC slot 0x00
};
