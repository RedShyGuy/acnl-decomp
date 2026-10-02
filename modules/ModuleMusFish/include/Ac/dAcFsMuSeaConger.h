#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2E3F8 in ModuleMusFish.cro, offset_to_top 0, 47 entries
// vtable +0x2E4C0 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2E4F4 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2E52C in ModuleMusFish.cro, offset_to_top -580, 11 entries
class AcFsMuSeaConger : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSeaConger>
{
public:
    AcFsMuSeaConger(); // ctor address unknown
    virtual ~AcFsMuSeaConger(); // ModuleMusFish.cro +0x00B280 slot 0x00
    virtual void Finalize(); // ModuleMusFish.cro +0x00B178 slot 0x18
    virtual void CanCalc() const; // ModuleMusFish.cro +0x00A984 slot 0x20
};
