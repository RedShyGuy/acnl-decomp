#include "Bs/dBsSvDemo.h"

#include "Audio/bgm/dBgmMgr.h"
#include "Bs/dBsSvMgr.h"
#include "Sv/dSvApi.h"
#include "demo/demo_Api.h"
#include "netgame/dNetGameMgr.h"
#include "netgame/netgame_Api.h"
#include "script/dMgr.h"
#include "script/dScriptExtern.h"
#include "script/dTalkSystem.h"
#include "Sound/sound_Extern.h"
#include "stage/dTransitionMgr.h"
#include "stage/stage_Api.h"

inline void BsSvDemo::StartMessage(s32 number)
{
    SetFile(BsSvMgr::s_MessageFile);
    SetNumberText(number);
    script::Mgr::s_pInstance->m_pTalk->SetRecept(this);
    m_pOwner->Next(true);
}

inline void BsSvDemo::EndMessage()
{
    m_pOwner->ReleaseRecept();
    demo::EndSave();
    Change(&BsSvDemo::State_Wait);
}

// 0x006A2A20 (name is ours)
void BsSvDemo::State_Wait()
{
    if (!demo::CheckOrder(demo::SAVE)) {
        return;
    }
    s32 number;
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    if (player != NULL && !player->playerFlags.finishedFirstDay) {
        number = 17;
    } else {
        Func31336C(stage::GetCurrent());
        void* entry = Func2FE4A0(Func31336C(stage::GetCurrent()));
        if (entry != NULL && (static_cast<u32*>(entry)[17] & 0x20)) {
            number = 19;
        } else {
            number = -1;
        }
    }
    m_MessageNumber = number;
    if (number != -1) {
        Change(&BsSvDemo::State_CannotSave1);
    } else if (stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent())) {
        Change(&BsSvDemo::State_CannotSave3);
    } else if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        Change(&BsSvDemo::State_CannotSave4);
    } else if (netgame::IsMultiPlayer() && netgame::IsMultiplePlayer()) {
        if (!netgame::IsHost()) {
            Change(&BsSvDemo::State_WaitQuestion);
        } else if (Func5C505C(netgame::PLAYER_NO_MY_NO)) {
            Change(&BsSvDemo::State_CannotSave5);
        } else {
            Change(&BsSvDemo::State_WaitNet);
        }
    } else {
        Change(&BsSvDemo::State_Save);
    }
}

// 0x006A2B68 (name is ours)
void BsSvDemo::State_Save()
{
    stage::Transition(stage::NAME_SAVE, 0, 0, 1, 1);
}

// 0x006A2B8C (name is ours)
void BsSvDemo::Enter_Wait()
{
    // empty (in the original too)
}

// 0x006A2B90 slot 0x44 (name after ITalkRecept)
// 0x00775CC4 (thunk 20)
void BsSvDemo::OnPageEnd(u8 value)
{
    (void)value;
    if (m_pOwner->GetMessageNumber() == 22) {
        m_pOwner->SetWaitIcon(1);
    }
}

// 0x006A2BC4 (name is ours)
void BsSvDemo::State_DoNothing()
{
    // empty (in the original too)
}

// 0x006A2BC8 (name is ours)
void BsSvDemo::Enter_DoNothing()
{
    // empty (in the original too)
}

// 0x006A2BCC slot 0x40 (name after ITalkRecept)
// 0x00775CF4 (thunk 20)
void BsSvDemo::OnChoice(const script::ChoiceItem* item, s32 cursor)
{
    (void)item;
    m_ChoiceCursor = cursor;
    if (unk_0xBC != cursor) {
        audio::bgm::GetStageOpusMgr()->m_Flags |= audio::bgm::StageOpusMgr::FLAG_8;
    }
}

// 0x006A2BF4 (name is ours)
void BsSvDemo::State_WaitChoice()
{
    if (m_ChoiceCursor == unk_0xBC) {
        demo::EndSave();
        Change(&BsSvDemo::State_Wait);
    } else if (m_ChoiceCursor == 0) {
        Change(&BsSvDemo::State_Save);
    } else {
        Change(&BsSvDemo::State_SaveContinue);
    }
}

// 0x006A2C58 (name is ours)
void BsSvDemo::State_Out()
{
    // empty (in the original too)
}

