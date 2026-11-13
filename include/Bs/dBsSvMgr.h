#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvStep.h"
#include "netgame/netgame_Api.h"

// The process of the saves and loads of the game: its modes (state::Mode, table s_ModeInfos with Japanese debug names)
// each run a sequence of steps (SaveStep). Another code asks for a mode with the static setters Set*, the mode "Wait"
// changes to it. Class name and the libgarden setter names from the binary/references; the other names are ours (the
// modes after the debug names "BsSvMgr::stepXxx" of their step tables and the Japanese mode names).
// RTTI 7BsSvMgr @ 0x008CD354
// vtable 0x008F8EB8 (vptr 0x008F8EC0), offset_to_top 0, 18 entries
// vtable 0x008F8F08 (vptr 0x008F8F10), offset_to_top -20, 63 entries
// vtable 0x008F900C (vptr 0x008F9014), offset_to_top -144, 3 entries
// the CPU time limit of the system core for the application while it is raised (game code, not decompiled yet; names are
// ours): Set raises it (nn::applet::SetApplicationCpuTimeLimit, only on the old 3DS), Reset sets the standard 30 % again
struct CpuTimeLimit
{
    CpuTimeLimit() : isSet(false), percent(30) {}
    ~CpuTimeLimit()
    {
        if (isSet) {
            Reset();
        }
    }
    void Set(s32 percent); // 0x002C25F0
    void Reset(); // 0x002C2624

    bool isSet;     // 0x0
    s16 percent;    // 0x2
};

// (game code, not decompiled yet; names after the addresses) whether the mode "Wait" may change the mode
bool Func566AD0(); // 0x00566AD0
// (called by the steps "reset initialization" and "end")
void Func10C760(); // 0x0010C760
struct SvGardenPlus;
void Func2510F0(SvGardenPlus* data); // 0x002510F0
// (called by the step "initialization of the game start")
void Func6066F4(s32 value); // 0x006066F4
void Func27EC2C(SvStepInner* request); // 0x0027EC2C
void Func118A7C(SvTime* data); // 0x00118A7C
void Func114CA8(bool value); // 0x00114CA8
// (called by the step "delete all save data")
void Func27EBF0(SvStepInner* request); // 0x0027EBF0
// (called by the steps of the loads and the disconnection; names after the addresses)
struct SvPlayer;
void Func6BD94C(SvPlayer* dst, const SvPlayer* src); // 0x006BD94C
void Func283100(netgame::PlayerNo player); // 0x00283100
// (called by the steps after the network error, back in the own town and after the game start)
void Func2932C4(); // 0x002932C4
void Func6ABAE4(); // 0x006ABAE4
void Func17D478(); // 0x0017D478
void Func293988(); // 0x00293988
void Func6F0C80(s32 a, s32 b); // 0x006F0C80
void Func24BD74(s32 a, s32 b); // 0x0024BD74
void Func1266A0(); // 0x001266A0
void Func30C948(); // 0x0030C948
void Func27E778(s32 a, s32 b); // 0x0027E778
void Func293814(); // 0x00293814
// (the callback at the end of the commit in a network game)
void Func5D0334(); // 0x005D0334
// (called by the steps of the saves of the network game; names after the addresses)
struct SvPlayerOutfit;
class SvFgName;
const SvFgName* Func71B3B8(SvPlayerOutfit* outfit); // 0x0071B3B8
void Func27B1EC(SvPlayerOutfit* outfit); // 0x0027B1EC
void Func27B1F4(SvPlayerOutfit* outfit, const SvFgName* item); // 0x0027B1F4
void Func5C344C(const SvFgName* item); // 0x005C344C
void Func5C73D0(); // 0x005C73D0
void Func5C4BA4(); // 0x005C4BA4
void Func5C6F6C(); // 0x005C6F6C
// (the exhibition steps: an object of the exhibition (pointer at 0x00953630) is set up; the callbacks of its save)
extern void* g_pUnknown953630;
void Func2E1DE4(void* object); // 0x002E1DE4
void Func5378CC(); // 0x005378CC
void Func5D0A80(); // 0x005D0A80
void Func537F90(); // 0x00537F90
void Func5D0EC0(); // 0x005D0EC0
void Func5D0F3C(); // 0x005D0F3C
// an object of the game (pointer at 0x0094F4C8): BsSvMgr sets the byte at +0xA10 before the growth processing
extern u8* g_pUnknown94F4C8;
// (a pointer of another file: Finalize calls Func1266A0 while it is set)
extern void* g_pUnknown94FD5C;
// the counter of the visits of other players is raised (town flags), the flag of the player set (name is ours)
void SvCountVisit(); // 0x006079F8

