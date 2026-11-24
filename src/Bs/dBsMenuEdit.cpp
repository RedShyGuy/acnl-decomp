#include "Bs/dBsMenuEdit.h"

#include "Bs/dBsCommonDialog.h"
#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuBg.h"
#include "Bs/dBsMenuMgr.h"
#include "Bs/dBsMyDesignEditModel.h"
#include "Bs/dBsSkb.h"
#include "Bs/dBsSvMgr.h"
#include "Font/dMgr.h"
#include "Other/dController.h"
#include "Other/dFormatBuffer.h"
#include "Other/dPaneVisible.h"
#include "Other/dProcMgr.h"
#include "Other/dRegion.h"
#include "Other/dTextureImage.h"
#include "Other/dTouchState.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "g3d/g3d_System.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Picture.h"
#include "script/dScriptExtern.h"
#include "script/dWordCPtr.h"
#include "script/dWordCPtrSv.h"
#include "script/dWordFix.h"
#include "ssys/ma/lyt/dLayoutMgr.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Extern.h"
#include "sys/dSwkbd.h"

#include <cstring>

namespace {
// the command cache sizes of the layouts
const u32 EDIT_CMD_CACHE_SIZE = 0x18000;
const u32 MARK_CMD_CACHE_SIZE = 4;
const u32 MODEL_CMD_CACHE_SIZE = 0x1000;
// the frames of the cursor before repeating
const s32 CURSOR_TIMER = 9;
// the heap of the textures of the canvas
const u32 HEAP_SIZE = 102400;
// the pan of a sound from an x on the bottom screen (0..320) / of the layout (-160..160)
const f32 TOUCH_PAN_SCALE = 1.0f / 160.0f;
// the low nibble of a pixel outside of the shape of the design
const u16 PIXEL_OUTSIDE = 15;
// the pixels of the frame of the special designs (the two colors of the checker) and the index that marks the other
// pixels outside of the shape
const u16 SPECIAL_FRAME_COLOR_A = 0x008F;
const u16 SPECIAL_FRAME_COLOR_B = 0x888F;
const u16 SPECIAL_OUTSIDE_INDEX = 12;
// the turn of the model per frame
const s16 MODEL_ROTATE_SPEED = 0x111;
// the size of the grid picture with the smallest pixels
const f32 GRID_SIZE_SMALL = 156.0f;
// the label file of the dialogs and of the common buttons
const char* const MESSAGE_FILE = "SYS_2D_UI";
// the labels of the common buttons (quit, save) and of the cancel dialog
const s32 LABEL_QUIT = 76;
const s32 LABEL_SAVE = 82;
const s32 LABEL_CANCEL_CONFIRM = 336;
const s32 LABEL_CANCEL_CONFIRM_YES = 337;
const s32 LABEL_CANCEL_CONFIRM_NO = 338;
// the length of the title in the regions with wide characters (Japan, Korea; the index of GetRegionIndex)
const u16 TITLE_LENGTH_SHORT = 12;
const u16 TITLE_LENGTH = 20;
const u32 REGION_JAPAN = 0;
const u32 REGION_KOREA = 3;
} // namespace

// the visible part of the canvas per zoom level and size kind: the frame moves at most this far from the middle
// 0x0084A550
extern const nn::math::VEC2 s_EditFrameLimits[BsMenuEdit::ZOOM_LEVEL_MAX_SPECIAL + 1][BsMenuEdit::SIZE_KIND_MAX + 1] = {
    {nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(0.0f, 16.0f),
     nn::math::VEC2(0.0f, 6.0f)},
    {nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(0.0f, 16.0f), nn::math::VEC2(16.0f, 16.0f), nn::math::VEC2(16.0f, 32.0f),
     nn::math::VEC2(10.0f, 16.0f)},
    {nn::math::VEC2(6.0f, 6.0f), nn::math::VEC2(6.0f, 22.0f), nn::math::VEC2(22.0f, 22.0f), nn::math::VEC2(22.0f, 38.0f),
     nn::math::VEC2(18.0f, 24.0f)},
    {nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(0.0f, 0.0f),
     nn::math::VEC2(21.0f, 27.0f)},
};
// the width and height of the canvas per size kind and their halves
// 0x0084A5F0
extern const s32 s_EditCanvasSizes[BsMenuEdit::SIZE_KIND_MAX + 1][2] = {{16, 16}, {16, 32}, {32, 32}, {32, 48}, {52, 64}};
// 0x0084A618
extern const s32 s_EditCanvasHalfSizes[BsMenuEdit::SIZE_KIND_MAX + 1][2] = {{8, 8}, {8, 16}, {16, 16}, {16, 24}, {26, 32}};
// the scale of the grid texture and the size of a pixel of the canvas per pixel scale
// 0x0084A640
extern const f32 s_EditGridScales[4] = {0.0234375f, 0.01953125f, 0.0390625f, 0.0625f};
// 0x0084A650
extern const f32 s_EditPixelSizes[4] = {3.0f, 5.0f, 10.0f, 16.0f};
// the stamps (16x16 bits, a row per entry) and the patterns of the tools
// 0x0084A660
extern const u16 s_EditStamps[2][16] = {
    {0x0000, 0x0000, 0x0000, 0x0000, 0x1C70, 0x3EF8, 0x3FF8, 0x3FF8, 0x3FF8, 0x1FF0, 0x0FE0, 0x07C0, 0x0100, 0x0000, 0x0000,
     0x0000},
    {0x0000, 0x0000, 0x0000, 0x0100, 0x0100, 0x0380, 0x3FF8, 0x0FE0, 0x07C0, 0x07C0, 0x0EE0, 0x0C60, 0x1010, 0x0000, 0x0000,
     0x0000},
};
// 0x0084A6A0
extern const u16 s_EditPatterns[2][16] = {
    {0xF00F, 0xF00F, 0xE007, 0xC003, 0x03C0, 0x07E0, 0x0FF0, 0x0FF0, 0x0FF0, 0x0FF0, 0x07E0, 0x03C0, 0xC003, 0xE007, 0xF00F,
     0xF00F},
    {0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000,
     0x0000},
};
// the messages of the first notice (in SYS_2D_UI; -1 ends) and their second texts
// 0x0084A6E0
extern const s32 s_EditNoticeLabels[2][4] = {{0x14D, 0x14E, 0x14F, -1}, {0x4E, 0x4E, 0x57, -1}};
// the margin around the frame per pixel scale
// 0x0084AC50
extern const f32 s_EditFrameMargins[4] = {26.0f, 16.0f, 8.0f, 5.0f};
// the size kind of each part per pattern type of the wearable designs (5: a part that does not exist)
// 0x0084AC60
extern const u8 s_EditSizeKinds[ProButtonNode::PATTERN_TYPE_NUM][ProButtonNode::PART_NUM] = {
    {3, 3, 1, 1}, {3, 3, 0, 0}, {3, 3, 5, 5}, {2, 2, 1, 1}, {2, 2, 0, 0}, {2, 2, 5, 5},
};
// a quarter of the cosine (then the sine) in 1/256 for the ellipses
// 0x0084AC78
extern const u16 s_EditEllipseTable[128] = {
    0,   6,   12,  18,  25,  31,  37,  43,  49,  56,  62,  68,  74,  80,  86,  92,  97,  103, 109, 115, 120, 126,
    131, 136, 142, 147, 152, 157, 162, 167, 171, 176, 181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222,
    225, 228, 231, 234, 236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255, 256, 255,
    255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238, 236, 234, 231, 228, 225, 222, 219, 216,
    212, 209, 205, 201, 197, 193, 189, 185, 181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115,
    109, 103, 97,  92,  86,  80,  74,  68,  62,  56,  49,  43,  37,  31,  25,  18,  12,  6,
};
// the steps of the flood fill (down, left, right, up)
// 0x0084AEF8
extern const s32 s_EditFillSteps[2][4] = {{0, -1, 1, 0}, {1, 0, 0, -1}};
// the frame of the special designs in their texture (32 x 128) and the checker of its colors
// 0x0084A150
extern const u32 s_EditSpecialFrame[128] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFC0E800, 0x00000000, 0xFF0CFFCC,
    0x00000000, 0x00004D00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xF0800000, 0xFAFA0000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0x0F04FFFF, 0x5F5FFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xB0F20000, 0x00000000, 0xFFFFFFFF, 0xB3FF00F3, 0xFFFFFFFF, 0x37FFFFFF,
    0x051FFFFF, 0x0000003F, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};
// 0x0084A350
extern const u32 s_EditSpecialChecker[128] = {
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
    0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666, 0x66666666,
};
// the masks that clear one step (2 bits) of a byte of the flood fill
// 0x00849F5B
extern const u8 s_EditFillMasks[4] = {0xFC, 0xF3, 0xCF, 0x3F};
// the buttons of edit.bclyt (zoom in, zoom out, undo)
// 0x00950890
const char* s_EditButtonNames[3] = {"B_zoom_01", "B_zoom_00", "B_undo"};
// 0x0095089C
const char* s_EditButtonGroups[3] = {"G_zoom_01", "G_zoom_00", "G_undo"};

// 0x0098C2B8
SvMyDesign BsMenuEdit::s_EditDesign;
// 0x0095086C
BsMenuEdit::Settings BsMenuEdit::s_Settings = {false, false, false, 0, -1, -1};

// the states with their names (Japanese, debug); the machine starts with the edit window coming in
// 0x0084AAD4
const BsMenuEdit::StateInfo BsMenuEdit::s_StateInfos[19] = {
    {"編集ウィンドウ入場処理", &BsMenuEdit::Enter_EditIn, &BsMenuEdit::State_EditIn},
    {"汎用ボタン入場処理", &BsMenuEdit::Enter_ButtonsIn, &BsMenuEdit::State_ButtonsIn},
    {"監視処理", &BsMenuEdit::Enter_Watch, &BsMenuEdit::State_Watch},
    {"汎用ボタン退場処理", &BsMenuEdit::Enter_ButtonsOut, &BsMenuEdit::State_ButtonsOut},
    {"編集ウィンドウ退場処理", &BsMenuEdit::Enter_EditOut, &BsMenuEdit::State_EditOut},
    {"退場", &BsMenuEdit::Enter_Exit, &BsMenuEdit::State_Exit},
    {"ツール表示処理", &BsMenuEdit::Enter_ToolIn, &BsMenuEdit::State_ToolIn},
    {"ツール処理", &BsMenuEdit::Enter_Tool, &BsMenuEdit::State_Tool},
    {"ツール終了処理", &BsMenuEdit::Enter_ToolOut, &BsMenuEdit::State_ToolOut},
    {"パレット監視", &BsMenuEdit::Enter_Palette, &BsMenuEdit::State_Palette},
    {"パレット退場", &BsMenuEdit::Enter_PaletteOut, &BsMenuEdit::State_PaletteOut},
    {"プロ表示処理", &BsMenuEdit::Enter_ProIn, &BsMenuEdit::State_ProIn},
    {"プロ処理", &BsMenuEdit::Enter_Pro, &BsMenuEdit::State_Pro},
    {"プロ終了処理", &BsMenuEdit::Enter_ProOut, &BsMenuEdit::State_ProOut},
    {"名前入力ウィンドウ", &BsMenuEdit::Enter_NameInput, &BsMenuEdit::State_NameInput},
    {"初回注意メッセージ入場", &BsMenuEdit::Enter_FirstNoticeIn, &BsMenuEdit::State_FirstNoticeIn},
    {"初回注意メッセージ", &BsMenuEdit::Enter_FirstNotice, &BsMenuEdit::State_FirstNotice},
    {"キャンセル確認ウィンドウ入場待ち", &BsMenuEdit::Enter_CancelConfirmIn, &BsMenuEdit::State_CancelConfirmIn},
    {"キャンセル確認応答待ち", &BsMenuEdit::Enter_CancelConfirm, &BsMenuEdit::State_CancelConfirm},
};

// the tools in the order of ToolButtonNode: pens (small, medium, large), line, circle, rectangle, bucket, two stamps,
// fill, two patterns
// 0x0084A0F0
const BsMenuEdit::ToolFunc BsMenuEdit::s_ToolFuncs[TOOL_NUM] = {
    &BsMenuEdit::ToolPen<1>,
    &BsMenuEdit::ToolPen<2>,
    &BsMenuEdit::ToolPen<3>,
    &BsMenuEdit::ToolLine,
    &BsMenuEdit::ToolShape<&BsMenuEdit::PlotEllipse>,
    &BsMenuEdit::ToolShape<&BsMenuEdit::PlotRect>,
    &BsMenuEdit::ToolBucket,
    &BsMenuEdit::ToolStamp<0>,
    &BsMenuEdit::ToolStamp<1>,
    &BsMenuEdit::ToolFill,
    &BsMenuEdit::ToolPattern<1>,
    &BsMenuEdit::ToolPattern<0>,
};

