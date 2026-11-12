#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"
#include "netgame/netgame_Api.h"

// The save demo: the order demo::SAVE asks whether the game is saved here (messages of "SYS_Save"); a save that is not
// possible is told (the first day, a stage of the tours, Dream Suite, as a guest of a network game, ...), in a network
// game all players are locked first. Class name from the RTTI; the modes after their Japanese debug names (table
// s_ModeInfos), all other names are ours.
// RTTI 8BsSvDemo @ 0x008CD4B0
// vtable 0x008F9A50 (vptr 0x008F9A58), offset_to_top 0, 18 entries
// vtable 0x008F9AA0 (vptr 0x008F9AA8), offset_to_top -20, 63 entries
// vtable 0x008F9BA4 (vptr 0x008F9BAC), offset_to_top -144, 3 entries
class BsSvDemo : public ::Base, public ::script::ITalkRecept, public ::state::Mode<BsSvDemo>
{
public:
    // (inline in Process::Create<BsSvDemo>; the members behind the mode are not set)
    BsSvDemo() {}
    virtual ~BsSvDemo();
    oml::framework::Result Initialize() override; // 0x006A358C slot 0x0C
    oml::framework::Result Finalize() override; // 0x006A36B0 slot 0x18
    oml::framework::Result Calc() override; // 0x006A3620 slot 0x24
    oml::framework::Result Draw() override; // 0x006A3584 slot 0x30
    // (ARMCC gives the overrides of ITalkRecept new slots of the first vtable, in this order)
    void OnChoice(const script::ChoiceItem* item, s32 cursor) override; // 0x006A2BCC slot 0x40
    void OnPageEnd(u8 value) override; // 0x006A2B90 slot 0x44

    // "待機" (wait for the order)
    void Enter_Wait(); // 0x006A2B8C
    void State_Wait(); // 0x006A2A20
    // "セーブ不可" (no save possible: the message of the reason, then back)
    void Enter_CannotSave1(); // 0x006A2E74
    void State_CannotSave1(); // 0x006A2E30
    void Enter_CannotSave2(); // 0x006A3284
    void State_CannotSave2(); // 0x006A31D0
    void Enter_CannotSave3(); // 0x006A33C4
    void State_CannotSave3(); // 0x006A3328
    void Enter_CannotSave4(); // 0x006A34E0
    void State_CannotSave4(); // 0x006A3498
    void Enter_CannotSave5(); // 0x006A336C
    void State_CannotSave5(); // 0x006A32E4
    // "wait" (the question, then the choice)
    void Enter_WaitQuestion(); // 0x006A2D34
    void State_WaitQuestion(); // 0x006A2CC0
    void Enter_WaitChoice(); // 0x006A2C5C
    void State_WaitChoice(); // 0x006A2BF4
    // "out"
    void Enter_Out(); // 0x006A2C78
    void State_Out(); // 0x006A2C58
    // "save" (armlink folded the transition with BsSvMgr::SetSaveNormalOrExhibition)
    void Enter_Save(); // 0x00608088 (folded)
    void State_Save(); // 0x006A2B68
    // "継続セーブ" (save and continue; the transition folded with BsSvMgr::SetSaveContinuePlayingWithReturn)
    void Enter_SaveContinue(); // 0x00608A28 (folded)
    void State_SaveContinue(); // 0x006A2F84
    // "通信時待機" (wait in a network game: the question)
    void Enter_WaitNet(); // 0x006A3170
    void State_WaitNet(); // 0x006A30E0
    // "選択終了" (end of the choice)
    void Enter_EndChoice(); // 0x006A3000
    void State_EndChoice(); // 0x006A2F20
    // "ロック失敗" (the lock failed)
    void Enter_LockFailed(); // 0x006A2CF4
    void State_LockFailed(); // 0x006A2C7C
    // "ロック" / "ロック結果" / "ロック成功" (lock all players for the save and end the session)
    void Enter_Lock(); // 0x006A3118
    void State_Lock(); // 0x006A308C
    void Enter_LockResult(); // 0x006A32DC
    void State_LockResult(); // 0x006A3214
    void Enter_LockSucceeded(); // 0x006A3538
    void State_LockSucceeded(); // 0x006A34DC
    // the same for a save that continues the network game (the transition folded with netgame::StartSaveContinue)
    void Enter_LockContinue(); // 0x006A2FA8
    void State_LockContinue(); // 0x006A2ECC
    void Enter_LockContinueResult(); // 0x006A3114
    void State_LockContinueResult(); // 0x006A301C
    void Enter_LockContinueSucceeded(); // 0x006273E8 (folded)
    void State_LockContinueSucceeded(); // 0x006A32E0
    // "なにもしない" (do nothing)
    void Enter_DoNothing(); // 0x006A2BC8
    void State_DoNothing(); // 0x006A2BC4

    // (inline) the message of the number from "SYS_Save" starts with this recept
    DECOMP_ALWAYS_INLINE void StartMessage(s32 number);
    // (inline) at the end of a message: back to the wait for the order
    DECOMP_ALWAYS_INLINE void EndMessage();

    // the modes (table 0x00855798)
    static const Info s_ModeInfos[21];

    s32 m_MessageNumber;    // 0xB4 the message of the reason why no save is possible (-1: the save is possible)
    s32 m_ChoiceCursor;     // 0xB8 the chosen item
    s32 unk_0xBC;           // 0xBC (1 or 2 by the question; a choice of this item ends the demo without a save)
};
ASSERT_SIZE(BsSvDemo, 0xC0);

// (game code that State_Wait calls, not decompiled yet; names after the addresses) the entry of an index of a table of 10
// (its word +0x44 has the bit 0x20: no save here) and a test on the actor of the player (PlayerGetActor, byte +0x1A9)
void* Func2FE4A0(s32 index); // 0x002FE4A0
bool Func5C505C(netgame::PlayerNo player); // 0x005C505C
