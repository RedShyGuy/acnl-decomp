#pragma once

#include "decomp.h"
#include "Other/dLayout.h"
#include "Other/dWaitIcon.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "state/dMode.h"

namespace nw {
namespace lyt {
class TextBox;
class Pane;
} // namespace lyt
} // namespace nw

// RTTI 9MsgWindow @ 0x008CD7A8
// vtable 0x008FA6C0 (vptr 0x008FA6C8), offset_to_top 0, 3 entries
//
// The message window of the conversations (member of script's TalkWin): two layouts, nine animations, the wait icon
// and the text boxes of the main and ruby texts. Only the layout and the members script uses are known; member names
// are ours. The functions of the window (0x6E6xxx) are game code that is not decompiled yet.
class MsgWindow : public ::state::Mode<MsgWindow>
{
public:
    static const u32 EXTRA_TEXT_BOX_NUM = 5;

    MsgWindow(); // 0x006E751C
    virtual ~MsgWindow(); // 0x006E7754 slot 0x00
    // 0x006E76A8 (deleting dtor)
    virtual const char* GetName(Func func) const; // 0x0082D7B8 slot 0x08

    // (game code that is not decompiled yet; names are ours, after what script does with them)
    // the layouts and the fonts are prepared; true when ready
    bool Setup(); // 0x006E6F84
    // the panes and layouts are released
    void Release(); // 0x006E7014
    // the layouts of the window are drawn (registered with the layout manager)
    void Draw(); // 0x006C97AC
    // the wait icon is shown or hidden
    void SetWaitIconVisible(bool isVisible); // 0x006E6AA8
    // whether the window is in its state 0x0087BA9C (purpose unknown)
    bool Func770A18() const; // 0x00770A18
    // the window opens (with its animation; the extra text boxes of Stage 101/102: without)
    void Open(); // 0x006E743C
    void OpenSpecial(); // 0x006E63C8
    // the window closes
    void Close(); // 0x006E74AC
    void CloseSpecial(); // 0x006E641C
    // the frame of the animation of the group G_win_nam (the color of the name)
    void SetNameColor(u32 frame); // 0x006E6470
    // the frame of the animation of the group G_winColor (the color of the window)
    void SetWindowColor(u32 frame); // 0x006E6510
    // the pane N_pageicn_00 (the icon at the end of a page) is shown or hidden
    void SetPageIconVisible(bool isVisible); // 0x006E69E4

    WaitIcon m_WaitIcon;                                        // 0x024
    Layout m_Layout;                                            // 0x460
    Layout m_SubLayout;                                         // 0x5AC
    ssys::ma::lyt::ArcResAccReader m_ResAccReader;              // 0x6F8
    ssys::ma::lyt::Animation m_Animation0;                                   // 0x940
    ssys::ma::lyt::Animation m_Animation1;                                   // 0x968
    ssys::ma::lyt::Animation m_Animation2;                                   // 0x990
    ssys::ma::lyt::Animation m_Animation3;                                   // 0x9B8
    ssys::ma::lyt::Animation m_Animation4;                                   // 0x9E0
    ssys::ma::lyt::Animation m_Animation5;                                   // 0xA08
    ssys::ma::lyt::Animation m_Animation6;                                   // 0xA30
    ssys::ma::lyt::Animation m_Animation7;                                   // 0xA58
    ssys::ma::lyt::Animation m_Animation8;                                   // 0xA80
    nw::lyt::TextBox* m_pTextBox;                               // 0xAA8 the main text
    nw::lyt::TextBox* m_pTextBox2;                              // 0xAAC the main text (second)
    nw::lyt::TextBox* m_pRubyTextBox;                           // 0xAB0 the ruby text
    nw::lyt::TextBox* m_pRubyTextBox2;                          // 0xAB4 the ruby text (second)
    nw::lyt::TextBox* m_pNameTextBox;                           // 0xAB8 a text box of 9 characters
    nw::lyt::TextBox* m_pSpecialTextBox;                        // 0xABC (Stage 101/102)
    nw::lyt::TextBox* m_pSpecialTextBoxes[EXTRA_TEXT_BOX_NUM];  // 0xAC0 (NULL-terminated)
    nw::lyt::Pane* m_pShakePane;                                // 0xAD4 the pane the window shakes with
    u32 m_Unknown0xAD8[5];                                      // 0xAD8
    u8 m_IsNameShown;                                           // 0xAEC (TalkSystem: the name text box has a name)
    u8 m_Unknown0xAED;                                          // 0xAED
    u8 m_Unknown0xAEE;                                          // 0xAEE
    u8 m_Unknown0xAEF;                                          // 0xAEF
    u8 m_Unknown0xAF0;                                          // 0xAF0
    u8 m_Unknown0xAF1;                                          // 0xAF1 (TalkSystem: the windows 6, 8, 9)
};
ASSERT_SIZE(MsgWindow, 0xAF4);
