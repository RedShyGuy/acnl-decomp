#include "state/dMode.h"
#include "Other/dBase.h"
#include "Bs/dBsMenuBg.h"

// 0x00950718
BsMenuBg* BsMenuBg::s_pInstance;

// ctor candidate(s) 0x00695EC8 (unverified)
BsMenuBg::BsMenuBg()
{
}

// 0x00695FF4 slot 0x00 | slot vf_0x00 of oml::framework::Process
// 0x00775C5C (thunk)
// 0x00775BF0 (deleting thunk)
BsMenuBg::~BsMenuBg()
{
}

// 0x006959E4 slot 0x0C | slot vf_0x0C of oml::framework::Process
oml::framework::Result BsMenuBg::Initialize()
{
}

// 0x00695CB0 slot 0x18 | slot vf_0x18 of oml::framework::Process
oml::framework::Result BsMenuBg::Finalize()
{
}

// 0x00695BF4 slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result BsMenuBg::Calc()
{
}

// 0x00695988 slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result BsMenuBg::Draw()
{
}

// 0x006953F0 | libgarden [tier A]
void BsMenuBg::ChangeBg(long)
{
}

// 0x00695E90
void BsMenuBg::StartChange(s8 kind, u8 frames)
{
}

// 0x006952B8
void BsMenuBg::State_Wait()
{
}

// 0x00695468
void BsMenuBg::State_Change()
{
}
