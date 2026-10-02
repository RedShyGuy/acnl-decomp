#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x6715C in ModuleIndoor.cro, offset_to_top 0, 25 entries
// vtable +0x671C8 in ModuleIndoor.cro, offset_to_top -40, 3 entries
class BsMenuAudioPlayerMgr : public ::MenuBase, public ::state::Mode<BsMenuAudioPlayerMgr>
{
public:
    BsMenuAudioPlayerMgr(); // ctor address unknown
    virtual ~BsMenuAudioPlayerMgr(); // ModuleIndoor.cro +0x03CC60 slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x03C9A4 slot 0x0C
    virtual void Finalize(); // ModuleIndoor.cro +0x03CAD4 slot 0x18
    virtual void Calc(); // ModuleIndoor.cro +0x03CA44 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x03C99C slot 0x30
};