namespace {
// the pixels of the image of a texture of the canvas
inline u16* GetPixels(nw::gfx::res::ResImageTexture& texture)
{
    return texture.ptr()->image.To<nw::gfx::res::ResImageData>()->data.To<u16>();
}

// the pan of a sound from the last touch
inline f32 GetTouchPan()
{
    return g_TouchState.lastX * TOUCH_PAN_SCALE - 1.0f;
}

// the frame of the special designs is drawn into their pixels; the other pixels outside of the shape get the index 12
inline void DrawSpecialFrame(u16* pixels)
{
    for (s32 row = 0; row < 128; row++) {
        u16* p = pixels + row * 32;
        for (s32 bit = 0; bit < 32; bit++, p++) {
            if ((s_EditSpecialFrame[row] & (1 << bit)) != 0) {
                *p = (s_EditSpecialChecker[row] & (1 << bit)) != 0 ? SPECIAL_FRAME_COLOR_A : SPECIAL_FRAME_COLOR_B;
            } else if ((*p & PIXEL_OUTSIDE) == PIXEL_OUTSIDE) {
                *p = (*p & ~PIXEL_OUTSIDE) | SPECIAL_OUTSIDE_INDEX;
            }
        }
    }
}

// the arrow of undo points back again: no undo is available
inline void ResetUndo(BsMenuEdit* edit)
{
    nw::lyt::Size& size = edit->m_pUndoPicture->m_Size;
    size.width = size.width < 0.0f ? -size.width : size.width;
    edit->m_Buttons[BsMenuEdit::BUTTON_UNDO].m_SeOk = SE_SYS_BTN_ACT_INVALID;
    edit->m_Buttons[BsMenuEdit::BUTTON_UNDO].m_OkSoundEnabled = true;
    edit->m_IsUndoEmpty = true;
}

// the buttons of the control can be used again (the zoom buttons not at the end of the zoom)
inline void ActivateButtons(BsMenuEdit* edit)
{
    const s32 maxLevel = BsMenuEdit::s_Settings.isSpecial ? BsMenuEdit::ZOOM_LEVEL_MAX_SPECIAL : BsMenuEdit::ZOOM_LEVEL_MAX;
    edit->m_Control.SetNodeActive(-1, true);
    edit->m_Buttons[BsMenuEdit::BUTTON_ZOOM_IN].m_IsActive = edit->m_ZoomLevel != maxLevel;
    edit->m_Buttons[BsMenuEdit::BUTTON_ZOOM_OUT].m_IsActive = edit->m_ZoomLevel != 0;
}

// the frame that is not used is hidden again after G_visible
inline void HideUnusedFrame(BsMenuEdit* edit)
{
    if (BsMenuEdit::s_Settings.isSpecial) {
        SetFlagVisible(edit->m_EditLayout.FindPane("W_frame"), false);
    } else {
        SetFlagVisible(edit->m_EditLayout.FindPane("W_frame_3px"), false);
    }
}

// the mark of a pixel at its corner towards the middle of the canvas
inline void MoveToPixelCorner(nn::math::VEC3* pos, s32 x, s32 y, s32 sizeKind)
{
    pos->x -= s_EditCanvasHalfSizes[sizeKind][0] > x ? 0.5f : -0.5f;
    pos->y += s_EditCanvasHalfSizes[sizeKind][1] > y ? 0.5f : -0.5f;
}
} // namespace

// 0x00198E20
BsMenuEdit::BsMenuEdit()
    : m_InitStep(0), m_pNaviGridPane(NULL), m_pNaviBgPane(NULL), m_pFramePane(NULL), m_Unknown0x994(0), m_pMainPane(NULL),
      m_pMainGroup(NULL), m_pNaviGroup(NULL), m_pZoomInGroup(NULL), m_pZoomOutGroup(NULL), m_pGridPicture(NULL),
      m_pCanvasPicture(NULL), m_pNaviPicture(NULL), m_pUndoPicture(NULL), m_ColorIndex(0), m_Texture(0), m_UndoTexture(0),
      m_Unknown0xA34(0), m_ZoomLevel(0), m_PixelScale(1), m_IsFrameTouched(false), m_IsDrawing(false),
      m_IsSubMenuOpen(false), m_IsUndoEmpty(true), m_IsComButtonDisabled(true), m_IsModelReady(false), m_Tool(0),
      m_Notice(0), m_ModelAngle(0), m_IsModelRotating(true), m_ModelView(0)
{
    m_pLinePoints[0] = NULL;
    m_pLinePoints[1] = NULL;
    m_pSquarePoints[0] = NULL;
    m_pSquarePoints[1] = NULL;
    m_pSquarePointPictures[0] = NULL;
    m_pSquarePointPictures[1] = NULL;
    const u32 request = s_Settings.request;
    s_Settings.specialSlot = (request >> 8) & 0xF;
    s_Settings.designSlot = request & 0xF;
    s_Settings.isSpecial = (request & 0x1000) >> 12;
    s_EditDesign.Assign(Func713388(g_pSvCurrentPlayer->patterns, s_Settings.designSlot));
    if (s_Settings.isSpecial) {
        m_CanvasWidth = 64;
        m_CanvasHeight = 64;
        m_PixelScale = 0;
        if (s_EditDesign.patternType != SvMyDesign::PATTERN_TYPE_SPECIAL || !s_EditDesign.IsCreatedByCurrentPlayer()) {
            s_EditDesign.patternType = SvMyDesign::PATTERN_TYPE_SPECIAL;
            s_EditDesign.ResetPattern();
        }
        s_Settings.isPro = false;
    } else {
        if (!s_EditDesign.IsCreatedByCurrentPlayer()) {
            s_EditDesign.InitNew();
        }
        // a new pattern type (12: keep the one of the design)
        if (((request >> 4) & 0xF) != 12) {
            s_EditDesign.patternType = (request >> 4) & 0xF;
            s_EditDesign.FillUnusedParts();
        }
        s_Settings.isPro = s_EditDesign.patternType < 6;
        if (s_Settings.isPro) {
            m_CanvasWidth = 32;
            m_CanvasHeight = 64;
        } else {
            s_EditDesign.FillExtraParts();
            m_CanvasWidth = 32;
            m_CanvasHeight = 32;
        }
    }
    m_CanvasBytes = m_CanvasWidth * m_CanvasHeight * 2;
}

// 0x0019931C slot 0x00
// 0x00776E48 (thunk)
// 0x00776E34 (deleting thunk)
// 0x0019930C (deleting dtor)
BsMenuEdit::~BsMenuEdit()
{
    // (only the members are destroyed)
}

// 0x00196E98 slot 0x0C
oml::framework::Result BsMenuEdit::Initialize()
{
    s_Settings.isOpen = true;
    const bool isEditRead = m_Reader.ReadArc("Layout/Edit/edit.arc");
    if ((m_ComButton.ReadArchive() & isEditRead) == 0) {
        return oml::framework::RESULT_RETRY;
    }
    switch (m_InitStep) {
    case 0: {
        const font::Mgr* fonts = font::Mgr::s_pInstance;
        m_Reader.GetResourceAccessorVRAM()->RegistFont(fonts->GetFontName(font::GARDEN_MSG_16)->getCString(),
                                                       fonts->GetFont(font::GARDEN_MSG_16));
        m_EditLayout.SetArcResAccReader(&m_Reader);
        m_EditLayout.Build("edit.bclyt", NULL, EDIT_CMD_CACHE_SIZE);
        ToolButtonNode::SetupArg arg = {&m_Reader, m_EditLayout.FindPane("N_posB")};
        m_ToolButton.Setup(arg);
        m_Control.AddNode(&m_ToolButton, false);
        m_InitStep++;
        break;
    }
    case 1:
        if (s_Settings.isPro) {
            ProButtonNode::SetupArg arg = {&m_Reader, m_EditLayout.FindPane("N_posB")};
            if (!m_ProButton.Setup(arg)) {
                break;
            }
            m_Control.AddNode(&m_ProButton, false);
        }
        m_Palette.Setup(&m_Reader);
        m_LineLayout.SetArcResAccReader(&m_Reader);
        m_LineLayout.Build("edit_t_line.bclyt", NULL, MARK_CMD_CACHE_SIZE);
        m_InitStep++;
        break;
    case 2:
        m_SquareLayout.SetArcResAccReader(&m_Reader);
        m_SquareLayout.Build("edit_t_square.bclyt", NULL, MARK_CMD_CACHE_SIZE);
        m_ModelLayout.SetArcResAccReader(&m_Reader);
        m_ModelLayout.Build("edit_model.bclyt", NULL, MODEL_CMD_CACHE_SIZE);
        m_InitStep++;
        break;
    case 3: {
        const ButtonActionControl::Description description = {
            &m_Reader,           &m_EditLayout,         NULL,         "edit_touch.bclan", "edit_select.bclan",
            "edit_touch_ok.bclan", "edit_select_ok.bclan", CURSOR_TIMER, NULL};
        m_Control.Initialize(description, NULL);
        GridButtonNode::SetupArg gridArg = {&m_Reader, m_EditLayout.FindPane("N_pos_swich")};
        m_GridButton.Setup(gridArg);
        m_GridButton.m_Se = SE_SYS_EDIT_BTN_ACTIVE;
        m_GridButton.m_SoundEnabled = true;
        m_GridButton.m_SoundMode = true;
        m_Control.AddNode(&m_GridButton, false);
        ButtonActionNode::Info info = {NULL, NULL, nn::math::VEC2(0.0f, 0.0f), 0, -1, NULL};
        info.pos = nn::math::VEC2(0.0f, 0.0f);
        const SeID seOks[3] = {SE_SYS_EDIT_ZOOM_IN, SE_SYS_EDIT_ZOOM_OUT, SE_SYS_BTN_ACT_INVALID};
        for (s32 i = 0; i < 3; i++) {
            info.pane = m_EditLayout.FindBounding(s_EditButtonNames[i]);
            info.group = m_EditLayout.FindGroup(s_EditButtonGroups[i]);
            info.index = i;
            m_Buttons[i].Initialize(info);
            m_Buttons[i].m_SeOk = seOks[i];
            m_Buttons[i].m_OkSoundEnabled = true;
            m_Buttons[i].m_Se = SE_SYS_EDIT_BTN_ACTIVE;
            m_Buttons[i].m_SoundEnabled = true;
            m_Buttons[i].m_SoundMode = true;
            m_Control.AddNode(&m_Buttons[i], true);
        }
        // the model of the design (not for hats, the wearable and the special designs)
        m_IsModelShown = false;
        if (static_cast<u8>(s_EditDesign.patternType - 6) > 1 && !(s_Settings.isPro | s_Settings.isSpecial)) {
            ButtonActionCstmNode::Description modelDescription = {
                &m_Reader,
                &m_ModelLayout,
                "edit_model_touch.bclan",
                "edit_model_select.bclan",
                "edit_model_touch_ok.bclan",
                "edit_model_select_ok.bclan",
                {NULL, NULL, nn::math::VEC2(0.0f, 0.0f), 0, BUTTON_MODEL, NULL}};
            modelDescription.info.pane = m_ModelLayout.FindBounding("B_btn_00");
            modelDescription.info.group = m_ModelLayout.FindGroup("G_btn_00");
            modelDescription.info.pos = VEC2_ZERO;
            m_ModelButton.Initialize(modelDescription);
            m_ModelButton.m_SeOk = SE_SYS_EDIT_MODEL_CHANGE;
            m_ModelButton.m_OkSoundEnabled = true;
            m_ModelButton.m_Se = SE_SYS_EDIT_BTN_ACTIVE;
            m_ModelButton.m_SoundEnabled = true;
            m_ModelButton.m_SoundMode = true;
            m_Control.AddNode(&m_ModelButton, false);
            m_IsModelShown = true;
        }
        m_Control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH;
        m_ComButton.Setup();
        m_ComButton.m_Control.SetText(LABEL_QUIT, LABEL_SAVE, MESSAGE_FILE);
        m_ComButton.m_Control.SetDecide(false, true);
        m_ComButton.m_Control.SetSeOk(SE_SYS_CANCEL, SE_SYS_DECIDE_L);
        m_ComButton.m_Control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH | ButtonActionControl::ENABLE_NODE_SHORTCUT;
        m_InitStep++;
        break;
    }
    case 4:
        m_InAnim.Initialize("edit_in.bclan", &m_Reader);
        m_OutAnim.Initialize("edit_out.bclan", &m_Reader);
        m_ScaleAnim.Initialize("edit_scale.bclan", &m_Reader);
        m_SizeAnim.Initialize("edit_size.bclan", &m_Reader);
        m_NgAnim.Initialize("edit_ng.bclan", &m_Reader);
        m_ToolInAnim.Initialize("edit_tool_in.bclan", &m_Reader);
        m_ToolOutAnim.Initialize("edit_tool_out.bclan", &m_Reader);
        m_LineScaleAnim.Initialize("edit_t_line_scale.bclan", &m_Reader);
        m_SquareScaleAnim.Initialize("edit_t_square_scale.bclan", &m_Reader);
        m_ModelInAnim.Initialize("edit_model_in.bclan", &m_Reader);
        m_ModelOutAnim.Initialize("edit_model_out.bclan", &m_Reader);
        FindPanes();
        SetFlagVisible(m_pGridPicture, m_GridButton.IsGridOn());
        SetFlagVisible(m_pNaviGridPane, m_GridButton.IsGridOn());
        SetupCanvas();
        if (s_Settings.isSpecial) {
            SetFlagVisible(m_EditLayout.FindPane("W_frame"), false);
            m_pFramePane = m_EditLayout.FindPane("W_frame_3px");
        } else {
            SetFlagVisible(m_EditLayout.FindPane("W_frame_3px"), false);
            m_pFramePane = m_EditLayout.FindPane("W_frame");
        }
        UpdateCanvasSize();
        Start("BsMenuEdit", s_StateInfos, 19);
        BsMenuMgr::Get()->m_pBsHandCursor->Disable();
        return oml::framework::RESULT_SUCCESS;
    }
    return oml::framework::RESULT_RETRY;
}

