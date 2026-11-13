#include "Bs/dBsSvMgr_SaveStep.h"

#include "Audio/bgm/dBgmMgr.h"
#include "applet/dErrEulaMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Bs/dBsPartsSaveMgr.h"
#include "Sound/sound_Extern.h"
#include "Other/dHeapInit.h"
#include "Sv/dSvHolder.h"
#include "net/dNet.h"
#include "Other/dVisitorLocalInfo.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvCensus.h"
#include "Sv/dSvBestFriendList.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"
#include "time/time_Api.h"
#include "Other/dSaveProcess.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGlobals.h"
#include "stage/dTransitionMgr.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"
#include "math/dAngle16.h"
#include "Sv/dSvProc.h"
#include "Sv/dSvSaveMemory.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dGameGlobals.h"
#include "netgame/dNetGameMgr.h"
#include "netgame/dGateMgr.h"
#include "netgame/dStageMgr.h"
#include "netgame/dIslandMgr.h"
#include "netgame/dLeaveMgr.h"
#include "netgame/dAvatarMgr.h"
#include "netgame/netgame_Api.h"
#include "script/dMgr.h"
#include "script/dTalkSystem.h"

inline void BsSvMgr::SaveStep::UpdatePlayersForSave()
{
    Func5C344C(&SvFgName::s_Empty);
    Func5C73D0();
    netgame::Func5C401C();
    netgame::Func5C69C4();
}

// 0x0060B8C4
// 0x0060B8AC (deleting dtor)
BsSvMgr::SaveStep::~SaveStep()
{
    // (only the members are destroyed)
}

// 0x0060B7E4 (name is ours)
void BsSvMgr::SaveStep::EndSave()
{
    if (!m_Request.m_IsDone) {
        return;
    }
    if (m_Request.m_Result == SvStepInner::RESULT_OK) {
        Next();
        return;
    }
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return;
    }
    netgame::NetGameMgr::s_pInstance->unk_0x132B4 = true;
    if (!netgame::NetGameMgr::s_pInstance->StopNetAndConnection()) {
        return;
    }
    script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
    talk->HideWaitIcon(false);
    talk->Jump(m_Request.m_Detail, NULL, NULL, 3);
    talk->SetJumped();
    netgame::NetGameMgr::s_pInstance->unk_0x132B4 = false;
    m_Func = &SaveStep::Step_WaitMessageEndAndUnlock;
}

// 0x0060B700 (name is ours)
void BsSvMgr::SaveStep::EndLoad(bool isLast)
{
    if (!m_Request.m_IsDone) {
        return;
    }
    if (m_Request.m_Result == SvStepInner::RESULT_OK) {
        Next();
        return;
    }
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return;
    }
    netgame::NetGameMgr::s_pInstance->unk_0x132B4 = true;
    if (!netgame::NetGameMgr::s_pInstance->StopNetAndConnection()) {
        return;
    }
    if (isLast) {
        script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
        talk->HideWaitIcon(false);
        talk->Jump(m_Request.m_Detail, NULL, NULL, 3);
        talk->SetJumped();
    } else {
        m_pOwner->StartMessage(m_Request.m_Detail);
    }
    netgame::NetGameMgr::s_pInstance->unk_0x132B4 = false;
    m_Func = &SaveStep::Step_WaitMessageEndAndUnlock;
}

// 0x00609410 (name is ours, "成長処理")
void BsSvMgr::SaveStep::Step_Growup()
{
    if (s_ApplyRequest.m_IsDone) {
        Next();
    }
}

// 0x00609430 (name is ours, "リセット初期化")
void BsSvMgr::SaveStep::Step_InitializeReset()
{
    Func10C760();
    Next();
}

// 0x00609448 (name is ours, "終了")
void BsSvMgr::SaveStep::Step_End()
{
    GetTalk()->HideWaitIcon(false);
    GetTalk()->Jump(5, NULL, NULL, 3);
    GetTalk()->SetJumped();
    Func2510F0(SvGetSaveMemoryChecked());
    Next();
}

// 0x006094AC (name is ours, "データ送信終了待ち")
void BsSvMgr::SaveStep::Step_WaitDataSent()
{
    if (netgame::IsJoinDone()) {
        Next();
    }
}

// 0x006094C8 (name is ours, "メッセージ終了待ち")
void BsSvMgr::SaveStep::Step_WaitMessageEnd()
{
    if (GetTalk()->IsIdle()) {
        script::Mgr::s_pInstance->m_pTalk->ReleaseRecept();
        audio::bgm::GetTrackMixer()->RemoveMix(36);
        Next();
    }
}

// 0x00609514 (name is ours, "開いているなら門を閉じる")
void BsSvMgr::SaveStep::Step_CloseGateIfOpen()
{
    if (netgame::IsMultiPlayer()) {
        netgame::g_EnteringPlayer = netgame::PLAYER_NO_INVALID;
    }
    if (netgame::NetGameMgr::s_pInstance->m_ConnectMgr.IsConnectedOrConnecting(
            netgame::ConnectMgr::CONNECTION_BEST_FRIEND)) {
        netgame::StopConnectBestFriend();
    }
    Next();
}

// 0x00609570 (name is ours, "メッセージロックまで待つ")
void BsSvMgr::SaveStep::Step_WaitMessageLock()
{
    if (GetTalk()->IsPageWait() && GetTalk()->IsWaitIconActive()) {
        Next();
    }
}

// 0x006095B0 (name is ours, "ロック解除")
void BsSvMgr::SaveStep::Step_Unlock()
{
    if (s_Unknown9516E4 == 0) {
        GetTalk()->HideWaitIcon(false);
        Next();
    }
}

// 0x006095EC (name is ours, "WiFiコネクション切断")
void BsSvMgr::SaveStep::Step_DisconnectWiFi()
{
    if (netgame::StopNet()) {
        Next();
    }
}

// 0x00609608 (name is ours, "コミット")
void BsSvMgr::SaveStep::Step_Commit()
{
    m_Request.Reset();
    m_Request.m_Flags = SvStepInner::FLAG_COMMIT;
    if (m_Request.Start()) {
        Next();
    }
}

// 0x0060963C (name is ours, "デモ終了待ち")
void BsSvMgr::SaveStep::Step_WaitDemoEnd()
{
    if (netgame::IsMgr8Done()) {
        netgame::SetJoinFinished();
        Next();
    }
}

// 0x00609660 (name is ours, "ネットホストセーブ終了設定 / ネットビジターセーブ終了設定")
void BsSvMgr::SaveStep::Step_SetNetSaveEnd()
{
    netgame::NetGameMgr::s_pInstance->m_JoinMgr.unk_0x16 = true;
    Next();
}

