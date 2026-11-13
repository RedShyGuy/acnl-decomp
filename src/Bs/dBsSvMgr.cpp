#include "Bs/dBsSvMgr.h"

#include "Sv/dSvHolder.h"

#include <cstddef>

#include "Sv/dSvApi.h"
#include "Sv/dSvGlobals.h"
#include "Sv/dSvSaveIo.h"
#include "Sv/dSvSaveMemory.h"
#include "netgame/netgame_Api.h"
#include "script/dMgr.h"
#include "script/dTalkSystem.h"
#include "script/dChoiceStandard.h"
#include "Sv/dSvProc.h"
#include "stage/stage_Api.h"
#include "stage/dTransitionMgr.h"

// 0x009516CD
u8 BsSvMgr::s_SaveNo = 4;
// 0x009516CE
bool BsSvMgr::s_IsAfterGrowup;
// 0x009516CF
u8 BsSvMgr::s_Unknown9516CF;
// 0x009516D0
u8 BsSvMgr::s_Unknown9516D0;
// 0x009516D2
u8 BsSvMgr::s_Unknown9516D2;
// 0x009516D3
u8 BsSvMgr::s_Unknown9516D3;
// 0x009516D4
u8 BsSvMgr::s_Unknown9516D4;
// 0x009516D5
u8 BsSvMgr::s_Unknown9516D5;
// 0x009516D8
const char* BsSvMgr::s_MessageFile = "SYS_Save";
// 0x009516E0
BsSvMgr* BsSvMgr::s_pInstance;
// 0x009516E4
u32 BsSvMgr::s_Unknown9516E4;
// 0x009516F0
BsSvMgr::Func BsSvMgr::s_NextMode;
// (built by the static initializer of the file 0x00786020 together with BsPartsSaveMgr<SvBestFriendList>::s_Request)
// 0x00998DB0
SvStepInner BsSvMgr::s_ApplyRequest(SvStepInner::FLAG_KEEP_ERROR);

// 0x0060B9DC slot 0x00
// 0x00774A2C (thunk 144)
// 0x007749D8 (deleting thunk 144)
BsSvMgr::~BsSvMgr()
{
    // (only the members and the bases are destroyed)
}

