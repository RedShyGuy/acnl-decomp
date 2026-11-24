#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMelodyPlayerBase.h"
#include "lyt/dTextAreaMail.h"
#include "script/dITalkRecept.h"
#include "script/dWordCPtr.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "state/dMode.h"

class ComButton1Lyt;
class ComButton2Lyt;
class ComButton3Lyt;
namespace gui {
class ButtonEx2;
class CellPhoneKeySet;
class GridKeySet;
class KanaKeySet;
class KeySet;
class QwertyKeySet;
class RadioButton;
class RepeatButton;
class TextArea;
class TextAreaEx;
class Widget;
} // namespace gui
namespace lyt {
class AnmObject;
class Object;
class TextArea;
} // namespace lyt

// The software keyboard menu (class name from the RTTI, the state names from the table of the binary, all other names are
// ours): the text area of the keyboard kind (sys::g_Unknown95F0EB: 0 chat, 1 comment, 2 birthday, 3 friend, 4..14/17/18
// titles, 15 bulletin board, 16 letter) with the widget that edits it, one of four key sets (Layout/Swkbd/swkbd_qwerty,
// _kana, _grid, _cell; the selection buttons char_type_00..03), the buttons of the bottom bar (ComButton1/2/3Lyt after the
// number of buttons of sys::g_KeyboardParam), the melody player of the town tune keyboard and the UGC check of the text.
// RTTI 5BsSkb @ 0x008CD2B0
// vtable 0x008F8A44 (vptr 0x008F8A4C), offset_to_top 0, 25 entries
// vtable 0x008F8AB0 (vptr 0x008F8AB8), offset_to_top -40, 63 entries
// vtable 0x008F8BB4 (vptr 0x008F8BBC), offset_to_top -164, 3 entries
class BsSkb : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsSkb>
{
public:
    // the key sets (m_KeySetKind, m_RequestedKeySet)
    enum KeySetKind
    {
        KEY_SET_QWERTY = 0,
        KEY_SET_KANA = 1,
        KEY_SET_GRID = 2,
        KEY_SET_CELL_PHONE = 3,
        KEY_SET_NONE = -1,
    };
    // the keyboard kinds of sys::g_Unknown95F0EB that BsSkb tells apart
    enum Kind
    {
        KIND_CHAT = 0,
        KIND_COMMENT = 1,
        KIND_BIRTHDAY = 2,
        KIND_FRIEND = 3,
        KIND_BOARD = 15,
        KIND_LETTER = 16,
        KIND_17 = 17,
        KIND_18 = 18,
    };

    BsSkb(); // 0x00581870
    // 0x00581AA0 (deleting dtor)
    ~BsSkb() override; // 0x00581AB8 slot 0x00
    oml::framework::Result Initialize() override; // 0x0057FDF4 slot 0x0C
    oml::framework::Result Finalize() override; // 0x0058100C slot 0x18
    oml::framework::Result Calc() override; // 0x00580E0C slot 0x24
    oml::framework::Result Draw() override; // 0x0057FDB8 slot 0x30
    void OnClose() override; // 0x0057C900 slot 0x44

    // the key set of the kind is made and becomes the current one
    bool ChangeKeySet(s32 kind); // 0x0057C9A8
    // the text area of the keyboard kind (and its widget) is made
    void CreateTextArea(); // 0x0057D1B8
    // the selection button of the key set is selected; the kind goes into the settings
    void SelectKeySetButton(s32 kind); // 0x0057DEBC
    // the keyboard closes (from another menu)
    void Close(); // 0x0057E044
    // the lists of the grid and cellphone pages that have characters
    void SetupPages(); // 0x0057E8DC
    // the in / out animation of the text area (for other menus) and whether it has ended
    void StartTextIn(); // 0x0057FC80
    void StartTextOut(); // 0x0057FC90
    bool IsTextInDone(); // 0x0057FD88
    bool IsTextOutDone(); // 0x0057FDA0
    // the buttons of the bottom bar take input (and the widgets, gui::g_Unknown95F1A4)
    void SetInputEnabled(bool isEnabled); // 0x00580F78
    // the widget callback of the keyboard
    static void OnWidgetEvent(gui::Widget* sender, s32 event, s32 value, void* user); // 0x005814D0
    void OnEvent(gui::Widget* sender, s32 event, s32 value); // 0x00581548

