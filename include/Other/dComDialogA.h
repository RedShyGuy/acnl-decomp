#pragma once

#include "decomp.h"
#include "Other/dLayout.h"
#include "Sound/dSoundLabel.h"
#include "script/dDialogCompo.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "state/dMode.h"

class Control1Button;
class Control2Button;
namespace script {
class RenderRubySet;
class UiMgr;
} // namespace script

// The text of SYS_2D_Dialog_Text that the next common dialog shows (DialogCompo by index or number) and the button pressed
// in it (names are ours; Clear 0x001A9920 is also called by other menus).
struct DialogTextRequest
{
    void Clear(); // 0x001A9920

    s32 index;        // 0x00 the string by index (-1: by number)
    s32 number;       // 0x04 the string by number (-1: none; both -1: the message of g_CommonDialogArg)
    u32 arg0;         // 0x08 (set by the script flow, IFlowRecept)
    u32 arg1;         // 0x0C
    s32 _unknown0x10; // 0x10 (cleared to -1 when the dialog comes in)
    s32 pressed;      // 0x14 the button pressed on the last page (-1: none)
    bool isCancel;    // 0x18 (the window goes out with the ng animation)
};
ASSERT_SIZE(DialogTextRequest, 0x1C);

extern DialogTextRequest g_DialogTextRequest;
// (a sound held while the save icon turns; the dialog keeps its pan at 0; name is ours)
extern u32 g_Unknown952674;

// The window of the common dialog (BsCommonDialog; class name from the RTTI, the state names from the table of the binary,
// all other names are ours): cmn_dialog_A.bclyt with a text (a message of g_CommonDialogArg with inserted words, or the
// pages of a dialog text, see g_DialogTextRequest), one or two buttons and the turning save icon of save_wait.bclyt.
// RTTI 10ComDialogA @ 0x008CB3B0
// vtable 0x008EC160 (vptr 0x008EC168), offset_to_top 0, 3 entries
class ComDialogA : public ::state::Mode<ComDialogA>
{
public:
    // the buttons of the window (g_CommonDialogArg kind; m_Kind)
    enum Kind : u8
    {
        KIND_ONE_BUTTON = 0,
        KIND_TWO_BUTTONS = 1,
        KIND_NONE = 3,
    };

    ComDialogA(); // 0x001AA4FC
    // 0x001AA63C (deleting dtor)
    virtual ~ComDialogA(); // 0x001AA6B8 slot 0x00

    // the archive (and the save icon of another object) is read; the button control of the kind is made
    bool Load(); // 0x001A9F44
    // the layouts, animations, the text renderer and the first text; the machine starts idle
    bool Initialize(); // 0x001A994C
    // the texts and the buttons of a new message
    void Open(); // 0x001A9C90
    bool Finalize(); // 0x001A9D58
    void Calc(); // 0x001A9E70
    void Draw(); // 0x0056A934
    // the window comes in / goes out (only while idle)
    void StartIn(); // 0x001AA430
    void StartOut(); // 0x001AA484
    // the window closes (the result of the buttons into BsMenuMgr::s_Result) or does so when it is idle again
    void Close(); // 0x001A9394

    // (copies for the game) the window is busy (not idle and not waiting) / the button decided / the decision is done
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsBusy() const // 0x0070B550
    {
        return GetState() != &ComDialogA::State_Idle && GetState() != &ComDialogA::State_Wait;
    }
    u8 GetDecidedIndex() const; // 0x0070B5D4
    bool IsSelectOkDone() const; // 0x0070B60C

    // the states (the names in the comments are the debug names of the binary)
    void State_Idle();          // 0x001A7F74 idle
    void Enter_Idle();          // 0x001A80C4
    void State_In();            // 0x001A81B0 in
    void Enter_In();            // 0x001A8324
    void State_Out();           // 0x001A8540 out
    void Enter_Out();           // 0x001A86BC
    void State_IconIn();        // 0x001A84B8 icon in
    void Enter_IconIn();        // 0x001A8638
    void State_IconOut();       // 0x001A8854 icon out
    void Enter_IconOut();       // 0x001A88EC
    void State_Wait();          // 0x001A7FDC wait
    void Enter_Wait();          // 0x001A80C8
    void State_PageChange();    // 0x001A9480 pagechnage
    void Enter_PageChange();    // 0x001A9524