// 0x00609258 slot 0x0C
oml::framework::Result BsSvMgr::Initialize()
{
    s_pInstance = this;
    s_Unknown9516CF = 0;
    m_pInfos = s_ModeInfos;
    m_Count = 33;
    m_pName = "BsSvMgr";
    m_Calling = false;
    m_Enabled = true;
    if (m_pInfos[0].transition != NULL) {
        (this->*m_pInfos[0].transition)();
    }
    m_Callback = m_pInfos[0].state;
    unk_0x118 = SvStepInner::FLAG_KEEP_ERROR;
    if (stage::GetCurrent() == stage::NAME_LOAD && s_NextMode == NULL) {
        g_SvProc.ResetTownMapIndex();
        s_NextMode = g_Unknown95D35D ? &BsSvMgr::State_LoadAfterSaveError : &BsSvMgr::State_Load;
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x0060B8D8 slot 0x18
oml::framework::Result BsSvMgr::Finalize()
{
    s_pInstance = NULL;
    if (unk_0x118 != SvStepInner::FLAG_KEEP_ERROR) {
        if (s_ApplyRequest.m_Result == 0) {
            s_ApplyRequest.Reset();
            s_ApplyRequest.m_Flags = unk_0x118;
            s_ApplyRequest.Start();
            goto retry;
        }
        if (!s_ApplyRequest.m_IsDone) {
            goto retry;
        }
    }
    if (g_pUnknown94FD5C != NULL) {
        Func1266A0();
    }
    g_pUnknown94F4C8[0xA10] = 0;
    return oml::framework::RESULT_SUCCESS;
retry:
    s_IsAfterGrowup = false;
    return oml::framework::RESULT_RETRY;
}

// 0x00609368 slot 0x24
oml::framework::Result BsSvMgr::Calc()
{
    if (s_Unknown9516E4 != 0) {
        s_Unknown9516E4--;
    }
    Call();
    return oml::framework::RESULT_SUCCESS;
}

// 0x00607E38 slot 0x40 (name after ITalkRecept)
// 0x00775980 (thunk)
void BsSvMgr::OnPageEnd(u8 value)
{
    (void)value;
    if (m_pOwner->GetMessageNumber() == 4 || m_pOwner->GetMessageNumber() == 9 || m_pOwner->GetMessageNumber() == 15) {
        m_pOwner->SetWaitIcon(1);
        s_Unknown9516E4 = 90;
    }
}

// 0x00607FE0 slot 0x44 (name after ITalkRecept)
// 0x007759E8 (thunk)
void BsSvMgr::OnChoice(const script::ChoiceItem* item, s32 cursor)
{
    (void)item;
    const u32 step = m_DeleteConfirmStep;
    m_ChoiceCursor = cursor;
    if (step == 0) {
        if (cursor == 0) {
            m_pOwner->Jump(35, NULL, NULL, 3);
            m_pOwner->SetJumped();
            m_DeleteConfirmStep = m_DeleteConfirmStep + 1;
        }
    } else if (step == 1 && cursor == 1) {
        m_DeleteConfirmStep = 2;
    }
}

// 0x006079A0 (name is ours)
void BsSvMgr::StartMessage(s32 number)
{
    SetFile(s_MessageFile);
    SetNumberText(number);
    script::Mgr::s_pInstance->m_pTalk->SetRecept(this);
    m_pOwner->Next(true);
}

// 0x00607CF0 (name is ours)
void BsSvMgr::StartDeleteChoice()
{
    script::ChoiceItemData data;
    script::TalkSystem* talk = m_pOwner;
    talk->StartChoice();
    switch (m_DeleteConfirmStep) {
    case 0:
        data.SetLabel("SYS_2D_UI", 80, 0, 0, 0, 0);
        talk->SetChoiceItem(0, &data);
        data.SetLabel("SYS_2D_UI", 81, 0, 0, 0, 0);
        talk->SetChoiceItem(1, &data);
        talk->SetChoiceNum(2, 1);
        break;
    case 1:
        data.SetLabel("SYS_2D_UI", 81, 0, 0, 0, 0);
        talk->SetChoiceItem(0, &data);
        data.SetLabel("SYS_2D_UI", 80, 0, 0, 0, 0);
        talk->SetChoiceItem(1, &data);
        talk->SetChoiceNum(2, -1);
        break;
    }
    m_ChoiceCursor = -1;
}

// 0x006079F8 (name is ours)
void SvCountVisit()
{
    GetGardenData()->townData.townFlags.AddCounter10<59>();
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerFlags.unk_0xC_7 = 1;
        Func24BD74(8, 1);
    }
}

// 0x00607A80 (name is ours)
bool BsSvMgr::AfterLoadExhibition()
{
    Func5378CC();
    Func5D0A80();
    Func537F90();
    return true;
}

// 0x00607C60 (name is ours)
bool BsSvMgr::BeforeCommitExhibition()
{
    Func5D0EC0();
    return true;
}

// --- the setters of the next mode ---

// 0x00607EC4
void BsSvMgr::SetSaveAll()
{
    s_NextMode = &BsSvMgr::State_SaveAll;
}

// 0x00608088 (name is ours)
void BsSvMgr::SetSaveNormalOrExhibition()
{
    if (SvGetExhibitionAlways() != NULL) {
        SetSaveAllAndExhibition();
        return;
    }
    SetSaveNormal();
}

// 0x006080A8
void BsSvMgr::SetSaveNormal()
{
    s_NextMode = &BsSvMgr::State_SaveNormal;
}

// 0x0060814C
void BsSvMgr::SetErrorNetwork()
{
    s_NextMode = &BsSvMgr::State_ErrorNetwork;
}

// 0x0060827C
void BsSvMgr::SetSaveAllAndReturn()
{
    s_NextMode = &BsSvMgr::State_SaveAllAndReturn;
}

// 0x00608360
void BsSvMgr::SetLoadAfterSaveError()
{
    s_NextMode = &BsSvMgr::State_LoadAfterSaveError;
}

// 0x0060837C
void BsSvMgr::SetInitGameInitFormat()
{
    s_NextMode = &BsSvMgr::State_InitGameInitFormat;
}

// 0x00608398
void BsSvMgr::SetLoadAndSaveExhibition()
{
    s_NextMode = &BsSvMgr::State_LoadAndSaveExhibition;
}

// 0x006084CC
void BsSvMgr::SetSaveSeeOffNetHst()
{
    s_NextMode = &BsSvMgr::State_SaveSeeOffNetHst;
}

// 0x006084E8
void BsSvMgr::SetSaveSeeOffNetVst()
{
    s_NextMode = &BsSvMgr::State_SaveSeeOffNetVst;
}

// 0x00608660
void BsSvMgr::SetDelete()
{
    s_NextMode = &BsSvMgr::State_Delete;
}

// 0x0060867C
void BsSvMgr::SetSaveConnectNetHst()
{
    s_NextMode = &BsSvMgr::State_SaveConnectNetHst;
}

// 0x00608698
void BsSvMgr::SetSaveConnectNetVst()
{
    s_NextMode = &BsSvMgr::State_SaveConnectNetVst;
}

// 0x0060885C
void BsSvMgr::SetSaveContinueGrowup()
{
    s_NextMode = &BsSvMgr::State_SaveContinueGrowup;
}

// 0x00608878
void BsSvMgr::SetSaveContinueNetHst()
{
    s_NextMode = &BsSvMgr::State_SaveContinueNetHst;
}

// 0x00608894
void BsSvMgr::SetSaveContinueNetVst()
{
    s_NextMode = &BsSvMgr::State_SaveContinueNetVst;
}

// 0x00608A3C
void BsSvMgr::SetSaveContinuePlaying()
{
    s_NextMode = &BsSvMgr::State_SaveContinuePlaying;
}

// 0x00608A58
void BsSvMgr::SetSaveInterruptNetHst()
{
    s_NextMode = &BsSvMgr::State_SaveInterruptNetHst;
}

// 0x00608C10
void BsSvMgr::SetSaveAllAndExhibition()
{
    s_NextMode = &BsSvMgr::State_SaveAllAndExhibition;
}

// 0x00608CBC
void BsSvMgr::SetSaveSeeOffRandomMatchNetHst()
{
    s_NextMode = &BsSvMgr::State_SaveSeeOffRandomMatchNetHst;
}

// 0x00608CD8
void BsSvMgr::SetSaveSeeOffRandomMatchNetVst()
{
    s_NextMode = &BsSvMgr::State_SaveSeeOffRandomMatchNetVst;
}

// 0x00608DF4
void BsSvMgr::SetSaveConnectRandomMatchNetHst()
{
    s_NextMode = &BsSvMgr::State_SaveConnectRandomMatchNetHst;
}

// 0x00608E10
void BsSvMgr::SetSaveConnectRandomMatchNetVst()
{
    s_NextMode = &BsSvMgr::State_SaveConnectRandomMatchNetVst;
}

// 0x00608F44
void BsSvMgr::SetSaveStartTourFriendMatchNetHst()
{
    s_NextMode = &BsSvMgr::State_SaveStartTourFriendMatchNetHst;
}

// 0x00608F60
void BsSvMgr::SetSaveStartTourFriendMatchNetVst()
{
    s_NextMode = &BsSvMgr::State_SaveStartTourFriendMatchNetVst;
}

// 0x00608F7C
void BsSvMgr::SetSaveStartTourRandomMatchNetHst()
{
    s_NextMode = &BsSvMgr::State_SaveStartTourRandomMatchNetHst;
}

// 0x00608F98
void BsSvMgr::SetSaveStartTourRandomMatchNetVst()
{
    s_NextMode = &BsSvMgr::State_SaveStartTourRandomMatchNetVst;
}

// 0x006091B4
void BsSvMgr::SetSaveSeeOffNetHostRandomMatchNetVst()
{
    s_NextMode = &BsSvMgr::State_SaveSeeOffNetHostRandomMatchNetVst;
}

// 0x00608A28 (name is ours)
void BsSvMgr::SetSaveContinuePlayingWithReturn()
{
    stage::GetTransitionMgr()->SetReturnParam();
    // (falls into SetSaveContinuePlaying in the original)
    SetSaveContinuePlaying();
}

// 0x00608A74 | libgarden [tier A]
void BsSvMgr::SetSaveInterruptNetVst()
{
    // (the player number is fetched, its use was a removed log)
    netgame::GetMyPlayerNo();
    s_SaveNo = SvGetPlayerSaveNo();
    s_NextMode = &BsSvMgr::State_SaveInterruptNetVst;
}

// 0x00607AAC (name is ours)
void BsSvMgr::State_Wait()
{
    if (Func566AD0() && s_NextMode != NULL) {
        ChangeInline(s_NextMode);
        s_NextMode = NULL;
    }
}

// 0x00607C58 (name is ours)
void BsSvMgr::Enter_None()
{
    // empty (in the original too)
}

// 0x00607AA8 (name is ours)
void BsSvMgr::State_None()
{
    // empty (in the original too)
}

// 0x006080F0 (name is ours)
void BsSvMgr::Enter_SaveNormal()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveNormal", s_StepSaveNormal, 20, false);
}

