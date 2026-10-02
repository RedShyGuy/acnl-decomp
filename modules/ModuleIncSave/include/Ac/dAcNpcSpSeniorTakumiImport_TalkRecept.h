#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSeniorTakumiImport.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x1E884 in ModuleIncSave.cro, offset_to_top 0, 92 entries
// vtable +0x1E9FC in ModuleIncSave.cro, offset_to_top -124, 14 entries
class AcNpcSpSeniorTakumiImport::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIncSave.cro +0x008764 slot 0x00
};