// 0x00609680 (name is ours, "成長処理前処理")
void BsSvMgr::SaveStep::Step_BeforeGrowup()
{
    g_SvProc.ResetTownMapIndex();
    s_ApplyRequest.Reset();
    s_ApplyRequest.m_Flags = SvStepInner::FLAG_KEEP_ERROR | (10 << 6);
    if (s_ApplyRequest.Start()) {
        Next();
    }
}

// 0x006096CC (name is ours, "ゲーム開始処理初期化")
void BsSvMgr::SaveStep::Step_InitializeGameStart()
{
    Func6066F4(0);
    Func27EC2C(&m_Request);
    GetGardenData()->townData.townFlags.unk_0xE_0 = 1;
    Func118A7C(&GetGardenData()->townData.unk_0x1CA84);
    s_Unknown9516D4 = 1;
    if (GetGardenPlus()->data.townData.townFlags.unk_0xD_3) {
        bool value = true;
        for (u32 i = 0; i < 4; i++) {
            SvPlayer* player = netgame::Func3038EC(static_cast<u8>(i));
            if (player != NULL && player->playerInfo.IsValid() && player->playerFlags.unk_0x27_5) {
                value = false;
                break;
            }
        }
        Func114CA8(value);
    }
    if (s_Unknown9516D5) {
        m_Request.m_Flags = SvStepInner::FLAG_EXHIBITION | (5 << 6);
        s_Unknown9516D5 = 0;
    } else {
        m_Request.m_Flags = SvStepInner::FLAG_EXHIBITION | (5 << 6) | SaveProcess::LOAD_FILE_GARDEN_PLUS;
    }
    if (m_Request.Start()) {
        Next();
    }
}

// 0x006097F0 (name is ours, "タイトルへ")
void BsSvMgr::SaveStep::Step_ToTitle()
{
    stage::Transition(stage::NAME_TITLE, 0, 0, 1, 1);
    Next();
}

// 0x00609820 (name is ours, "迎えデモへ")
void BsSvMgr::SaveStep::Step_ToWelcomeDemo()
{
    if (netgame::IsMultiPlayer() && netgame::g_EnteringPlayer < netgame::PLAYER_NO_MAX) {
        SvGetNetPlayer(static_cast<netgame::PlayerNo>(netgame::g_EnteringPlayer));
        stage::Transition(stage::NAME_STATION_ENTRY_DEMO, 2, 1, 1, 1);
        Next();
    }
}

// 0x00609874 (name is ours, "村作成")
void BsSvMgr::SaveStep::Step_CreateTown()
{
    s_ApplyRequest.Reset();
    const s32 result = m_Request.m_Result;
    if (result == SvStepInner::RESULT_OK) {
        s_ApplyRequest.m_Flags = SvStepInner::FLAG_KEEP_ERROR | (7 << 6);
    } else {
        s_ApplyRequest.m_Flags = SvStepInner::FLAG_KEEP_ERROR | SvStepInner::FLAG_CLEAR_SAVE_MEMORY | (6 << 6);
        if (result == 3) {
            g_SvErrorCode = 11;
        } else if (result == 5) {
            g_SvErrorCode = 12;
        }
    }
    if (s_ApplyRequest.Start()) {
        Next();
    }
}

// 0x00609900 (name is ours, "初期化処理")
void BsSvMgr::SaveStep::Step_Initialize()
{
    Next();
}

// 0x0060990C (name is ours, "メッセージ終了待ち")
void BsSvMgr::SaveStep::Step_WaitMessageEndAndUnlock()
{
    if (GetTalk()->IsIdle()) {
        script::Mgr::s_pInstance->m_pTalk->ReleaseRecept();
        audio::bgm::GetTrackMixer()->RemoveMix(36);
        Next();
    }
}

// 0x00609958 (name is ours, "街へ / 元居た場所へ / 元のステージへ / 元のシーンへ")
void BsSvMgr::SaveStep::Step_ToPreviousScene()
{
    if (s_Unknown9516D2) {
        const u32 entrance = SvGetPlayerSaveNo() + 2;
        stage::GetTransitionMgr()->TransitionByEntrance(entrance, 1, 0, 1, 1);
        s_Unknown9516D2 = 0;
    } else {
        stage::GetTransitionMgr()->TransitionToReturnPoint(0, 0, 1, 1);
    }
    Next();
}

// 0x006099DC (name is ours, "元のステージへ")
void BsSvMgr::SaveStep::Step_ToPreviousStage()
{
    if (netgame::IsMultiPlayer() && netgame::g_EnteringPlayer < netgame::PLAYER_NO_MAX) {
        stage::GetTransitionMgr()->TransitionToReturnPoint(0, 0, 1, 1);
        Next();
    }
}

// 0x00609A2C (name is ours, "デモ終了通知完了待ち")
void BsSvMgr::SaveStep::Step_WaitDemoEndNotified()
{
    if (netgame::IsJoinNotFinished()) {
        netgame::ResetMgr8();
        netgame::ClearLockSource();
        netgame::SetLockUnlocked();
        Next();
    }
}

// 0x00609A58 (name is ours, "他の村への手紙処理開始")
void BsSvMgr::SaveStep::Step_StartLettersToOtherTowns()
{
    Next();
}

// 0x00609A64 (name is ours, "メッセージ強制送り")
void BsSvMgr::SaveStep::Step_ForceMessage()
{
    GetTalk()->HideWaitIcon(false);
    GetTalk()->SetJumped();
    Next();
}

// 0x00609A9C (name is ours, "「心のさけび」ログアウト")
void BsSvMgr::SaveStep::Step_Unknown609A9C()
{
    if (netgame::NetGameMgr::s_pInstance->m_ConnectMgr.IsConnectedOrConnecting(netgame::ConnectMgr::CONNECTION_BEST_FRIEND)) {
        netgame::StopConnectBestFriend();
    }
    if (!netgame::NetGameMgr::s_pInstance->m_ConnectMgr.IsConnectingOrDisconnecting(netgame::ConnectMgr::CONNECTION_BEST_FRIEND)) {
        netgame::NetGameMgr::s_pInstance->m_BestFriendMgr.Stop();
        Next();
    }
}

// 0x00609B00 (name is ours, "ゲーム開始処理初期化")
void BsSvMgr::SaveStep::Step_InitializeGameStartNet()
{
    Func6066F4(0);
    Func27EC2C(&m_Request);
    if (m_Request.Start()) {
        Next();
    }
}