class BsSvMgr : public ::Base, public ::script::ITalkRecept, public ::state::Mode<BsSvMgr>
{
public:
    typedef ::state::Mode<BsSvMgr>::Info Info;
    // The step sequences of BsSvMgr: each mode of BsSvMgr sets a table of steps (the arrays s_Step* in dBsSvMgr.cpp, debug
    // names "BsSvMgr::stepXxx"). Class name from the RTTI; the step names are ours, after the Japanese debug names of the
    // tables (in the comments).
    // RTTI N7BsSvMgr8SaveStepE @ 0x008D3ED0
    // vtable 0x0090BC58 (vptr 0x0090BC60), offset_to_top 0, 5 entries
    class SaveStep : public ::SvStep<BsSvMgr, SaveStep>
    {
    public:
        SaveStep() {}
        // (inline) the talk of the owner
        script::TalkSystem* GetTalk() const { return m_pOwner->m_pOwner; }
        // (inline) the state of the players before a save of the network game (name is ours)
        DECOMP_ALWAYS_INLINE static void UpdatePlayersForSave();
        virtual ~SaveStep(); // 0x0060B8C4 slot 0x00
        // 0x0060B8AC slot 0x04 (deleting dtor)
        void EndSave() override; // 0x0060B7E4 slot 0x0C
        void EndLoad(bool isLast) override; // 0x0060B700 slot 0x10

