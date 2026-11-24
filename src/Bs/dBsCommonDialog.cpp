#include "Bs/dBsCommonDialog.h"

#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuMgr.h"
#include "oml/framework/dProcess.h"

// 0x0094FE4C
bool BsCommonDialog::s_IsResultUsed = true;
// 0x0094FE50
BsCommonDialog* BsCommonDialog::s_pInstance;
// 0x0098D56C
CommonDialogArg g_CommonDialogArg;

// the states with their names (debug); the machine starts coming in
// 0x00847540
const BsCommonDialog::StateInfo BsCommonDialog::s_StateInfos[5] = {
    {"In", &BsCommonDialog::Enter_In, &BsCommonDialog::State_In},
    {"Out", &BsCommonDialog::Enter_Out, &BsCommonDialog::State_Out},
    {"Wait", &BsCommonDialog::Enter_Wait, &BsCommonDialog::State_Wait},
    {"待機", &BsCommonDialog::Enter_Idle, &BsCommonDialog::State_Idle},
    {"待機閉じる", &BsCommonDialog::Enter_WaitClose, &BsCommonDialog::State_WaitClose},
};

// 0x007EE8C4
template oml::framework::Process* oml::framework::Process::Create<BsCommonDialog>();

// 0x00251E88 slot 0x00
// 0x00251E5C (deleting dtor)
// 0x00777210 (thunk)
// 0x007771E4 (deleting thunk)
BsCommonDialog::~BsCommonDialog()
{
    // (only the members are destroyed)
}

// 0x00251B70 slot 0x0C
oml::framework::Result BsCommonDialog::Initialize()
{
    if (!m_Dialog.Load()) {
        return oml::framework::RESULT_RETRY;
    }
    m_Dialog.Initialize();
    if (s_IsResultUsed) {
        BsMenuMgr::s_Result = 2;
    }
    s_pInstance = this;
    Start("BsCommonDialog", s_StateInfos, 5);
    return oml::framework::RESULT_SUCCESS;
}

// 0x00251D90 slot 0x18
oml::framework::Result BsCommonDialog::Finalize()
{
    BsMenuMgr::Get()->m_pBsHandCursor->m_Unknown0x4D7 = false;
    BsMenuMgr::Get()->m_HandMode = 0;
    m_Dialog.Finalize();
    s_pInstance = NULL;
    s_IsResultUsed = true;
    return oml::framework::RESULT_SUCCESS;
}

// 0x00251CD0 slot 0x24
oml::framework::Result BsCommonDialog::Calc()
{
    if ((BsMenuMgr::s_Flags & 0x20) != 0) {
        Change(&BsCommonDialog::State_Out);
    }
    Call();
    m_Dialog.Calc();
    return oml::framework::RESULT_SUCCESS;
}

// 0x00251B5C slot 0x30
oml::framework::Result BsCommonDialog::Draw()
{
    m_Dialog.Draw();
    return oml::framework::RESULT_SUCCESS;
}

// 0x00251940 slot 0x44
void BsCommonDialog::OnClose()
{
    Change(&BsCommonDialog::State_Out);
}

// 0x00251908
void BsCommonDialog::ShowNext()
{
    m_Dialog.Open();
    Change(&BsCommonDialog::State_In);
}

// 0x00251B34
void BsCommonDialog::Close()
{
    m_Dialog.Close();
    Change(&BsCommonDialog::State_WaitClose);
}

// 0x00251E24
void BsCommonDialog::CloseLater()
{
    Change(&BsCommonDialog::State_WaitClose);
}

// 0x00251E40
void BsCommonDialog::GoOut()
{
    Change(&BsCommonDialog::State_Out);
}

// 0x00251930
BsCommonDialog* BsCommonDialog::GetInstance()
{
    return s_pInstance;
}

// 0x00251AA0
void BsCommonDialog::State_In()
{
    if (!m_Dialog.IsBusy()) {
        Change(&BsCommonDialog::State_Wait);
    }
}

// 0x001AA408
void BsCommonDialog::Enter_In()
{
    BsHandCursor* cursor = BsMenuMgr::Get()->m_pBsHandCursor;
    cursor->m_Unknown0x4D7 = true;
    cursor->Disable();
    m_Dialog.StartIn();
}

// 0x00251AD0
void BsCommonDialog::State_Out()
{
    if (!m_Dialog.IsBusy()) {
        if (g_CommonDialogArg.isLast == true) {
            StopSelf();
        }
        Change(&BsCommonDialog::State_Idle);
    }
}

// 0x00251B18
void BsCommonDialog::Enter_Out()
{
    BsMenuMgr::s_Flags &= ~0x20;
    m_Dialog.StartOut();
}

// 0x00251960
void BsCommonDialog::State_Wait()
{
    if (m_Dialog.IsSelectOkDone() && m_Dialog.m_IsDecided) {
        if (s_IsResultUsed) {
            BsMenuMgr::s_Result = m_Dialog.GetDecidedIndex();
        }
        Change(&BsCommonDialog::State_Out);
    }
    BsMenuMgr* mgr = BsMenuMgr::Get();
    const bool isCancel = mgr != NULL ? mgr->m_IsCancel : false;
    if (BsMenuMgr::Func6D520C() || isCancel) {
        if (m_Dialog.m_Kind == ComDialogA::KIND_ONE_BUTTON) {
            BsMenuMgr::s_Result = 1;
        } else if (m_Dialog.m_Kind == ComDialogA::KIND_TWO_BUTTONS) {
            BsMenuMgr::s_Result = 0;
        }
        Change(&BsCommonDialog::State_Out);
    }
}

// 0x00251A38
void BsCommonDialog::Enter_Wait()
{
    // empty (in the original too)
}

// 0x0025195C
void BsCommonDialog::State_Idle()
{
    // empty (in the original too)
}

// 0x00251A34
void BsCommonDialog::Enter_Idle()
{
    // empty (in the original too)
}

// 0x00251A3C
void BsCommonDialog::State_WaitClose()
{
    if (!m_Dialog.IsBusy()) {
        if (g_CommonDialogArg.isLast == true) {
            StopSelf();
        }
        Change(&BsCommonDialog::State_Idle);
    }
}

// 0x00251A84
void BsCommonDialog::Enter_WaitClose()
{
    BsMenuMgr::s_Flags &= ~0x20;
    m_Dialog.StartOut();
}
