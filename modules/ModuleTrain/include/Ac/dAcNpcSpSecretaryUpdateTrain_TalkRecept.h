#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryUpdateTrain.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0xFE7C in ModuleTrain.cro, offset_to_top 0, 91 entries
// vtable +0xFFF0 in ModuleTrain.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryUpdateTrain::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleTrain.cro +0x00ADFC slot 0x00
};
