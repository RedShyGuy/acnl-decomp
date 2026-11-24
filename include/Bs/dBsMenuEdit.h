#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Button/dButtonActionCstmNode.h"
#include "Button/dButtonActionNode.h"
#include "Other/dComButton2Lyt.h"
#include "Other/dGridButtonNode.h"
#include "Other/dLayout.h"
#include "Other/dMenuBase.h"
#include "Other/dPalette.h"
#include "Other/dProButtonNode.h"
#include "Other/dToolButtonNode.h"
#include "Sv/dSvMyDesign.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/lyt/lyt_TexMap.h"
#include "nw/lyt/lyt_Types.h"
#include "sead/seadWFixedSafeString.h"
#include "ssys/ma/dHeapAllocator.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "state/dMode.h"

namespace nn {
namespace math {
struct VEC2;
struct VEC3;
} // namespace math
} // namespace nn
class BsSkb;
namespace nw {
namespace lyt {
class Group;
class Pane;
class Picture;
} // namespace lyt
} // namespace nw

// The editor of the designs (class name from the RTTI, the state names from the table of the binary, all other names
// are ours): edit.bclyt with the canvas of the edited part of the design (BsMenuEdit::s_EditDesign), the palette, the
// tools (ToolButtonNode: pens of three sizes, line, circle, rectangle, fill, stamps, patterns), the parts of the pro
// designs (ProButtonNode), zoom, the grid switch, undo, the model of the design (edit_model.bclyt) and the common
// buttons quit / save (ComButton2Lyt). The canvas is a texture of the part (RGBA4 with the color index in the low
// nibble, 15 = outside of the shape) that is drawn in two copies (one is shown while the other is written) and a second
// pair of textures for undo.
// RTTI 10BsMenuEdit @ 0x008CAFC4
// vtable 0x008EBEBC (vptr 0x008EBEC4), offset_to_top 0, 25 entries
// vtable 0x008EBF28 (vptr 0x008EBF30), offset_to_top -40, 3 entries
class BsMenuEdit : public ::MenuBase, public ::state::Mode<BsMenuEdit>
{
public:
    // a function that sets one pixel of the canvas (the tools)
    typedef void (BsMenuEdit::*PlotFunc)(s32 x, s32 y);
    // a function that draws a shape between two pixels with a PlotFunc
    typedef void (BsMenuEdit::*ShapeFunc)(s32 x0, s32 y0, s32 x1, s32 y1, PlotFunc plot);
    // a tool of ToolButtonNode (the index of s_ToolFuncs)
    typedef void (BsMenuEdit::*ToolFunc)();

    // the size kinds of the canvas (the index of s_CanvasSizes): the sleeves, the normal designs, the pro bodies, the
    // special designs of 64x64
    static const s32 SIZE_KIND_MAX = 4;
    static const s32 SIZE_KIND_SPECIAL = 4;
    // the zoom levels (the special designs have one more)
    static const s32 ZOOM_LEVEL_MAX = 2;
    static const s32 ZOOM_LEVEL_MAX_SPECIAL = 3;
    static const s32 TOOL_NUM = 12;
    // the views of the model (edit_model.bclyt, P_icon_00..03)
    static const u8 MODEL_VIEW_NUM = 4;
    // the buttons of the control
    enum Button
    {
        BUTTON_ZOOM_IN = 0,   // B_zoom_01
        BUTTON_ZOOM_OUT = 1,  // B_zoom_00
        BUTTON_UNDO = 2,      // B_undo
        BUTTON_GRID = 3,      // the grid switch (GridButtonNode)
        BUTTON_MODEL = 4,     // the model (edit_model.bclyt)
        BUTTON_TOOL = ToolButtonNode::BUTTON_INDEX_SELF,
        BUTTON_PRO = ProButtonNode::BUTTON_INDEX_SELF,
    };
    // the common buttons (ComButton2Lyt)
    enum ComButton
    {
        COM_BUTTON_QUIT = 0,
        COM_BUTTON_SAVE = 1,
    };