// 0x006080E0 (name is ours)
void BsSvMgr::State_SaveNormal()
{
    m_SaveStep.Exec();
}

// 0x00608C3C (name is ours)
void BsSvMgr::Enter_SaveAllAndExhibition()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveNormal", s_StepSaveAllAndExhibition, 20, false);
}

// 0x00608C2C (name is ours)
void BsSvMgr::State_SaveAllAndExhibition()
{
    m_SaveStep.Exec();
}

// 0x00608438 (name is ours)
void BsSvMgr::Enter_LoadAndSaveExhibition()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveExhibition", s_StepSaveExhibition, 19, false);
}

// 0x006083C4 (name is ours)
void BsSvMgr::State_LoadAndSaveExhibition()
{
    m_SaveStep.Exec();
}

// 0x00607EF0 (name is ours)
void BsSvMgr::Enter_SaveAll()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveAll", s_StepSaveAll, 17, false);
}

// 0x00607EE0 (name is ours)
void BsSvMgr::State_SaveAll()
{
    m_SaveStep.Exec();
}

// 0x00608304 (name is ours)
void BsSvMgr::Enter_SaveAllAndReturn()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveAll", s_StepSaveAllAndReturn, 17, false);
}

// 0x006082F4 (name is ours)
void BsSvMgr::State_SaveAllAndReturn()
{
    m_SaveStep.Exec();
}