        // "成長処理"
        void Step_Growup(); // 0x00609410
        // "リセット初期化"
        void Step_InitializeReset(); // 0x00609430
        // "終了"
        void Step_End(); // 0x00609448
        // "データ送信終了待ち"
        void Step_WaitDataSent(); // 0x006094AC
        // "メッセージ終了待ち"
        void Step_WaitMessageEnd(); // 0x006094C8
        // "開いているなら門を閉じる"
        void Step_CloseGateIfOpen(); // 0x00609514
        // "メッセージロックまで待つ"
        void Step_WaitMessageLock(); // 0x00609570
        // "ロック解除"
        void Step_Unlock(); // 0x006095B0
        // "WiFiコネクション切断"
        void Step_DisconnectWiFi(); // 0x006095EC
        // "コミット"
        void Step_Commit(); // 0x00609608
        // "デモ終了待ち"
        void Step_WaitDemoEnd(); // 0x0060963C
        // "ネットホストセーブ終了設定 / ネットビジターセーブ終了設定"
        void Step_SetNetSaveEnd(); // 0x00609660
        // "成長処理前処理"
        void Step_BeforeGrowup(); // 0x00609680
        // "ゲーム開始処理初期化"
        void Step_InitializeGameStart(); // 0x006096CC
        // "タイトルへ"
        void Step_ToTitle(); // 0x006097F0
        // "迎えデモへ"
        void Step_ToWelcomeDemo(); // 0x00609820
        // "村作成"
        void Step_CreateTown(); // 0x00609874
        // "初期化処理"
        void Step_Initialize(); // 0x00609900
        // "メッセージ終了待ち"
        void Step_WaitMessageEndAndUnlock(); // 0x0060990C
        // "街へ / 元居た場所へ / 元のステージへ / 元のシーンへ"
        void Step_ToPreviousScene(); // 0x00609958
        // "元のステージへ"
        void Step_ToPreviousStage(); // 0x006099DC
        // "デモ終了通知完了待ち"
        void Step_WaitDemoEndNotified(); // 0x00609A2C
        // "他の村への手紙処理開始"
        void Step_StartLettersToOtherTowns(); // 0x00609A58
        // "メッセージ強制送り"
        void Step_ForceMessage(); // 0x00609A64
        // "「心のさけび」ログアウト"
        void Step_Unknown609A9C(); // 0x00609A9C
        // "ゲーム開始処理初期化"
        void Step_InitializeGameStartNet(); // 0x00609B00
        // "駅の中へ"
        void Step_IntoStation(); // 0x00609B30
        // "帰村デモへ"
        void Step_ToReturnDemo(); // 0x00609BC8
        // "削除メッセージ"
        void Step_DeleteMessage(); // 0x00609BF8
        // "退場デモ終了"
        void Step_EndLeaveDemo(); // 0x00609C24
        // "セーブデータ全削除処理"
        void Step_DeleteAllSaveData(); // 0x00609C4C
        // "初期化処理"
        void Step_InitializeDelete(); // 0x00609C7C
        // "他の村への手紙処理終了"
        void Step_EndLettersToOtherTowns(); // 0x00609C98
        // "ロード処理完了"
        void Step_LoadDone(); // 0x00609CA4
        // "引越し NPC設定"
        void Step_SetMovingNpc(); // 0x00609DA4
        // "ロードエラーチェック"
        void Step_CheckLoadError(); // 0x00609DC8
        // "切断待ち"
        void Step_WaitDisconnect(); // 0x00609E18
        // "ベストフレンドロード"
        void Step_LoadBestFriends(); // 0x00609E4C
        // "セーブ失敗"
        void Step_SaveFailedEnd(); // 0x00609F00
        // "セーブメッセージ要求"
        void Step_RequestSaveMessage(); // 0x00609F18
        // "削除メッセージ待ち"
        void Step_WaitDeleteMessage(); // 0x00609F44
        // "引越しチェック"
        void Step_CheckMoving(); // 0x00609F88
        // "専用処理"
        void Step_Special(); // 0x00609FA0
        // "コミット終了待ち / コミット終了"
        void Step_WaitCommitEnd(); // 0x00609FBC
        // "ロード終了"
        void Step_LoadEnd(); // 0x00609FC8
        // "通信エラー前初期化処理"
        void Step_InitializeBeforeNetError(); // 0x0060A00C
        // "最小待ち"
        void Step_MinimumWait(); // 0x0060A040
        // "削除確認選択肢"
        void Step_DeleteConfirmChoice(); // 0x0060A060
        // "強制村移行"
        void Step_ForceToTown(); // 0x0060A094
        // "フォーマット用メッセージ要求"
        void Step_RequestFormatMessage(); // 0x0060A1E8
        // "村作成待機"
        void Step_WaitCreateTown(); // 0x0060A214
        // "通信エラー後初期化処理"
        void Step_InitializeAfterNetError(); // 0x0060A234
        // "自分の村に帰ってきた"
        void Step_BackInOwnTown(); // 0x0060A24C
        // "削除再確認メッセージ待ち"
        void Step_WaitDeleteReconfirmMessage(); // 0x0060A308
        // "入場デモ終了通知待ち"
        void Step_WaitEnterDemoEndNotice(); // 0x0060A34C
        // "街へ"
        void Step_ToCity(); // 0x0060A378
        // "ゲーム開始処理終了待ち"
        void Step_WaitGameStart(); // 0x0060A3BC
        // "ゲーム開始処理終了後処理"
        void Step_AfterGameStart(); // 0x0060A3C8
        // "通信セーブメッセージ要求 / セーブメッセージ要求"
        void Step_RequestNetSaveMessage(); // 0x0060A408
        // "公開中の親機情報を更新"
        void Step_UpdatePublicHostInfo(); // 0x0060A434
        // "この村にいる人のリストを更新"
        void Step_UpdateVisitorList(); // 0x0060A484
        // "村作成"
        void Step_CreateTownAfterError(); // 0x0060A558
        // "削除再確認選択肢"
        void Step_DeleteReconfirmChoice(); // 0x0060A5A4
        // "展示場データクラスの初期化"
        void Step_InitializeExhibition(); // 0x0060A5D8
        // "展示場セーブ前処理"
        void Step_BeforeExhibitionSave(); // 0x0060A5FC
        // "ベストフレンドエラーチェック"
        void Step_CheckBestFriendError(); // 0x0060A63C
        // "セーブ完了"
        void Step_SaveDone(); // 0x0060A72C
        // "エントランスへ"
        void Step_ToEntrance(); // 0x0060A754
        // "展示場データのロード"
        void Step_LoadExhibition(); // 0x0060A784
        // "展示場データのセーブ"
        void Step_SaveExhibition(); // 0x0060A824
        // "シーン切り替え"
        void Step_ChangeScene(); // 0x0060A8C4
        // "コミット待ち"
        void Step_WaitCommitNet(); // 0x0060A914
        // "全削除終了待機"
        void Step_WaitDeleteAll(); // 0x0060A930
        // "遊びに来てもらった回数をカウント"
        void Step_CountVisits(); // 0x0060A96C
        // "フォーマット用メッセージ要求"
        void Step_RequestFormatMessageFirst(); // 0x0060AA08
        // "全体セーブ開始 / 全体セーブ / 全体セーブ開始(コミットなし)"
        void Step_SaveAll(); // 0x0060AA34
        // "ロック受付キャンセルクリア"
        void Step_ClearLockCancel(); // 0x0060AA88
        // "セーブ完了通知"
        void Step_NotifySaveDone(); // 0x0060AAA0
        // "「セーブ終了」をネットホストに送信"
        void Step_SendSaveEndToHost(); // 0x0060AABC
        // "セーブ許可待ち"
        void Step_WaitSavePermission(); // 0x0060AAE4
        // "セーブの許可待ち"
        void Step_WaitSavePermissionRandomMatch(); // 0x0060AB00
        // "プレイヤーの家から出現"
        void Step_AppearFromPlayerHouse(); // 0x0060AB1C
        // "人数変化設定"
        void Step_SetPlayerCountChange(); // 0x0060AB64
        // "全マシンのセーブ終了待ち"
        void Step_WaitAllMachinesSaved(); // 0x0060AB88
        // "セーブデモの終了命令がきたら"
        void Step_WaitSaveDemoEndOrder(); // 0x0060ABC8
        // "コミット"
        void Step_CommitNet(); // 0x0060ABF4
        // "次のステージへ"
        void Step_ToNextStage(); // 0x0060AC44
        // "ネットビジターセーブ初期化(コミットなし) / ネットビジターセーブ初期化"
        void Step_InitializeNetVisitorSave(); // 0x0060AC70
        // "ロード開始"
        void Step_StartLoadExhibition(); // 0x0060ACAC
        // "展示場セーブ前処理"
        void Step_BeforeExhibitionSaveAll(); // 0x0060ACE4
        // "一斉コミット送信"
        void Step_SendCommitAll(); // 0x0060AD44
        // "セーブ完了通知"
        void Step_NotifySaveDoneVisitor(); // 0x0060AD60
        // "コミット待ち"
        void Step_WaitCommitAll(); // 0x0060ADC8
        // "一斉セーブ時にセーブできる状態か"
        void Step_CanSaveTogether(); // 0x0060ADE4
        // "遊びに来てもらった回数をカウント"
        void Step_CountVisitsHost(); // 0x0060AE00
        // "退場中解除設定"
        void Step_ClearLeaving(); // 0x0060AE68
        // "入場デモ終了通知"
        void Step_NotifyEnterDemoEnd(); // 0x0060AE8C
        // "セーブ完了通知"
        void Step_NotifySaveDoneHost(); // 0x0060AEA4
        // "全員が退場デモを終了可能になるまで待つ"
        void Step_WaitAllCanEndLeaveDemo(); // 0x0060AEBC
        // "切断待ち"
        void Step_WaitDisconnectNet(); // 0x0060AEE0
        // "迎えデモ移行許可待ち"
        void Step_WaitWelcomeDemoPermission(); // 0x0060AF04
        // "新規参入プレイヤ受信完了待ち"
        void Step_WaitNewPlayerReceived(); // 0x0060AF30
        // "再接続待ち"
        void Step_WaitReconnect(); // 0x0060AF54
        // "コミット待ち"
        void Step_WaitCommitVisitor(); // 0x0060AF90
        // "中断デモ終了"
        void Step_EndInterruptDemo(); // 0x0060AFB8
        // "セーブ前準備処理"
        void Step_PrepareSave(); // 0x0060AFD8
        // "切断待ち"
        void Step_WaitDisconnectVisitor(); // 0x0060B028
        // "ツアー準備完了待ち"
        void Step_WaitTourReady(); // 0x0060B044
        // "コミット完了通知"
        void Step_NotifyCommitDone(); // 0x0060B080
        // "一斉セーブ時にセーブできる？"
        void Step_CanSaveTogetherRandomMatch(); // 0x0060B0AC
        // "コミット開始送信"
        void Step_SendCommitStart(); // 0x0060B0C8
        // "新規参入制限解除"
        void Step_LiftNewPlayerLimit(); // 0x0060B124
        // "セーブできる状態か"
        void Step_CanSave(); // 0x0060B190
        // "ネットホストセーブ初期化 / ネットビジターセーブ初期化"
        void Step_InitializeNetSaveTour(); // 0x0060B1D4
        // "新規参入制限解除"
        void Step_LiftNewPlayerLimitRandomMatch(); // 0x0060B33C
        // "コミット待ち"
        void Step_WaitCommitHost(); // 0x0060B3A0
        // "ネットホストセーブ初期化 / ネットビジターセーブ初期化 / ネットビジターセーブ初期化(コミットなし)"
        void Step_InitializeNetSaveRandomMatch(); // 0x0060B3E4
        // "プレーヤー状態回復"
        void Step_RestorePlayerState(); // 0x0060B4A0
        // "全体セーブ開始"
        void Step_StartSaveAllNet(); // 0x0060B518
        // "終了"
        void Step_EndAll(); // 0x0060B5E0
    };