// 0x00609B30 (name is ours, "駅の中へ")
void BsSvMgr::SaveStep::Step_IntoStation()
{
    math::Angle16Vec3 rotation = {0, 0xA800, 0};
    nn::math::VEC3_ position;
    position.x = 237.0f;
    position.y = 32.0f;
    position.z = 368.0f;
    stage::GetTransitionMgr()->TransitionTo(stage::NAME_STATION, &position, &rotation, PLAYER_ACTION_GREET, 0, 0, 1, 1);
    Next();
}

// 0x00609BC8 (name is ours, "帰村デモへ")
void BsSvMgr::SaveStep::Step_ToReturnDemo()
{
    stage::Transition(stage::NAME_STATION_COME_HOME_DEMO, 2, 1, 1, 1);
    Next();
}

// 0x00609BF8 (name is ours, "削除メッセージ")
void BsSvMgr::SaveStep::Step_DeleteMessage()
{
    m_pOwner->StartMessage(34);
    m_pOwner->m_DeleteConfirmStep = 0;
    Next();
}

// 0x00609C24 (name is ours, "退場デモ終了")
void BsSvMgr::SaveStep::Step_EndLeaveDemo()
{
    netgame::NetGameMgr::s_pInstance->m_LeaveMgr.unk_0x253C = true;
    Next();
}

// 0x00609C4C (name is ours, "セーブデータ全削除処理")
void BsSvMgr::SaveStep::Step_DeleteAllSaveData()
{
    Func6066F4(0);
    Func27EBF0(&m_Request);
    if (m_Request.Start()) {
        Next();
    }
}

// 0x00609C7C (name is ours, "初期化処理")
void BsSvMgr::SaveStep::Step_InitializeDelete()
{
    netgame::g_EnteringPlayer = netgame::PLAYER_NO_INVALID;
    Next();
}

// 0x00609C98 (name is ours, "他の村への手紙処理終了")
void BsSvMgr::SaveStep::Step_EndLettersToOtherTowns()
{
    Next();
}

// 0x00609CA4 (name is ours, "ロード処理完了")
void BsSvMgr::SaveStep::Step_LoadDone()
{
    script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
    if (m_Request.m_Result != SvStepInner::RESULT_OK) {
        talk->HideWaitIcon(false);
        talk->Jump(m_Request.m_Detail, NULL, NULL, 3);
        talk->SetJumped();
        m_Func = &SaveStep::Step_Next;
        return;
    }
    const netgame::PlayerNo myNo = netgame::GetMyPlayerNo();
    const u8 saveNo = s_SaveNo;
    netgame::SetPlayerActive(myNo, netgame::GetSaveNo(), NULL);
    SvPlayer* player = SvGetPlayer(static_cast<PlayerSaveNumber>(saveNo));
    SvCopyPlayer(SvGetCurrentPlayer(), player);
    SvRestoreAfterNetVisit(SvGetSaveMemoryChecked());
    time::GetCurrentDate();
    Next();
}

// 0x00609DA4 (name is ours, "引越し NPC設定")
void BsSvMgr::SaveStep::Step_SetMovingNpc()
{
    netgame::NetGameMgr::s_pInstance->ResetSaveBlock();
    Next();
}

// 0x00609DC8 (name is ours, "ロードエラーチェック")
void BsSvMgr::SaveStep::Step_CheckLoadError()
{
    if (!m_Request.m_IsDone) {
        return;
    }
    if (m_Request.m_Result != SvStepInner::RESULT_OK) {
        m_pOwner->StartMessage(m_Request.m_Detail);
        m_Func = &SaveStep::Step_WaitMessageEndAndUnlock;
        return;
    }
    Next();
}

// 0x00609E18 (name is ours, "切断待ち")
void BsSvMgr::SaveStep::Step_WaitDisconnect()
{
    if (netgame::IsLeavingPlayerGone()) {
        if (netgame::GetNetgameType() == netgame::TYPE_RANDOM_MATCH) {
            Func283100(netgame::GetLeavingPlayer());
        }
        Next();
    }
}

// 0x00609E4C (name is ours, "ベストフレンドロード")
void BsSvMgr::SaveStep::Step_LoadBestFriends()
{
    if (BsPartsSaveMgr<SvBestFriendList>::s_Request.m_Result != SvStepInner::RESULT_RUNNING) {
        BsPartsSaveMgr<SvBestFriendList>::s_HasData = false;
        BsPartsSaveMgr<SvBestFriendList>::s_Request.Reset();
        BsPartsSaveMgr<SvBestFriendList>::s_Request.m_Flags = BsPartsSaveMgr<SvBestFriendList>::LOAD_FLAGS;
        if (BsPartsSaveMgr<SvBestFriendList>::s_pData == NULL) {
            BsPartsSaveMgr<SvBestFriendList>::s_pData = new (HeapInit::s_pStageHeap, BsPartsSaveMgr<SvBestFriendList>::ALIGNMENT) SvBestFriendList();
            BsPartsSaveMgr<SvBestFriendList>::s_SaveNo = SvGetPlayerSaveNo();
            if (!SvIsValidPlayerSaveNo(static_cast<PlayerSaveNumber>(BsPartsSaveMgr<SvBestFriendList>::s_SaveNo))) {
                BsPartsSaveMgr<SvBestFriendList>::s_SaveNo = 0;
            }
        }
    }
    Next();
}

// 0x00609F00 (name is ours, "セーブ失敗")
void BsSvMgr::SaveStep::Step_SaveFailedEnd()
{
    SvReturnToLoad();
    Next();
}

// 0x00609F18 (name is ours, "セーブメッセージ要求")
void BsSvMgr::SaveStep::Step_RequestSaveMessage()
{
    audio::bgm::GetTrackMixer()->AddMix(36);
    m_pOwner->StartMessage(4);
    Next();
}

// 0x00609F44 (name is ours, "削除メッセージ待ち")
void BsSvMgr::SaveStep::Step_WaitDeleteMessage()
{
    const s32 choice = m_pOwner->m_ChoiceCursor;
    if (choice == -1) {
        return;
    }
    if (choice == 0) {
        Next();
    } else {
        m_Func = &SaveStep::Step_ForceMessage;
    }
}

// 0x00609F88 (name is ours, "引越しチェック")
void BsSvMgr::SaveStep::Step_CheckMoving()
{
    SvCheckMovePlayerFiles();
    Next();
}

// 0x00609FA0 (name is ours, "専用処理")
void BsSvMgr::SaveStep::Step_Special()
{
    audio::bgm::s_TownState.isShopMixOff = 1;
    Next();
}

// 0x00609FBC (name is ours, "コミット終了待ち / コミット終了")
void BsSvMgr::SaveStep::Step_WaitCommitEnd()
{
    EndSave();
}