// 0x00197A40 slot 0x18
oml::framework::Result BsMenuEdit::Finalize()
{
    m_EditLayout.UnbindAllAnimation();
    m_LineLayout.UnbindAllAnimation();
    m_SquareLayout.UnbindAllAnimation();
    m_ModelLayout.UnbindAllAnimation();
    if (!static_cast<BsMenuBg*>(BsMenuMgr::Get()->m_pBsMenuBg)->IsChangeDone()) {
        return oml::framework::RESULT_RETRY;
    }
    s_Settings.isOpen = false;
    m_pMainPane->RemoveChild(m_LineLayout.GetRootPane());
    m_pMainPane->RemoveChild(m_SquareLayout.GetRootPane());
    m_Textures[0].Cleanup();
    m_Textures[0].DestroyDynamic();
    m_Textures[1].Cleanup();
    m_Textures[1].DestroyDynamic();
    m_UndoTextures[0].Cleanup();
    m_UndoTextures[0].DestroyDynamic();
    m_UndoTextures[1].Cleanup();
    m_UndoTextures[1].DestroyDynamic();
    m_ComButton.Finalize();

    m_GridButton.m_SwitchLayout.UnbindAllAnimation();
    nw::lyt::Pane* gridRoot = m_GridButton.m_SwitchLayout.GetRootPane();
    if (gridRoot->m_pParent != NULL) {
        gridRoot->m_pParent->RemoveChild(gridRoot);
    }

    m_ToolButton.m_Layout.UnbindAllAnimation();
    m_ToolButton.m_Touch.Finalize();
    m_ToolButton.m_Select.Finalize();
    m_ToolButton.m_TouchOk.Finalize();
    m_ToolButton.m_SelectOk.Finalize();
    if (m_ToolButton.m_IsAttached == true) {
        m_ToolButton.m_IsAttached = false;
        m_ToolButton.m_pBasePane->RemoveChild(m_ToolButton.m_Layout.GetRootPane());
    }

    m_Palette.m_PaletteLayout.UnbindAllAnimation();
    m_Palette.m_ButtonLayout.UnbindAllAnimation();
    nw::lyt::Pane* paletteBase = m_Palette.m_PaletteLayout.GetRootPane()->m_pParent;
    if (paletteBase != NULL) {
        paletteBase->RemoveChild(m_Palette.m_PaletteLayout.GetRootPane());
        paletteBase->RemoveChild(m_Palette.m_ButtonLayout.GetRootPane());
    }
    m_Palette.m_PickerCursor.m_Layout.UnbindAllAnimation();
    nw::lyt::Pane* pickRoot = m_Palette.m_PickerCursor.m_Layout.GetRootPane();
    if (pickRoot->m_pParent != NULL) {
        pickRoot->m_pParent->RemoveChild(pickRoot);
    }

    if (s_Settings.isPro) {
        m_ProButton.m_ProLayout.UnbindAllAnimation();
        m_ProButton.m_Choice.Func2BABBC();
        if (m_ProButton.m_IsAttached) {
            m_ProButton.m_pBasePane->RemoveChild(m_ProButton.m_ProLayout.GetRootPane());
            m_ProButton.m_IsAttached = false;
        }
        m_ProButton.m_DragControl.Finalize();
        m_ProButton.m_Touch.Finalize();
        m_ProButton.m_Select.Finalize();
        m_ProButton.m_TouchOk.Finalize();
        m_ProButton.m_SelectOk.Finalize();
    }
    m_Reader.Clear();

    if (m_ComButton.m_Control.GetPressedIndex() == COM_BUTTON_SAVE) {
        script::WordCPtr title(reinterpret_cast<const char16*>(m_Title.getCString()), m_Title.getBufferSize());
        s_EditDesign.SetCreatorToCurrentPlayer();
        s_EditDesign.SetTitle(title);
        if (!s_Settings.isSpecial) {
            Func5C4398(s_Settings.designSlot);
        }
        if (s_Settings.isSpecial) {
            GetGardenData()->strcData.stands[s_Settings.specialSlot].pattern.Assign(&s_EditDesign);
        }
        static_cast<SvMyDesign*>(Func20C4C8(SvGetCurrentPlayer()->patterns, s_Settings.designSlot))->Assign(&s_EditDesign);
        BsMenuMgr::s_Result = 1;
        Func6F0C80(17, 1);
    } else {
        BsMenuMgr::s_Result = 0;
    }
    if (BsMenuMgr::s_Unknown950D26 != 2) {
        BsMenuBg::s_pInstance->ChangeBg(10);
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x00197654 slot 0x24
oml::framework::Result BsMenuEdit::Calc()
{
    // a control is disabled while another one is busy
    const bool isBusy = m_IsComButtonDisabled || m_ComButton.m_Control.HasCurrent() || m_Control.HasCurrent() ||
                        m_Palette.m_ColorControl.HasCurrent();
    m_Control.SetFlags(ButtonActionControl::DISABLED, isBusy && !m_Control.HasCurrent());
    m_Control.Update();
    m_ComButton.m_Control.SetFlags(ButtonActionControl::DISABLED,
                                   (isBusy && !m_ComButton.m_Control.HasCurrent()) || m_IsSubMenuOpen);
    m_ComButton.Update();
    m_Palette.m_ColorControl.SetFlags(ButtonActionControl::DISABLED,
                                      (isBusy && !m_Palette.m_ColorControl.HasCurrent()) || m_ProButton.m_IsShown);
    m_Palette.Call();
    m_Palette.m_PaletteLayout.Animate();
    m_Palette.m_ButtonLayout.Animate();
    Call();

    // the model turns unless L or R is held
    m_IsModelRotating = !(Controller::IsHold(Controller::L) || Controller::IsHold(Controller::R));
    if (m_IsModelRotating) {
        m_ModelAngle += MODEL_ROTATE_SPEED;
    }
    if (BsMyDesignEditModel::s_pInstance != NULL) {
        BsMyDesignEditModel::s_pInstance->m_Angle = m_ModelAngle;
    }

    // another pro part was chosen
    if (m_EditedPart != m_ProButton.m_EditedPart) {
        m_EditedPart = m_ProButton.m_EditedPart;
        m_SizeKind = s_Settings.isSpecial ? SIZE_KIND_SPECIAL
                     : !s_Settings.isPro  ? 2
                                          : s_EditSizeKinds[s_EditDesign.patternType][m_EditedPart];
        m_pFramePane->SetTranslate(nn::math::VEC3(0.0f, 0.0f, 0.0f));
        LoadCanvas();
        ResetUndo(this);
        UpdateCanvasSize();
    }
    m_EditLayout.Animate();
    m_ModelLayout.Animate();
    return oml::framework::RESULT_SUCCESS;
}

// 0x00196C0C slot 0x30
oml::framework::Result BsMenuEdit::Draw()
{
    // the copy that was written is shown, the other one is written next
    nw::gfx::res::ResImageData* image = m_Textures[m_Texture].ptr()->image.To<nw::gfx::res::ResImageData>();
    const void* address = image->pLocationAddress != NULL ? image->pLocationAddress : image->data.ToPtr();
    m_TexMap.m_PhysicalAddress = nngxGetPhysicalAddr(reinterpret_cast<uptr>(address));
    m_pCanvasPicture->GetMaterial(0)->SetTexMap(0, &m_TexMap);
    nw::lyt::TexMap naviMap = m_TexMap;
    naviMap.m_Size.width = s_EditCanvasSizes[m_SizeKind][0];
    naviMap.m_Size.height = s_EditCanvasSizes[m_SizeKind][1];
    naviMap.Update();
    m_pNaviPicture->GetMaterial(0)->SetTexMap(0, &naviMap);
    m_Texture ^= 1;
    m_pPixels = GetPixels(m_Textures[m_Texture]);
    std::memcpy(m_pPixels, GetPixels(m_Textures[m_Texture ^ 1]), m_CanvasBytes);

    if (!m_ToolButton.m_IsAttached) {
        ssys::ma::lyt::LayoutMgr::Get()->Register(&m_ToolButton.m_Layout, true);
    }
    if (s_Settings.isPro && !m_ProButton.m_IsAttached) {
        m_ProButton.m_ProLayout.Animate();
        ssys::ma::lyt::LayoutMgr::Get()->Register(&m_ProButton.m_ProLayout, true);
        m_ProButton.m_Choice.Draw();
        m_ProButton.m_DragControl.DrawDraggedIcons();
    }
    SetFlagVisible(m_Palette.m_PickerCursor.m_Layout.GetRootPane(), m_Palette.m_IsPickerActive);
    if (m_Palette.m_IsPickerActive) {
        m_Palette.m_PickerCursor.m_Layout.Animate();
        m_Palette.m_IsPickerActive = false;
    }
    ssys::ma::lyt::LayoutMgr::Get()->Register(&m_EditLayout, true);
    if (m_IsModelShown == true) {
        ssys::ma::lyt::LayoutMgr::Get()->Register(&m_ModelLayout, true);
    }
    m_ComButton.Draw();
    return oml::framework::RESULT_SUCCESS;
}

// 0x00195178
void BsMenuEdit::SetSettings(u32 slot, u32 patternType)
{
    s_Settings.request = (slot & 0xF) | ((patternType & 0xF) << 4);
}

// 0x00195A40
void BsMenuEdit::SetSpecialSettings(u32 specialSlot, u32 slot)
{
    s_Settings.request = (slot & 0xF) | ((specialSlot & 0xF) << 8) | 0x1000;
}

// 0x00194308
void BsMenuEdit::UpdateCanvasSize()
{
    m_SizeKind = m_SizeKind < 0 ? 0 : (m_SizeKind > SIZE_KIND_MAX ? SIZE_KIND_MAX : m_SizeKind);
    m_EditLayout.BindAnimation(&m_SizeAnim, m_pNaviGroup, false);
    m_SizeAnim.SetFrame(m_SizeKind);
    m_EditLayout.Animate();
    m_EditLayout.UnbindAnimation(&m_SizeAnim, m_pNaviGroup, false);
    m_EditLayout.BindAnimation(&m_ScaleAnim);
    m_ScaleAnim.SetFrame(m_PixelScale);
    m_EditLayout.BindAnimation(&m_SizeAnim, m_pMainGroup, false);
    if (m_ZoomLevel == 0 && m_SizeKind == 0) {
        m_SizeAnim.SetFrame(0.0f);
    } else if (m_ZoomLevel == 0 && m_SizeKind == 1) {
        m_SizeAnim.SetFrame(1.0f);
    } else if (m_SizeKind == SIZE_KIND_SPECIAL) {
        m_SizeAnim.SetFrame(4.0f);
    } else {
        m_SizeAnim.SetFrame(2.0f);
    }
    m_EditLayout.Animate();
    m_EditLayout.UnbindAnimation(&m_ScaleAnim);
    m_EditLayout.UnbindAnimation(&m_SizeAnim, m_pMainGroup, false);
    UpdateGridTexture();
    ClampFrame();
}

// 0x0019449C
void BsMenuEdit::PlotEllipse(s32 x0, s32 y0, s32 x1, s32 y1, PlotFunc plot)
{
    const s32 left = x0 < x1 ? x0 : x1;
    const s32 right = x0 < x1 ? x1 : x0;
    const s32 top = y0 < y1 ? y0 : y1;
    const s32 bottom = y0 < y1 ? y1 : y0;
    const s32 width = right - left;
    const s32 height = bottom - top;
    const s32 radiusX = width >> 1;
    const s32 radiusY = height >> 1;
    const s32 centerX = left + radiusX;
    const s32 centerY = top + radiusY;
    // the points along the perimeter (an approximation of it)
    const s32 sum = radiusX + radiusY;
    const s32 diff = radiusY < radiusX ? radiusX - radiusY : radiusY - radiusX;
    const s32 perimeter = (sum - ((sum >> 1) - (sum >> 3) - (diff >> 1) - 5)) | 1;
    const s32 step = 64 / (perimeter + 1);
    const s32 oddX = width & 1;
    const s32 oddY = height & 1;
    for (s16 i = 0; i < 64; i = i + step) {
        const s32 dx = (s_EditEllipseTable[64 + i] * radiusX + 45) >> 8;
        const s32 dy = (s_EditEllipseTable[i] * radiusY + 45) >> 8;
        (this->*plot)(centerX + dx + oddX, centerY + dy + oddY);
        (this->*plot)(centerX + dx + oddX, centerY - dy);
        (this->*plot)(centerX - dx, centerY + dy + oddY);
        (this->*plot)(centerX - dx, centerY - dy);
    }
}

// 0x00194648
void BsMenuEdit::PlotRect(s32 x0, s32 y0, s32 x1, s32 y1, PlotFunc plot)
{
    s32 x = x0;
    do {
        (this->*plot)(x, y0);
        (this->*plot)(x, y1);
    } while (!ssys::ma::Chase(&x, x1, 1));
    (this->*plot)(x1, y0);
    (this->*plot)(x1, y1);
    s32 y = y0;
    while (!ssys::ma::Chase(&y, y1, 1)) {
        (this->*plot)(x0, y);
        (this->*plot)(x1, y);
    }
}

// 0x00198724
void BsMenuEdit::PlotLine(s32 x0, s32 y0, s32 x1, s32 y1, PlotFunc plot)
{
    if (x0 > x1) {
        PlotLine(x1, y1, x0, y0, plot);
        return;
    }
    const s32 dx = x1 - x0;
    s32 stepY;
    s32 dy;
    if (y0 < y1) {
        stepY = 1;
        dy = y1 - y0;
    } else {
        stepY = -1;
        dy = y0 - y1;
    }
    s32 x = x0;
    s32 y = y0;
    if (dx >= dy) {
        s32 error = -dx;
        for (s32 i = 0; i <= dx; i++) {
            (this->*plot)(x, y);
            x++;
            error += dy * 2;
            if (error >= 0) {
                y += stepY;
                error -= dx * 2;
            }
        }
    } else {
        s32 error = -dy;
        for (s32 i = 0; i <= dy; i++) {
            (this->*plot)(x, y);
            error += dx * 2;
            y += stepY;
            if (error >= 0) {
                x++;
                error -= dy * 2;
            }
        }
    }
}

// 0x0019491C
void BsMenuEdit::SaveUndo()
{
    const u8 previous = m_UndoTexture;
    m_UndoTexture ^= 1;
    std::memcpy(m_pUndoPixels[m_UndoTexture], m_pUndoPixels[previous], m_CanvasBytes);
    nw::lyt::Size& size = m_pUndoPicture->m_Size;
    size.width = size.width < 0.0f ? -size.width : size.width;
    m_Buttons[BUTTON_UNDO].m_SeOk = SE_SYS_EDIT_UNDO;
    m_Buttons[BUTTON_UNDO].m_OkSoundEnabled = true;
    m_IsUndoEmpty = false;
}

// 0x001949BC
void BsMenuEdit::ClampFrame()
{
    nn::math::VEC3 pos = m_pFramePane->m_Translate;
    const nn::math::VEC2& limit = s_EditFrameLimits[m_ZoomLevel][m_SizeKind];
    pos.x = pos.x < -limit.x ? -limit.x : (pos.x <= limit.x ? pos.x : limit.x);
    pos.y = pos.y < -limit.y ? -limit.y : (pos.y <= limit.y ? pos.y : limit.y);
    if (m_SizeKind != SIZE_KIND_SPECIAL) {
        pos.y = static_cast<s32>(pos.y) & ~1;
        pos.x = static_cast<s32>(pos.x) & ~1;
    }
    m_pFramePane->SetTranslate(pos);
    UpdateCanvasView();
}

// 0x00194ABC
void BsMenuEdit::GetCanvasPixel(s32* x, s32* y, const nn::math::VEC3* pos, f32 offset)
{
    const nw::lyt::Size& size = m_pCanvasPicture->m_Size;
    s32 px = static_cast<s32>((pos->x + size.width * 0.5f) / s_EditPixelSizes[m_PixelScale] + offset) + m_ScrollX;
    const s32 maxX = s_EditCanvasSizes[m_SizeKind][0] - 1;
    *x = px < 0 ? 0 : (maxX < px ? maxX : px);
    s32 py = static_cast<s32>((size.height * 0.5f - pos->y) / s_EditPixelSizes[m_PixelScale] + offset) + m_ScrollY;
    const s32 maxY = s_EditCanvasSizes[m_SizeKind][1] - 1;
    *y = py < 0 ? 0 : (maxY < py ? maxY : py);
}

// 0x00194BC0
void BsMenuEdit::SetupCanvas()
{
    m_Allocator.CreateExpHeap(HEAP_SIZE, ssys::ma::lyt::GetAllocator()->m_pHeap, sead::SafeStringBase<char>("BsMenuEdit"),
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
    nw::gfx::res::ResImageTexture::DynamicDescription description;
    description.height = m_CanvasHeight;
    description.width = m_CanvasWidth;
    description.mipmapLevels = 1;
    description.locationFlag = nw::gfx::res::ResTexture::LOCATION_VRAM_A;
    description.pLocationAddress = NULL;
    description.format = 4;
    description.isCleared = false;
    description.isAllocated = true;
    m_Textures[0] = nw::gfx::res::ResImageTexture::CreateDynamic(description, &m_Allocator);
    m_Textures[1] = nw::gfx::res::ResImageTexture::CreateDynamic(description, &m_Allocator);
    // 0x00950880 (guard 0x0095087C)
    static const nw::ut::Color8 s_CanvasColor(0, 0, 0, 255);
    m_pCanvasPicture->GetMaterial(0)->m_Colors[0] = s_CanvasColor;
    m_pNaviPicture->GetMaterial(0)->m_Colors[0] = s_CanvasColor;
    m_TexMap.m_TexObject = 0;
    m_TexMap.m_PhysicalAddress = 0;
    const nw::lyt::TexSize size(m_CanvasWidth, m_CanvasHeight);
    m_TexMap.m_Size = size;
    m_TexMap.m_RealSize = size;
    m_TexMap.m_Bits.format = nw::lyt::TEXFORMAT_RGBA4;
    m_TexMap.Update();
    m_TexMap.SetFilter(nw::lyt::TexMap::FILTER_NEAREST, nw::lyt::TexMap::FILTER_NEAREST);
    m_pPixels = GetPixels(m_Textures[0]);

    m_EditedPart = m_ProButton.m_EditedPart;
    m_SizeKind = s_Settings.isSpecial ? SIZE_KIND_SPECIAL
                 : !s_Settings.isPro  ? 2
                                      : s_EditSizeKinds[s_EditDesign.patternType][m_EditedPart];
    s_EditDesign.WriteToTexture(m_Textures[0].ptr(), static_cast<u8>(m_EditedPart));
    if (s_Settings.isSpecial) {
        DrawSpecialFrame(m_pPixels);
        // (the undo textures do not exist yet)
        s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
    }
    for (s32 i = 0; i < 2; i++) {
        m_UndoTextures[i] = nw::gfx::res::ResImageTexture::CreateDynamic(description, &m_Allocator);
        m_pUndoPixels[i] = GetPixels(m_UndoTextures[i]);
        std::memcpy(m_pUndoPixels[i], m_pPixels, m_CanvasBytes);
    }
    SetFlagVisible(m_pFacePicture, false);
}

// 0x001955AC
void BsMenuEdit::GetPixelPosition(nn::math::VEC3* pos, s32 x, s32 y)
{
    const nw::lyt::Size& size = m_pCanvasPicture->m_Size;
    pos->x = (static_cast<f32>(x - m_ScrollX) + 0.5f) * s_EditPixelSizes[m_PixelScale] - size.width * 0.5f;
    pos->y = size.height * 0.5f - (static_cast<f32>(y - m_ScrollY) + 0.5f) * s_EditPixelSizes[m_PixelScale];
    pos->z = 0.0f;
}

// 0x0019530C
void BsMenuEdit::UpdateCanvasView()
{
    const s32* canvas = s_EditCanvasSizes[m_SizeKind];
    f32 frameX = m_pFramePane->m_Translate.x;
    f32 frameY = m_pFramePane->m_Translate.y;
    if (m_SizeKind != SIZE_KIND_SPECIAL) {
        frameX *= 0.5f;
        frameY *= 0.5f;
    }
    // the visible pixels and their texture coordinates
    f32 left = frameX - s_EditFrameMargins[m_PixelScale];
    if (left < -canvas[0] * 0.5f) {
        left = -canvas[0] * 0.5f;
    }
    m_ScrollX = static_cast<u16>(left + canvas[0] * 0.5f);
    const f32 u0 = static_cast<f32>(m_ScrollX) / m_CanvasWidth;
    f32 right = frameX + s_EditFrameMargins[m_PixelScale];
    if (right > canvas[0] * 0.5f) {
        right = canvas[0] * 0.5f;
    }
    const f32 u1 = (right + canvas[0] * 0.5f) / m_CanvasWidth;
    f32 low = frameY - s_EditFrameMargins[m_PixelScale];
    if (low < -canvas[1] * 0.5f) {
        low = -canvas[1] * 0.5f;
    }
    const f32 vBottom = (canvas[1] * 0.5f - low) / m_CanvasHeight;
    f32 high = frameY + s_EditFrameMargins[m_PixelScale];
    if (high > canvas[1] * 0.5f) {
        high = canvas[1] * 0.5f;
    }
    m_ScrollY = static_cast<s32>(canvas[1] * 0.5f - high);
    const f32 vTop = static_cast<f32>(m_ScrollY) / m_CanvasHeight;
    const nn::math::VEC2 texCoords[4] = {nn::math::VEC2(u0, vTop), nn::math::VEC2(u1, vTop), nn::math::VEC2(u0, vBottom),
                                         nn::math::VEC2(u1, vBottom)};
    m_pCanvasPicture->SetTexCoord(0, texCoords);
    m_pFacePicture->SetTexCoord(0, texCoords);

    // the grid moves with the frame
    nw::lyt::TexSRT srt = static_cast<const nw::lyt::Material*>(m_pGridPicture->GetMaterial(0))->GetTexSRTAry()[0];
    const f32 margin = s_EditFrameMargins[m_PixelScale];
    f32 gridX = frameX;
    if (gridX < -margin - 1.0f) {
        gridX = -margin - 1.0f;
    } else if (gridX > margin + 1.0f) {
        gridX = margin + 1.0f;
    }
    srt.translate.x = gridX * s_EditGridScales[m_PixelScale];
    f32 gridY = -frameY;
    if (gridY < -margin - 1.0f) {
        gridY = -margin - 1.0f;
    } else if (gridY > margin + 1.0f) {
        gridY = margin + 1.0f;
    }
    srt.translate.y = gridY * s_EditGridScales[m_PixelScale];
    nw::lyt::Material* material = m_pGridPicture->GetMaterial(0);
    material->GetTexSRTAry()[0] = srt;
    material->SetCacheDirty();
}

// 0x0019632C
void BsMenuEdit::ApplyCanvas()
{
    TextureImage16 image(m_pPixels, m_CanvasWidth, m_CanvasHeight);
    s_EditDesign.ReadFromImage(&image, static_cast<u8>(m_EditedPart));
}

// 0x0019638C
void BsMenuEdit::LoadCanvas()
{
    s_EditDesign.WriteToTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
    std::memcpy(m_pPixels, m_pUndoPixels[m_UndoTexture], m_CanvasBytes);
    if (s_Settings.isSpecial) {
        DrawSpecialFrame(m_pPixels);
    }
}

// 0x00197DC4
void BsMenuEdit::FindPanes()
{
    m_pNaviGridPane = m_EditLayout.FindPane("P_navi_grid");
    m_pNaviBgPane = m_EditLayout.FindPane("P_naviBG_00");
    m_pMainGroup = m_EditLayout.FindGroup("G_main");
    m_pNaviGroup = m_EditLayout.FindGroup("G_navi");
    m_pZoomInGroup = m_EditLayout.FindGroup("G_zoom_01");
    m_pZoomOutGroup = m_EditLayout.FindGroup("G_zoom_00");
    m_pGridPicture = m_EditLayout.FindPicture("P_main_grid");
    m_pCanvasPicture = m_EditLayout.FindPicture("P_main_00");
    m_pNaviPicture = m_EditLayout.FindPicture("P_navi_00");
    m_pUndoPicture = m_EditLayout.FindPicture("P_arrow");
    m_pFacePicture = m_EditLayout.FindPicture("P_face_00");
    m_Buttons[BUTTON_ZOOM_OUT].m_IsActive = false;
    m_pMainPane = m_EditLayout.FindPane("N_main");
    m_pLinePoints[0] = m_LineLayout.FindPane("N_point_00");
    m_pLinePoints[1] = m_LineLayout.FindPane("N_point_01");
    m_pSquarePoints[0] = m_SquareLayout.FindPane("N_point_00");
    m_pSquarePoints[1] = m_SquareLayout.FindPane("N_point_01");
    m_pSquarePointPictures[0] = m_SquareLayout.FindPicture("P_point_00");
    m_pSquarePointPictures[1] = m_SquareLayout.FindPicture("P_point_01");
    for (u32 i = 0; i < MODEL_VIEW_NUM; i++) {
        g_FormatBuffer.format("%s_%02d", "P_icon", i);
        m_pModelViewPanes[i] = m_ModelLayout.FindPane(g_FormatBuffer.getCString());
        SetFlagVisible(m_pModelViewPanes[i], m_ModelView == i);
    }
    m_pMainPane->AppendChild(m_LineLayout.GetRootPane());
    SetFlagVisible(m_LineLayout.GetRootPane(), false);
    m_pMainPane->AppendChild(m_SquareLayout.GetRootPane());
    SetFlagVisible(m_SquareLayout.GetRootPane(), false);
    // the palette below the canvas
    nw::lyt::Pane* basePane = m_EditLayout.FindPane("N_posB");
    nw::lyt::Pane* paletteRoot = m_Palette.m_PaletteLayout.GetRootPane();
    if (paletteRoot->m_pParent != NULL) {
        paletteRoot->m_pParent->RemoveChild(paletteRoot);
    }
    basePane->AppendChild(m_Palette.m_PaletteLayout.GetRootPane());
    basePane->AppendChild(m_Palette.m_ButtonLayout.GetRootPane());
    m_EditLayout.BindAnimation(&m_NgAnim, m_pZoomOutGroup, false);
    m_NgAnim.SetFrame(0.0f);
    m_EditLayout.Animate();
    m_EditLayout.UnbindAnimation(&m_NgAnim, m_pZoomOutGroup, false);
    m_GridTexSize = m_pGridPicture->GetMaterial(0)->GetTexMapAry()[0].m_Size;
}

// 0x0019816C
void BsMenuEdit::UpdateGridTexture()
{
    m_GridTexSize = m_pGridPicture->GetMaterial(0)->GetTexMapAry()[0].m_Size;
    nw::lyt::Size size = m_pGridPicture->m_Size;
    if (m_PixelScale == 0) {
        size = nw::lyt::Size(GRID_SIZE_SMALL, GRID_SIZE_SMALL);
        m_pGridPicture->m_Size = size;
        m_pCanvasPicture->m_Size = size;
    }
    m_pFacePicture->m_Size = size;
    // the grid texture repeats every 128 units
    const f32 scale = 128.0f / m_GridTexSize.width * (1.0f / 256.0f);
    const f32 u = size.width * scale;
    const f32 v = size.height * scale;
    const nn::math::VEC2 texCoords[4] = {nn::math::VEC2(-u, -v), nn::math::VEC2(u, -v), nn::math::VEC2(-u, v),
                                         nn::math::VEC2(u, v)};
    m_pGridPicture->SetTexCoord(0, texCoords);
}

// 0x00198270
void BsMenuEdit::Zoom(bool in)
{
    const s32 maxLevel = s_Settings.isSpecial ? ZOOM_LEVEL_MAX_SPECIAL : ZOOM_LEVEL_MAX;
    if (in ? m_ZoomLevel >= maxLevel : m_ZoomLevel <= 0) {
        return;
    }
    if (m_ZoomLevel == 0) {
        m_Buttons[BUTTON_ZOOM_OUT].m_IsActive = true;
    } else if (m_ZoomLevel == maxLevel) {
        m_Buttons[BUTTON_ZOOM_IN].m_IsActive = true;
    }
    if (in) {
        m_ZoomLevel++;
    } else {
        m_ZoomLevel--;
    }
    if (in) {
        m_PixelScale++;
    } else {
        m_PixelScale--;
    }
    if (m_ZoomLevel == 0) {
        m_Buttons[BUTTON_ZOOM_OUT].m_IsActive = false;
    } else if (m_ZoomLevel == maxLevel) {
        m_Buttons[BUTTON_ZOOM_IN].m_IsActive = false;
    }

    m_EditLayout.BindAnimation(&m_ScaleAnim, m_pNaviGroup, false);
    m_ScaleAnim.SetFrame(m_PixelScale);
    m_EditLayout.BindAnimation(&m_SizeAnim, m_pMainGroup, false);
    if (m_ZoomLevel == 0 && m_SizeKind == 0) {
        m_SizeAnim.SetFrame(0.0f);
    } else if (m_ZoomLevel == 0 && m_SizeKind == 1) {
        m_SizeAnim.SetFrame(1.0f);
    } else if (m_SizeKind == SIZE_KIND_SPECIAL) {
        m_SizeAnim.SetFrame(4.0f);
    } else {
        m_SizeAnim.SetFrame(2.0f);
    }
    m_EditLayout.BindAnimation(&m_NgAnim, m_pZoomInGroup, false);
    if (m_Buttons[BUTTON_ZOOM_IN].m_IsActive) {
        m_NgAnim.SetLastFrame();
    } else {
        m_NgAnim.SetFrame(0.0f);
    }
    m_EditLayout.Animate();
    m_EditLayout.UnbindAnimation(&m_SizeAnim, m_pMainGroup, false);
    m_EditLayout.UnbindAnimation(&m_ScaleAnim, m_pNaviGroup, false);
    m_EditLayout.UnbindAnimation(&m_NgAnim, m_pZoomInGroup, false);

    m_EditLayout.BindAnimation(&m_NgAnim, m_pZoomOutGroup, false);
    if (m_Buttons[BUTTON_ZOOM_OUT].m_IsActive) {
        m_NgAnim.SetLastFrame();
    } else {
        m_NgAnim.SetFrame(0.0f);
    }
    m_EditLayout.BindAnimation(&m_ScaleAnim, m_pMainGroup, false);
    m_ScaleAnim.SetFrame(m_PixelScale);
    m_EditLayout.Animate();
    m_EditLayout.UnbindAnimation(&m_NgAnim, m_pZoomOutGroup, false);
    m_EditLayout.UnbindAnimation(&m_ScaleAnim, m_pMainGroup, false);
    UpdateGridTexture();
    ClampFrame();
}

// 0x0019855C
void BsMenuEdit::FloodFill(s32 x, s32 y, u16 target)
{
    // the step to each pixel (2 bits each) is kept to go back
    u8 steps[1024];
    std::memset(steps, 0, sizeof(steps));
    TextureImage16 image(m_Textures[m_Texture].ptr());
    image.SetPixel(x, y, m_DrawColor);
    s32 depth = 0;
    while (true) {
        u32 step = (steps[depth >> 2] >> ((depth & 3) * 2)) & 3;
        x += s_EditFillSteps[0][step];
        y += s_EditFillSteps[1][step];
        if (x >= 0 && x < s_EditCanvasSizes[m_SizeKind][0] && y >= 0 && y < s_EditCanvasSizes[m_SizeKind][1] &&
            image.GetPixel(x, y) == target) {
            image.SetPixel(x, y, m_DrawColor);
            depth++;
            steps[depth >> 2] = (steps[depth >> 2] & s_EditFillMasks[depth & 3]) | (0 << ((depth & 3) * 2));
            continue;
        }
        // back, then the next step
        x -= s_EditFillSteps[0][step];
        y -= s_EditFillSteps[1][step];
        while (step == 3) {
            if (--depth < 0) {
                return;
            }
            step = (steps[depth >> 2] >> ((depth & 3) * 2)) & 3;
            x -= s_EditFillSteps[0][step];
            y -= s_EditFillSteps[1][step];
        }
        steps[depth >> 2] = (steps[depth >> 2] & s_EditFillMasks[depth & 3]) | (((step + 1) & 3) << ((depth & 3) * 2));
    }
}

// 0x00198888
void BsMenuEdit::PlotStamp(s32 x, s32 y, const u16* stamp)
{
    const s32 left = x - 8;
    const s32 top = y - 8;
    TextureImage16 image(m_Textures[m_Texture].ptr());
    for (s32 i = 0; i < 16; i++) {
        const s32 px = left + i;
        if (px < 0 || px >= s_EditCanvasSizes[m_SizeKind][0]) {
            continue;
        }
        for (s32 j = 0; j < 16; j++) {
            const s32 py = top + j;
            if (py < 0 || py >= s_EditCanvasSizes[m_SizeKind][1]) {
                continue;
            }
            if ((stamp[j] & (1 << i)) == 0) {
                continue;
            }
            if ((image.GetPixel(px, py) & PIXEL_OUTSIDE) == PIXEL_OUTSIDE) {
                continue;
            }
            image.SetPixel(px, py, m_DrawColor);
        }
    }
}

// 0x00193E08
bool BsMenuEdit::GetTouchPosition(nw::lyt::Pane* pane, nn::math::VEC3* pos)
{
    const nn::math::VEC2 screen(g_TouchState.lastX, g_TouchState.lastY);
    nn::math::VEC2 layoutPos;
    m_EditLayout.ScreenToLayout(screen, &layoutPos);
    nn::math::MTX34 inverse;
    nn::math::ARMv6::MTX34InverseAsm(&inverse, &pane->m_GlbMtx);
    const nn::math::VEC3 global(layoutPos.x, layoutPos.y, 0.0f);
    nn::math::VEC3 local;
    nn::math::ARMv6::VEC3TransformAsm(&local, &inverse, &global);
    const nw::ut::Rect rect = pane->GetPaneRect();
    if (rect.left < local.x && local.x < rect.right && rect.bottom < local.y && local.y < rect.top) {
        if (pos != NULL) {
            *pos = local;
        }
        return true;
    }
    return false;
}

// 0x00193F08
void BsMenuEdit::UpdateFrame()
{
    const nn::math::VEC2& limit = s_EditFrameLimits[m_ZoomLevel][m_SizeKind];
    if (g_TouchState.isTouch) {
        // a touch that began on the navigation moves the frame
        if (!m_IsFrameTouched && g_TouchState.prevIsTouch) {
            return;
        }
        const nn::math::VEC2 screen(g_TouchState.lastX, g_TouchState.lastY);
        nn::math::VEC2 layoutPos;
        m_EditLayout.ScreenToLayout(screen, &layoutPos);
        nn::math::MTX34 inverse;
        nn::math::ARMv6::MTX34InverseAsm(&inverse, &m_pNaviBgPane->m_GlbMtx);
        const nn::math::VEC3 global(layoutPos.x, layoutPos.y, 0.0f);
        nn::math::VEC3 local;
        nn::math::ARMv6::VEC3TransformAsm(&local, &inverse, &global);
        if (!g_TouchState.prevIsTouch) {
            const nw::ut::Rect rect = m_pNaviBgPane->GetPaneRect();
            if (!(rect.left <= local.x && local.x < rect.right)) {
                return;
            }
            if (!(rect.bottom <= local.y && local.y <= rect.top)) {
                return;
            }
            m_IsFrameTouched = true;
            const nn::math::VEC2& limit2 = s_EditFrameLimits[m_ZoomLevel][m_SizeKind];
            if (limit2.x != 0.0f || limit2.y != 0.0f) {
                SoundMgr::PlaySound(SE_SYS_EDIT_PREVIEW_ACTIVE, local.x * TOUCH_PAN_SCALE);
            }
        }
        local.z = 0.0f;
        m_pFramePane->SetTranslate(local);
        ClampFrame();
        const nn::math::VEC3 pos = m_pFramePane->m_Translate;
        const nn::math::VEC2& limit3 = s_EditFrameLimits[m_ZoomLevel][m_SizeKind];
        if (limit3.x != 0.0f || limit3.y != 0.0f) {
            nn::math::VEC3 preview = pos;
            SoundMgr::SPlayEditPreview(&preview.x, preview.x * TOUCH_PAN_SCALE);
        }
        return;
    }
    // the circle pad moves the frame
    const f32 step = m_SizeKind == SIZE_KIND_SPECIAL ? 1.0f : 2.0f;
    nn::math::VEC3 pos = m_pFramePane->m_Translate;
    if (BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_UP)) {
        pos.y += step;
    } else if (BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_DOWN)) {
        pos.y -= step;
    }
    if (BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_LEFT)) {
        pos.x -= step;
    } else if (BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_RIGHT)) {
        pos.x += step;
    }
    pos.z = 0.0f;
    if (BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_UP | BsMenuMgr::INPUT_CPAD_DOWN | BsMenuMgr::INPUT_CPAD_LEFT |
                                       BsMenuMgr::INPUT_CPAD_RIGHT)) {
        const nn::math::VEC3 old = m_pFramePane->m_Translate;
        m_pFramePane->SetTranslate(pos);
        ClampFrame();
        const nn::math::VEC3& now = m_pFramePane->m_Translate;
        if ((limit.x != 0.0f || limit.y != 0.0f) && (old.x != now.x || old.y != now.y || old.z != now.z)) {
            nn::math::VEC3 preview = now;
            SoundMgr::SPlayEditPreview(&preview.x, preview.x * TOUCH_PAN_SCALE);
        }
    }
    m_IsFrameTouched = false;
}