// 0x006A2C5C (name is ours)
void BsSvDemo::Enter_WaitChoice()
{
    script::Mgr::s_pInstance->m_pTalk->ReleaseRecept();
}

// 0x006A2C78 (name is ours)
void BsSvDemo::Enter_Out()
{
    // empty (in the original too)
}

// 0x006A2C7C (name is ours)
void BsSvDemo::State_LockFailed()
{
    if (m_pOwner->IsFinished()) {
        EndMessage();
    }
}

// 0x006A2CC0 (name is ours)
void BsSvDemo::State_WaitQuestion()
{
    if (m_pOwner->IsFinished()) {
        Change(&BsSvDemo::State_WaitChoice);
    }
}

// 0x006A2CF4 (name is ours)
void BsSvDemo::Enter_LockFailed()
{
    script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
    talk->Jump(25, NULL, NULL, 3);
    talk->SetJumped();
}

// 0x006A2D34 (name is ours)
void BsSvDemo::Enter_WaitQuestion()
{
    if (stage::GetCurrent() == stage::NAME_RESET_MONITORING_CENTER || g_Byte0x951155 || g_Unknown951157) {
        StartMessage(1);
        unk_0xBC = 1;
    } else {
        StartMessage(3);
        unk_0xBC = 2;
    }
}

// 0x006A2E30 (name is ours)
void BsSvDemo::State_CannotSave1()
{
    if (m_pOwner->IsFinished()) {
        EndMessage();
    }
}

// 0x006A2E74 (name is ours)
void BsSvDemo::Enter_CannotSave1()
{
    StartMessage(m_MessageNumber);
}

// 0x006A2ECC (name is ours)
void BsSvDemo::State_LockContinue()
{
    if (m_pOwner->IsPageWait() && m_pOwner->IsWaitIconActive() && netgame::RequestLock2()) {
        Change(&BsSvDemo::State_LockContinueResult);
    }
}

// 0x006A2F20 (name is ours)
void BsSvDemo::State_EndChoice()
{
    switch (m_ChoiceCursor) {
    case 0:
        Change(&BsSvDemo::State_Lock);
        break;
    case 1:
        Change(&BsSvDemo::State_LockContinue);
        break;
    default:
        demo::EndSave();
        Change(&BsSvDemo::State_Wait);
        break;
    }
}

// 0x006A2F84 (name is ours)
void BsSvDemo::State_SaveContinue()
{
    stage::Transition(stage::NAME_SAVE, 0, 0, 1, 1);
}

// 0x006A2FA8 (name is ours)
void BsSvDemo::Enter_LockContinue()
{
    StartMessage(22);
}

// 0x006A3000 (name is ours)
void BsSvDemo::Enter_EndChoice()
{
    script::Mgr::s_pInstance->m_pTalk->ReleaseRecept();
}

// 0x006A301C (name is ours)
void BsSvDemo::State_LockContinueResult()
{
    const s32 result = netgame::GetLockResult();
    if (result == 0) {
        return;
    }
    script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
    talk->HideWaitIcon(false);
    if (result > 0) {
        talk->SetJumped();
        Change(&BsSvDemo::State_LockContinueSucceeded);
    } else {
        Change(&BsSvDemo::State_LockFailed);
    }
}

// 0x006A308C (name is ours)
void BsSvDemo::State_Lock()
{
    if (m_pOwner->IsPageWait() && m_pOwner->IsWaitIconActive() && netgame::RequestLock3()) {
        Change(&BsSvDemo::State_LockResult);
    }
}

// 0x006A30E0 (name is ours)
void BsSvDemo::State_WaitNet()
{
    if (m_pOwner->IsFinished()) {
        Change(&BsSvDemo::State_EndChoice);
    }
}

// 0x006A3114 (name is ours)
void BsSvDemo::Enter_LockContinueResult()
{
    // empty (in the original too)
}

// 0x006A3118 (name is ours)
void BsSvDemo::Enter_Lock()
{
    StartMessage(22);
}

// 0x006A3170 (name is ours)
void BsSvDemo::Enter_WaitNet()
{
    StartMessage(21);
    unk_0xBC = 2;
}

// 0x006A31D0 (name is ours)
void BsSvDemo::State_CannotSave2()
{
    if (m_pOwner->IsFinished()) {
        EndMessage();
    }
}

