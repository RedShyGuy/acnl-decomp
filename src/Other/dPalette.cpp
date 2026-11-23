#include "Other/dPalette.h"

#include "Bs/dBsMenuEdit.h"
#include "Other/dFormatBuffer.h"
#include "Other/dHeapInit.h"
#include "Other/dPaneVisible.h"
#include "Other/dTouchState.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvMyDesign.h"
#include "nn/math/math_ARMv6.h"
#include "sead/seadExpHeap.h"
#include "nw/lyt/lyt_Api.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Picture.h"
#include "nw/lyt/lyt_TextBox.h"
#include "nw/ut/ut_Color8.h"
#include "nw/ut/ut_Rect.h"
#include "script/dUiMgr.h"
#include "script/dWordFix.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

namespace {
// the pan of the sounds: the x position of the picker cursor / 160
const f32 PAN_SCALE = 0.00625f;
// the command cache sizes of the layouts
const u32 PALETTE_CMD_CACHE_SIZE = 4;
const u32 PICKER_CMD_CACHE_SIZE = 0x1000;
// the texts of the buttons of edit_btn.bclyt in SYS_2D_UI
const u32 TEXT_DECIDE = 304;
const u32 TEXT_CANCEL = 305;
// the buttons of edit_palt.bclyt (the others are in edit_btn.bclyt)
const s32 PALETTE_BUTTON_NUM = Palette::BUTTON_ARROW_R + 1;
// the frames of the cursor before repeating
const s32 CURSOR_TIMER = 9;

// (inline) whether a control chooses a button
inline bool IsChoosing(const ButtonActionControl& control)
{
    return control.IsSelectOkDone() || control.IsSelectOkOngoing();
}
} // namespace

// the states with their names (Japanese, debug); the machine starts selecting a color
// 0x0084A80C
const Palette::Info Palette::s_StateInfos[11] = {
    {"色選択処理", &Palette::Enter_SelectColor, &Palette::State_SelectColor},
    {"ボタン入場", &Palette::Enter_ButtonsIn, &Palette::State_ButtonsIn},
    {"何もしない", &Palette::Enter_None, &Palette::State_None},
    {"メイン入場処理", &Palette::Enter_MainIn, &Palette::State_MainIn},
    {"メイン退場処理", &Palette::Enter_MainOut, &Palette::State_MainOut},
    {"サンプルパレット入場", &Palette::Enter_SamplesIn, &Palette::State_SamplesIn},
    {"サンプルパレット退場", &Palette::Enter_SamplesOut, &Palette::State_SamplesOut},
    {"サンプル処理", &Palette::Enter_Samples, &Palette::State_Samples},
    {"カラーピッカー入場", &Palette::Enter_PickerIn, &Palette::State_PickerIn},
    {"カラーピッカー退場", &Palette::Enter_PickerOut, &Palette::State_PickerOut},
    {"カラーピッカー処理", &Palette::Enter_Picker, &Palette::State_Picker},
};

// 0x009508A8
const char* Palette::s_ButtonPaneNames[BUTTON_NUM] = {
    "B_color_00", "B_color_01", "B_color_02", "B_color_03", "B_color_04", "B_color_05", "B_color_06",
    "B_color_07", "B_color_08", "B_color_09", "B_color_10", "B_color_11", "B_color_12", "B_color_13",
    "B_color_14", "B_btn_00",   "B_btn_01",   "B_arrow_L",  "B_arrow_R",  "B_btn_L",    "B_btn_R",
};
// 0x009508FC
const char* Palette::s_ButtonGroupNames[BUTTON_NUM] = {
    "G_color_00", "G_color_01", "G_color_02", "G_color_03", "G_color_04", "G_color_05", "G_color_06",
    "G_color_07", "G_color_08", "G_color_09", "G_color_10", "G_color_11", "G_color_12", "G_color_13",
    "G_color_14", "G_btn_00",   "G_btn_01",   "G_arrow_L",  "G_arrow_R",  "G_btn_L",    "G_btr_R",
};