// 0x00195194
void BsMenuEdit::ToolFill()
{
    if (!g_TouchState.isTouch || g_TouchState.prevIsTouch) {
        return;
    }
    if (GetTouchPosition(m_pCanvasPicture, NULL)) {
        SaveUndo();
        TextureImage16 image(m_Textures[m_Texture].ptr());
        for (s32 y = 0; y < s_EditCanvasSizes[m_SizeKind][1]; y++) {
            for (s32 x = 0; x < s_EditCanvasSizes[m_SizeKind][0]; x++) {
                if ((image.GetPixel(x, y) & PIXEL_OUTSIDE) != PIXEL_OUTSIDE) {
                    image.SetPixel(x, y, m_DrawColor);
                }
            }
        }
        SoundMgr::PlaySound(SE_SYS_EDIT_PAINT_ALL, 0.0f);
    }
    std::memcpy(m_pUndoPixels[m_UndoTexture], m_pPixels, m_CanvasBytes);
    s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
}

// 0x00198988
void BsMenuEdit::ToolBucket()
{
    if (!g_TouchState.isTouch || g_TouchState.prevIsTouch) {
        return;
    }
    if (!GetTouchPosition(m_pCanvasPicture, NULL)) {
        return;
    }
    SaveUndo();
    nn::math::VEC3 pos;
    if (!GetTouchPosition(m_pCanvasPicture, &pos)) {
        return;
    }
    s32 x;
    s32 y;
    GetCanvasPixel(&x, &y, &pos, 0.0f);
    TextureImage16 image(m_Textures[m_Texture].ptr());
    const u16 target = image.GetPixel(x, y);
    if (m_DrawColor != target && (target & PIXEL_OUTSIDE) != PIXEL_OUTSIDE) {
        FloodFill(x, y, target);
        SoundMgr::PlaySound(SE_SYS_EDIT_PAINT_REGION, GetTouchPan());
    }
    std::memcpy(m_pUndoPixels[m_UndoTexture], m_pPixels, m_CanvasBytes);
    s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
}