// 0x006A3214 (name is ours)
void BsSvDemo::State_LockResult()
{
    const s32 result = netgame::GetLockResult();
    if (result == 0) {
        return;
    }
    script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
    talk->HideWaitIcon(false);
    if (result > 0) {
        talk->SetJumped();
        Change(&BsSvDemo::State_LockSucceeded);
    } else {
        Change(&BsSvDemo::State_LockFailed);
    }
}

// 0x006A3284 (name is ours)
void BsSvDemo::Enter_CannotSave2()
{
    StartMessage(27);
}

// 0x006A32DC (name is ours)
void BsSvDemo::Enter_LockResult()
{
    // empty (in the original too)
}

// 0x006A32E0 (name is ours)
void BsSvDemo::State_LockContinueSucceeded()
{
    // empty (in the original too)
}

// 0x006A32E4 (name is ours)
void BsSvDemo::State_CannotSave5()
{
    if (m_pOwner->IsFinished()) {
        EndMessage();
    }
}

// 0x006A3328 (name is ours)
void BsSvDemo::State_CannotSave3()
{
    if (m_pOwner->IsFinished()) {
        EndMessage();
    }
}

// 0x006A336C (name is ours)
void BsSvDemo::Enter_CannotSave5()
{
    StartMessage(20);
}

// 0x006A33C4 (name is ours)
void BsSvDemo::Enter_CannotSave3()
{
    if (netgame::IsMultiPlayer() && netgame::GetNetgameType() != netgame::TYPE_RANDOM_MATCH && netgame::IsHost() &&
        netgame::GetPlayerCount() > 1) {
        StartMessage(29);
    } else {
        StartMessage(28);
    }
}

// 0x006A3498 (name is ours)
void BsSvDemo::State_CannotSave4()
{
    if (m_pOwner->IsFinished()) {
        EndMessage();
    }
}

// 0x006A34DC (name is ours)
void BsSvDemo::State_LockSucceeded()
{
    // empty (in the original too)
}

// 0x006A34E0 (name is ours)
void BsSvDemo::Enter_CannotSave4()
{
    StartMessage(18);
}

// 0x006A3538 (name is ours)
void BsSvDemo::Enter_LockSucceeded()
{
    for (u32 i = 0; i < netgame::PLAYER_NO_MAX; i++) {
        if (netgame::GetMyPlayerNo() != i && netgame::IsInSession(static_cast<netgame::PlayerNo>(i & 0xFF), true)) {
            SvCountVisit();
        }
    }
    netgame::StartSave(true);
}

// 0x00608088 (folded)
void BsSvDemo::Enter_Save()
{
    BsSvMgr::SetSaveNormalOrExhibition();
}

// 0x00608A28 (folded)
void BsSvDemo::Enter_SaveContinue()
{
    BsSvMgr::SetSaveContinuePlayingWithReturn();
}

// 0x006273E8 (folded)
void BsSvDemo::Enter_LockContinueSucceeded()
{
    netgame::StartSave(false);
}

// 0x006A3584 slot 0x30
oml::framework::Result BsSvDemo::Draw()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x006A358C slot 0x0C
oml::framework::Result BsSvDemo::Initialize()
{
    Start("BsSvDemo", s_ModeInfos, 21);
    return oml::framework::RESULT_SUCCESS;
}

// 0x006A3620 slot 0x24
oml::framework::Result BsSvDemo::Calc()
{
    Call();
    return oml::framework::RESULT_SUCCESS;
}

// 0x006A36B0 slot 0x18
oml::framework::Result BsSvDemo::Finalize()
{
    if (demo::CheckOrder(demo::SAVE)) {
        demo::EndSave();
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x006A36EC slot 0x00
// 0x006A36D0 (deleting dtor)
// 0x00775D34 (thunk 20)
// 0x00775D1C (deleting thunk 20)
// 0x00774A98 (thunk 144)
// 0x00774A7C (deleting thunk 144)
BsSvDemo::~BsSvDemo()
{
    // (only the bases are destroyed)
}

// the copy of Change for BsSvDemo
// 0x0081C424
template void state::Mode<BsSvDemo>::Change(Func func);

// the factory of the process
// 0x007F34CC
template oml::framework::Process* oml::framework::Process::Create<BsSvDemo>();
