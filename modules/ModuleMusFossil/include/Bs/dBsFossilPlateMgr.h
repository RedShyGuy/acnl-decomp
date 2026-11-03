#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x237C in ModuleMusFossil.cro, offset_to_top 0, 22 entries
class BsFossilPlateMgr : public ::UtlBase<Base>
{
public:
    BsFossilPlateMgr(); // ctor address unknown
    virtual ~BsFossilPlateMgr(); // ModuleMusFossil.cro +0x000A44 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleMusFossil.cro +0x000FE4 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleMusFossil.cro +0x001080 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleMusFossil.cro +0x000A0C slot 0x24
    virtual oml::framework::Result Draw(); // ModuleMusFossil.cro +0x000A04 slot 0x30
    virtual void Unk0(); // ModuleMusFossil.cro +0x000E90 slot 0x3C
};