// 0x00609FC8 (name is ours, "ロード終了")
void BsSvMgr::SaveStep::Step_LoadEnd()
{
    if (m_Request.m_Result != SvStepInner::RESULT_OK) {
        m_pOwner->StartMessage(m_Request.m_Detail);
        m_Func = &SaveStep::Step_WaitMessageEndAndUnlock;
        return;
    }
    Next();
}

// 0x0060A00C (name is ours, "通信エラー前初期化処理")
void BsSvMgr::SaveStep::Step_InitializeBeforeNetError()
{
    const PlayerSaveNumber saveNo = SvGetPlayerSaveNo();
    Func10C760();
    netgame::SetPlayerActive(0, saveNo, NULL);
    Next();
}

// 0x0060A040 (name is ours, "最小待ち")
void BsSvMgr::SaveStep::Step_MinimumWait()
{
    if (s_Unknown9516E4 == 0) {
        Next();
    }
}

// 0x0060A060 (name is ours, "削除確認選択肢")
void BsSvMgr::SaveStep::Step_DeleteConfirmChoice()
{
    if (GetTalk()->IsPageWait()) {
        m_pOwner->StartDeleteChoice();
        Next();
    }
}

// 0x0060A094 (name is ours, "強制村移行")
void BsSvMgr::SaveStep::Step_ForceToTown()
{
    nn::math::VEC3_ position;
    if (s_Unknown9516D0) {
        if (!netgame::GetStructurePosition(&position, STRUCTURE_ISLAND_STORAGE_BIN)) {
            goto fallback;
        }
        position.x += FIELD_UNIT_SIZE_X;
        position.z += FIELD_UNIT_SIZE_Z * 5.0f;
    } else {
        if (!netgame::GetStructurePosition(&position, STRUCTURE_STATION_CLASSIC) &&
            !netgame::GetStructurePosition(&position, STRUCTURE_STATION_ZEN) &&
            !netgame::GetStructurePosition(&position, STRUCTURE_STATION_FAIRY_TALE) &&
            !netgame::GetStructurePosition(&position, STRUCTURE_STATION_MODERN)) {
            goto fallback;
        }
        position.z += FIELD_UNIT_SIZE_Z * 1.5f;
    }
    stage::GetTransitionMgr()->TransitionTo(stage::NAME_VILLAGE, &position, &ANGLE16_VEC3_ZERO, PLAYER_ACTION_IDLE, 0, 0, 1, 1);
    goto end;
fallback:
    stage::Transition(stage::NAME_VILLAGE, 0, 0, 1, 1);
end:
    s_Unknown9516D0 = 0;
    Next();
}

// 0x0060A1E8 (name is ours, "フォーマット用メッセージ要求")
void BsSvMgr::SaveStep::Step_RequestFormatMessage()
{
    audio::bgm::GetTrackMixer()->AddMix(36);
    m_pOwner->StartMessage(15);
    Next();
}

// 0x0060A214 (name is ours, "村作成待機")
void BsSvMgr::SaveStep::Step_WaitCreateTown()
{
    if (s_ApplyRequest.m_IsDone) {
        Next();
    }
}

// 0x0060A234 (name is ours, "通信エラー後初期化処理")
void BsSvMgr::SaveStep::Step_InitializeAfterNetError()
{
    Func2932C4();
    Next();
}

// 0x0060A24C (name is ours, "自分の村に帰ってきた")
void BsSvMgr::SaveStep::Step_BackInOwnTown()
{
    Func6ABAE4();
    Func17D478();
    Func293988();
    GetGardenData()->townData.townFlags.AddCounter10<76>();
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerFlags.unk_0xC_6 = 1;
        Func6F0C80(12, 1);
        Func24BD74(7, 1);
    }
    VisitorLocalInfo::s_pInstance->Func2C2B28();
    Next();
}

// 0x0060A308 (name is ours, "削除再確認メッセージ待ち")
void BsSvMgr::SaveStep::Step_WaitDeleteReconfirmMessage()
{
    const s32 choice = m_pOwner->m_ChoiceCursor;
    if (choice == -1) {
        return;
    }
    if (choice == 1) {
        Next();
    } else {
        m_Func = &SaveStep::Step_ForceMessage;
    }
}

// 0x0060A34C (name is ours, "入場デモ終了通知待ち")
void BsSvMgr::SaveStep::Step_WaitEnterDemoEndNotice()
{
    if (netgame::GetMgr8Flag()) {
        netgame::ResetMgr8();
        netgame::ClearLockSource();
        netgame::SetLockUnlocked();
        Next();
    }
}

// 0x0060A378 (name is ours, "街へ")
void BsSvMgr::SaveStep::Step_ToCity()
{
    stage::GetTransitionMgr()->TransitionByEntrance(1, 0, 0, 1, 1);
    Next();
}

// 0x0060A3BC (name is ours, "ゲーム開始処理終了待ち")
void BsSvMgr::SaveStep::Step_WaitGameStart()
{
    EndSave();
}

// 0x0060A3C8 (name is ours, "ゲーム開始処理終了後処理")
void BsSvMgr::SaveStep::Step_AfterGameStart()
{
    Func1266A0();
    Func30C948();
    Func27E778(0, 0);
    Func114CA8(false);
    s_Unknown9516D4 = 0;
    Next();
}

// 0x0060A408 (name is ours, "通信セーブメッセージ要求 / セーブメッセージ要求")
void BsSvMgr::SaveStep::Step_RequestNetSaveMessage()
{
    audio::bgm::GetTrackMixer()->AddMix(36);
    m_pOwner->StartMessage(4);
    Next();
}

// 0x0060A434 (name is ours, "公開中の親機情報を更新")
void BsSvMgr::SaveStep::Step_UpdatePublicHostInfo()
{
    if (!net::Net::Get()->IsError() || net::Net::Get()->GetErrorLevel() < 2) {
        if (net::Net::Get()->SetSessionData(netgame::s_SessionData.text, netgame::s_SessionData.length)) {
            Next();
        }
    }
}

// 0x0060A484 (name is ours, "この村にいる人のリストを更新")
void BsSvMgr::SaveStep::Step_UpdateVisitorList()
{
    SvPersonalId ids[4];
    u32 count = 0;
    for (u32 i = 0; i < 4; i++) {
        const netgame::PlayerNo player = static_cast<netgame::PlayerNo>(i);
        if (!netgame::ExistsPlayer(player)) {
            continue;
        }
        if (netgame::GetLeavingPlayer() == player) {
            continue;
        }
        SvPlayer* data = netgame::GetSvPlayer(player);
        if (data == NULL) {
            continue;
        }
        netgame::AssignPersonalId(&ids[count], &data->playerInfo);
        count++;
    }
    for (; count < 4; count++) {
        netgame::ClearPersonalId(&ids[count]);
    }
    netgame::BuildSessionData(ids);
    Next();
}