// 0x006088E0 (name is ours)
void BsSvMgr::Enter_SaveContinueGrowup()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveContinueGrowup", s_StepSaveContinueGrowup, 18, false);
    s_IsAfterGrowup = true;
}

// 0x006088B0 (name is ours)
void BsSvMgr::State_SaveContinueGrowup()
{
    m_SaveStep.Exec();
}

// 0x00608AD8 (name is ours)
void BsSvMgr::Enter_SaveContinuePlaying()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveContinuePlaying", s_StepSaveContinuePlaying, 18, false);
}

// 0x00608AA8 (name is ours)
void BsSvMgr::State_SaveContinuePlaying()
{
    m_SaveStep.Exec();
}

// 0x00608208 (name is ours)
void BsSvMgr::Enter_UpdateGrowup()
{
    m_SaveStep.Set(this, "BsSvMgr::modeUpdateGrowup", s_StepUpdateGrowup, 8, false);
    g_pUnknown94F4C8[0xA10] = 1;
}

// 0x00608198 (name is ours)
void BsSvMgr::State_UpdateGrowup()
{
    m_SaveStep.Exec();
}

// 0x00608598 (name is ours)
void BsSvMgr::Enter_SaveSeeOffNetHst()
{
    m_SaveStep.Set(this, "BsSvMgrc::stepSaveSeeOffNetHst", s_StepSaveSeeOffNetHst, 26, true);
}

