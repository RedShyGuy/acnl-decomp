#include "state/dMode.h"
#include "Other/dBase.h"
#include "Bs/dBsMenuMgr.h"

// 0x00950D25
u8 BsMenuMgr::s_Unknown950D25;
// 0x00950D35
u8 BsMenuMgr::s_SelectedSlot;
// 0x00950D3B
u8 BsMenuMgr::s_Result;
// 0x00950D42
s16 BsMenuMgr::s_Unknown950D42;
// 0x00950D44
u16 BsMenuMgr::s_PocketItemMask;
// 0x00950D70
u32 BsMenuMgr::s_Unknown950D70;
// 0x00950D8C
u32 BsMenuMgr::s_MenuParam;
// 0x00950D94
u32 BsMenuMgr::s_Unknown950D94;
// 0x00950DB0
u32 BsMenuMgr::s_Unknown950DB0;
// 0x00950D4C
BsMenuMgr* BsMenuMgr::s_pInstance;
// 0x00950D68
u32 BsMenuMgr::s_Flags;

// ctor candidate(s) 0x006D52B4 (unverified)
BsMenuMgr::BsMenuMgr()
{
}

// 0x006D53A4 slot 0x00 | slot vf_0x00 of oml::framework::Process
BsMenuMgr::~BsMenuMgr()
{
}

// 0x006D4978 slot 0x0C | slot vf_0x0C of oml::framework::Process
oml::framework::Result BsMenuMgr::Initialize()
{
}

// 0x006D5158 slot 0x18 | slot vf_0x18 of oml::framework::Process
oml::framework::Result BsMenuMgr::Finalize()
{
}

// 0x006D4CE0 slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result BsMenuMgr::Calc()
{
}

// 0x006D4970 slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result BsMenuMgr::Draw()
{
}

// 0x005C4B04 | libgarden [tier A]
void BsMenuMgr::CanOpenMenuThink()
{
}

// 0x006D2B4C | libgarden [tier A]
void BsMenuMgr::CreateMenuProcess(BsMenuMgr::Menu, unsigned char)
{
}

// 0x006D4C24 | libgarden [tier A]
bool BsMenuMgr::IsPressed(unsigned long)
{
}