    // the request of the game for the editor (BsMenuEdit::s_Settings, set by SetSettings before the menu opens)
    struct Settings
    {
        bool isOpen;      // 0x0 between Initialize and Finalize
        bool isPro;       // 0x1 the design is wearable (pattern types below 6): the parts can be chosen
        bool isSpecial;   // 0x2 a design of a design stand of 64x64 (pattern type 8, SvStrcData::stands)
        u32 request;      // 0x4 bits 0-3: the design slot, 4-7: the pattern type of a new design (12: keep), 8-11: the
                          //     slot of the special design in the garden data, 12: special
        s32 designSlot;   // 0x8 the design of the current player (-1: none)
        s32 specialSlot;  // 0xC the design stand (-1: none)
    };

    BsMenuEdit(); // 0x00198E20
    // 0x00776E48 (thunk)
    // 0x00776E34 (deleting thunk)
    // 0x0019930C (deleting dtor)
    ~BsMenuEdit() override; // 0x0019931C slot 0x00
    oml::framework::Result Initialize() override; // 0x00196E98 slot 0x0C
    oml::framework::Result Finalize() override; // 0x00197A40 slot 0x18
    oml::framework::Result Calc() override; // 0x00197654 slot 0x24
    oml::framework::Result Draw() override; // 0x00196C0C slot 0x30

    // the request of the next editor (a normal design: slot and pattern type; a special one: garden slot and slot)
    static void SetSettings(u32 slot, u32 patternType); // 0x00195178
    static void SetSpecialSettings(u32 specialSlot, u32 slot); // 0x00195A40

    // the size animation of the navigation and the canvas, the pixel scale, then the frame
    void UpdateCanvasSize(); // 0x00194308
    // the shapes of the tools in the rectangle (x0, y0)..(x1, y1)
    void PlotEllipse(s32 x0, s32 y0, s32 x1, s32 y1, PlotFunc plot); // 0x0019449C
    void PlotRect(s32 x0, s32 y0, s32 x1, s32 y1, PlotFunc plot); // 0x00194648
    // a line from (x0, y0) to (x1, y1)
    void PlotLine(s32 x0, s32 y0, s32 x1, s32 y1, PlotFunc plot); // 0x00198724
    // the pixels of the canvas into the undo copy before a change (the undo button becomes available)
    void SaveUndo(); // 0x0019491C
    // the frame of the visible part stays inside the canvas (even positions below the special size)
    void ClampFrame(); // 0x001949BC
    // the pixel of the canvas under a position in the canvas pane (offset: added before rounding)
    void GetCanvasPixel(s32* x, s32* y, const nn::math::VEC3* pos, f32 offset); // 0x00194ABC
    // the textures of the canvas and of undo from the edited part of the design
    void SetupCanvas(); // 0x00194BC0
    // the center of a pixel of the canvas in the canvas pane
    void GetPixelPosition(nn::math::VEC3* pos, s32 x, s32 y); // 0x001955AC
    // the texture coordinates of the canvas and the navigation for the frame
    void UpdateCanvasView(); // 0x0019530C
    // the canvas becomes the design / the design becomes the canvas (and the undo copy)
    void ApplyCanvas(); // 0x0019632C
    void LoadCanvas(); // 0x0019638C
    // the panes and the groups of the layouts
    void FindPanes(); // 0x00197DC4
    // the size of the grid picture follows its texture
    void UpdateGridTexture(); // 0x0019816C
    // one zoom level in / out
    void Zoom(bool in); // 0x00198270
    // the region of the color of a pixel takes the drawing color
    void FloodFill(s32 x, s32 y, u16 target); // 0x0019855C
    // a stamp of 16x16 bits around x, y
    void PlotStamp(s32 x, s32 y, const u16* stamp); // 0x00198888
    // whether the touch is inside of pane (pos: the touch in the pane)
    bool GetTouchPosition(nw::lyt::Pane* pane, nn::math::VEC3* pos); // 0x00193E08
    // the frame moves with a touch on the navigation or with the circle pad
    void UpdateFrame(); // 0x00193F08

    // the tools (s_ToolFuncs)
    void ToolFill(); // 0x00195194 the whole shape in the color
    void ToolBucket(); // 0x00198988 the region of a color
    void ToolLine(); // 0x00198AE0
    template <s32 N>
    void ToolPen(); // 0x007C7B28 <1>, 0x007C7DC0 <2>, 0x007C80B8 <3>
    template <ShapeFunc SHAPE>
    void ToolShape(); // 0x007C6E30 <PlotEllipse>, 0x007C71D4 <PlotRect>
    template <s32 PATTERN>
    void ToolPattern(); // 0x007C7578 <0>, 0x007C7728 <1>
    template <s32 STAMP>
    void ToolStamp(); // 0x007C8560 <0>, 0x007C83B0 <1>
    // N x N pixels from x, y (only inside of the shape)
    template <s32 N>
    void PlotBlock(s32 x, s32 y); // 0x007C78D8 <1>, 0x007C7988 <2>, 0x007C7A58 <3>

