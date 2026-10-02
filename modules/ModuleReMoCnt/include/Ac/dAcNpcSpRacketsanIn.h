#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x67B8 in ModuleReMoCnt.cro, offset_to_top 0, 89 entries
class AcNpcSpRacketsanIn : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpRacketsanIn(); // ctor address unknown
    virtual ~AcNpcSpRacketsanIn(); // ModuleReMoCnt.cro +0x0049E8 slot 0x00
    virtual void Initialize(); // ModuleReMoCnt.cro +0x005A3C slot 0x0C
    virtual void Finalize(); // ModuleReMoCnt.cro +0x005AD8 slot 0x18
    virtual void Unk0(); // ModuleReMoCnt.cro +0x005624 slot 0x3C
};