// 0x00198AE0
void BsMenuEdit::ToolLine()
{
    // the first pixel of the line (-1: none)
    // 0x00950884
    static s32 s_LineStartX = -1;
    // 0x00950888
    static s32 s_LineStartY = -1;
    if (g_TouchState.isTouch) {
        if (g_TouchState.prevIsTouch && !m_IsDrawing) {
            return;
        }
        nn::math::VEC3 touch;
        if (!GetTouchPosition(m_pCanvasPicture, &touch)) {
            return;
        }
        if (!g_TouchState.prevIsTouch) {
            SaveUndo();
            s_LineStartY = -1;
            s_LineStartX = -1;
            m_IsDrawing = true;
            SoundMgr::PlaySound(SE_SYS_EDIT_LINE_START, GetTouchPan());
        }
        if (!m_IsDrawing) {
            return;
        }
        s32 x;
        s32 y;
        GetCanvasPixel(&x, &y, &touch, 0.0f);
        if (s_LineStartX == -1) {
            s_LineStartX = x;
            s_LineStartY = y;
            SetFlagVisible(m_LineLayout.GetRootPane(), true);
            m_LineLayout.BindAnimation(&m_LineScaleAnim);
            m_LineScaleAnim.SetFrame(m_PixelScale);
            m_LineLayout.Animate();
            m_LineLayout.UnbindAnimation(&m_LineScaleAnim);
            nn::math::VEC3 start;
            GetPixelPosition(&start, s_LineStartX, s_LineStartY);
            MoveToPixelCorner(&start, s_LineStartX, s_LineStartY, m_SizeKind);
            m_pLinePoints[0]->SetTranslate(start);
        }
        nn::math::VEC3 end;
        GetPixelPosition(&end, x, y);
        MoveToPixelCorner(&end, x, y, m_SizeKind);
        m_pLinePoints[1]->SetTranslate(end);
        // the line is drawn again on the pixels before it
        std::memcpy(m_pPixels, m_pUndoPixels[m_UndoTexture], m_CanvasBytes);
        PlotLine(s_LineStartX, s_LineStartY, x, y, &BsMenuEdit::PlotBlock<1>);
        ApplyCanvas();
    } else {
        if (!g_TouchState.prevIsTouch || !m_IsDrawing) {
            return;
        }
        std::memcpy(m_pUndoPixels[m_UndoTexture], m_pPixels, m_CanvasBytes);
        s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
        SetFlagVisible(m_LineLayout.GetRootPane(), false);
        m_IsDrawing = false;
        SoundMgr::PlaySound(SE_SYS_EDIT_LINE_END, GetTouchPan());
    }
}