    // the states (the names in the comments are the debug names of the binary)
    void State_EditIn();             // 0x0019478C 編集ウィンドウ入場処理 (the edit window comes in)
    void Enter_EditIn();             // 0x00194874
    void State_ButtonsIn();          // 0x00195A60 汎用ボタン入場処理 (the common buttons come in)
    // (its code lies in front of ControlButtonBase::StartIn and falls into it)
    void Enter_ButtonsIn();          // 0x002CD850
    void State_Watch();              // 0x00195694 監視処理 (watching the input)
    void Enter_Watch();              // 0x00195A04
    void State_ButtonsOut();         // 0x00195C0C 汎用ボタン退場処理 (the common buttons go out)
    void Enter_ButtonsOut();         // 0x00195DC8
    void State_EditOut();            // 0x00194F48 編集ウィンドウ退場処理 (the edit window goes out)
    void Enter_EditOut();            // 0x00195104
    void State_Exit();               // 0x00195C54 退場 (exit)
    void Enter_Exit();               // 0x00195E18
    void State_ToolIn();             // 0x00195D88 ツール表示処理 (the tools come in)
    void Enter_ToolIn();             // 0x00195E44
    void State_Tool();               // 0x00195654 ツール処理 (choosing a tool)
    void Enter_Tool();               // 0x00195A00
    void State_ToolOut();            // 0x00196144 ツール終了処理 (the tools go out)
    void Enter_ToolOut();            // 0x0019630C
    void State_Palette();            // 0x00195F2C パレット監視 (watching the palette)
    void Enter_Palette();            // 0x001961CC
    void State_PaletteOut();         // 0x001969AC パレット退場 (the palette goes out)
    void Enter_PaletteOut();         // 0x00196B34
    void State_ProIn();              // 0x00195B2C プロ表示処理 (the pro parts come in)
    void Enter_ProIn();              // 0x00195B84
    void State_Pro();                // 0x00195028 プロ処理 (choosing a pro part)
    void Enter_Pro();                // 0x00195174
    void State_ProOut();             // 0x00195C7C プロ終了処理 (the pro parts go out)
    void Enter_ProOut();             // 0x00195E1C
    void State_NameInput();          // 0x00196550 名前入力ウィンドウ (the name input window)
    void Enter_NameInput();          // 0x00196720
    void State_FirstNoticeIn();      // 0x00196524 初回注意メッセージ入場 (the first notice comes in)
    void Enter_FirstNoticeIn();      // 0x00196664
    void State_FirstNotice();        // 0x00195E8C 初回注意メッセージ (the first notice)
    void Enter_FirstNotice();        // 0x001961C8
    void State_CancelConfirmIn();    // 0x00196B6C キャンセル確認ウィンドウ入場待ち (waiting for the cancel window)
    void Enter_CancelConfirmIn();    // 0x00196B98
    void State_CancelConfirm();      // 0x00196484 キャンセル確認応答待ち (waiting for the answer of the cancel window)
    void Enter_CancelConfirm();      // 0x00196660

    // the design being edited (a copy; the palette and the pickers work on it)
    static SvMyDesign s_EditDesign;
    static Settings s_Settings;
    typedef ::state::Mode<BsMenuEdit>::Info StateInfo;
    static const StateInfo s_StateInfos[19];
    static const ToolFunc s_ToolFuncs[TOOL_NUM];