    // the states (the names in the comments are the debug names of the binary)
    void State_WindowIn();       // 0x0057E1E8 window in
    void Enter_WindowIn();       // 0x0057E490
    void State_WindowOut();      // 0x0057E5CC window out
    void Enter_WindowOut();      // 0x0057E7C0
    void State_ButtonIn();       // 0x0057E0C8 button in
    void Enter_ButtonIn();       // 0x0057E364
    void State_ButtonOut();      // 0x0057E52C button out
    void Enter_ButtonOut();      // 0x0057E77C
    void State_Wait();           // 0x0057CE40 wait
    void Enter_Wait();           // 0x0057D1B4
    void State_ReturnWait();     // 0x0057EDE8 return wait
    void Enter_ReturnWait();     // 0x0057F06C
    void State_NgCheck();        // 0x0057F120 ng check
    void Enter_NgCheck();        // 0x0057F27C
    void State_NgCheck2();       // 0x0057F070 ng check
    void Enter_NgCheck2();       // 0x0057F254
    void State_NgError();        // 0x0057E750 ng error
    void Enter_NgError();        // 0x0057E83C
    void State_NgErrorWait();    // 0x0057F974 ng error wait
    void Enter_NgErrorWait();    // 0x0057FC74
    void State_Write();          // 0x0057DCF8 write
    void Enter_Write();          // 0x0057DE30
    void State_ButtonWait();     // 0x0057EA08 button wait
    void Enter_ButtonWait();     // 0x0057EF78
    void State_WaitCancel();     // 0x0057F228 waite cancel
    void Enter_WaitCancel();     // 0x0057F2EC
    void State_WaitCancel2();    // 0x0057FCA0 waite cancel
    void Enter_WaitCancel2();    // 0x0057FD84
    void State_Preview1();       // 0x0057F40C preview1
    void Enter_Preview1();       // 0x0057F9AC
    void State_Preview2();       // 0x0057F4A0 preview2
    void Enter_Preview2();       // 0x0057FA00
    void State_Preview3();       // 0x0057F5C8 preview3
    void Enter_Preview3();       // 0x0057FA94
    void State_Preview();        // 0x0057DF94 preview
    void Enter_Preview();        // 0x0057E014
    void State_PreviewEnd();     // 0x0057ED94 preview
    void Enter_PreviewEnd();     // 0x0057F024
    void State_PreviewCheck();   // 0x0057F378 preview
    void Enter_PreviewCheck();   // 0x0057F408
    void State_Rewrite0();       // 0x0057F6A4 rewrite0
    void Enter_Rewrite0();       // 0x0057FAF0
    void State_Rewrite1();       // 0x0057F6D8 rewrite1
    void Enter_Rewrite1();       // 0x0057FB54
    void State_Rewrite2();       // 0x0057F7E8 rewrite2
    void Enter_Rewrite2();       // 0x0057FBB0
    void State_Rewrite3();       // 0x0057F8F0 rewrite3
    void Enter_Rewrite3();       // 0x0057FC20
    void State_MelodyPlay();     // 0x0057ECBC melody play
    void Enter_MelodyPlay();     // 0x0057EF8C
    void State_KeySetLoad();     // 0x0057EB0C keyset load
    // (ARMCC put it in the code of ssys; it falls into ArcResourceAccessorVRAM::ReleaseTextures)
    void Enter_KeySetLoad();     // 0x00568CA4
    void State_WarningWait();    // 0x0057F1C4 warninig wait
    void Enter_WarningWait();    // 0x0057F280
    void State_Idle();           // 0x0057CE3C idle
    void Enter_Idle();           // 0x0057D1B0

