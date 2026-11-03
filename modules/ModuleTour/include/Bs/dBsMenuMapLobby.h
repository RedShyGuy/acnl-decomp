#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// vtable +0x106B4 in ModuleTour.cro, offset_to_top 0, 16 entries
// vtable +0x106FC in ModuleTour.cro, offset_to_top -20, 3 entries
class BsMenuMapLobby : public ::Base, public ::state::Mode<BsMenuMapLobby>
{
public:
    BsMenuMapLobby(); // ctor address unknown
    virtual ~BsMenuMapLobby(); // ModuleTour.cro +0x004648 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleTour.cro +0x0041D0 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleTour.cro +0x0045B4 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleTour.cro +0x0044E8 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleTour.cro +0x0041A4 slot 0x30
};
