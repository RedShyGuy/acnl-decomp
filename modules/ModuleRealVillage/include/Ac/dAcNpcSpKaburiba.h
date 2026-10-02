#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13250 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpKaburiba : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpKaburiba(); // ctor address unknown
    virtual ~AcNpcSpKaburiba(); // ModuleRealVillage.cro +0x005E70 slot 0x00
};
