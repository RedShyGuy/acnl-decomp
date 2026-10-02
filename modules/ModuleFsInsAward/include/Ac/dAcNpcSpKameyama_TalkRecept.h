#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpKameyama.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x5790 in ModuleFsInsAward.cro, offset_to_top 0, 91 entries
// vtable +0x5904 in ModuleFsInsAward.cro, offset_to_top -124, 14 entries
class AcNpcSpKameyama::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleFsInsAward.cro +0x004244 slot 0x00
};