    s32 m_InitStep;                               // 0x004C
    ssys::ma::HeapAllocator m_Allocator;          // 0x0050 "BsMenuEdit" (the textures of the canvas)
    ssys::ma::lyt::ArcResAccReader m_Reader;      // 0x0058 Layout/Edit/edit.arc
    ::Layout m_EditLayout;                        // 0x02A0 edit.bclyt
    ::Layout m_LineLayout;                        // 0x03EC edit_t_line.bclyt (the marks of the line tool)
    ::Layout m_SquareLayout;                      // 0x0538 edit_t_square.bclyt (the marks of the shape tools)
    ::Layout m_ModelLayout;                       // 0x0684 edit_model.bclyt
    ssys::ma::lyt::Animation m_InAnim;            // 0x07D0 edit_in
    ssys::ma::lyt::Animation m_OutAnim;           // 0x07F8 edit_out
    ssys::ma::lyt::Animation m_ScaleAnim;         // 0x0820 edit_scale (the pixel scale)
    ssys::ma::lyt::Animation m_SizeAnim;          // 0x0848 edit_size (the size kind)
    ssys::ma::lyt::Animation m_NgAnim;            // 0x0870 edit_ng (a zoom button that cannot be used)
    ssys::ma::lyt::Animation m_ToolInAnim;        // 0x0898 edit_tool_in
    ssys::ma::lyt::Animation m_ToolOutAnim;       // 0x08C0 edit_tool_out
    ssys::ma::lyt::Animation m_ModelInAnim;       // 0x08E8 edit_model_in
    ssys::ma::lyt::Animation m_ModelOutAnim;      // 0x0910 edit_model_out
    ssys::ma::lyt::Animation m_LineScaleAnim;     // 0x0938 edit_t_line_scale
    ssys::ma::lyt::Animation m_SquareScaleAnim;   // 0x0960 edit_t_square_scale
    nw::lyt::Pane* m_pNaviGridPane;               // 0x0988 P_navi_grid
    nw::lyt::Pane* m_pNaviBgPane;                 // 0x098C P_naviBG_00 (the frame is moved by touching it)
    nw::lyt::Pane* m_pFramePane;                  // 0x0990 W_frame_3px (special) or W_frame: the visible part
    u32 m_Unknown0x994;                           // 0x0994 (only cleared)
    nw::lyt::Pane* m_pMainPane;                   // 0x0998 N_main (the marks of the tools are its children)
    nw::lyt::Pane* m_pLinePoints[2];              // 0x099C edit_t_line N_point_00/01
    nw::lyt::Pane* m_pSquarePoints[2];            // 0x09A4 edit_t_square N_point_00/01
    nw::lyt::Group* m_pMainGroup;                 // 0x09AC G_main
    nw::lyt::Group* m_pNaviGroup;                 // 0x09B0 G_navi
    nw::lyt::Group* m_pZoomInGroup;               // 0x09B4 G_zoom_01
    nw::lyt::Group* m_pZoomOutGroup;              // 0x09B8 G_zoom_00
    nw::lyt::Picture* m_pGridPicture;             // 0x09BC P_main_grid
    nw::lyt::Picture* m_pCanvasPicture;           // 0x09C0 P_main_00
    nw::lyt::Picture* m_pNaviPicture;             // 0x09C4 P_navi_00
    nw::lyt::Picture* m_pUndoPicture;             // 0x09C8 P_arrow (flipped when undo changes)
    nw::lyt::Picture* m_pSquarePointPictures[2];  // 0x09CC edit_t_square P_point_00/01
    nw::lyt::Pane* m_pModelViewPanes[MODEL_VIEW_NUM]; // 0x09D4 edit_model P_icon_00..03
    u8 m_ColorIndex;                              // 0x09E4 the color button of the palette
    u16 m_DrawColor;                              // 0x09E6 RGBA4 with the color index in the low nibble
    nw::gfx::res::ResImageTexture m_Textures[2];  // 0x09E8 the two copies of the canvas
    u16* m_pPixels;                               // 0x09F0 the pixels of the copy that is written
    u8 m_Texture;                                 // 0x09F4 the copy that is written (Draw shows it and swaps)
    nw::lyt::TexMap m_TexMap;                     // 0x09F8 the shown copy in the canvas pictures
    nw::lyt::TexSize m_GridTexSize;               // 0x0A18 the texture size of P_main_grid
    nw::gfx::res::ResImageTexture m_UndoTextures[2]; // 0x0A1C
    u16* m_pUndoPixels[2];                        // 0x0A24
    u8 m_UndoTexture;                             // 0x0A2C the copy of the last state
    nw::lyt::Picture* m_pFacePicture;             // 0x0A30 P_face_00
    u32 m_Unknown0xA34;                           // 0x0A34 (only cleared)
    GridButtonNode m_GridButton;                  // 0x0A38
    ToolButtonNode m_ToolButton;                  // 0x0DA4
    ProButtonNode m_ProButton;                    // 0x1C04
    ButtonActionNode m_Buttons[3];                // 0x38B8 zoom in, zoom out, undo
    ButtonActionCstmNode m_ModelButton;           // 0x3B58 edit_model B_btn_00
    ButtonActionControl m_Control;                // 0x3C38
    Palette m_Palette;                            // 0x3D1C
    ComButton2Lyt m_ComButton;                    // 0x5C6C
    s32 m_SizeKind;                               // 0x6B4C the index of s_CanvasSizes
    s32 m_ZoomLevel;                              // 0x6B50
    s32 m_PixelScale;                             // 0x6B54 the index of s_PixelSizes
    s32 m_CanvasWidth;                            // 0x6B58
    s32 m_CanvasHeight;                           // 0x6B5C
    u32 m_CanvasBytes;                            // 0x6B60
    bool m_IsFrameTouched;                        // 0x6B64 a touch on the navigation moves the frame
    bool m_IsDrawing;                             // 0x6B65 a tool is being used
    bool m_IsSubMenuOpen;                         // 0x6B66 tools, pro parts or palette (the control is disabled)
    bool m_IsUndoEmpty;                           // 0x6B67 no undo yet
    bool m_IsComButtonDisabled;                   // 0x6B68
    bool m_IsModelReady;                          // 0x6B69 the model has its design (the background stays)
    bool m_IsModelShown;                          // 0x6B6A
    s8 m_Tool;                                    // 0x6B6B the index of s_ToolFuncs
    s32 m_EditedPart;                             // 0x6B6C
    s32 m_ScrollX;                                // 0x6B70 the first visible pixel
    s32 m_ScrollY;                                // 0x6B74
    u8 m_Unknown0x6B78[8];                        // 0x6B78 (never accessed)
    s32 m_Notice;                                 // 0x6B80 the message of the first notice
    s16 m_ModelAngle;                             // 0x6B84
    bool m_IsModelRotating;                       // 0x6B86 (stops while L or R is held)
    u8 m_ModelView;                               // 0x6B87
    sead::WFixedSafeString<21> m_Title;           // 0x6B88
    BsSkb* m_pKeyboard;                           // 0x6BC0 the keyboard for the title
};
ASSERT_OFFSET(BsMenuEdit, m_InitStep, 0x4C);
ASSERT_OFFSET(BsMenuEdit, m_EditLayout, 0x2A0);
ASSERT_OFFSET(BsMenuEdit, m_InAnim, 0x7D0);
ASSERT_OFFSET(BsMenuEdit, m_pNaviGridPane, 0x988);
ASSERT_OFFSET(BsMenuEdit, m_pModelViewPanes, 0x9D4);
ASSERT_OFFSET(BsMenuEdit, m_Textures, 0x9E8);
ASSERT_OFFSET(BsMenuEdit, m_TexMap, 0x9F8);
ASSERT_OFFSET(BsMenuEdit, m_UndoTextures, 0xA1C);
ASSERT_OFFSET(BsMenuEdit, m_pFacePicture, 0xA30);
ASSERT_OFFSET(BsMenuEdit, m_GridButton, 0xA38);
ASSERT_OFFSET(BsMenuEdit, m_ToolButton, 0xDA4);
ASSERT_OFFSET(BsMenuEdit, m_ProButton, 0x1C04);
ASSERT_OFFSET(BsMenuEdit, m_Buttons, 0x38B8);
ASSERT_OFFSET(BsMenuEdit, m_ModelButton, 0x3B58);
ASSERT_OFFSET(BsMenuEdit, m_Control, 0x3C38);
ASSERT_OFFSET(BsMenuEdit, m_Palette, 0x3D1C);
ASSERT_OFFSET(BsMenuEdit, m_ComButton, 0x5C6C);
ASSERT_OFFSET(BsMenuEdit, m_SizeKind, 0x6B4C);
ASSERT_OFFSET(BsMenuEdit, m_IsFrameTouched, 0x6B64);
ASSERT_OFFSET(BsMenuEdit, m_Notice, 0x6B80);
ASSERT_OFFSET(BsMenuEdit, m_Title, 0x6B88);
ASSERT_OFFSET(BsMenuEdit, m_pKeyboard, 0x6BC0);
ASSERT_SIZE(BsMenuEdit, 0x6BC4);