template <s32 N>
void BsMenuEdit::PlotBlock(s32 x, s32 y)
{
    TextureImage16 image(m_Textures[m_Texture].ptr());
    for (s32 j = 0; j < N; j++) {
        const s32 py = j + y;
        if (py < 0 || py >= s_EditCanvasSizes[m_SizeKind][1]) {
            continue;
        }
        for (s32 i = 0; i < N; i++) {
            const s32 px = i + x;
            if (px < 0 || px >= s_EditCanvasSizes[m_SizeKind][0]) {
                continue;
            }
            if ((image.GetPixel(px, py) & PIXEL_OUTSIDE) == PIXEL_OUTSIDE) {
                continue;
            }
            image.SetPixel(px, py, m_DrawColor);
        }
    }
}

template <s32 N>
void BsMenuEdit::ToolPen()
{
    // the pixel under the touch and the last one (-1: none; statics of each instance)
    static s32 s_PenX;
    static s32 s_PenY;
    static s32 s_PenLastX = -1;
    static s32 s_PenLastY = -1;
    static const SeID s_PenSounds[3] = {SE_SYS_EDIT_PEN_S_DRAW, SE_SYS_EDIT_PEN_M_DRAW, SE_SYS_EDIT_PEN_L_DRAW};
    if (g_TouchState.isTouch) {
        if (g_TouchState.prevIsTouch && !m_IsDrawing) {
            return;
        }
        nn::math::VEC3 touch;
        if (!GetTouchPosition(m_pCanvasPicture, &touch)) {
            return;
        }
        if (!g_TouchState.prevIsTouch) {
            SaveUndo();
            s_PenLastY = -1;
            s_PenLastX = -1;
            m_IsDrawing = true;
        }
        if (!m_IsDrawing) {
            return;
        }
        // (the larger pens are centered on the touch)
        GetCanvasPixel(&s_PenX, &s_PenY, &touch, (1 - N) * 0.5f);
        if (s_PenLastX == -1) {
            PlotBlock<N>(s_PenX, s_PenY);
        } else {
            PlotLine(s_PenLastX, s_PenLastY, s_PenX, s_PenY, &BsMenuEdit::PlotBlock<N>);
        }
        if (s_PenLastX != s_PenX || s_PenLastY != s_PenY) {
            SoundMgr::SPlaySystemSound(s_PenSounds[N - 1], GetTouchPan());
        }
        s_PenLastX = s_PenX;
        s_PenLastY = s_PenY;
        ApplyCanvas();
    } else {
        if (!g_TouchState.prevIsTouch || !m_IsDrawing) {
            return;
        }
        std::memcpy(m_pUndoPixels[m_UndoTexture], m_pPixels, m_CanvasBytes);
        s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
        m_IsDrawing = false;
    }
}

template <BsMenuEdit::ShapeFunc SHAPE>
void BsMenuEdit::ToolShape()
{
    // the first pixel of the shape (-1: none; statics of each instance)
    static s32 s_ShapeStartX = -1;
    static s32 s_ShapeStartY = -1;
    if (g_TouchState.isTouch) {
        if (g_TouchState.prevIsTouch && !m_IsDrawing) {
            return;
        }
        nn::math::VEC3 touch;
        if (!GetTouchPosition(m_pCanvasPicture, &touch)) {
            return;
        }
        if (!g_TouchState.prevIsTouch) {
            SaveUndo();
            s_ShapeStartY = -1;
            s_ShapeStartX = -1;
            m_IsDrawing = true;
            SoundMgr::PlaySound(SE_SYS_EDIT_LINE_START, GetTouchPan());
        }
        if (!m_IsDrawing) {
            return;
        }
        s32 x;
        s32 y;
        GetCanvasPixel(&x, &y, &touch, 0.0f);
        if (s_ShapeStartX == -1) {
            s_ShapeStartX = x;
            s_ShapeStartY = y;
            SetFlagVisible(m_SquareLayout.GetRootPane(), true);
            m_SquareLayout.BindAnimation(&m_SquareScaleAnim);
            m_SquareScaleAnim.SetFrame(m_PixelScale == 0 ? 0.0f : 1.0f);
            m_SquareLayout.Animate();
            m_SquareLayout.UnbindAnimation(&m_SquareScaleAnim);
            // the marks lie at the corner of their pixel (z is not set in the original)
            nn::math::VEC3 corner;
            corner.x = s_EditPixelSizes[m_PixelScale] * -0.5f;
            corner.y = s_EditPixelSizes[m_PixelScale] * 0.5f;
            m_pSquarePointPictures[0]->SetTranslate(corner);
            m_pSquarePointPictures[1]->SetTranslate(corner);
            nn::math::VEC3 start;
            GetPixelPosition(&start, s_ShapeStartX, s_ShapeStartY);
            m_pSquarePoints[0]->SetTranslate(start);
        }
        nn::math::VEC3 end;
        GetPixelPosition(&end, x, y);
        m_pSquarePoints[1]->SetTranslate(end);
        // the marks turn towards each other (x and y of the rotation are not set in the original)
        s32 corner = 0;
        if (x < s_ShapeStartX) {
            corner = 3;
        }
        if (y < s_ShapeStartY) {
            corner ^= 1;
        }
        nn::math::VEC3 rotate;
        rotate.z = corner * 90.0f;
        m_pSquarePoints[0]->SetRotate(rotate);
        rotate.z = (corner ^ 2) * 90.0f;
        m_pSquarePoints[1]->SetRotate(rotate);
        m_SquareLayout.Animate();
        // the shape is drawn again on the pixels before it
        std::memcpy(m_pPixels, m_pUndoPixels[m_UndoTexture], m_CanvasBytes);
        (this->*SHAPE)(s_ShapeStartX, s_ShapeStartY, x, y, &BsMenuEdit::PlotBlock<1>);
        ApplyCanvas();
    } else {
        if (!g_TouchState.prevIsTouch || !m_IsDrawing) {
            return;
        }
        std::memcpy(m_pUndoPixels[m_UndoTexture], m_pPixels, m_CanvasBytes);
        s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
        SetFlagVisible(m_SquareLayout.GetRootPane(), false);
        m_IsDrawing = false;
        SoundMgr::PlaySound(SE_SYS_EDIT_LINE_END, GetTouchPan());
    }
}

