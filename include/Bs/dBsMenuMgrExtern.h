#pragma once

#include "decomp.h"

class SvFgName;

// (game code, not decompiled yet; names after the addresses) what BsMenuMgr calls and reads

// in the code of the fades (Func566B2C, Func566AD0): a fade runs (true: the map and room are not updated)
bool Func566E5C(); // 0x00566E5C
// the two objects of that code (pointers 0x00976400/0x00976404) with a state slot (2/3 while busy)
struct Unknown976400
{
    virtual void vf_0x00();
    virtual void vf_0x04();
    virtual void vf_0x08();
    virtual u32 GetState(); // slot 0x0C
};
extern Unknown976400* g_pUnknown976400;
extern Unknown976400* g_pUnknown976404;

// (read by the map code; set elsewhere)
extern u8 g_Unknown950C30;
// (read by the room code; NULL in the RV: no room manager)
extern void* g_pUnknown951038;
// the kind of the catalog menu (BsMenuCatalog reads it)
extern u8 g_Unknown9507F0;

// the archives of the lasting menus are loaded (BsMenuMgr::AreArchivesLoaded)
bool Func6E7090(); // 0x006E7090
bool Func209D2C(); // 0x00209D2C
bool Func294D68(); // 0x00294D68
bool Func217678(); // 0x00217678
bool Func60C8A8(); // 0x0060C8A8
// (area select / QR encoder; values are the arguments)
void Func2AE4B4(u32 value); // 0x002AE4B4
void Func1503E0(u32 value); // 0x001503E0 (a static.crs import thunk)
// (on an object at +0x1EC0 of the open BsMenuItem)
void Func26D8E4(void* object); // 0x0026D8E4
// the item is a plant that the census counts when planted
bool Func76D5AC(const SvFgName* item); // 0x0076D5AC
// (CloseMenus asks it first)
bool Func1FE500(); // 0x001FE500
// a group (0..2, 3: none) of the structures 18..65 and the index in it (model house)
u8 Func6CC9F8(u8 structure); // 0x006CC9F8
u32 Func6CCA38(u8 structure); // 0x006CCA38

// a table of 118 entries of 24 bytes (BsMenuMgr::Func6D4824 searches the id)
struct Unknown851F14
{
    u8 _unknown0[4];  // 0x00
    u16 id;           // 0x04
    u8 type;          // 0x06
    u8 _unknown7[17]; // 0x07
};
extern const Unknown851F14 g_Unknown851F14[118];

// (read by the keyboard and two other menus: set, the buttons of the keyboard take the shortcut keys; starts with 1)
extern u8 g_Unknown95F320;
