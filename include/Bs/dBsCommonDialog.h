#pragma once

#include "decomp.h"
#include "Other/dComDialogA.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"
#include "Sound/dSoundLabel.h"

struct SvFgName;
struct SvPersonalId;
namespace script {
class IWord;
} // namespace script

// The menu of the common dialog (menu 138, see BsMenuMgr::Func6D406C; class name from the RTTI, the state names from the
// table of the binary, all other names are ours): its window ComDialogA shows the request g_CommonDialogArg; the button
// chosen goes to BsMenuMgr::s_Result.
// RTTI 14BsCommonDialog @ 0x008CBB54
// vtable 0x008EFB4C (vptr 0x008EFB54), offset_to_top 0, 25 entries
// vtable 0x008EFBB8 (vptr 0x008EFBC0), offset_to_top -40, 3 entries
class BsCommonDialog : public ::MenuBase, public ::state::Mode<BsCommonDialog>
{
public:
    // (inline in Process::Create<BsCommonDialog> 0x007EE8C4)
    BsCommonDialog() {}
    // 0x00251E5C (deleting dtor)
    ~BsCommonDialog() override; // 0x00251E88 slot 0x00
    oml::framework::Result Initialize() override; // 0x00251B70 slot 0x0C
    oml::framework::Result Finalize() override; // 0x00251D90 slot 0x18
    oml::framework::Result Calc() override; // 0x00251CD0 slot 0x24
    oml::framework::Result Draw() override; // 0x00251B5C slot 0x30
    void OnClose() override; // 0x00251940 slot 0x44

    // the next message of g_CommonDialogArg in the open window
    void ShowNext(); // 0x00251908
    // the window closes (now or when it is idle again) / goes out
    void Close(); // 0x00251B34
    void CloseLater(); // 0x00251E24
    void GoOut(); // 0x00251E40
    // (copies for the game) the dialog is idle (the message was answered) / idle or waiting for a button
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsIdle() const // 0x00251DD4
    {
        return ::state::Mode<BsCommonDialog>::GetState() == &BsCommonDialog::State_Idle;
    }
    DECOMP_EMIT_COPY bool IsIdleOrWaiting() const // 0x00251C4C
    {
        return ::state::Mode<BsCommonDialog>::GetState() == &BsCommonDialog::State_Idle ||
               ::state::Mode<BsCommonDialog>::GetState() == &BsCommonDialog::State_Wait;
    }
    // the open dialog (NULL: none)
    static BsCommonDialog* GetInstance(); // 0x00251930

    // the states (the names in the comments are the debug names of the binary)
    void State_In();         // 0x00251AA0 In
    // (its code lies in front of ComDialogA::StartIn and falls into it)
    void Enter_In();         // 0x001AA408
    void State_Out();        // 0x00251AD0 Out
    void Enter_Out();        // 0x00251B18
    void State_Wait();       // 0x00251960 Wait
    void Enter_Wait();       // 0x00251A38
    void State_Idle();       // 0x0025195C 待機 (idle)
    void Enter_Idle();       // 0x00251A34
    void State_WaitClose();  // 0x00251A3C 待機閉じる (idle, closing)
    void Enter_WaitClose();  // 0x00251A84

    typedef ::state::Mode<BsCommonDialog>::Info StateInfo;
    static const StateInfo s_StateInfos[5];
    static bool s_IsResultUsed; // the button chosen goes to BsMenuMgr::s_Result (set again by Finalize)
    static BsCommonDialog* s_pInstance;

    ComDialogA m_Dialog; // 0x4C
};
ASSERT_OFFSET(BsCommonDialog, m_Dialog, 0x4C);

// the request of the common dialog (BsMenuMgr::Func6D32D0 resets it; names are ours)
struct CommonDialogArg
{
    u8 kind;                    // 0x00 ComDialogA::Kind
    u8 icon;                    // 0x01 the sound of the window (0..7)
    s32 labels[4];              // 0x04 the message and the labels of the buttons (left, -, right / only; indices in pFile)
    SeID okSe;                  // 0x14 (-1: SE_SYS_DECIDE)
    SeID cancelSe;              // 0x18 (-1: SE_SYS_CANCEL)
    s32 wordKind;               // 0x1C the word inserted into the message (ComDialogA::Insert*; -1: none, 0..: a design)
    s32 number;                 // 0x20
    const char* pFile;          // 0x24 the message file
    const SvPersonalId* pPerson; // 0x28
    const SvFgName* pItem;      // 0x2C
    const script::IWord* pWord; // 0x30
    void (*callback)();         // 0x34 called once before the message is read
    bool isCentered;            // 0x38 the text is centered
    bool isLast;                // 0x39 the last message (the black background goes out with it)
    bool isIconShown;           // 0x3A the save icon turns
    bool input;                 // 0x3B (the pad bits of the only button: 2 instead of 1)
};
ASSERT_SIZE(CommonDialogArg, 0x3C);

extern CommonDialogArg g_CommonDialogArg;