    // (inline in the factory Process::Create<BsSvMgr>)
    BsSvMgr()
        : unk_0x118(SvStepInner::FLAG_KEEP_ERROR), unk_0x11C(NULL), m_ChoiceCursor(-1), m_DeleteConfirmStep(0), unk_0x12C(0)
    {
    }
    virtual ~BsSvMgr(); // 0x0060B9DC slot 0x00
    // 0x0060B984 slot 0x04 (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x00609258 slot 0x0C
    virtual oml::framework::Result Finalize(); // 0x0060B8D8 slot 0x18
    virtual oml::framework::Result Calc(); // 0x00609368 slot 0x24
    // (ITalkRecept: ARMCC put the overrides into new slots of the primary vtable, the vtable of ITalkRecept points at
    // thunks with copies of the bodies) the page of the messages 4, 9 and 15 waits with the icon for 90 frames / the
    // choice of the deletion
    void OnPageEnd(u8 value) override; // 0x00607E38 slot 0x40
    void OnChoice(const script::ChoiceItem* item, s32 cursor) override; // 0x00607FE0 slot 0x44

    // the message with the number of the file s_MessageFile is shown (name is ours)
    void StartMessage(s32 number); // 0x006079A0
    // the choices of the deletion of the save data (name is ours)
    void StartDeleteChoice(); // 0x00607CF0
    // the callbacks of the save of the exhibition data (names are ours)
    static bool AfterLoadExhibition(); // 0x00607A80
    static bool BeforeCommitExhibition(); // 0x00607C60

