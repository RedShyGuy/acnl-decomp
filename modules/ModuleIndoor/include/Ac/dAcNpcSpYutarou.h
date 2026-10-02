#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x65E0C in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpYutarou : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpYutarou(); // ctor address unknown
    virtual ~AcNpcSpYutarou(); // ModuleIndoor.cro +0x017A04 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x01739C slot 0x24
    virtual void CanDraw() const; // ModuleIndoor.cro +0x0177A8 slot 0x2C
};