    // (inline) the objects of the keyboard follow the window (N_All of the background)
    void MoveToWindow(bool isWithTextArea, bool isWithKeySet);

    typedef ::state::Mode<BsSkb>::Info StateInfo;
    static const StateInfo s_StateInfos[28];
    // the open keyboard (NULL: none; read by the menus before they open one)
    static BsSkb* s_pInstance;
    // the groups of the two page arrows of the grid
    static const char* s_GridArrowGroups[2];
    // the labels of the pages of the grid (0..3 signs) and of the cellphone (4..9 European letters); a page is used
    // when its label has a text
    static const char* s_PageLabels[10];

    sead::Heap* m_pHeap;                                // 0x0C8 the heap of the layout allocator
    ssys::ma::lyt::ArcResAccReader m_Reader;            // 0x0CC the archive of the text area (key16/board/ltr/cmn_title_win)
    ssys::ma::lyt::ArcResAccReader m_CommonReader;      // 0x314 Layout/Swkbd/swkbd_common.arc
    lyt::Object* m_pBg;                                 // 0x55C BG.bclyt
    lyt::AnmObject* m_pBgInAnim;                        // 0x560
    lyt::AnmObject* m_pBgOutAnim;                       // 0x564
    lyt::TextArea* m_pTextArea;                         // 0x568
    gui::TextArea* m_pTextWidget;                       // 0x56C (not for the board and the letter)
    lyt::AnmObject* m_pTextInAnim;                      // 0x570
    lyt::AnmObject* m_pTextOutAnim;                     // 0x574
    lyt::AnmObject* m_pMelodyInAnim;                    // 0x578 mld_ply_in
    lyt::AnmObject* m_pMelodyOutAnim;                   // 0x57C mld_ply_out
    lyt::AnmObject* m_pTitleMsgPosAnim;                 // 0x580 cmn_title_win_msg_pos (titles)
    lyt::AnmObject* m_pTitleIconAnim;                   // 0x584 cmn_title_win_Pattern_chara_icon (titles)
    ComButton1Lyt* m_pButton1;                          // 0x588 the bottom bar with one, two or three buttons
    ComButton2Lyt* m_pButton2;                          // 0x58C
    ComButton3Lyt* m_pButton3;                          // 0x590
    lyt::Object* m_pArrow;                              // 0x594 Key_arrow (board and letter: the scroll arrows)
    gui::TextAreaEx* m_pPageWidget;                     // 0x598 the widget of the board or the letter
    ssys::ma::lyt::ArcResAccReader m_KeyReader;         // 0x59C the archive of the current key set
    ssys::ma::lyt::ArcResAccReader m_Readers[4];        // 0x7E4 (only made and destroyed)
    s32 m_KeySetKind;                                   // 0x1104 KeySetKind
    s32 m_RequestedKeySet;                              // 0x1108 the key set to load (KEY_SET_NONE: none)
    lyt::Object* m_pKeySetSelect;                       // 0x110C KeytopModeSelect.bclyt
    lyt::Object* m_pGridArrow;                          // 0x1110 LncArw_00.bclyt (the page arrows of the grid)
    lyt::Object* m_pMelody;                             // 0x1114 mld_ply.bclyt
    lyt::AnmObject* m_pGridArrowAnims[2];               // 0x1118 LncArw_00_Appear
    gui::RadioButton* m_pKeySetButtons[4];              // 0x1120 char_type_00..03
    u32 m_Unknown0x1130;                                // 0x1130
    gui::ButtonEx2* m_pMelodyButton;                    // 0x1134
    gui::RepeatButton* m_pGridArrowButtons[2];          // 0x1138 the page arrows of the grid
    gui::KeySet* m_pKeySet;                             // 0x1140 the current key set
    bool m_IsLabel16;                                   // 0x1144 the label of the key set is 1 or 6
    bool m_IsLabel27;                                   // 0x1145 the label of the key set is 2 or 7
    bool m_IsShifted;                                   // 0x1146 (grid: the label is a shifted one)
    bool m_IsKeySetLoading;                             // 0x1147
    bool m_IsResultUsed;                                // 0x1148 the button chosen goes to BsMenuMgr::s_Result
    bool m_IsMelodyPlaying;                             // 0x1149 the melody button was pressed
    bool m_IsMelodyStarted;                             // 0x114A the melody has played a note
    s32 m_GridPageNum;                                  // 0x114C the pages of the grid with characters
    s32 m_GridPages[10];                                // 0x1150
    s32 m_CellPhonePageNum;                             // 0x1178
    s32 m_CellPhonePages[10];                           // 0x117C
    const s32* m_pPages;                                // 0x11A4 the pages of the key set (grid or cellphone)
    u32 m_Unknown0x11A8;                                // 0x11A8
    s32 m_Unknown0x11AC;                                // 0x11AC
    bool m_HasTextAnim;                                 // 0x11B0 the text area has in/out animations of its own
    bool m_IsTextAcceptable;                            // 0x11B1 the last state of sys::IsTextAcceptable (the ok button)
    SeID m_OkSe;                                        // 0x11B4 the sound of the ok button
    SeID m_CancelSe;                                    // 0x11B8
    s32 m_Unknown0x11BC;                                // 0x11BC
    s32 m_PageIndex;                                    // 0x11C0 the page of the key set (KeyboardSetting)
    bool m_IsSettingInitialized;                        // 0x11C4
    bool m_Unknown0x11C5;                               // 0x11C5 the keyboard closes
    lyt::TextCheck m_TextCheck;                         // 0x11C8 the UGC check of the text
    script::WordCPtr m_CheckText;                       // 0x11D8
    s32 m_Result;                                       // 0x11E4 the result of the keyboard (sys::SetFinished)
    s32 m_ErrorLabel;                                   // 0x11E8 the label of the error dialog
    SoundMelody m_Melody;                               // 0x11EC the town tune that is played
    gui::QwertyKeySet* m_pQwerty;                       // 0x11FC
    gui::KanaKeySet* m_pKana;                           // 0x1200
    gui::GridKeySet* m_pGrid;                           // 0x1204
    gui::CellPhoneKeySet* m_pCellPhone;                 // 0x1208
    bool m_IsInputEnabled;                              // 0x120C
    s8 m_IsWriting;                                     // 0x120D the text area is being written (lyt::TextArea +0x738)
    bool m_IsRandomMatch;                               // 0x120E
    s32 m_InitStep;                                     // 0x1210 the step of Initialize
    u32 m_Unknown0x1214;                                // 0x1214
    u32 m_Unknown0x1218[2];                             // 0x1218
    f32 m_Unknown0x1220;                                // 0x1220
    u32 m_Unknown0x1224;                                // 0x1224
};
ASSERT_OFFSET(BsSkb, m_pHeap, 0xC8);
ASSERT_OFFSET(BsSkb, m_pBg, 0x55C);
ASSERT_OFFSET(BsSkb, m_KeyReader, 0x59C);
ASSERT_OFFSET(BsSkb, m_KeySetKind, 0x1104);
ASSERT_OFFSET(BsSkb, m_pKeySet, 0x1140);
ASSERT_OFFSET(BsSkb, m_IsResultUsed, 0x1148);
ASSERT_OFFSET(BsSkb, m_TextCheck, 0x11C8);
ASSERT_OFFSET(BsSkb, m_Result, 0x11E4);
ASSERT_OFFSET(BsSkb, m_pQwerty, 0x11FC);
ASSERT_OFFSET(BsSkb, m_InitStep, 0x1210);
ASSERT_SIZE(BsSkb, 0x1228);