// 0x00608514 (name is ours)
void BsSvMgr::State_SaveSeeOffNetHst()
{
    m_SaveStep.Exec();
}

// 0x006085FC (name is ours)
void BsSvMgr::Enter_SaveSeeOffNetVst()
{
    m_SaveStep.Set(this, "BsSvMgrc::stepSaveSeeOffNetVst", s_StepSaveSeeOffNetVst, 23, true);
}

// 0x00608524 (name is ours)
void BsSvMgr::State_SaveSeeOffNetVst()
{
    m_SaveStep.Exec();
}

// 0x00608534 (name is ours)
void BsSvMgr::Enter_SaveRetireNetVst()
{
    m_SaveStep.Set(this, "BsSvMgrc::stepSaveRetireNetVst", s_StepSaveRetireNetVst, 25, true);
}

// 0x00608504 (name is ours)
void BsSvMgr::State_SaveRetireNetVst()
{
    m_SaveStep.Exec();
}

// 0x0060877C (name is ours)
void BsSvMgr::Enter_SaveConnectNetHst()
{
    m_CpuTimeLimit.Set(23);
    m_SaveStep.Set(this, "BsSvMgr::stepSaveConnectNetHst", s_StepSaveConnectNetHst, 20, true);
}

// 0x00608704 (name is ours)
void BsSvMgr::State_SaveConnectNetHst()
{
    m_SaveStep.Exec();
}

// 0x006087EC (name is ours)
void BsSvMgr::Enter_SaveConnectNetVst()
{
    m_CpuTimeLimit.Set(23);
    m_SaveStep.Set(this, "BsSvMgr::stepSaveConnectNetVst", s_StepSaveConnectNetVst, 17, true);
}

// 0x00608714 (name is ours)
void BsSvMgr::State_SaveConnectNetVst()
{
    m_SaveStep.Exec();
}

// 0x00608954 (name is ours)
void BsSvMgr::Enter_SaveContinueNetHst()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveContinueNetHst", s_StepSaveContinueNetHst, 19, true);
}

// 0x006088C0 (name is ours)
void BsSvMgr::State_SaveContinueNetHst()
{
    m_SaveStep.Exec();
}

// 0x006089B8 (name is ours)
void BsSvMgr::Enter_SaveContinueNetVst()
{
    m_CpuTimeLimit.Set(23);
    m_SaveStep.Set(this, "BsSvMgr::stepSaveContinueNetVst", s_StepSaveContinueNetVst, 20, true);
}

// 0x006088D0 (name is ours)
void BsSvMgr::State_SaveContinueNetVst()
{
    m_SaveStep.Exec();
}

// 0x00608B40 (name is ours)
void BsSvMgr::Enter_SaveInterruptNetHst()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveInterruptNetHst", s_StepSaveInterruptNetHst, 23, true);
}

// 0x00608AB8 (name is ours)
void BsSvMgr::State_SaveInterruptNetHst()
{
    m_SaveStep.Exec();
}

// 0x00608BA8 (name is ours)
void BsSvMgr::Enter_SaveInterruptNetVst()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveInterruptNetVst", s_StepSaveInterruptNetVst, 27, true);
}

// 0x00608AC8 (name is ours)
void BsSvMgr::State_SaveInterruptNetVst()
{
    m_SaveStep.Exec();
}

// 0x00607C00 (name is ours)
void BsSvMgr::Enter_Load()
{
    m_SaveStep.Set(this, "BsSvMgr::stepLoad", s_StepLoad, 11, true);
}

// 0x00607A98 (name is ours)
void BsSvMgr::State_Load()
{
    m_SaveStep.Exec();
}

// 0x00608E4C (name is ours)
void BsSvMgr::Enter_SaveConnectRandomMatchNetHst()
{
    m_CpuTimeLimit.Set(23);
    m_SaveStep.Set(this, "BsSvMgr::stepSaveConnectRandomMatchNetHst", s_StepSaveConnectRandomMatchNetHst, 25, true);
}

