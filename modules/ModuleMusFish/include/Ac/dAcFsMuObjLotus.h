#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x2E038 in ModuleMusFish.cro, offset_to_top 0, 20 entries
// vtable +0x2E094 in ModuleMusFish.cro, offset_to_top -72, 11 entries
// vtable +0x2E0F0 in ModuleMusFish.cro, offset_to_top -392, 11 entries
class AcFsMuObjLotus : public ::Actor, public ::ResourceLoadAsyncSkeletal
{
public:
    AcFsMuObjLotus(); // ctor address unknown
    virtual ~AcFsMuObjLotus(); // ModuleMusFish.cro +0x00A504 slot 0x00
    virtual void Initialize(); // ModuleMusFish.cro +0x00A230 slot 0x0C
    virtual void Finalize(); // ModuleMusFish.cro +0x00A3BC slot 0x18
    virtual void Calc(); // ModuleMusFish.cro +0x00A308 slot 0x24
    virtual void CanDraw() const; // ModuleMusFish.cro +0x00A3A8 slot 0x2C
    virtual void Draw(); // ModuleMusFish.cro +0x00A20C slot 0x30
};