// 0x0060A558 (name is ours, "村作成")
void BsSvMgr::SaveStep::Step_CreateTownAfterError()
{
    s_ApplyRequest.Reset();
    s_ApplyRequest.m_Flags = SvStepInner::FLAG_KEEP_ERROR | SvStepInner::FLAG_CLEAR_SAVE_MEMORY | (6 << 6);
    g_Unknown95D35D = 0;
    if (s_ApplyRequest.Start()) {
        Next();
    }
}

// 0x0060A5A4 (name is ours, "削除再確認選択肢")
void BsSvMgr::SaveStep::Step_DeleteReconfirmChoice()
{
    if (GetTalk()->IsPageWait()) {
        m_pOwner->StartDeleteChoice();
        Next();
    }
}

// 0x0060A5D8 (name is ours, "展示場データクラスの初期化")
void BsSvMgr::SaveStep::Step_InitializeExhibition()
{
    Func2E1DE4(g_pUnknown953630);
    Next();
}

// 0x0060A5FC (name is ours, "展示場セーブ前処理")
void BsSvMgr::SaveStep::Step_BeforeExhibitionSave()
{
    g_SvExhibitionHolder.PrepareSave(BsSvMgr::AfterLoadExhibition, BsSvMgr::BeforeCommitExhibition, Func5D0F3C);
    g_SvExhibitionHolder.SetNoCommit();
    Next();
}

// 0x0060A63C (name is ours, "ベストフレンドエラーチェック")
void BsSvMgr::SaveStep::Step_CheckBestFriendError()
{
    s32 result = BsPartsSaveMgr<SvBestFriendList>::s_Request.m_Result;
    if (BsPartsSaveMgr<SvBestFriendList>::s_HasData) {
        if (result == 8) {
            goto next;
        }
    } else {
        if (result == 0) {
            BsPartsSaveMgr<SvBestFriendList>::s_Request.Start();
            return;
        }
        if (!BsPartsSaveMgr<SvBestFriendList>::s_Request.m_IsDone) {
            return;
        }
        // no file: a new one
        if (result != SvStepInner::RESULT_OK) {
            BsPartsSaveMgr<SvBestFriendList>::s_pData->SetUpNew();
            BsPartsSaveMgr<SvBestFriendList>::s_pData->checksum = BsPartsSaveMgr<SvBestFriendList>::s_pData->CalcChecksum();
        }
        BsPartsSaveMgr<SvBestFriendList>::s_HasData = true;
        if (result == 8) {
            g_SvErrorCode = -1;
            result = SvStepInner::RESULT_OK;
        }
        BsPartsSaveMgr<SvBestFriendList>::s_Detail = BsPartsSaveMgr<SvBestFriendList>::s_Request.m_Detail;
    }
    if (result != SvStepInner::RESULT_OK) {
        m_pOwner->StartMessage(BsPartsSaveMgr<SvBestFriendList>::s_Detail);
        m_Func = &SaveStep::Step_WaitMessageEndAndUnlock;
        return;
    }
next:
    Next();
}

// 0x0060A72C (name is ours, "セーブ完了")
void BsSvMgr::SaveStep::Step_SaveDone()
{
    netgame::NetGameMgr::s_pInstance->m_LeaveMgr.unk_0x253F = true;
    Next();
}

// 0x0060A754 (name is ours, "エントランスへ")
void BsSvMgr::SaveStep::Step_ToEntrance()
{
    stage::Transition(stage::NAME_EXHIBITION_ENTRANCE, 0, 0, 1, 1);
    Next();
}

// 0x0060A784 (name is ours, "展示場データのロード")
void BsSvMgr::SaveStep::Step_LoadExhibition()
{
    s32 result;
    if (!g_SvExhibitionHolder.UpdateLoad(&result)) {
        return;
    }
    if (result != SvStepInner::RESULT_OK) {
        script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
        talk->HideWaitIcon(false);
        talk->Jump(g_SvExhibitionHolder.m_Detail, NULL, NULL, 3);
        talk->SetJumped();
        m_Func = &SaveStep::Step_WaitMessageEndAndUnlock;
        return;
    }
    Next();
}

// 0x0060A824 (name is ours, "展示場データのセーブ")
void BsSvMgr::SaveStep::Step_SaveExhibition()
{
    s32 result;
    if (!g_SvExhibitionHolder.UpdateSave(&result)) {
        return;
    }
    if (result != SvStepInner::RESULT_OK) {
        script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
        talk->HideWaitIcon(false);
        talk->Jump(g_SvExhibitionHolder.m_Detail, NULL, NULL, 3);
        talk->SetJumped();
        m_Func = &SaveStep::Step_WaitMessageEndAndUnlock;
        return;
    }
    Next();
}

// 0x0060A8C4 (name is ours, "シーン切り替え")
void BsSvMgr::SaveStep::Step_ChangeScene()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerFlags.unk_0x2B_7 = 1;
    }
    stage::Transition(stage::NAME_GARDEN_PLUS_UPDATE, 1, 0, 1, 1);
    Next();
}

// 0x0060A914 (name is ours, "コミット待ち")
void BsSvMgr::SaveStep::Step_WaitCommitNet()
{
    if (netgame::IsLeaveSaved()) {
        Next();
    }
}

// 0x0060A930 (name is ours, "全削除終了待機")
void BsSvMgr::SaveStep::Step_WaitDeleteAll()
{
    if (m_Request.m_IsDone) {
        Func1266A0();
        g_SvErrorCode = -1;
        g_Unknown9516D1 = 0;
        Next();
    }
}

// 0x0060A96C (name is ours, "遊びに来てもらった回数をカウント")
void BsSvMgr::SaveStep::Step_CountVisits()
{
    GetGardenData()->townData.townFlags.AddCounter10<59>();
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerFlags.unk_0xC_7 = 1;
        Func24BD74(8, 1);
    }
    Func293814();
    Next();
}

// 0x0060AA08 (name is ours, "フォーマット用メッセージ要求")
void BsSvMgr::SaveStep::Step_RequestFormatMessageFirst()
{
    audio::bgm::GetTrackMixer()->AddMix(36);
    m_pOwner->StartMessage(9);
    Next();
}

