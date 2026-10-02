#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x2E2E4 in ModuleMusFish.cro, offset_to_top 0, 20 entries
// vtable +0x2E340 in ModuleMusFish.cro, offset_to_top -72, 11 entries
// vtable +0x2E39C in ModuleMusFish.cro, offset_to_top -456, 11 entries
class AcFsMuObjSimple : public ::Actor, public ::ResourceLoadAsyncSkeletal
{
public:
    AcFsMuObjSimple(); // ctor address unknown
    virtual ~AcFsMuObjSimple(); // ModuleMusFish.cro +0x00A958 slot 0x00
    virtual void Initialize(); // ModuleMusFish.cro +0x00A618 slot 0x0C
    virtual void Finalize(); // ModuleMusFish.cro +0x00A89C slot 0x18
    virtual void Calc(); // ModuleMusFish.cro +0x00A798 slot 0x24
    virtual void Draw(); // ModuleMusFish.cro +0x00A5F4 slot 0x30
};
