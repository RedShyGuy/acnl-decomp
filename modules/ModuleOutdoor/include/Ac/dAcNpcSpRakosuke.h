#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x956C4 in ModuleOutdoor.cro, offset_to_top 0, 89 entries
class AcNpcSpRakosuke : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpRakosuke(); // ctor address unknown
    virtual ~AcNpcSpRakosuke(); // ModuleOutdoor.cro +0x033828 slot 0x00
};