    static void SetSaveAll(); // 0x00607EC4
    // with exhibition.dat in memory SetSaveAllAndExhibition, else SetSaveNormal (falls through into it)
    static void SetSaveNormalOrExhibition(); // 0x00608088
    static void SetSaveNormal(); // 0x006080A8
    static void SetErrorNetwork(); // 0x0060814C
    static void SetSaveAllAndReturn(); // 0x0060827C
    static void SetLoadAfterSaveError(); // 0x00608360
    static void SetInitGameInitFormat(); // 0x0060837C
    static void SetLoadAndSaveExhibition(); // 0x00608398
    static void SetSaveSeeOffNetHst(); // 0x006084CC
    static void SetSaveSeeOffNetVst(); // 0x006084E8
    static void SetDelete(); // 0x00608660
    static void SetSaveConnectNetHst(); // 0x0060867C
    static void SetSaveConnectNetVst(); // 0x00608698
    static void SetSaveContinueGrowup(); // 0x0060885C
    static void SetSaveContinueNetHst(); // 0x00608878
    static void SetSaveContinueNetVst(); // 0x00608894
    static void SetSaveContinuePlaying(); // 0x00608A3C
    static void SetSaveInterruptNetHst(); // 0x00608A58
    static void SetSaveAllAndExhibition(); // 0x00608C10
    static void SetSaveSeeOffRandomMatchNetHst(); // 0x00608CBC
    static void SetSaveSeeOffRandomMatchNetVst(); // 0x00608CD8
    static void SetSaveConnectRandomMatchNetHst(); // 0x00608DF4
    static void SetSaveConnectRandomMatchNetVst(); // 0x00608E10
    static void SetSaveStartTourFriendMatchNetHst(); // 0x00608F44
    static void SetSaveStartTourFriendMatchNetVst(); // 0x00608F60
    static void SetSaveStartTourRandomMatchNetHst(); // 0x00608F7C
    static void SetSaveStartTourRandomMatchNetVst(); // 0x00608F98
    static void SetSaveSeeOffNetHostRandomMatchNetVst(); // 0x006091B4
    static void SetSaveInterruptNetVst(); // 0x00608A74 | libgarden [tier A]
    // the return parameter of the stage transition is set, then SetSaveContinuePlaying
    static void SetSaveContinuePlayingWithReturn(); // 0x00608A28

