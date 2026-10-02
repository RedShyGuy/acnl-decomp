#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x6752C in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpTakumiChipTutorial : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpTakumiChipTutorial(); // ctor address unknown
    virtual ~AcNpcSpTakumiChipTutorial(); // ModuleIndoor.cro +0x041190 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x040F6C slot 0x24
};