// 0x00611804
Palette::Palette()
    : m_IsPickerActive(false), m_pPickArea(NULL), m_pPageText(NULL), m_DecideText(16), m_CancelText(16),
      m_SelectedButton(0), m_PickedColor(0), m_SamplePalette(0), m_MinSamplePalette(0), m_SavedSamplePalette(0),
      m_TouchStartColor(0), m_IsAnimating(false), m_IsCursorOnButton(false), m_IsPickTouching(false),
      m_IsColorChanged(false), m_SubScreen(SUB_SCREEN_NONE)
{
}

// 0x00611AC8 slot 0x00
// 0x00611A00 (deleting dtor)
Palette::~Palette()
{
    // (only the members are destroyed)
}

inline void Palette::UpdatePageText()
{
    m_PageString.format(L"%02d/16", m_SamplePalette);
    m_pPageText->SetString(m_PageString.getCString(), 0);
}

// 0x0060F7F0
void Palette::Setup(ssys::ma::lyt::ArcResAccReader* reader)
{
    m_PaletteLayout.m_pArcReader = reader;
    m_PaletteLayout.Build("edit_palt.bclyt", NULL, PALETTE_CMD_CACHE_SIZE);
    m_InAnim.Initialize("edit_palt_in.bclan", reader);
    m_OutAnim.Initialize("edit_palt_out.bclan", reader);
    m_InPickAnim.Initialize("edit_palt_in_pick.bclan", reader);
    m_OutPickAnim.Initialize("edit_palt_out_pick.bclan", reader);
    m_InSetAnim.Initialize("edit_palt_in_set.bclan", reader);
    m_OutSetAnim.Initialize("edit_palt_out_set.bclan", reader);
    m_PaletteLayout.FindPane("N_arrow_L")->m_Alpha = 0;
    m_PaletteLayout.FindPane("N_arrow_R")->m_Alpha = 0;
    m_pPickArea = m_PaletteLayout.FindPane("B_pick");
    m_pPageText = m_PaletteLayout.FindTextBox("T_page");
    // the palette starts out of the screen
    m_PaletteLayout.BindAnimation(&m_InAnim);
    m_InAnim.SetLastFrame();
    m_PaletteLayout.Animate();
    m_PaletteLayout.UnbindAnimation(&m_InAnim);

    m_ButtonLayout.m_pArcReader = reader;
    g_FormatBuffer.format("%s.bclyt", "edit_btn");
    m_ButtonLayout.Build(g_FormatBuffer.getCString(), NULL, PALETTE_CMD_CACHE_SIZE);
    g_FormatBuffer.format("%s_%s.bclan", "edit_btn", "in");
    m_ButtonInAnim.Initialize(g_FormatBuffer.getCString(), reader);
    g_FormatBuffer.format("%s_%s.bclan", "edit_btn", "out");
    m_ButtonOutAnim.Initialize(g_FormatBuffer.getCString(), reader);
    g_FormatBuffer.format("%s_W", "S_btn_R");
    nw::lyt::TextBox* shadow = m_ButtonLayout.FindTextBox(g_FormatBuffer.getCString());
    g_FormatBuffer.format("%s_W", "T_btn_R");
    m_DecideText.SetTextBoxes(m_ButtonLayout.FindTextBox(g_FormatBuffer.getCString()), shadow);
    g_FormatBuffer.format("%s_W", "S_btn_L");
    shadow = m_ButtonLayout.FindTextBox(g_FormatBuffer.getCString());
    g_FormatBuffer.format("%s_W", "T_btn_L");
    m_CancelText.SetTextBoxes(m_ButtonLayout.FindTextBox(g_FormatBuffer.getCString()), shadow);
    script::WordFix<32> word;
    script::UiMgr::s_pInstance->Begin(NULL);
    script::UiMgr::s_pInstance->ReadWord(&word, "SYS_2D_UI", TEXT_DECIDE, 0);
    m_DecideText.SetText(&word);
    m_DecideText.CommitText();
    script::UiMgr::s_pInstance->ReadWord(&word, "SYS_2D_UI", TEXT_CANCEL, 0);
    m_CancelText.SetText(&word);
    m_CancelText.CommitText();
    script::UiMgr::s_pInstance->End();

    Initialize(reader);

    ButtonActionControl::Description colorDesc = {reader,
                                                  &m_PaletteLayout,
                                                  NULL,
                                                  "edit_palt_touch.bclan",
                                                  "edit_palt_select.bclan",
                                                  "edit_palt_touch_ok.bclan",
                                                  "edit_palt_select_ok.bclan",
                                                  CURSOR_TIMER,
                                                  NULL};
    colorDesc.bounding = static_cast<nw::lyt::Bounding*>(m_PaletteLayout.FindPane("W_palt_00"));
    m_ColorControl.Initialize(colorDesc, NULL);
    m_ColorControl.m_Flags2 = ButtonActionControl::ENABLE_TOUCH;
    ButtonActionControl::Description subDesc = {reader,
                                                &m_ButtonLayout,
                                                NULL,
                                                "edit_btn_touch.bclan",
                                                "edit_btn_select.bclan",
                                                "edit_btn_touch_ok.bclan",
                                                "edit_btn_select_ok.bclan",
                                                CURSOR_TIMER,
                                                NULL};
    m_SubControl.Initialize(subDesc, NULL);
    m_SubControl.m_Flags2 = ButtonActionControl::ENABLE_TOUCH;

    ButtonActionNode::Info info = {NULL, NULL, nn::math::VEC2(0.0f, 0.0f), 0, -1, NULL};
    for (s32 i = 0; i < BUTTON_NUM; i++) {
        if (i < PALETTE_BUTTON_NUM) {
            info.pane = m_PaletteLayout.FindBounding(s_ButtonPaneNames[i]);
            info.group = m_PaletteLayout.FindGroup(s_ButtonGroupNames[i]);
            info.index = i;
            m_Buttons[i].Initialize(info);
            if (i < BUTTON_COLOR_NUM) {
                m_Buttons[i].SetSeOk(SE_SYS_EDIT_COLOR_SELECTED);
                m_Buttons[i].SetOkSoundEnabled(true);
                m_Buttons[i].SetSe(SE_SYS_EDIT_COLOR_ACTIVE);
            } else if (i < BUTTON_ARROW_L) {
                m_Buttons[i].SetSeOk(SE_SYS_DECIDE_S);
                m_Buttons[i].SetOkSoundEnabled(true);
                m_Buttons[i].SetSe(SE_SYS_EDIT_BTN_ACTIVE);
            } else {
                if (i == BUTTON_ARROW_L) {
                    m_Buttons[i].SetSeOk(SE_SYS_EDIT_PALETTE_DEC);
                    m_Buttons[i].SetOkSoundEnabled(true);
                } else {
                    m_Buttons[i].SetSeOk(SE_SYS_EDIT_PALETTE_INC);
                    m_Buttons[i].SetOkSoundEnabled(true);
                }
                m_Buttons[i].SetSe(SE_SYS_BTN_ACTIVE_S);
            }
            m_Buttons[i].SetSoundEnabled(true);
            m_Buttons[i].m_SoundMode = true;
            m_ColorControl.AddNode(&m_Buttons[i], true);
            // the arrows only work with the sample palettes
            if (i >= BUTTON_ARROW_L) {
                m_Buttons[i].Deactivate();
            }
        } else {
            info.pane = m_ButtonLayout.FindBounding(s_ButtonPaneNames[i]);
            info.group = m_ButtonLayout.FindGroup(s_ButtonGroupNames[i]);
            info.index = i;
            m_Buttons[i].Initialize(info);
            m_Buttons[i].SetSe(SE_SYS_BTN_ACTIVE);
            m_Buttons[i].SetSoundEnabled(true);
            m_Buttons[i].m_SoundMode = true;
            if (i == BUTTON_CANCEL) {
                m_Buttons[i].SetSeOk(SE_SYS_CANCEL);
                m_Buttons[i].SetOkSoundEnabled(true);
            } else {
                m_Buttons[i].SetSeOk(SE_SYS_DECIDE);
                m_Buttons[i].SetOkSoundEnabled(true);
            }
            m_SubControl.AddNode(&m_Buttons[i], true);
        }
    }

    // (the original indexes the selected button with m_PickedColor here; both are 0)
    m_PickedColor = 0;
    m_Buttons[m_PickedColor].Select(true, false);
    m_Buttons[m_PickedColor].Select(true);
    m_Buttons[m_PickedColor].Deactivate();
    SetFlagVisible(m_ButtonLayout.GetRootPane(), false);
    m_pPageText->AllocStringBuffer(6, 0);
    UpdatePageText();
    m_SubScreen = SUB_SCREEN_NONE;
    Start("BsMenuEdit::Palette", s_StateInfos, 11);
}