// 0x0060AA34 (name is ours, "全体セーブ開始 / 全体セーブ / 全体セーブ開始(コミットなし)")
void BsSvMgr::SaveStep::Step_SaveAll()
{
    m_Request.Reset();
    const u64 deleteAll = s_Unknown9516D3 ? SvStepInner::FLAG_DELETE_ALL : 0;
    s_Unknown9516D3 = 0;
    m_Request.m_Flags = deleteAll | SvStepInner::FLAG_SAVE_FRIENDS | SvStepInner::FLAG_NO_COMMIT | (1 << 11);
    if (m_Request.Start()) {
        Next();
    }
}

// 0x0060AA88 (name is ours, "ロック受付キャンセルクリア")
void BsSvMgr::SaveStep::Step_ClearLockCancel()
{
    netgame::SetLockUnlocked();
    Next();
}

// 0x0060AAA0 (name is ours, "セーブ完了通知")
void BsSvMgr::SaveStep::Step_NotifySaveDone()
{
    if (netgame::ConfirmLeaving()) {
        Next();
    }
}

// 0x0060AABC (name is ours, "「セーブ終了」をネットホストに送信")
void BsSvMgr::SaveStep::Step_SendSaveEndToHost()
{
    netgame::EnqueuePacket(36, netgame::PLAYER_NO_0);
    Next();
}

// 0x0060AAE4 (name is ours, "セーブ許可待ち")
void BsSvMgr::SaveStep::Step_WaitSavePermission()
{
    if (netgame::IsPastLeavingPoint()) {
        Next();
    }
}

// 0x0060AB00 (name is ours, "セーブの許可待ち")
void BsSvMgr::SaveStep::Step_WaitSavePermissionRandomMatch()
{
    if (netgame::IsJoinFlag15Set()) {
        Next();
    }
}

// 0x0060AB1C (name is ours, "プレイヤーの家から出現")
void BsSvMgr::SaveStep::Step_AppearFromPlayerHouse()
{
    const u32 entrance = SvGetPlayerSaveNo() + 2;
    stage::GetTransitionMgr()->TransitionByEntrance(entrance, 1, 0, 1, 1);
    Next();
}

// 0x0060AB64 (name is ours, "人数変化設定")
void BsSvMgr::SaveStep::Step_SetPlayerCountChange()
{
    netgame::NetGameMgr::s_pInstance->m_NewPlayerJoined = true;
    Next();
}

// 0x0060AB88 (name is ours, "全マシンのセーブ終了待ち")
void BsSvMgr::SaveStep::Step_WaitAllMachinesSaved()
{
    if (netgame::IsSaveConfirmed()) {
        netgame::EnqueuePacket(37, netgame::PLAYER_NO_TARGET_ALL);
        netgame::ResetSaveDemo();
        netgame::ClearLockSource();
        netgame::SetLockUnlocked();
        Next();
    }
}

// 0x0060ABC8 (name is ours, "セーブデモの終了命令がきたら")
void BsSvMgr::SaveStep::Step_WaitSaveDemoEndOrder()
{
    if (netgame::IsMySaveConfirmed()) {
        netgame::ResetSaveDemo();
        netgame::ClearLockSource();
        netgame::SetLockUnlocked();
        Next();
    }
}

// 0x0060ABF4 (name is ours, "コミット")
void BsSvMgr::SaveStep::Step_CommitNet()
{
    m_Request.Reset();
    m_Request.m_AfterLoad = NULL;
    m_Request.m_BeforeCommit = NULL;
    m_Request.m_OnEnd = Func5D0334;
    m_Request.m_Flags = SvStepInner::FLAG_COMMIT;
    if (m_Request.Start()) {
        Next();
    }
}

// 0x0060AC44 (name is ours, "次のステージへ")
void BsSvMgr::SaveStep::Step_ToNextStage()
{
    netgame::NetGameMgr::s_pInstance->m_IslandMgr.StartTourTransition();
    Next();
}

// 0x0060AC70 (name is ours, "ネットビジターセーブ初期化(コミットなし) / ネットビジターセーブ初期化")
void BsSvMgr::SaveStep::Step_InitializeNetVisitorSave()
{
    m_Request.Reset();
    m_Request.m_Flags = SvStepInner::FLAG_SAVE_FRIENDS | SvStepInner::FLAG_NO_COMMIT | (3 << 11);
    if (m_Request.Start()) {
        Next();
    }
}

// 0x0060ACAC (name is ours, "ロード開始")
void BsSvMgr::SaveStep::Step_StartLoadExhibition()
{
    m_Request.Reset();
    m_Request.m_Flags = SvStepInner::FLAG_SAVE_FIRST | SvStepInner::FLAG_CHECK | (17 << 11) | SaveProcess::LOAD_FILE_GARDEN_PLUS;
    if (m_Request.Start()) {
        Next();
    }
}

// 0x0060ACE4 (name is ours, "展示場セーブ前処理")
void BsSvMgr::SaveStep::Step_BeforeExhibitionSaveAll()
{
    if (g_pSvExhibition != NULL) {
        g_SvExhibitionHolder.PrepareSave(NULL, NULL, NULL);
        g_SvExhibitionHolder.SetNoCommit();
        Next();
        return;
    }
    m_Func = &SaveStep::Step_SaveAll;
}

// 0x0060AD44 (name is ours, "一斉コミット送信")
void BsSvMgr::SaveStep::Step_SendCommitAll()
{
    if (netgame::StartLeaveExchange()) {
        Next();
    }
}

// 0x0060AD60 (name is ours, "セーブ完了通知")
void BsSvMgr::SaveStep::Step_NotifySaveDoneVisitor()
{
    const u8 count = netgame::NetGameMgr::s_pInstance->GetPlayerCount();
    if (count == netgame::NetGameMgr::s_pInstance->m_StageMgr.m_Stages.GetPlayerCount(stage::NAME_PREPARE_TOUR) &&
        netgame::NetGameMgr::s_pInstance->m_StageMgr.GetTransitioningPlayer() >= netgame::PLAYER_NO_MAX) {
        netgame::NetGameMgr::s_pInstance->m_IslandMgr.ConfirmTourGuest();
        Next();
    }
}

// 0x0060ADC8 (name is ours, "コミット待ち")
void BsSvMgr::SaveStep::Step_WaitCommitAll()
{
    if (netgame::IsSaveDemoDone()) {
        Next();
    }
}

// 0x0060ADE4 (name is ours, "一斉セーブ時にセーブできる状態か")
void BsSvMgr::SaveStep::Step_CanSaveTogether()
{
    if (netgame::AreAllPlayersInSave()) {
        Next();
    }
}

// 0x0060AE00 (name is ours, "遊びに来てもらった回数をカウント")
void BsSvMgr::SaveStep::Step_CountVisitsHost()
{
    GetGardenData()->townData.townFlags.AddCounter10<59>();
    Next();
}

