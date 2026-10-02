#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x133BC in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpPalloncino : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPalloncino(); // ctor address unknown
    virtual ~AcNpcSpPalloncino(); // ModuleRealVillage.cro +0x0073EC slot 0x00
};