// 0x00608E2C (name is ours)
void BsSvMgr::State_SaveConnectRandomMatchNetHst()
{
    m_SaveStep.Exec();
}

// 0x00608EC8 (name is ours)
void BsSvMgr::Enter_SaveConnectRandomMatchNetVst()
{
    m_CpuTimeLimit.Set(23);
    m_SaveStep.Set(this, "BsSvMgr::stepSaveConnectRandomMatchNetVst", s_StepSaveConnectRandomMatchNetVst, 23, true);
}

// 0x00608E3C (name is ours)
void BsSvMgr::State_SaveConnectRandomMatchNetVst()
{
    m_SaveStep.Exec();
}

// 0x00608D14 (name is ours)
void BsSvMgr::Enter_SaveSeeOffRandomMatchNetHst()
{
    m_SaveStep.Set(this, "BsSvMgrc::stepSaveSeeOffRandomMatchNetHst", s_StepSaveSeeOffRandomMatchNetHst, 25, true);
}

// 0x00608CF4 (name is ours)
void BsSvMgr::State_SaveSeeOffRandomMatchNetHst()
{
    m_SaveStep.Exec();
}

// 0x00608D84 (name is ours)
void BsSvMgr::Enter_SaveSeeOffRandomMatchNetVst()
{
    m_SaveStep.Set(this, "BsSvMgrc::stepSaveSeeOffRandomMatchNetVst", s_StepSaveSeeOffRandomMatchNetVst, 22, true);
}

// 0x00608D04 (name is ours)
void BsSvMgr::State_SaveSeeOffRandomMatchNetVst()
{
    m_SaveStep.Exec();
}

// 0x006091E0 (name is ours)
void BsSvMgr::Enter_SaveSeeOffNetHostRandomMatchNetVst()
{
    m_SaveStep.Set(this, "BsSvMgrc::stepSaveSeeOffNetHostRandomMatchNetVst", s_StepSaveSeeOffNetHostRandomMatchNetVst, 22, true);
}

// 0x006091D0 (name is ours)
void BsSvMgr::State_SaveSeeOffNetHostRandomMatchNetVst()
{
    m_SaveStep.Exec();
}

// 0x006090D4 (name is ours)
void BsSvMgr::Enter_SaveStartTourRandomMatchNetHst()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveStartTourRandomMatchNetHst", s_StepSaveStartTourRandomMatchNetHst, 21, true);
}

// 0x00608FD4 (name is ours)
void BsSvMgr::State_SaveStartTourRandomMatchNetHst()
{
    m_SaveStep.Exec();
}

// 0x00609144 (name is ours)
void BsSvMgr::Enter_SaveStartTourRandomMatchNetVst()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveStartTourRandomMatchNetVst", s_StepSaveStartTourRandomMatchNetVst, 20, true);
}

// 0x00608FE4 (name is ours)
void BsSvMgr::State_SaveStartTourRandomMatchNetVst()
{
    m_SaveStep.Exec();
}

// 0x00608FF4 (name is ours)
void BsSvMgr::Enter_SaveStartTourFriendMatchNetHst()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveStartTourFriendMatchNetHst", s_StepSaveStartTourFriendMatchNetHst, 20, true);
}

// 0x00608FB4 (name is ours)
void BsSvMgr::State_SaveStartTourFriendMatchNetHst()
{
    m_SaveStep.Exec();
}

// 0x00609064 (name is ours)
void BsSvMgr::Enter_SaveStartTourFriendMatchNetVst()
{
    m_SaveStep.Set(this, "BsSvMgr::stepSaveStartTourFriendMatchNetVst", s_StepSaveStartTourFriendMatchNetVst, 21, true);
}

// 0x00608FC4 (name is ours)
void BsSvMgr::State_SaveStartTourFriendMatchNetVst()
{
    m_SaveStep.Exec();
}

