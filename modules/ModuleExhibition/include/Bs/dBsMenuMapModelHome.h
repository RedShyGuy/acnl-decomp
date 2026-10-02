#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// vtable +0x8648 in ModuleExhibition.cro, offset_to_top 0, 16 entries
// vtable +0x8690 in ModuleExhibition.cro, offset_to_top -20, 3 entries
class BsMenuMapModelHome : public ::Base, public ::state::Mode<BsMenuMapModelHome>
{
public:
    BsMenuMapModelHome(); // ctor address unknown
    virtual ~BsMenuMapModelHome(); // ModuleExhibition.cro +0x003FE4 slot 0x00
    virtual void Initialize(); // ModuleExhibition.cro +0x0036BC slot 0x0C
    virtual void Finalize(); // ModuleExhibition.cro +0x004A98 slot 0x18
    virtual void Calc(); // ModuleExhibition.cro +0x004938 slot 0x24
    virtual void Draw(); // ModuleExhibition.cro +0x00365C slot 0x30
    virtual void Unk0(); // ModuleExhibition.cro +0x006F58 slot 0x3C
};