    // "待機"
    void Enter_Wait(); // 0x00607C5C
    void State_Wait(); // 0x00607AAC
    // "処理なし"
    void Enter_None(); // 0x00607C58
    void State_None(); // 0x00607AA8
    // "通常セーブ"
    void Enter_SaveNormal(); // 0x006080F0
    void State_SaveNormal(); // 0x006080E0
    // "全体＋展示場セーブ"
    void Enter_SaveAllAndExhibition(); // 0x00608C3C
    void State_SaveAllAndExhibition(); // 0x00608C2C
    // "展示場ロード＆セーブ"
    void Enter_LoadAndSaveExhibition(); // 0x00608438
    void State_LoadAndSaveExhibition(); // 0x006083C4
    // "全体セーブ"
    void Enter_SaveAll(); // 0x00607EF0
    void State_SaveAll(); // 0x00607EE0
    // "全体セーブ後元のシーンへ"
    void Enter_SaveAllAndReturn(); // 0x00608304
    void State_SaveAllAndReturn(); // 0x006082F4
    // "成長処理後のセーブ関係処理"
    void Enter_SaveContinueGrowup(); // 0x006088E0
    void State_SaveContinueGrowup(); // 0x006088B0
    // "ゲーム継続セーブ"
    void Enter_SaveContinuePlaying(); // 0x00608AD8
    void State_SaveContinuePlaying(); // 0x00608AA8
    // "更新処理説明フロー前成長処理"
    void Enter_UpdateGrowup(); // 0x00608208
    void State_UpdateGrowup(); // 0x00608198
    // "見送りセーブ (居残りホスト用)"
    void Enter_SaveSeeOffNetHst(); // 0x00608598
    void State_SaveSeeOffNetHst(); // 0x00608514
    // "見送りセーブ (居残りビジター用)"
    void Enter_SaveSeeOffNetVst(); // 0x006085FC
    void State_SaveSeeOffNetVst(); // 0x00608524
    // "退場者セーブ"
    void Enter_SaveRetireNetVst(); // 0x00608534
    void State_SaveRetireNetVst(); // 0x00608504
    // "ネット接続セーブ（親）"
    void Enter_SaveConnectNetHst(); // 0x0060877C
    void State_SaveConnectNetHst(); // 0x00608704
    // "ネット接続セーブ（子）"
    void Enter_SaveConnectNetVst(); // 0x006087EC
    void State_SaveConnectNetVst(); // 0x00608714
    // "ネット継続セーブ（親）"
    void Enter_SaveContinueNetHst(); // 0x00608954
    void State_SaveContinueNetHst(); // 0x006088C0
    // "ネット継続セーブ（子）"
    void Enter_SaveContinueNetVst(); // 0x006089B8
    void State_SaveContinueNetVst(); // 0x006088D0
    // "ネット中断セーブ（親）"
    void Enter_SaveInterruptNetHst(); // 0x00608B40
    void State_SaveInterruptNetHst(); // 0x00608AB8
    // "ネット中断セーブ（子）"
    void Enter_SaveInterruptNetVst(); // 0x00608BA8
    void State_SaveInterruptNetVst(); // 0x00608AC8
    // "ソフトリセット用ロード処理"
    void Enter_Load(); // 0x00607C00
    void State_Load(); // 0x00607A98
    // "ネット接続セーブ（親）(ランダムマッチ)"
    void Enter_SaveConnectRandomMatchNetHst(); // 0x00608E4C
    void State_SaveConnectRandomMatchNetHst(); // 0x00608E2C
    // "ネット接続セーブ（子）(ランダムマッチ)"
    void Enter_SaveConnectRandomMatchNetVst(); // 0x00608EC8
    void State_SaveConnectRandomMatchNetVst(); // 0x00608E3C
    // "見送りセーブ (居残りホスト用)(ランダムマッチ)"
    void Enter_SaveSeeOffRandomMatchNetHst(); // 0x00608D14
    void State_SaveSeeOffRandomMatchNetHst(); // 0x00608CF4
    // "見送りセーブ (居残りビジター用)(ランダムマッチ)"
    void Enter_SaveSeeOffRandomMatchNetVst(); // 0x00608D84
    void State_SaveSeeOffRandomMatchNetVst(); // 0x00608D04
    // "見送りセーブ (ネットホスト退場居残りビジター用)(ランダムマッチ)"
    void Enter_SaveSeeOffNetHostRandomMatchNetVst(); // 0x006091E0
    void State_SaveSeeOffNetHostRandomMatchNetVst(); // 0x006091D0
    // "ツアー開始セーブ（親）(ランダムマッチ)"
    void Enter_SaveStartTourRandomMatchNetHst(); // 0x006090D4
    void State_SaveStartTourRandomMatchNetHst(); // 0x00608FD4
    // "ツアー開始セーブ（子）(ランダムマッチ)"
    void Enter_SaveStartTourRandomMatchNetVst(); // 0x00609144
    void State_SaveStartTourRandomMatchNetVst(); // 0x00608FE4
    // "ツアー開始セーブ (親) (フレンドマッチ)"
    void Enter_SaveStartTourFriendMatchNetHst(); // 0x00608FF4
    void State_SaveStartTourFriendMatchNetHst(); // 0x00608FB4
    // "ツアー開始セーブ (子) (フレンドマッチ)"
    void Enter_SaveStartTourFriendMatchNetVst(); // 0x00609064
    void State_SaveStartTourFriendMatchNetVst(); // 0x00608FC4
    // "ゲーム初回フォーマット"
    void Enter_InitGameInitFormat(); // 0x006083D4
    void State_InitGameInitFormat(); // 0x006083B4
    // "セーブデータ全削除処理"
    void Enter_Delete(); // 0x00608724
    void State_Delete(); // 0x006086F4
    // "セーブエラー用村作成処理"
    void Enter_LoadAfterSaveError(); // 0x00607C98
    void State_LoadAfterSaveError(); // 0x00607C88
    // "通信エラー用再読み込み処理"
    void Enter_ErrorNetwork(); // 0x006081A8
    void State_ErrorNetwork(); // 0x00608188

