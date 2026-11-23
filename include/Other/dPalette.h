#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Button/dButtonActionNode.h"
#include "Other/dLayout.h"
#include "Other/dPickerBase.h"
#include "Other/dPickerMono.h"
#include "Other/dPickerParts.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "script/dRenderGarden.h"
#include "sead/seadWFixedSafeString.h"
#include "ssys/ma/dHeapAllocator.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "state/dMode.h"

namespace nw {
namespace lyt {
class Pane;
class Picture;
class TextBox;
} // namespace lyt
} // namespace nw
namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
} // namespace lyt
} // namespace ma
} // namespace ssys

// The palette of the design editor (BsMenuEdit; class name from the RTTI, the state names from the table of the binary,
// all other names are ours): the 15 colors of the edited design (BsMenuEdit::s_EditDesign) as buttons B_color_NN in
// edit_palt.bclyt, and two sub screens that change the color of the selected one: the color picker (16 pickers of 3x3
// colors of one hue and one of the 15 shades without hue, touched in the area B_pick) and the sample palettes (the 12
// palettes of SvMyDesign, paged with the arrows B_arrow_L/R). Both close with the buttons B_btn_L (cancel: the old colors
// come back) and B_btn_R (decide) of edit_btn.bclyt.
// RTTI 7Palette @ 0x008CD3D4
// vtable 0x008F9184 (vptr 0x008F918C), offset_to_top 0, 4 entries
class Palette : public ::state::Mode<Palette>
{
public:
    // the buttons of the two controls (the pane and group names in s_ButtonPaneNames)
    enum ButtonIndex
    {
        BUTTON_COLOR_NUM = 15,  // 0..14 B_color_NN: the colors of the palette
        BUTTON_PICKER = 15,     // B_btn_00: opens the color picker
        BUTTON_SAMPLES = 16,    // B_btn_01: opens the sample palettes
        BUTTON_ARROW_L = 17,    // the previous sample palette
        BUTTON_ARROW_R = 18,    // the next sample palette
        BUTTON_CANCEL = 19,     // B_btn_L (edit_btn.bclyt)
        BUTTON_DECIDE = 20,     // B_btn_R (edit_btn.bclyt)
        BUTTON_NUM = 21,
    };
    // the open sub screen (m_SubScreen)
    enum SubScreen
    {
        SUB_SCREEN_NONE = 0,
        SUB_SCREEN_PICKER = 1,
        SUB_SCREEN_SAMPLES = 2,
    };
    static const s32 PICKER_PARTS_NUM = 16;
    static const s32 PICKER_NUM = PICKER_PARTS_NUM + 1;
    // the sample palettes: 1..12 = SvMyDesign::GetPaletteColor(n - 1), 0 = the own colors of the design (unsigned: the
    // original compares m_SamplePalette > 12 unsigned)
    static const u32 SAMPLE_PALETTE_NUM = 12;

    Palette(); // 0x00611804
    // 0x00611A00 (deleting dtor)
    ~Palette() override; // 0x00611AC8 slot 0x00

    // whether a color button (or arrow) is under the cursor
    virtual bool IsColorButtonCurrent() const { return m_ColorControl.HasCurrent(); } // 0x0075E96C slot 0x0C

    // the layouts, the animations, the texts, the buttons and the pickers; the machine starts selecting a color
    void Setup(ssys::ma::lyt::ArcResAccReader* reader); // 0x0060F7F0

    // the states (the names in the comments are the debug names of the binary)
    void State_SelectColor();   // 0x00610640 色選択処理 (selecting a color)
    void Enter_SelectColor();   // 0x006107B4
    void State_ButtonsIn();     // 0x006107B8 ボタン入場 (the buttons come in)
    void Enter_ButtonsIn();     // 0x00610868
    void State_None();          // 0x0061063C 何もしない (nothing)
    void Enter_None();          // 0x006107B0
    void State_MainIn();        // 0x00611400 メイン入場処理 (the palette comes in)
    void Enter_MainIn();        // 0x00611650
    void State_MainOut();       // 0x00611774 メイン退場処理 (the palette goes out)
    void Enter_MainOut();       // 0x006117CC
    void State_SamplesIn();     // 0x006110F8 サンプルパレット入場 (the sample palettes come in)
    void Enter_SamplesIn();     // 0x006112BC
    void State_SamplesOut();    // 0x00611570 サンプルパレット退場 (the sample palettes go out)
    void Enter_SamplesOut();    // 0x006116E4
    void State_Samples();       // 0x00610D5C サンプル処理 (the sample palettes)
    void Enter_Samples();       // 0x00611080
    void State_PickerIn();      // 0x006110A8 カラーピッカー入場 (the color picker comes in)
    void Enter_PickerIn();      // 0x00611140
    void State_PickerOut();     // 0x0061146C カラーピッカー退場 (the color picker goes out)
    void Enter_PickerOut();     // 0x00611688
    void State_Picker();        // 0x006109E4 カラーピッカー処理 (the color picker)
    void Enter_Picker();        // 0x00611060

private:
    // the texture of the colors of the pickers, the pickers and the colors before a sub screen opens
    void Initialize(ssys::ma::lyt::ArcResAccReader* reader); // 0x00610108
    // a touch in the area of the pickers picks the color under it for the selected button
    void UpdatePickTouch(); // 0x0061035C
    // the colors of the pictures P_color_NN (all or the selected one)
    void UpdateColorPictures(bool isAll); // 0x006108BC

