#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpManualImport.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x1E6D0 in ModuleIncSave.cro, offset_to_top 0, 91 entries
// vtable +0x1E844 in ModuleIncSave.cro, offset_to_top -124, 14 entries
class AcNpcSpManualImport::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIncSave.cro +0x0071F4 slot 0x00
};
