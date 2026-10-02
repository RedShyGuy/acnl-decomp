#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryTakumiImport.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x1EA3C in ModuleIncSave.cro, offset_to_top 0, 92 entries
// vtable +0x1EBB4 in ModuleIncSave.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryTakumiImport::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIncSave.cro +0x00907C slot 0x00
};
