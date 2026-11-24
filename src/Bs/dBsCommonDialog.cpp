#include "state/dMode.h"
#include "Other/dMenuBase.h"
#include "Bs/dBsCommonDialog.h"

// 0x0094FE4C
u8 BsCommonDialog::s_Unknown94FE4C = 1;
// 0x0094FE50
BsCommonDialog* BsCommonDialog::s_pInstance;
// 0x0098D56C
CommonDialogArg g_CommonDialogArg;

// ctor candidate(s) 0x007EE8C4 (unverified)
BsCommonDialog::BsCommonDialog()
{
}

// 0x00251E88 slot 0x00 | slot vf_0x00 of oml::framework::Process
// 0x00777210 (thunk)
// 0x007771E4 (deleting thunk)
BsCommonDialog::~BsCommonDialog()
{
}

// 0x00251B70 slot 0x0C | slot vf_0x0C of oml::framework::Process
oml::framework::Result BsCommonDialog::Initialize()
{
}

// 0x00251D90 slot 0x18 | slot vf_0x18 of oml::framework::Process
oml::framework::Result BsCommonDialog::Finalize()
{
}

// 0x00251CD0 slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result BsCommonDialog::Calc()
{
}

// 0x00251B5C slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result BsCommonDialog::Draw()
{
}

// 0x00251940 slot 0x44 | slot vf_0x44 of MenuBase
void BsCommonDialog::OnClose()
{
}

// 0x00251908
void BsCommonDialog::ShowNext()
{
}

// 0x0025195C
void BsCommonDialog::State_Wait()
{
}

// 0x00251930
BsCommonDialog* BsCommonDialog::GetInstance()
{
    return s_pInstance;
}