// 0x00610108
void Palette::Initialize(ssys::ma::lyt::ArcResAccReader* reader)
{
    m_Allocator.CreateExpHeap(51200, HeapInit::s_pStageHeap, sead::SafeStringBase<char>("BsMenuEdit"),
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
    nw::gfx::res::ResImageTexture::DynamicDescription description;
    description.height = 16;
    description.width = 16;
    description.mipmapLevels = 1;
    description.locationFlag = 0x2020000;
    description.pLocationAddress = NULL;
    description.format = 4;
    description.isCleared = false;
    description.isAllocated = true;
    m_PickerTexture = nw::gfx::res::ResImageTexture::CreateDynamic(description, &m_Allocator);
    UpdateColorPictures(true);

    // the sample palette that has the colors of the design (0: none)
    m_SavedSamplePalette = m_SamplePalette;
    for (u32 i = 0; i < BUTTON_COLOR_NUM; i++) {
        m_SavedColors[i] = BsMenuEdit::s_EditDesign.GetColor(i);
    }
    m_SamplePalette = 0;
    for (u32 palette = 0; palette < SAMPLE_PALETTE_NUM; palette++) {
        for (u32 i = 0; i < BUTTON_COLOR_NUM; i++) {
            if (SvMyDesign::GetPaletteColor(palette, i) != m_SavedColors[i]) {
                m_SamplePalette++;
                break;
            }
        }
        if (static_cast<u32>(m_SamplePalette) == palette) {
            break;
        }
    }
    m_SamplePalette = (m_SamplePalette + 1) % (SAMPLE_PALETTE_NUM + 1);

    m_PickerCursor.m_Layout.m_pArcReader = reader;
    m_PickerCursor.m_Layout.Build("edit_pick.bclyt", NULL, PICKER_CMD_CACHE_SIZE);
    m_PickerCursor.m_pAll = m_PickerCursor.m_Layout.FindPane("N_All");
    m_PickerCursor.m_pPickMaterial = m_PickerCursor.m_Layout.FindPicture("P_pick")->GetMaterial(0);
    for (s32 i = 0; i < PICKER_PARTS_NUM; i++) {
        m_pPickers[i] = &m_PickerParts[i];
    }
    m_pPickers[PICKER_PARTS_NUM] = &m_PickerMono;
    PickerBase::SetupArg arg = {reader, &m_PaletteLayout, &m_PickerCursor, NULL, 0};
    for (s32 i = 0; i < PICKER_NUM; i++) {
        arg.pTexture = m_PickerTexture.ptr();
        arg.index = i;
        m_pPickers[i]->Setup(arg);
    }
}

inline void Palette::MoveCursorToPickedColor()
{
    PickerBase::s_ColorEntries[m_PickedColor].pPicker->SetCursor(PickerBase::s_ColorEntries[m_PickedColor].index);
    PickerBase::s_ColorEntries[m_PickedColor].pPicker->Pick();
}

inline void Palette::ApplyPickedColor()
{
    BsMenuEdit::s_EditDesign.SetPaletteColor(m_SelectedButton, m_PickedColor);
    UpdateColorPictures(false);
    m_IsColorChanged = true;
    SoundMgr::PlaySound(SE_SYS_EDIT_ASSIGNED_COLOR_CHANGE, m_PickerCursor.m_pAll->m_GlbMtx.m[0][3] * PAN_SCALE);
}

// 0x0061035C
void Palette::UpdatePickTouch()
{
    if (!g_TouchState.isTouch) {
        if (m_IsPickTouching) {
            m_IsPickTouching = false;
        }
        return;
    }
    if (g_TouchState.prevIsTouch && !m_IsPickTouching) {
        return;
    }
    nn::math::VEC2 pos;
    m_PaletteLayout.ScreenToLayout(nn::math::VEC2(g_TouchState.lastX, g_TouchState.lastY), &pos);
    const bool isInArea = nw::lyt::IsContain(m_pPickArea, pos);
    if (!g_TouchState.prevIsTouch) {
        // a touch begins
        if (!isInArea) {
            return;
        }
        m_IsPickTouching = true;
        m_TouchStartColor = m_PickedColor;
    } else if (!isInArea) {
        // the touch left the area: the color at its beginning comes back
        if (m_PickedColor == m_TouchStartColor) {
            return;
        }
        m_PickedColor = m_TouchStartColor;
        MoveCursorToPickedColor();
        ApplyPickedColor();
        return;
    }
    for (s32 i = 0; i < PICKER_NUM; i++) {
        PickerBase* picker = m_pPickers[i];
        nn::math::MTX34 inverse;
        nn::math::ARMv6::MTX34InverseAsm(&inverse, &picker->m_pPicture->m_GlbMtx);
        const nn::math::VEC3 touch(pos.x, pos.y, 0.0f);
        nn::math::ARMv6::VEC3TransformAsm(&picker->m_Cursor, &inverse, &touch);
        const nw::ut::Rect rect = picker->m_pPicture->GetPaneRect();
        if (!(rect.left < picker->m_Cursor.x && picker->m_Cursor.x < rect.right)) {
            continue;
        }
        if (!(rect.bottom < picker->m_Cursor.y && picker->m_Cursor.y < rect.top)) {
            continue;
        }
        // 0..1 from the left and the top of the picture
        const nw::lyt::Size& size = picker->m_pPicture->m_Size;
        picker->m_Cursor.x = (picker->m_Cursor.x + size.width * 0.5f) / size.width;
        picker->m_Cursor.y = (size.height * 0.5f - picker->m_Cursor.y) / size.height;
        if (m_pPickers[i]->Pick() == m_PickedColor) {
            return;
        }
        m_PickedColor = m_pPickers[i]->Pick();
        ApplyPickedColor();
        return;
    }
}

// 0x0061063C
void Palette::State_None()
{
    // empty (in the original too)
}

inline void Palette::SelectColorButton(s32 index)
{
    m_Buttons[m_SelectedButton].Activate();
    m_SelectedButton = index;
    m_Buttons[index].Deactivate();
    m_IsCursorOnButton = false;
    m_ColorControl.Reset();
}

inline void Palette::UpdateSelectionVisibility()
{
    if (m_ColorControl.HasCurrent()) {
        if (!m_IsCursorOnButton) {
            if (m_ColorControl.GetSelectedIndex() < BUTTON_COLOR_NUM) {
                m_Buttons[m_SelectedButton].Select(false, false);
                m_Buttons[m_SelectedButton].Select(false);
            }
            m_IsCursorOnButton = true;
        }
    } else if (m_IsCursorOnButton) {
        m_Buttons[m_SelectedButton].Select(true, false);
        m_IsCursorOnButton = false;
    }
}

// 0x00610640
void Palette::State_SelectColor()
{
    m_ColorControl.Update();
    if (m_ColorControl.IsSelectOkDone()) {
        const s32 index = m_ColorControl.GetPressedIndex();
        if (index < BUTTON_COLOR_NUM) {
            SelectColorButton(index);
        } else if (index == BUTTON_PICKER) {
            Change(&Palette::State_PickerIn);
        } else if (index == BUTTON_SAMPLES) {
            Change(&Palette::State_SamplesIn);
        }
        return;
    }
    UpdateSelectionVisibility();
}

// 0x006107B0
void Palette::Enter_None()
{
    // empty (in the original too)
}

// 0x006107B4
void Palette::Enter_SelectColor()
{
    // empty (in the original too)
}

// 0x006107B8
void Palette::State_ButtonsIn()
{
    if (m_ButtonInAnim.IsDone()) {
        m_ButtonLayout.UnbindAnimation(&m_ButtonInAnim);
        if (m_SubScreen == SUB_SCREEN_PICKER) {
            Change(&Palette::State_Picker);
        } else {
            Change(&Palette::State_Samples);
        }
        m_IsAnimating = false;
        m_ColorControl.Reset();
    } else {
        m_ButtonInAnim.IncreaseFrame();
    }
    if (m_SubScreen == SUB_SCREEN_PICKER) {
        m_IsPickerActive = true;
    }
}

// 0x00610868
void Palette::Enter_ButtonsIn()
{
    m_ButtonLayout.BindAnimation(&m_ButtonInAnim);
    m_ButtonInAnim.SetFrame(0.0f);
    SetFlagVisible(m_ButtonLayout.GetRootPane(), true);
    m_SubControl.Reset();
    m_SubControl.UnselectActive();
}

// 0x006108BC
void Palette::UpdateColorPictures(bool isAll)
{
    s32 i;
    s32 end;
    if (isAll) {
        i = 0;
        end = BUTTON_COLOR_NUM;
    } else {
        i = m_SelectedButton;
        end = i + 1;
    }
    for (; i < end; i++) {
        g_FormatBuffer.format("%s_%02d", "P_color", i);
        m_pColorPictures[i] = m_PaletteLayout.FindPicture(g_FormatBuffer.getCString());
        const u16 value = SvMyDesign::GetColorValue(BsMenuEdit::s_EditDesign.GetColor(i), true);
        nw::lyt::Material* material = m_pColorPictures[i]->GetMaterial(0);
        // (the original fills the low nibbles of green and blue with the one of red)
        const u8 r = ((value & 0xF000) >> 8) | (((value & 0xF000) >> 8) >> 4);
        const u8 g = ((value & 0x0F00) >> 4) | (r >> 4);
        const u8 b = (value & 0x00F0) | (r >> 4);
        material->m_Colors[0] = nw::ut::Color8(r, g, b, 0);
    }
}

inline void Palette::UpdateControls()
{
    m_ColorControl.SetFlags(ButtonActionControl::DISABLED, IsChoosing(m_SubControl));
    m_ColorControl.Update();
    m_SubControl.SetFlags(ButtonActionControl::DISABLED, IsChoosing(m_ColorControl));
    m_SubControl.Update();
}

inline void Palette::RestoreColors()
{
    m_SamplePalette = m_SavedSamplePalette;
    for (u32 i = 0; i < BUTTON_COLOR_NUM; i++) {
        BsMenuEdit::s_EditDesign.SetPaletteColor(i, m_SavedColors[i]);
    }
    UpdateColorPictures(true);
    m_IsColorChanged = true;
}

inline bool Palette::UpdateSubScreenButtons()
{
    if (!m_SubControl.IsSelectOkDone()) {
        return false;
    }
    bool isDecided;
    if (m_SubControl.GetPressedIndex() == BUTTON_DECIDE) {
        isDecided = true;
    } else {
        isDecided = false;
        RestoreColors();
    }
    if (m_SubScreen == SUB_SCREEN_PICKER) {
        // picked colors are own ones
        if (isDecided) {
            m_SamplePalette = 0;
        }
        Change(&Palette::State_PickerOut);
    } else if (m_SubScreen == SUB_SCREEN_SAMPLES) {
        Change(&Palette::State_SamplesOut);
    }
    return true;
}

// 0x006109E4
void Palette::State_Picker()
{
    UpdateControls();
    if (m_ColorControl.IsSelectOkDone()) {
        const s32 index = m_ColorControl.GetPressedIndex();
        if (index < BUTTON_COLOR_NUM) {
            SelectColorButton(index);
            m_PickedColor = BsMenuEdit::s_EditDesign.GetColor(m_SelectedButton);
            MoveCursorToPickedColor();
        }
    } else if (m_ColorControl.HasCurrent() || m_IsCursorOnButton) {
        UpdateSelectionVisibility();
    } else if (UpdateSubScreenButtons()) {
        return;
    }
    if (!IsChoosing(m_SubControl) && !IsChoosing(m_ColorControl)) {
        UpdatePickTouch();
    }
    m_IsPickerActive = true;
}

// 0x00610D5C
void Palette::State_Samples()
{
    UpdateControls();
    if (m_ColorControl.IsSelectOkDone()) {
        const s32 old = m_SamplePalette;
        if (m_ColorControl.GetPressedIndex() == BUTTON_ARROW_L) {
            if (--m_SamplePalette < m_MinSamplePalette) {
                m_SamplePalette = SAMPLE_PALETTE_NUM;
            }
        } else if (m_ColorControl.GetPressedIndex() == BUTTON_ARROW_R) {
            if (static_cast<u32>(++m_SamplePalette) > SAMPLE_PALETTE_NUM) {
                m_SamplePalette = m_MinSamplePalette;
            }
        }
        if (m_SamplePalette != old) {
            if (m_SamplePalette == 0) {
                for (u32 i = 0; i < BUTTON_COLOR_NUM; i++) {
                    BsMenuEdit::s_EditDesign.SetPaletteColor(i, m_SavedColors[i]);
                }
            } else {
                BsMenuEdit::s_EditDesign.SetPalette(m_SamplePalette - 1);
            }
            UpdateColorPictures(true);
            UpdatePageText();
            m_IsColorChanged = true;
        }
        m_ColorControl.UnselectCurrent();
        m_ColorControl.Reset();
        return;
    }
    UpdateSubScreenButtons();
}

// 0x00611060
void Palette::Enter_Picker()
{
    m_IsPickTouching = false;
    m_IsCursorOnButton = false;
}

// 0x00611080
void Palette::Enter_Samples()
{
    // the own colors (0) can be paged to only when the design had them
    m_MinSamplePalette = m_SamplePalette > 1 ? 1 : m_SamplePalette;
}

// 0x006110A8
void Palette::State_PickerIn()
{
    if (m_InPickAnim.IsDone()) {
        m_PaletteLayout.UnbindAnimation(&m_InPickAnim);
        Change(&Palette::State_ButtonsIn);
    } else {
        m_InPickAnim.IncreaseFrame();
    }
    m_IsPickerActive = true;
}

// 0x006110F8
void Palette::State_SamplesIn()
{
    if (m_InSetAnim.IsDone()) {
        m_PaletteLayout.UnbindAnimation(&m_InSetAnim);
        Change(&Palette::State_ButtonsIn);
        return;
    }
    m_InSetAnim.IncreaseFrame();
}

inline void Palette::SaveColors()
{
    m_SavedSamplePalette = m_SamplePalette;
    for (u32 i = 0; i < BUTTON_COLOR_NUM; i++) {
        m_SavedColors[i] = BsMenuEdit::s_EditDesign.GetColor(i);
    }
}

// 0x00611140
void Palette::Enter_PickerIn()
{
    SaveColors();
    UpdatePageText();
    m_PaletteLayout.BindAnimation(&m_InPickAnim);
    m_InPickAnim.SetFrame(0.0f);
    m_Buttons[BUTTON_PICKER].Select(false);
    m_Buttons[m_SelectedButton].Select(true, false);
    m_Buttons[m_SelectedButton].Deactivate();
    m_Buttons[BUTTON_SAMPLES].Deactivate();
    m_Buttons[BUTTON_PICKER].Deactivate();
    m_IsAnimating = true;
    m_SubScreen = SUB_SCREEN_PICKER;
    m_PickedColor = BsMenuEdit::s_EditDesign.GetColor(m_SelectedButton);
    MoveCursorToPickedColor();
}

// 0x006112BC
void Palette::Enter_SamplesIn()
{
    SaveColors();
    // only the arrows of the color control stay active
    for (s32 i = 0; i < PALETTE_BUTTON_NUM; i++) {
        m_Buttons[i].m_IsActive = i >= BUTTON_ARROW_L;
    }
    m_PaletteLayout.BindAnimation(&m_InSetAnim);
    m_InSetAnim.SetFrame(0.0f);
    m_Buttons[BUTTON_SAMPLES].Select(false);
    m_Buttons[m_SelectedButton].Select(false);
    UpdatePageText();
    m_IsAnimating = true;
    m_SubScreen = SUB_SCREEN_SAMPLES;
}

// 0x00611400
void Palette::State_MainIn()
{
    if (m_InAnim.IsDone()) {
        m_PaletteLayout.UnbindAnimation(&m_InAnim);
        m_IsAnimating = false;
        m_ColorControl.SetFlags(ButtonActionControl::DISABLED, false);
        Change(&Palette::State_SelectColor);
        return;
    }
    m_InAnim.IncreaseFrame();
}

inline void Palette::UpdateButtonOut()
{
    if (m_ButtonOutAnim.IsBound()) {
        if (m_ButtonOutAnim.IsDone()) {
            m_ButtonLayout.UnbindAnimation(&m_ButtonOutAnim);
            SetFlagVisible(m_ButtonLayout.GetRootPane(), false);
        } else {
            m_ButtonOutAnim.IncreaseFrame();
        }
    }
}

// 0x0061146C
void Palette::State_PickerOut()
{
    UpdateButtonOut();
    if (m_OutPickAnim.IsDone()) {
        m_PaletteLayout.UnbindAnimation(&m_OutPickAnim);
        m_IsAnimating = false;
        m_SubScreen = SUB_SCREEN_NONE;
        m_Buttons[m_SelectedButton].Select(true, false);
        m_Buttons[m_SelectedButton].Deactivate();
        m_Buttons[BUTTON_SAMPLES].Activate();
        m_Buttons[BUTTON_PICKER].Activate();
        Change(&Palette::State_SelectColor);
    } else {
        m_OutPickAnim.IncreaseFrame();
    }
    m_IsPickerActive = true;
}

// 0x00611570
void Palette::State_SamplesOut()
{
    UpdateButtonOut();
    if (m_OutSetAnim.IsDone()) {
        m_PaletteLayout.UnbindAnimation(&m_OutSetAnim);
        m_IsAnimating = false;
        m_SubScreen = SUB_SCREEN_NONE;
        m_Buttons[m_SelectedButton].Select(true, false);
        m_Buttons[m_SelectedButton].Deactivate();
        Change(&Palette::State_SelectColor);
        return;
    }
    m_OutSetAnim.IncreaseFrame();
}

// 0x00611650
void Palette::Enter_MainIn()
{
    m_PaletteLayout.BindAnimation(&m_InAnim);
    m_InAnim.SetFrame(0.0f);
    m_IsAnimating = true;
}

// 0x00611688
void Palette::Enter_PickerOut()
{
    m_PaletteLayout.BindAnimation(&m_OutPickAnim);
    m_OutPickAnim.SetFrame(0.0f);
    m_ButtonLayout.BindAnimation(&m_ButtonOutAnim);
    m_ButtonOutAnim.SetFrame(0.0f);
    m_IsAnimating = true;
}

// 0x006116E4
void Palette::Enter_SamplesOut()
{
    // the colors and the sub screen buttons become active again, the arrows not
    for (s32 i = 0; i < PALETTE_BUTTON_NUM; i++) {
        m_Buttons[i].m_IsActive = i < BUTTON_ARROW_L;
    }
    m_PaletteLayout.BindAnimation(&m_OutSetAnim);
    m_OutSetAnim.SetFrame(0.0f);
    m_ButtonLayout.BindAnimation(&m_ButtonOutAnim);
    m_ButtonOutAnim.SetFrame(0.0f);
    m_IsAnimating = true;
}

// 0x00611774
void Palette::State_MainOut()
{
    if (m_OutAnim.IsDone()) {
        m_PaletteLayout.UnbindAnimation(&m_OutAnim);
        m_IsAnimating = false;
        Change(&Palette::State_None);
        return;
    }
    m_OutAnim.IncreaseFrame();
}

// 0x006117CC
void Palette::Enter_MainOut()
{
    m_PaletteLayout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetFrame(0.0f);
    m_IsAnimating = true;
}
