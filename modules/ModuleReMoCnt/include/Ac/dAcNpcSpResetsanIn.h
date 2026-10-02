#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x664C in ModuleReMoCnt.cro, offset_to_top 0, 89 entries
class AcNpcSpResetsanIn : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpResetsanIn(); // ctor address unknown
    virtual ~AcNpcSpResetsanIn(); // ModuleReMoCnt.cro +0x004068 slot 0x00
};