    // the text of g_DialogTextRequest (pages) or the message of g_CommonDialogArg
    void Compose(); // 0x001A7E0C
    // the message of g_CommonDialogArg with its inserted words
    void SetupText(); // 0x001A8928
    // the buttons of the current page of the dialog text
    void SetupButtons(); // 0x001A8E1C
    // the text renderer and the buttons of g_CommonDialogArg, the icon sound
    void SetupLayout(); // 0x001AA0A4

    // the inserted words of the dialog texts by g_CommonDialogArg::wordKind (DialogCompo::Callback)
    static void InsertWord(script::UiMgr* ui, void* arg);        // 0x001A8834 -6: the word
    static void InsertItem(script::UiMgr* ui, void* arg);        // 0x001A87D8 -5: the name of the item
    static void InsertNumber(script::UiMgr* ui, void* arg);      // 0x001A7F18 -4: the number
    static void InsertPersonNumber(script::UiMgr* ui, void* arg); // 0x001A9768 -3: person, town and number
    static void InsertPerson(script::UiMgr* ui, void* arg);      // 0x001A9600 -2: person and town
    static void InsertDesign(script::UiMgr* ui, void* arg);      // 0x001A92D4 others: the design of the slot

    typedef ::state::Mode<ComDialogA>::Info StateInfo;
    static const StateInfo s_StateInfos[7];

    script::RenderRubySet* m_pRender;            // 0x0024
    script::DialogCompo m_Text;                  // 0x0028
    ::Layout m_DialogLayout;                     // 0x1030 cmn_dialog_A.bclyt
    ::Layout m_WaitLayout;                       // 0x117C save_wait.bclyt
    ssys::ma::lyt::ArcResAccReader m_DialogReader; // 0x12C8 Layout/common/cmn_dialog_A.arc
    ssys::ma::lyt::ArcResAccReader m_WaitReader; // 0x1510 (the archive of another object)
    ssys::ma::lyt::Animation m_InAnim;           // 0x1758 cmn_dialog_A_in_00
    ssys::ma::lyt::Animation m_OutOkAnim;        // 0x1780 cmn_dialog_A_out_ok_00
    ssys::ma::lyt::Animation m_OutNgAnim;        // 0x17A8 cmn_dialog_A_out_ng_00
    ssys::ma::lyt::Animation* m_pOutAnim;        // 0x17D0 the out animation in use
    ssys::ma::lyt::Animation m_WaitInAnim;       // 0x17D4 save_wait_in
    ssys::ma::lyt::Animation m_WaitOutAnim;      // 0x17FC save_wait_out
    ssys::ma::lyt::Animation m_WaitLoopAnim;     // 0x1824 save_wait_wait_save
    Control1Button* m_pOneButton;                // 0x184C
    Control2Button* m_pTwoButtons;               // 0x1850
    u32 m_Unknown0x1854;                         // 0x1854 (only cleared)
    u32 m_Unknown0x1858;                         // 0x1858 (only cleared)
    bool m_IsBlackShown;                         // 0x185C the black background is in (it comes in only once)
    bool m_IsIconShown;                          // 0x185D the save icon turns
    bool m_IsCloseRequested;                     // 0x185E
    bool m_IsDecided;                            // 0x185F
    SeID m_OpenSe;                               // 0x1860
    u8 m_Kind;                                   // 0x1864 Kind
    s32 m_InfoKind;                              // 0x1868 the kind of the dialog text (DialogCompo::Info::kind)
    Func m_NextState;                            // 0x186C the state after coming in (NULL: idle)
};
ASSERT_OFFSET(ComDialogA, m_pRender, 0x24);
ASSERT_OFFSET(ComDialogA, m_DialogLayout, 0x1030);
ASSERT_OFFSET(ComDialogA, m_InAnim, 0x1758);
ASSERT_OFFSET(ComDialogA, m_pOneButton, 0x184C);
ASSERT_OFFSET(ComDialogA, m_OpenSe, 0x1860);
ASSERT_OFFSET(ComDialogA, m_NextState, 0x186C);
ASSERT_SIZE(ComDialogA, 0x1874);