// 0x0060AE68 (name is ours, "退場中解除設定")
void BsSvMgr::SaveStep::Step_ClearLeaving()
{
    netgame::NetGameMgr::s_pInstance->unk_0x132B7 = true;
    Next();
}

// 0x0060AE8C (name is ours, "入場デモ終了通知")
void BsSvMgr::SaveStep::Step_NotifyEnterDemoEnd()
{
    netgame::SetJoinFinished();
    Next();
}

// 0x0060AEA4 (name is ours, "セーブ完了通知")
void BsSvMgr::SaveStep::Step_NotifySaveDoneHost()
{
    netgame::ConfirmSave();
    Next();
}

// 0x0060AEBC (name is ours, "全員が退場デモを終了可能になるまで待つ")
void BsSvMgr::SaveStep::Step_WaitAllCanEndLeaveDemo()
{
    if (netgame::IsLeaveConfirmed()) {
        netgame::SendSessionOpen();
        Next();
    }
}

// 0x0060AEE0 (name is ours, "切断待ち")
void BsSvMgr::SaveStep::Step_WaitDisconnectNet()
{
    if (netgame::NetGameMgr::s_pInstance->m_LeaveMgr.m_State > 13) {
        Next();
    }
}

// 0x0060AF04 (name is ours, "迎えデモ移行許可待ち")
void BsSvMgr::SaveStep::Step_WaitWelcomeDemoPermission()
{
    netgame::NetGameMgr* mgr = netgame::NetGameMgr::s_pInstance;
    if (mgr->m_JoinMgr.unk_0x13) {
        mgr->m_JoinMgr.unk_0x13 = 0;
        Next();
    }
}

// 0x0060AF30 (name is ours, "新規参入プレイヤ受信完了待ち")
void BsSvMgr::SaveStep::Step_WaitNewPlayerReceived()
{
    if (netgame::IsJoining()) {
        netgame::OnPlayerEntered();
        Next();
    }
}

// 0x0060AF54 (name is ours, "再接続待ち")
void BsSvMgr::SaveStep::Step_WaitReconnect()
{
    netgame::NetGameMgr* mgr = netgame::NetGameMgr::s_pInstance;
    if (mgr->m_IslandMgr.unk_0x13E) {
        mgr->m_IslandMgr.unk_0x13E = false;
        Next();
    }
}

// 0x0060AF90 (name is ours, "コミット待ち")
void BsSvMgr::SaveStep::Step_WaitCommitVisitor()
{
    if (netgame::NetGameMgr::s_pInstance->m_IslandMgr.unk_0x141) {
        Next();
    }
}

// 0x0060AFB8 (name is ours, "中断デモ終了")
void BsSvMgr::SaveStep::Step_EndInterruptDemo()
{
    netgame::NetGameMgr::s_pInstance->m_SaveDemoMgr.m_CanDisconnect = 1;
    Next();
}

// 0x0060AFD8 (name is ours, "セーブ前準備処理")
void BsSvMgr::SaveStep::Step_PrepareSave()
{
    Func17D478();
    g_Unknown951718.unk_8 = 1;
    if (g_Unknown951718.mode == 2 && g_Unknown951718.flag5 == 0) {
        Func2932C4();
    }
    Next();
}

// 0x0060B028 (name is ours, "切断待ち")
void BsSvMgr::SaveStep::Step_WaitDisconnectVisitor()
{
    if (!netgame::IsMultiPlayer()) {
        Next();
    }
}

// 0x0060B044 (name is ours, "ツアー準備完了待ち")
void BsSvMgr::SaveStep::Step_WaitTourReady()
{
    netgame::NetGameMgr* mgr = netgame::NetGameMgr::s_pInstance;
    if (mgr->m_IslandMgr.unk_0x147) {
        mgr->m_IslandMgr.unk_0x147 = false;
        Next();
    }
}

// 0x0060B080 (name is ours, "コミット完了通知")
void BsSvMgr::SaveStep::Step_NotifyCommitDone()
{
    netgame::NetGameMgr::s_pInstance->m_IslandMgr.ConfirmTourGuest();
    Next();
}

// 0x0060B0AC (name is ours, "一斉セーブ時にセーブできる？")
void BsSvMgr::SaveStep::Step_CanSaveTogetherRandomMatch()
{
    if (netgame::IsSaveDemoWaiting()) {
        Next();
    }
}

// 0x0060B0C8 (name is ours, "コミット開始送信")
void BsSvMgr::SaveStep::Step_SendCommitStart()
{
    if (netgame::IsSaveConfirmed()) {
        netgame::NetGameMgr::s_pInstance->m_SaveDemoMgr.m_Confirm.Update(netgame::PLAYER_NO_0, true);
        netgame::EnqueuePacket(50, netgame::PLAYER_NO_TARGET_ALL);
        netgame::NetGameMgr::s_pInstance->m_SaveDemoMgr.m_IsDone = 1;
        Next();
    }
}

// 0x0060B124 (name is ours, "新規参入制限解除")
void BsSvMgr::SaveStep::Step_LiftNewPlayerLimit()
{
    if (netgame::IsHost()) {
        netgame::NetGameMgr* mgr = netgame::NetGameMgr::s_pInstance;
        if (!mgr->m_IslandMgr.m_IsRejoining && mgr->m_GateMgr.Open(netgame::GateMgr::FLAG_RANDOM_MATCH_PLAYER_JOINING)) {
            // (no station is joining any more)
            netgame::NetGameMgr::s_pInstance->m_AvatarMgr.m_JoiningStation = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
        }
    }
    Next();
}

// 0x0060B190 (name is ours, "セーブできる状態か")
void BsSvMgr::SaveStep::Step_CanSave()
{
    netgame::StageMgr& stageMgr = netgame::NetGameMgr::s_pInstance->m_StageMgr;
    if (stageMgr.AreAllPlayersIn(stage::NAME_PREPARE_TOUR) && stageMgr.GetTransitioningPlayer() >= netgame::PLAYER_NO_MAX) {
        Next();
    }
}