template <s32 PATTERN>
void BsMenuEdit::ToolPattern()
{
    if (!g_TouchState.isTouch || g_TouchState.prevIsTouch) {
        return;
    }
    if (GetTouchPosition(m_pCanvasPicture, NULL)) {
        SaveUndo();
        TextureImage16 image(m_Textures[m_Texture].ptr());
        for (s32 y = 0; y < s_EditCanvasSizes[m_SizeKind][1]; y++) {
            for (s32 x = 0; x < s_EditCanvasSizes[m_SizeKind][0]; x++) {
                if ((s_EditPatterns[PATTERN][y & 15] & (1 << (x & 15))) == 0) {
                    continue;
                }
                if ((image.GetPixel(x, y) & PIXEL_OUTSIDE) == PIXEL_OUTSIDE) {
                    continue;
                }
                image.SetPixel(x, y, m_DrawColor);
            }
        }
        SoundMgr::PlaySound(SE_SYS_EDIT_PAINT_ALL, 0.0f);
    }
    std::memcpy(m_pUndoPixels[m_UndoTexture], m_pPixels, m_CanvasBytes);
    s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
}

template <s32 STAMP>
void BsMenuEdit::ToolStamp()
{
    if (g_TouchState.isTouch) {
        if (g_TouchState.prevIsTouch && !m_IsDrawing) {
            return;
        }
        nn::math::VEC3 touch;
        if (!GetTouchPosition(m_pCanvasPicture, &touch)) {
            return;
        }
        if (!g_TouchState.prevIsTouch) {
            SaveUndo();
            m_IsDrawing = true;
            SoundMgr::PlaySound(SE_SYS_EDIT_STAMP_START, GetTouchPan());
        }
        if (!m_IsDrawing) {
            return;
        }
        s32 x;
        s32 y;
        GetCanvasPixel(&x, &y, &touch, 0.5f);
        std::memcpy(m_pPixels, m_pUndoPixels[m_UndoTexture], m_CanvasBytes);
        PlotStamp(x, y, s_EditStamps[STAMP]);
        ApplyCanvas();
    } else {
        if (!g_TouchState.prevIsTouch || !m_IsDrawing) {
            return;
        }
        std::memcpy(m_pUndoPixels[m_UndoTexture], m_pPixels, m_CanvasBytes);
        s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
        m_IsDrawing = false;
        SoundMgr::PlaySound(SE_SYS_EDIT_STAMP_END, GetTouchPan());
    }
}

// 0x007C78D8
template void BsMenuEdit::PlotBlock<1>(s32 x, s32 y);
// 0x007C7988
template void BsMenuEdit::PlotBlock<2>(s32 x, s32 y);
// 0x007C7A58
template void BsMenuEdit::PlotBlock<3>(s32 x, s32 y);
// 0x007C7B28
template void BsMenuEdit::ToolPen<1>();
// 0x007C7DC0
template void BsMenuEdit::ToolPen<2>();
// 0x007C80B8
template void BsMenuEdit::ToolPen<3>();
// 0x007C6E30
template void BsMenuEdit::ToolShape<&BsMenuEdit::PlotEllipse>();
// 0x007C71D4
template void BsMenuEdit::ToolShape<&BsMenuEdit::PlotRect>();
// 0x007C7578
template void BsMenuEdit::ToolPattern<0>();
// 0x007C7728
template void BsMenuEdit::ToolPattern<1>();
// 0x007C8560
template void BsMenuEdit::ToolStamp<0>();
// 0x007C83B0
template void BsMenuEdit::ToolStamp<1>();

// 0x0019478C
void BsMenuEdit::State_EditIn()
{
    if (m_InAnim.IsDone() && m_ModelInAnim.IsDone() && BsMenuBg::GetInstance()->IsWaiting()) {
        m_EditLayout.UnbindAnimation(&m_InAnim);
        m_ModelLayout.UnbindAnimation(&m_ModelInAnim);
        if (m_ToolButton.m_IsAttached == true) {
            m_ToolButton.m_IsAttached = false;
            m_ToolButton.m_pBasePane->RemoveChild(m_ToolButton.m_Layout.GetRootPane());
        }
        Change(&BsMenuEdit::State_ButtonsIn);
        return;
    }
    m_InAnim.IncreaseFrame();
    m_ModelInAnim.IncreaseFrame();
}

// 0x00194874
void BsMenuEdit::Enter_EditIn()
{
    if (!m_IsModelReady) {
        BsMenuBg::s_pInstance->ChangeBg(1);
    }
    if (!m_ToolButton.m_IsAttached) {
        m_ToolButton.m_pBasePane->AppendChild(m_ToolButton.m_Layout.GetRootPane());
        m_ToolButton.m_IsAttached = true;
    }
    m_EditLayout.BindAnimation(&m_InAnim);
    m_InAnim.SetFrame(0.0f);
    m_ModelLayout.BindAnimation(&m_ModelInAnim);
    m_ModelInAnim.SetFrame(0.0f);
}

// 0x00195A60
void BsMenuEdit::State_ButtonsIn()
{
    if (m_ComButton.m_Control.IsBusy() || !static_cast<BsMenuBg*>(BsMenuMgr::Get()->m_pBsMenuBg)->IsChangeDone()) {
        return;
    }
    SvPlayer* player = g_pSvCurrentPlayer;
    if (m_ModelView == 0) {
        m_IsModelReady = true;
        g3d::BeginRender();
        BsMyDesignEditModel::s_pInstance->Func2F56DC(&s_EditDesign, 0);
    }
    if (player == NULL || player->playerFlags.editDesignIntro) {
        Change(&BsMenuEdit::State_Watch);
    } else {
        Change(&BsMenuEdit::State_FirstNoticeIn);
    }
}

// 0x002CD850
void BsMenuEdit::Enter_ButtonsIn()
{
    if (!m_IsModelReady) {
        static_cast<BsMenuBg*>(BsMenuMgr::Get()->m_pBsMenuBg)->StartChange(1, 2);
    }
    m_ComButton.m_Control.StartIn();
}

// 0x00195694
void BsMenuEdit::State_Watch()
{
    UpdateFrame();
    m_ToolButton.Update();
    ButtonActionControl& comControl = m_ComButton.m_Control;
    const s32 comButton = comControl.GetPressedIndex();
    if (comButton != -1) {
        if (comButton == COM_BUTTON_QUIT) {
            Change(&BsMenuEdit::State_CancelConfirmIn);
        } else if (comButton == COM_BUTTON_SAVE) {
            Change(&BsMenuEdit::State_ButtonsOut);
        }
        return;
    }
    const s32 button = m_Control.GetPressedIndex();
    if (button != -1) {
        switch (button) {
        case BUTTON_ZOOM_IN:
            Zoom(true);
            break;
        case BUTTON_ZOOM_OUT:
            Zoom(false);
            break;
        case BUTTON_UNDO:
            // the last state and the current one change places
            if (!m_IsUndoEmpty) {
                nw::lyt::Size& size = m_pUndoPicture->m_Size;
                size.width = -size.width;
                m_Buttons[BUTTON_UNDO].m_SeOk = size.width < 0.0f ? SE_SYS_EDIT_REDO : SE_SYS_EDIT_UNDO;
                m_Buttons[BUTTON_UNDO].m_OkSoundEnabled = true;
                m_UndoTexture ^= 1;
                std::memcpy(m_pPixels, m_pUndoPixels[m_UndoTexture], m_CanvasBytes);
            }
            s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
            break;
        case BUTTON_GRID:
            m_GridButton.Toggle();
            SetFlagVisible(m_pGridPicture, m_GridButton.IsGridOn());
            SetFlagVisible(m_pNaviGridPane, m_GridButton.IsGridOn());
            break;
        case BUTTON_TOOL:
            Change(&BsMenuEdit::State_ToolIn);
            return;
        case BUTTON_PRO:
            Change(&BsMenuEdit::State_ProIn);
            return;
        case BUTTON_MODEL: {
            const s32 view = m_ModelView + 1;
            m_ModelView = view < 0 ? MODEL_VIEW_NUM - 1 : (view > MODEL_VIEW_NUM - 1 ? 0 : view);
            for (u32 i = 0; i < MODEL_VIEW_NUM; i++) {
                SetFlagVisible(m_pModelViewPanes[i], m_ModelView == i);
            }
            BsMyDesignEditModel::s_pInstance->SetView(m_ModelView);
            break;
        }
        }
        m_Control.Reset();
        m_Control.UnselectActive();
        comControl.SetFlags(ButtonActionControl::DISABLED, false);
    }
    if (m_Palette.m_SubScreen != Palette::SUB_SCREEN_NONE) {
        Change(&BsMenuEdit::State_Palette);
        return;
    }
    // the color of the selected palette button and the tool
    m_ColorIndex = m_Palette.m_SelectedButton;
    m_DrawColor = (SvMyDesign::GetColorValue(s_EditDesign.GetColor(static_cast<u8>(m_Palette.m_SelectedButton)), true) &
                   ~PIXEL_OUTSIDE) |
                  m_ColorIndex;
    (this->*s_ToolFuncs[m_Tool])();
}

// 0x00195A04
void BsMenuEdit::Enter_Watch()
{
    m_IsComButtonDisabled = false;
    m_ComButton.m_Control.SetFlags(ButtonActionControl::DISABLED, false);
    m_Control.SetFlags(ButtonActionControl::DISABLED, false);
}

// 0x00195C0C
void BsMenuEdit::State_ButtonsOut()
{
    if (!m_ComButton.m_Control.IsBusy() && static_cast<BsMenuBg*>(BsMenuMgr::Get()->m_pBsMenuBg)->IsChangeDone()) {
        Change(&BsMenuEdit::State_EditOut);
    }
}

// 0x00195DC8
void BsMenuEdit::Enter_ButtonsOut()
{
    // (quit after the cancel dialog: the model and the background end)
    if (m_ComButton.m_Control.GetPressedIndex() == COM_BUTTON_QUIT) {
        g3d::EndRender();
        static_cast<BsMenuBg*>(BsMenuMgr::Get()->m_pBsMenuBg)->StartChange(0, 6);
        BsMyDesignEditModel::s_pInstance->Func53761C();
    }
    m_ComButton.m_Control.StartOut();
}

// 0x00194F48
void BsMenuEdit::State_EditOut()
{
    if (m_OutAnim.IsDone() && m_ModelOutAnim.IsDone() && BsMenuBg::GetInstance()->IsWaiting()) {
        m_EditLayout.UnbindAnimation(&m_OutAnim);
        m_ModelLayout.UnbindAnimation(&m_ModelOutAnim);
        if (m_ComButton.m_Control.GetPressedIndex() != COM_BUTTON_SAVE) {
            Stop();
            return;
        }
        Change(&BsMenuEdit::State_NameInput);
        return;
    }
    m_OutAnim.IncreaseFrame();
    m_ModelOutAnim.IncreaseFrame();
}

// 0x00195104
void BsMenuEdit::Enter_EditOut()
{
    if (!m_ToolButton.m_IsAttached) {
        m_ToolButton.m_pBasePane->AppendChild(m_ToolButton.m_Layout.GetRootPane());
        m_ToolButton.m_IsAttached = true;
    }
    m_EditLayout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetFrame(0.0f);
    m_ModelLayout.BindAnimation(&m_ModelOutAnim);
}

// 0x00195C54
void BsMenuEdit::State_Exit()
{
    if (BsMenuBg::GetInstance()->IsChangeDone()) {
        Stop();
    }
}

// 0x00195E18
void BsMenuEdit::Enter_Exit()
{
    // empty (in the original too)
}

