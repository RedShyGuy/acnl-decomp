#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x2DABC in ModuleMusFish.cro, offset_to_top 0, 22 entries
class BsAquariumMgr : public ::UtlBase<Base>
{
public:
    BsAquariumMgr(); // ctor address unknown
    virtual ~BsAquariumMgr(); // ModuleMusFish.cro +0x00EC00 slot 0x00
    virtual void Initialize(); // ModuleMusFish.cro +0x01E814 slot 0x0C
    virtual void Finalize(); // ModuleMusFish.cro +0x01E8B0 slot 0x18
    virtual void Calc(); // ModuleMusFish.cro +0x008260 slot 0x24
    virtual void Draw(); // ModuleMusFish.cro +0x008078 slot 0x30
    virtual void Unk0(); // ModuleMusFish.cro +0x01AAE4 slot 0x3C
};