// 0x0060B1D4 (name is ours, "ネットホストセーブ初期化 / ネットビジターセーブ初期化")
void BsSvMgr::SaveStep::Step_InitializeNetSaveTour()
{
    if (m_Counter == 0) {
        if (netgame::GetNetgameType() != netgame::TYPE_RANDOM_MATCH ||
            netgame::NetGameMgr::s_pInstance->m_IslandMgr.IsDepartingInRandomMatch()) {
            SvPlayer* player = SvGetCurrentPlayer();
            if (player != NULL) {
                const u32 first = player->SellPocketsToBank();
                const u32 second = player->bankAmount.Get();
                GetCensus()->UpdateAbdBalance(second);
                GetCensus()->AddBellsEarned(first);
            }
            netgame::Func5C6688();
        } else {
            SvPlayer* player = SvGetCurrentPlayer();
            if (player != NULL) {
                SvPlayerOutfit* outfit = &player->playerAppearance.playerOutfit;
                if (Func769644(Func71B3B8(outfit))) {
                    Func27B1EC(outfit);
                    s_Unknown9516CF = 1;
                }
            }
        }
        UpdatePlayersForSave();
    }
    m_Request.Reset();
    m_Request.m_Flags = SvStepInner::FLAG_SAVE_FRIENDS | SvStepInner::FLAG_NO_COMMIT | (3 << 11);
    if (m_Request.Start()) {
        Next();
    }
}

// 0x0060B33C (name is ours, "新規参入制限解除")
void BsSvMgr::SaveStep::Step_LiftNewPlayerLimitRandomMatch()
{
    if (netgame::IsHost()) {
        netgame::NetGameMgr* mgr = netgame::NetGameMgr::s_pInstance;
        if (!mgr->m_IslandMgr.m_IsRejoining && netgame::g_TourStatus.unk_0x4 == 0) {
            mgr->m_GateMgr.Open(netgame::GateMgr::FLAG_RANDOM_MATCH_PLAYER_LEAVING);
        }
    }
    Next();
}

// 0x0060B3A0 (name is ours, "コミット待ち")
void BsSvMgr::SaveStep::Step_WaitCommitHost()
{
    if (netgame::IsSaveDemoDone()) {
        const netgame::PlayerNo myNo = netgame::NetGameMgr::s_pInstance->GetMyPlayerNo();
        netgame::NetGameMgr::s_pInstance->m_SaveDemoMgr.m_Confirm.Update(myNo, false);
        Next();
    }
}

// 0x0060B3E4 (name is ours, "ネットホストセーブ初期化 / ネットビジターセーブ初期化 / ネットビジターセーブ初期化(コミットなし)")
void BsSvMgr::SaveStep::Step_InitializeNetSaveRandomMatch()
{
    if (m_Counter == 0) {
        SvPlayer* player = SvGetCurrentPlayer();
        if (player != NULL) {
            SvPlayerOutfit* outfit = &player->playerAppearance.playerOutfit;
            if (Func769644(Func71B3B8(outfit))) {
                Func27B1EC(outfit);
                s_Unknown9516CF = 1;
            }
        }
        UpdatePlayersForSave();
    }
    m_Request.Reset();
    m_Request.m_Flags = SvStepInner::FLAG_SAVE_FRIENDS | SvStepInner::FLAG_NO_COMMIT | (3 << 11);
    if (m_Request.Start()) {
        Next();
    }
}

// 0x0060B4A0 (name is ours, "プレーヤー状態回復")
void BsSvMgr::SaveStep::Step_RestorePlayerState()
{
    netgame::Func5C4D04(&SvFgName::s_Empty);
    netgame::Func5C7050();
    Func5C4BA4();
    Func5C6F6C();
    if (s_Unknown9516CF) {
        SvPlayer* player = SvGetCurrentPlayer();
        if (player != NULL) {
            SvFgName item(static_cast<SvFgName::ID>(SvFgName::NAME_RENTAL_WET_SUIT));
            Func27B1F4(&player->playerAppearance.playerOutfit, &item);
        }
        s_Unknown9516CF = 0;
    }
    Next();
}

// 0x0060B518 (name is ours, "全体セーブ開始")
void BsSvMgr::SaveStep::Step_StartSaveAllNet()
{
    if (m_Counter == 0) {
        SvPlayer* player = SvGetCurrentPlayer();
        if (player != NULL) {
            const u32 first = player->SellPocketsToBank();
            const u32 second = player->bankAmount.Get();
            GetCensus()->UpdateAbdBalance(second);
            GetCensus()->AddBellsEarned(first);
        }
        netgame::Func5C6688();
        UpdatePlayersForSave();
    }
    m_Request.Reset();
    m_Request.m_Flags = SvStepInner::FLAG_SAVE_FRIENDS | SvStepInner::FLAG_NO_COMMIT | (1 << 11);
    if (m_Request.Start()) {
        Next();
    }
}

// 0x0060B5E0 (name is ours, "終了")
void BsSvMgr::SaveStep::Step_EndAll()
{
    m_pOwner->ChangeInline(&BsSvMgr::State_None);
}

// the step machine and the common steps of SvStep for SaveStep (ARMCC put them in the template area)
// 0x0081D1A0 (name is ours)
template void state::Step<BsSvMgr, BsSvMgr::SaveStep>::Next();
// 0x0081D230 (name is ours)
template void state::Step<BsSvMgr, BsSvMgr::SaveStep>::Exec();
// 0x0082D998 (name after state::Base)
template const char* state::Step<BsSvMgr, BsSvMgr::SaveStep>::GetName(Func func) const;
// 0x0081D2BC
// 0x0081D2B8 (deleting dtor)
template state::Step<BsSvMgr, BsSvMgr::SaveStep>::~Step();
// 0x0081D2C0 (name is ours, "フォーマット")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_Format();
// 0x0081D2F4 (name is ours)
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_Next();
// 0x0081D300 (name is ours, "終了待ち")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_WaitDone();
// 0x0081D318 (name is ours, "ロード完了？")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_EndLoad();
// 0x0081D328 (name is ours, "全体セーブ終了")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_EndSave();
// 0x0081D334 (name is ours, "ロード開始")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_StartLoad();
// 0x0081D36C (name is ours, "ネットホスト/ビジターセーブ初期化")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_InitializeNetSave();
// 0x0081D388 (name is ours, "セーブ失敗")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_SaveFailed();
// 0x0081D38C (name is ours, "ロード完了？")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_EndLoadLast();
// 0x0081D39C (name is ours, "ネットホスト/ビジターセーブ終了")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_EndNetSave();
// 0x0081D3A8 (name is ours, "ネットホスト/ビジターセーブ開始")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_StartNetSave();
// 0x0081D3C8 (name is ours, "ベストフレンドデータの破棄")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_DiscardBestFriendData();
// 0x0081D3E4 (name is ours, "村＋その他データセーブ終了")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_EndSaveTown();
// 0x0081D3F0 (name is ours, "全体セーブ開始")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_StartSaveAll();
// 0x0081D440 (name is ours, "村＋その他データセーブ開始")
template void SvStep<BsSvMgr, BsSvMgr::SaveStep>::Step_StartSaveTown();
