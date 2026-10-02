#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x139C4 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpYutarouVisit : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpYutarouVisit(); // ctor address unknown
    virtual ~AcNpcSpYutarouVisit(); // ModuleRealVillage.cro +0x009A4C slot 0x00
    virtual void Calc(); // ModuleRealVillage.cro +0x009488 slot 0x24
    virtual void CanDraw() const; // ModuleRealVillage.cro +0x0094AC slot 0x2C
};
