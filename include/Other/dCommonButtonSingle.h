#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"
#include "Other/dLayout.h"
#include "Sound/dSoundMgr.h"
#include "script/dRenderGarden.h"
#include "script/dWordFix.h"
#include "ssys/ma/dLoadSplit.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAcc.h"

namespace nw {
namespace font {
class Font;
} // namespace font
namespace lyt {
class Pane;
class TextBox;
} // namespace lyt
} // namespace nw

// A common button of the menus (class name from the RTTI, all other names are ours): its own layout cmn_btn.bclyt (or
// cmn_btn_back.bclyt for the back button without a text) from the archive Layout/common/cmn_btn.arc, shown on a pane of
// the menu (N_cmnbtn_pos_*), with a text from the messages and in/out/disabled animations.
// RTTI 18CommonButtonSingle @ 0x008CC820
// vtable 0x008F4684 (vptr 0x008F468C), offset_to_top 0, 16 entries
class CommonButtonSingle : public ::ButtonActionNode
{
public:
    typedef void (CommonButtonSingle::*StateFunc)();

    CommonButtonSingle(); // 0x002E0F60
    // 0x002E1078 (deleting dtor)
    ~CommonButtonSingle() override; // 0x002E1108 slot 0x00

    // the state (the in and out animations)
    void FUN_002b72e0() override; // 0x002E0E6C slot 0x10
    void PlayOkSound() override { SoundMgr::PlaySound(m_SeOk, 0.0f); } // 0x00721270 slot 0x38

    // the archive of the layouts of the buttons (once for all buttons)
    static void LoadArchive(); // 0x002E0998
    // the layout with the two fonts of the text; the button starts out of the screen
    bool Setup(const char* fontName, const nw::font::Font* font, const char* fontName2, const nw::font::Font* font2,
               bool unused); // 0x002E0A64
    // the button comes in / goes out (animation); out at once
    void StartIn(); // 0x002E0ED4
    void StartOut(); // 0x002E0778
    void SetOut(); // 0x002E0E38
    // in at once (the text is updated)
    void SetIn(); // 0x00569E84
    // the button leaves its pane; the animations and the text are released
    void Finalize(); // 0x005E3894
    // decide (A) or cancel (B) button: the index, the ok sound and the input
    void SetDecide(bool isDecide); // 0x002E0814
    void SetVisible(bool visible); // 0x002E0854
    // the text: the message index of the label (-1: none)
    void SetText(s32 index, const char* label); // 0x002E08D4
    // the button is shown on pane (NULL: nowhere)
    void SetPane(nw::lyt::Pane* pane); // 0x002E0948
    // the disabled look (the button is inactive)
    void SetDisabled(bool disabled); // 0x002E09F4
    // updates the text and animates the layout of the button
    void AnimateLayout(); // 0x002E0E94
    // whether an in/out animation runs
    bool IsBusy() const { return m_pState != &CommonButtonSingle::State_Idle; } // 0x0072122C

    void State_In(); // 0x002E07B8
    void State_Out(); // 0x002E0878
    void State_Idle(); // 0x002E0F5C

    // 0x009AC084
    static ssys::ma::LoadSplit s_Archive;

    ssys::ma::lyt::ArcResAcc m_ResAcc;        // 0x0E0
    ::Layout m_ButtonLayout;                  // 0x238
    ssys::ma::lyt::Animation m_InAnim;        // 0x384 cmn_btn_in
    ssys::ma::lyt::Animation m_OutAnim;       // 0x3AC cmn_btn_out
    ssys::ma::lyt::Animation m_NgAnim;        // 0x3D4 cmn_btn_ng (disabled)
    nw::lyt::TextBox* m_pText;                // 0x3FC
    nw::lyt::TextBox* m_pShadow;              // 0x400
    script::WordFix<16> m_Word;               // 0x404
    script::RenderGarden m_Garden;            // 0x43C
    nw::lyt::Pane* m_pParentPane;             // 0x500
    nw::lyt::Pane* m_pAll;                    // 0x504 N_all
    bool m_IsVisible;                         // 0x508
    StateFunc m_pState;                       // 0x50C
    u8 m_Unknown0x514;                        // 0x514 (never accessed)
    bool m_IsSetup;                           // 0x515
    bool m_IsBack;                            // 0x516 the back button (no text)
    bool m_IsDisabled;                        // 0x517
};
ASSERT_OFFSET(CommonButtonSingle, m_ResAcc, 0xE0);
ASSERT_OFFSET(CommonButtonSingle, m_ButtonLayout, 0x238);
ASSERT_OFFSET(CommonButtonSingle, m_Word, 0x404);
ASSERT_OFFSET(CommonButtonSingle, m_Garden, 0x43C);
ASSERT_OFFSET(CommonButtonSingle, m_pState, 0x50C);
ASSERT_SIZE(CommonButtonSingle, 0x518);