// 0x00195D88
void BsMenuEdit::State_ToolIn()
{
    m_ToolButton.Update();
    if (!m_ToolButton.m_IsAnimating) {
        Change(&BsMenuEdit::State_Tool);
    }
}

// 0x00195E44
void BsMenuEdit::Enter_ToolIn()
{
    m_ToolButton.Change(&ToolButtonNode::State_In);
    m_Control.Erase(&m_ToolButton);
    m_IsSubMenuOpen = true;
}

// 0x00195654
void BsMenuEdit::State_Tool()
{
    m_ToolButton.Update();
    if (m_ToolButton.m_Control.IsSelectOkDone()) {
        Change(&BsMenuEdit::State_ToolOut);
    }
}

// 0x00195A00
void BsMenuEdit::Enter_Tool()
{
    // empty (in the original too)
}

// 0x00196144
void BsMenuEdit::State_ToolOut()
{
    m_ToolButton.Update();
    if (!m_ToolButton.m_IsAnimating) {
        m_Control.AddNode(&m_ToolButton, false);
        m_Control.Reset();
        m_ToolButton.m_IsActive = true;
        m_IsSubMenuOpen = false;
        m_Tool = m_ToolButton.m_SelectedTool;
        Change(&BsMenuEdit::State_Watch);
    }
}

// 0x0019630C
void BsMenuEdit::Enter_ToolOut()
{
    m_ToolButton.Change(&ToolButtonNode::State_Out);
}

// 0x00195F2C
void BsMenuEdit::State_Palette()
{
    if (m_Control.IsSelectOkDone()) {
        m_Control.Reset();
    }
    if (m_Palette.m_IsAnimating) {
        return;
    }
    if (m_Palette.m_IsColorChanged) {
        m_Palette.m_IsColorChanged = false;
        LoadCanvas();
        UpdateDesignTexturesEntry(&s_EditDesign);
        return;
    }
    if (m_Palette.m_SubScreen != Palette::SUB_SCREEN_NONE) {
        return;
    }
    // the sub screen of the palette has closed
    m_EditLayout.SetGroupVisible("G_visible", true);
    HideUnusedFrame(this);
    SetFlagVisible(m_pNaviGridPane, m_GridButton.IsGridOn());
    ActivateButtons(this);
    SetFlagVisible(m_ToolButton.m_Layout.GetRootPane(), true);
    if (s_Settings.isPro) {
        SetFlagVisible(m_ProButton.m_ProLayout.GetRootPane(), true);
    }
    SetFlagVisible(m_ModelLayout.GetRootPane(), true);
    m_ModelButton.m_IsActive = true;
    m_ComButton.m_Control.SetButtonsVisible(true);
    m_IsSubMenuOpen = false;
    Change(&BsMenuEdit::State_Watch);
}

// 0x001961CC
void BsMenuEdit::Enter_Palette()
{
    s_EditDesign.ReadFromTexture(m_UndoTextures[m_UndoTexture].ptr(), static_cast<u8>(m_EditedPart));
    ResetUndo(this);
    m_EditLayout.SetGroupVisible("G_visible", false);
    SetFlagVisible(m_ToolButton.m_Layout.GetRootPane(), false);
    if (s_Settings.isPro) {
        SetFlagVisible(m_ProButton.m_ProLayout.GetRootPane(), false);
    }
    m_Control.SetNodeActive(-1, false);
    m_ModelButton.m_IsActive = true;
    SetFlagVisible(m_ModelLayout.GetRootPane(), false);
    m_ModelButton.m_IsActive = false;
    m_ComButton.m_Control.SetButtonsVisible(false);
    m_IsSubMenuOpen = true;
}

// 0x001969AC
void BsMenuEdit::State_PaletteOut()
{
    if (m_Palette.m_IsAnimating) {
        return;
    }
    m_EditLayout.SetGroupVisible("G_visible", true);
    HideUnusedFrame(this);
    SetFlagVisible(m_pNaviGridPane, m_GridButton.IsGridOn());
    SetFlagVisible(m_ToolButton.m_Layout.GetRootPane(), true);
    if (s_Settings.isPro) {
        SetFlagVisible(m_ProButton.m_ProLayout.GetRootPane(), true);
    }
    ActivateButtons(this);
    m_ComButton.m_Control.SetButtonsVisible(true);
    m_IsSubMenuOpen = false;
    Change(&BsMenuEdit::State_Watch);
}

// 0x00196B34
void BsMenuEdit::Enter_PaletteOut()
{
    if (m_ComButton.m_Control.GetPressedIndex() == COM_BUTTON_QUIT) {
        LoadCanvas();
        UpdateDesignTexturesEntry(&s_EditDesign);
    }
}

// 0x00195B2C
void BsMenuEdit::State_ProIn()
{
    m_ProButton.Update();
    if (!m_ProButton.m_IsAnimating) {
        m_Control.UnselectCurrent();
        m_Control.Reset();
        Change(&BsMenuEdit::State_Pro);
    }
}

// 0x00195B84
void BsMenuEdit::Enter_ProIn()
{
    if (m_ProButton.m_IsAttached) {
        m_ProButton.m_pBasePane->RemoveChild(m_ProButton.m_ProLayout.GetRootPane());
        m_ProButton.m_IsAttached = false;
    }
    m_ProButton.Change(&ProButtonNode::State_In);
    m_Control.SetNodeActive(-1, false);
    m_ProButton.m_IsActive = true;
    m_IsSubMenuOpen = true;
}

// 0x00195028
void BsMenuEdit::State_Pro()
{
    if (!m_Control.HasCurrent()) {
        m_ProButton.Update();
    }
    m_IsComButtonDisabled = m_ProButton.IsBusy();
    if (m_Control.IsSelectOkDone() || m_ProButton.m_IsChosen) {
        Change(&BsMenuEdit::State_ProOut);
    }
}

// 0x00195174
void BsMenuEdit::Enter_Pro()
{
    // empty (in the original too)
}

// 0x00195C7C
void BsMenuEdit::State_ProOut()
{
    m_ProButton.Update();
    if (m_ProButton.m_IsAnimating) {
        return;
    }
    m_Control.UnselectCurrent();
    m_Control.Reset();
    ActivateButtons(this);
    if (!m_ProButton.m_IsAttached) {
        m_ProButton.m_pBasePane->AppendChild(m_ProButton.m_ProLayout.GetRootPane());
        m_ProButton.m_IsAttached = true;
    }
    m_ProButton.m_IsActive = true;
    m_IsSubMenuOpen = false;
    Change(&BsMenuEdit::State_Watch);
}

// 0x00195E1C
void BsMenuEdit::Enter_ProOut()
{
    LoadCanvas();
    m_ProButton.Change(&ProButtonNode::State_Out);
}

// 0x00196550
void BsMenuEdit::State_NameInput()
{
    // the keyboard for the title
    if (g_Unknown951658 != NULL || m_pKeyboard != NULL) {
        if (m_pKeyboard != NULL && sys::g_IsFinished) {
            m_pKeyboard->OnClose();
            m_pKeyboard = NULL;
        }
        return;
    }
    if (sys::g_KeyboardParam.result == 1 || Controller::IsTrig(Controller::B)) {
        // cancelled: back to the editor
        m_ComButton.m_Control.UnselectCurrent();
        m_ComButton.m_Control.Reset();
        Change(&BsMenuEdit::State_EditIn);
        return;
    }
    g3d::EndRender();
    static_cast<BsMenuBg*>(BsMenuMgr::Get()->m_pBsMenuBg)->StartChange(0, 6);
    BsMyDesignEditModel::s_pInstance->Func53761C();
    Change(&BsMenuEdit::State_Exit);
}

// 0x00196720
void BsMenuEdit::Enter_NameInput()
{
    sys::KeyboardParam& param = sys::g_KeyboardParam;
    param._unknown0x004 = 1;
    param._unknown0x115[5] = 1;
    param._unknown0x115[6] = 1;
    param._unknown0x115[7] = 1;
    param.acceptMode = 2;
    param.flags = 48;
    param.maxLength = (GetRegionIndex() == REGION_JAPAN || GetRegionIndex() == REGION_KOREA) ? TITLE_LENGTH_SHORT
                                                                                              : TITLE_LENGTH;
    param.isNewlineAllowed = false;
    param.isFullWidthMask = false;
    BsMenuMgr::Func6D32A0();
    g_Unknown98D508[0] = 77;
    g_Unknown98D508[1] = 79;
    sys::g_Unknown95F0EB = 13;
    sys::g_pInitialText = reinterpret_cast<wchar_t*>(s_EditDesign.title);
    script::WordFix<21> word;
    word.Copy(script::WordCPtrSv(s_EditDesign.title, 21));
    m_Title.copy(sead::SafeStringBase<wchar_t>(reinterpret_cast<const wchar_t*>(word.GetWord())));
    sys::g_pTextBuffer = const_cast<wchar_t*>(m_Title.getCString());
    sys::g_IsBufferOwned = false;
    m_pKeyboard = static_cast<BsSkb*>(ProcMgr::Create(PROC_BS_SKB, this, 0, PROC_TYPE_BASE));
    if (m_pKeyboard != NULL) {
        m_pKeyboard->m_Unknown0x1148 = false;
    }
}

// 0x00196524
void BsMenuEdit::State_FirstNoticeIn()
{
    if (BsCommonDialog::GetInstance() != NULL) {
        Change(&BsMenuEdit::State_FirstNotice);
    }
}

// 0x00196664
void BsMenuEdit::Enter_FirstNoticeIn()
{
    BsMenuMgr::Func6D32D0();
    CommonDialogArg& arg = g_CommonDialogArg;
    arg.kind = 0;
    arg.pFile = MESSAGE_FILE;
    arg.labels[0] = s_EditNoticeLabels[0][m_Notice];
    arg.labels[3] = s_EditNoticeLabels[1][m_Notice];
    arg._unknown0x38 = true;
    arg.isLast = false;
    if (BsCommonDialog::GetInstance() == NULL) {
        BsMenuMgr::Func6D406C(true);
        return;
    }
    if (s_EditNoticeLabels[0][m_Notice + 1] == -1) {
        arg.isLast = true;
        arg.se = SE_SYS_DECIDE_L;
    }
    BsCommonDialog::GetInstance()->ShowNext();
}

// 0x00195E8C
void BsMenuEdit::State_FirstNotice()
{
    if (BsCommonDialog::GetInstance() != NULL) {
        if (BsCommonDialog::GetInstance()->IsWaiting()) {
            m_Notice++;
            if (s_EditNoticeLabels[0][m_Notice] != -1) {
                Change(&BsMenuEdit::State_FirstNoticeIn);
            }
        }
        return;
    }
    // the dialog has closed: the notice is not shown again
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerFlags.editDesignIntro = true;
    }
    Change(&BsMenuEdit::State_Watch);
}

// 0x001961C8
void BsMenuEdit::Enter_FirstNotice()
{
    // empty (in the original too)
}

// 0x00196B6C
void BsMenuEdit::State_CancelConfirmIn()
{
    if (BsCommonDialog::GetInstance() != NULL) {
        Change(&BsMenuEdit::State_CancelConfirm);
    }
}

// 0x00196B98
void BsMenuEdit::Enter_CancelConfirmIn()
{
    CommonDialogArg& arg = g_CommonDialogArg;
    arg.kind = 1;
    arg.pFile = MESSAGE_FILE;
    arg.labels[3] = LABEL_CANCEL_CONFIRM_NO;
    arg.labels[0] = LABEL_CANCEL_CONFIRM;
    arg.labels[1] = LABEL_CANCEL_CONFIRM_YES;
    arg._unknown0x38 = true;
    arg.isLast = true;
    arg._unknown0x3A = false;
    arg.se = SE_SYS_DECIDE_QUIT;
    BsMenuMgr::Func6D406C(true);
    m_IsComButtonDisabled = true;
}

// 0x00196484
void BsMenuEdit::State_CancelConfirm()
{
    if (BsCommonDialog::GetInstance() != NULL) {
        return;
    }
    if (BsMenuMgr::s_Result != 0) {
        // quit without saving
        BsMyDesignEditModel::s_pInstance->Func53761C();
        Change(&BsMenuEdit::State_ButtonsOut);
        return;
    }
    m_IsComButtonDisabled = false;
    m_ComButton.m_Control.UnselectCurrent();
    m_ComButton.m_Control.Reset();
    Change(&BsMenuEdit::State_Watch);
}

// 0x00196660
void BsMenuEdit::Enter_CancelConfirm()
{
    // empty (in the original too)
}