// 0x006083D4 (name is ours)
void BsSvMgr::Enter_InitGameInitFormat()
{
    m_SaveStep.Set(this, "BsSvMgr::modeInitGameInitFormat", s_StepInitGameInitFormat, 8, false);
}

// 0x006083B4 (name is ours)
void BsSvMgr::State_InitGameInitFormat()
{
    m_SaveStep.Exec();
}

// 0x00608724 (name is ours)
void BsSvMgr::Enter_Delete()
{
    m_SaveStep.Set(this, "BsSvMgr::stepDelete", s_StepDelete, 14, true);
}

// 0x006086F4 (name is ours)
void BsSvMgr::State_Delete()
{
    m_SaveStep.Exec();
}

// 0x00607C98 (name is ours)
void BsSvMgr::Enter_LoadAfterSaveError()
{
    m_SaveStep.Set(this, "BsSvMgr::stepLoad", s_StepLoadAfterSaveError, 5, true);
}

// 0x00607C88 (name is ours)
void BsSvMgr::State_LoadAfterSaveError()
{
    m_SaveStep.Exec();
}

// 0x006081A8 (name is ours)
void BsSvMgr::Enter_ErrorNetwork()
{
    m_SaveStep.Set(this, "BsSvMgr::stepErrorNetwork", s_StepErrorNetwork, 13, true);
}

// 0x00608188 (name is ours)
void BsSvMgr::State_ErrorNetwork()
{
    m_SaveStep.Exec();
}

// --- the offsets of the parts of the save file and the checks of the running write (free functions of this file;
// names are ours, IsSaveIoOn* after their use in the access functions) ---

// 0x00607C70 (name is ours)
u32 SvGetGardenDataOffset()
{
    return reinterpret_cast<u8*>(&GetGardenPlus()->data) - reinterpret_cast<u8*>(GetGardenPlus());
}

// 0x00607EA4 (name is ours)
u32 SvGetNpcDataOffset()
{
    return reinterpret_cast<u8*>(&GetGardenPlus()->data) - reinterpret_cast<u8*>(GetGardenPlus()) + offsetof(SvGardenData, npcData);
}

// 0x00607F4C (name is ours)
bool IsSaveIoOnWholeFile()
{
    const u32 size = g_SvGardenPlusSize;
    return SvSaveIo::IsSaving() && SvSaveIo::GetOffset() == 0 && SvSaveIo::GetSize() == size;
}

// 0x00607F8C (name is ours)
bool IsSaveIoOnNpcData()
{
    const u32 offset = reinterpret_cast<u8*>(&GetGardenPlus()->data) - reinterpret_cast<u8*>(GetGardenPlus()) + offsetof(SvGardenData, npcData);
    return SvSaveIo::IsSaving() && SvSaveIo::GetOffset() == offset && SvSaveIo::GetSize() == 0x22BCC;
}

// 0x00608060 (name is ours)
u32 SvGetPlayerOffset(u32 saveNo)
{
    return reinterpret_cast<u8*>(GetGardenPlus()->data.players.Get(saveNo)) - reinterpret_cast<u8*>(GetGardenPlus());
}

// 0x006080C4 (name is ours)
u32 SvGetSharedDataOffset()
{
    return reinterpret_cast<u8*>(&GetGardenPlus()->sharedData) - reinterpret_cast<u8*>(GetGardenPlus());
}

// 0x00608168 (name is ours)
u32 SvGetSharedDataOffset2()
{
    return reinterpret_cast<u8*>(GetGardenPlus()) - reinterpret_cast<u8*>(GetGardenPlus()) + offsetof(SvGardenPlus, data) +
           sizeof(SvGardenData);
}

// 0x006086B4 (name is ours)
bool IsSaveIoOnMainData()
{
    GetGardenPlus();
    const u32 size = SvGetGardenDataSaveSize();
    return SvSaveIo::IsSaving() && SvSaveIo::GetOffset() == 0 && SvSaveIo::GetSize() == size;
}

// the factory of the process class
// 0x007F3078
template oml::framework::Process* oml::framework::Process::Create<BsSvMgr>();