    // the save number of the player for the interrupting save (SetSaveInterruptNetVst), set by the mode after the growth
    // processing, cleared by Initialize
    static u8 s_SaveNo; // (4: none)
    static bool s_IsAfterGrowup;
    static u8 s_Unknown9516CF;
    // the request of the growth processing (the loaded data is taken over, SvStepInner apply kind 10)
    static SvStepInner s_ApplyRequest;
    // (the step "unlock" waits until it is 0)
    static u32 s_Unknown9516E4;
    // (flags of the steps: the entrance of the player is used by "to the previous scene"; set by the game start; the
    // next game start loads no garden_plus.dat)
    static u8 s_Unknown9516D0;
    static u8 s_Unknown9516D2;
    static u8 s_Unknown9516D3;
    static u8 s_Unknown9516D4;
    static u8 s_Unknown9516D5;
    // the file of the messages ("SYS_Save")
    static const char* s_MessageFile;
    // the instance and the mode the next "Wait" changes to
    static BsSvMgr* s_pInstance;
    static Func s_NextMode;
    // the modes and the step sequences (dBsSvMgrTables.cpp)
    static const Info s_ModeInfos[33];
    static const SaveStep::Info s_StepLoad[11];
    static const SaveStep::Info s_StepLoadAfterSaveError[5];
    static const SaveStep::Info s_StepErrorNetwork[13];
    static const SaveStep::Info s_StepSaveNormal[20];
    static const SaveStep::Info s_StepSaveAllAndExhibition[20];
    static const SaveStep::Info s_StepSaveExhibition[19];
    static const SaveStep::Info s_StepSaveAll[17];
    static const SaveStep::Info s_StepSaveAllAndReturn[17];
    static const SaveStep::Info s_StepSaveContinueGrowup[18];
    static const SaveStep::Info s_StepSaveContinuePlaying[18];
    static const SaveStep::Info s_StepUpdateGrowup[8];
    static const SaveStep::Info s_StepSaveSeeOffNetHst[26];
    static const SaveStep::Info s_StepSaveSeeOffNetVst[23];
    static const SaveStep::Info s_StepSaveRetireNetVst[25];
    static const SaveStep::Info s_StepSaveConnectNetHst[20];
    static const SaveStep::Info s_StepSaveConnectNetVst[17];
    static const SaveStep::Info s_StepSaveContinueNetHst[19];
    static const SaveStep::Info s_StepSaveContinueNetVst[20];
    static const SaveStep::Info s_StepSaveInterruptNetHst[23];
    static const SaveStep::Info s_StepSaveInterruptNetVst[27];
    static const SaveStep::Info s_StepSaveConnectRandomMatchNetHst[25];
    static const SaveStep::Info s_StepSaveConnectRandomMatchNetVst[23];
    static const SaveStep::Info s_StepSaveSeeOffRandomMatchNetHst[25];
    static const SaveStep::Info s_StepSaveSeeOffRandomMatchNetVst[22];
    static const SaveStep::Info s_StepSaveSeeOffNetHostRandomMatchNetVst[22];
    static const SaveStep::Info s_StepSaveStartTourRandomMatchNetHst[21];
    static const SaveStep::Info s_StepSaveStartTourRandomMatchNetVst[20];
    static const SaveStep::Info s_StepSaveStartTourFriendMatchNetHst[20];
    static const SaveStep::Info s_StepSaveStartTourFriendMatchNetVst[21];
    static const SaveStep::Info s_StepInitGameInitFormat[8];
    static const SaveStep::Info s_StepDelete[14];

    u8 unk_0xB4[4];                 // 0xB4
    SaveStep m_SaveStep;            // 0xB8
    u32 unk_0x118;                  // 0x118 the flags of the request that Finalize starts (FLAG_KEEP_ERROR: none)
    Func unk_0x11C;                 // 0x11C
    s32 m_ChoiceCursor;             // 0x124 the chosen item (-1: none yet)
    u32 m_DeleteConfirmStep;        // 0x128 the step of the confirmation of the deletion (0, 1, 2: confirmed twice)
    u8 unk_0x12C;                   // 0x12C
    CpuTimeLimit m_CpuTimeLimit;    // 0x12E
};
ASSERT_SIZE(BsSvMgr::SaveStep, 0x60);
ASSERT_SIZE(BsSvMgr, 0x138);
