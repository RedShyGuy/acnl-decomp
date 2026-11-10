#include "state/dMode.h"
#include "Other/dMenuBase.h"
#include "Bs/dBsMenuLetter.h"

// ctor address unknown
BsMenuLetter::BsMenuLetter()
{
}

// 0x001FCB64 slot 0x00 | slot vf_0x00 of oml::framework::Process
// 0x00777144 (thunk)
// 0x00777130 (deleting thunk)
BsMenuLetter::~BsMenuLetter()
{
}

// 0x001FBDAC slot 0x0C | slot vf_0x0C of oml::framework::Process
oml::framework::Result BsMenuLetter::Initialize()
{
}

// 0x001FC400 slot 0x18 | slot vf_0x18 of oml::framework::Process
oml::framework::Result BsMenuLetter::Finalize()
{
}

// 0x001FC33C slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result BsMenuLetter::Calc()
{
}

// 0x001FBD80 slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result BsMenuLetter::Draw()
{
}

// 0x001FB344 slot 0x44 | slot vf_0x44 of MenuBase
void BsMenuLetter::OnClose()
{
}

