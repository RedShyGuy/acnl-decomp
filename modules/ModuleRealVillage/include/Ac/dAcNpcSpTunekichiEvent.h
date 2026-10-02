#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13F74 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpTunekichiEvent : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpTunekichiEvent(); // ctor address unknown
    virtual ~AcNpcSpTunekichiEvent(); // ModuleRealVillage.cro +0x00DBF8 slot 0x00
    virtual void Calc(); // ModuleRealVillage.cro +0x00D8E4 slot 0x24
};