    // (inline) the cursor of the pickers at m_PickedColor
    void MoveCursorToPickedColor();
    // (inline) m_PickedColor becomes the color of the selected button (with the sound)
    void ApplyPickedColor();
    // (inline) the button index becomes the selected one
    void SelectColorButton(s32 index);
    // (inline) the selected button hides its selection while the cursor is on a button
    void UpdateSelectionVisibility();
    // (inline) the controls block each other while one of them chooses a button
    void UpdateControls();
    // (inline) the page of the sample palette in T_page
    void UpdatePageText();
    // (inline) the colors before the sub screen opens, and their return
    void SaveColors();
    void RestoreColors();
    // (inline) the buttons of edit_btn.bclyt go out with a sub screen
    void UpdateButtonOut();
    // (inline) cancel or decide of a sub screen; whether one was chosen
    bool UpdateSubScreenButtons();

    static const Info s_StateInfos[11];
    // the panes and groups of the buttons (the group of B_btn_R is "G_btr_R" in the binary)
    static const char* s_ButtonPaneNames[BUTTON_NUM];
    static const char* s_ButtonGroupNames[BUTTON_NUM];

public:
    ssys::ma::HeapAllocator m_Allocator;           // 0x0024 "BsMenuEdit" (the texture of the pickers)
    ::Layout m_PaletteLayout;                       // 0x002C edit_palt.bclyt
    ::Layout m_ButtonLayout;                        // 0x0178 edit_btn.bclyt
    ssys::ma::lyt::Animation m_InAnim;              // 0x02C4 edit_palt_in
    ssys::ma::lyt::Animation m_OutAnim;             // 0x02EC edit_palt_out
    ssys::ma::lyt::Animation m_InPickAnim;          // 0x0314 edit_palt_in_pick
    ssys::ma::lyt::Animation m_OutPickAnim;         // 0x033C edit_palt_out_pick
    ssys::ma::lyt::Animation m_InSetAnim;           // 0x0364 edit_palt_in_set (the sample palettes)
    ssys::ma::lyt::Animation m_OutSetAnim;          // 0x038C edit_palt_out_set
    ssys::ma::lyt::Animation m_ButtonInAnim;        // 0x03B4 edit_btn_in
    ssys::ma::lyt::Animation m_ButtonOutAnim;       // 0x03DC edit_btn_out
    nw::gfx::res::ResImageTexture m_PickerTexture;  // 0x0404 16x16 RGBA4: the colors of the pickers
    PickerCursor m_PickerCursor;                    // 0x0408 edit_pick.bclyt
    // set in each frame of the color picker (also while it comes in and goes out; read by BsMenuEdit)
    bool m_IsPickerActive;                          // 0x055C
    PickerParts m_PickerParts[PICKER_PARTS_NUM];    // 0x0560
    PickerMono m_PickerMono;                        // 0x08A0
    PickerBase* m_pPickers[PICKER_NUM];             // 0x08D0
    ButtonActionNode m_Buttons[BUTTON_NUM];         // 0x0914
    ButtonActionControl m_ColorControl;             // 0x1B74 the buttons 0..18
    ButtonActionControl m_SubControl;               // 0x1C58 cancel and decide
    nw::lyt::Pane* m_pPickArea;                     // 0x1D3C B_pick
    nw::lyt::Picture* m_pColorPictures[BUTTON_COLOR_NUM]; // 0x1D40 P_color_NN
    nw::lyt::TextBox* m_pPageText;                  // 0x1D7C T_page
    sead::WFixedSafeString<6> m_PageString;         // 0x1D80
    script::RenderGarden m_DecideText;              // 0x1D98 S_btn_R/T_btn_R
    script::RenderGarden m_CancelText;              // 0x1E5C S_btn_L/T_btn_L
    s32 m_SelectedButton;                           // 0x1F20 the selected color button
    u32 m_PickedColor;                              // 0x1F24 the color (index of the color table) of the pickers
    s32 m_SamplePalette;                            // 0x1F28
    s32 m_MinSamplePalette;                         // 0x1F2C 0 when the colors were own ones, else 1
    s32 m_SavedSamplePalette;                       // 0x1F30 before the sub screen opened
    u8 m_SavedColors[BUTTON_COLOR_NUM];             // 0x1F34
    u32 m_TouchStartColor;                          // 0x1F44 the picked color when the touch began
    bool m_IsAnimating;                             // 0x1F48 an in/out animation runs
    bool m_IsCursorOnButton;                        // 0x1F49 (the selected button hides its selection)
    bool m_IsPickTouching;                          // 0x1F4A a touch began in the area of the pickers
    bool m_IsColorChanged;                          // 0x1F4B a color of the design changed
    u8 m_SubScreen;                                 // 0x1F4C SubScreen
};
ASSERT_OFFSET(Palette, m_Allocator, 0x24);
ASSERT_OFFSET(Palette, m_PickerTexture, 0x404);
ASSERT_OFFSET(Palette, m_IsPickerActive, 0x55C);
ASSERT_OFFSET(Palette, m_Buttons, 0x914);
ASSERT_OFFSET(Palette, m_ColorControl, 0x1B74);
ASSERT_OFFSET(Palette, m_pPickArea, 0x1D3C);
ASSERT_OFFSET(Palette, m_DecideText, 0x1D98);
ASSERT_OFFSET(Palette, m_SelectedButton, 0x1F20);
ASSERT_OFFSET(Palette, m_TouchStartColor, 0x1F44);
ASSERT_SIZE(Palette, 0x1F50);
