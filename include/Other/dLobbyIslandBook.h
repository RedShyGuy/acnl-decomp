#pragma once

#include "decomp.h"
#include "Other/dAddressBook.h"

// RTTI 15LobbyIslandBook @ 0x008CC0BC
// vtable 0x008F1AF8 (vptr 0x008F1B00), offset_to_top 0, 8 entries
class LobbyIslandBook : public ::AddressBook
{
public:
    LobbyIslandBook(); // ctor candidate(s) 0x0029BE60 (unverified)
    virtual void vf_0x00(); // 0x0029BE74 slot 0x00 | virtual slot, introduced by LobbyIslandBook
    virtual void vf_0x04(); // 0x0029BE70 slot 0x04 | virtual slot, introduced by LobbyIslandBook
    virtual void vf_0x08(); // 0x0029BE58 slot 0x08 | virtual slot, introduced by LobbyIslandBook
    virtual void vf_0x0C(); // 0x0029BE5C slot 0x0C | virtual slot, introduced by LobbyIslandBook
    virtual void vf_0x10(); // 0x0071BF20 slot 0x10 | virtual slot, introduced by LobbyIslandBook
    virtual void vf_0x14(); // 0x0071BE4C slot 0x14 | virtual slot, introduced by LobbyIslandBook
    virtual void vf_0x18(); // 0x001C0B40 slot 0x18 | virtual slot, introduced by AutoCampBook
    virtual void vf_0x1C(); // 0x001C0B3C slot 0x1C | virtual slot, introduced by DowntownBook
};
