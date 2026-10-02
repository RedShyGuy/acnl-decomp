#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x3420 in ModuleMuseum.cro, offset_to_top 0, 22 entries
class BsArtPlateMgr : public ::UtlBase<Base>
{
public:
    BsArtPlateMgr(); // ctor address unknown
    virtual ~BsArtPlateMgr(); // ModuleMuseum.cro +0x00240C slot 0x00
    virtual void Initialize(); // ModuleMuseum.cro +0x00290C slot 0x0C
    virtual void Finalize(); // ModuleMuseum.cro +0x0029A8 slot 0x18
    virtual void Calc(); // ModuleMuseum.cro +0x0023D4 slot 0x24
    virtual void Draw(); // ModuleMuseum.cro +0x0023CC slot 0x30
    virtual void Unk0(); // ModuleMuseum.cro +0x0027B0 slot 0x3C
};
