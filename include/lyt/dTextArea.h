#pragma once

#include "decomp.h"
#include "lyt/dObject.h"
#include "lyt/dTextAreaTagProcessor.h"
#include "nn/math/math_MTX44.h"
#include "nn/math/math_Vector2.h"
#include "nw/font/font_TextWriterBase.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
class Group;
class TextBox;
} // namespace lyt
} // namespace nw

namespace lyt {
class AnmObject;

// The text being typed on the software keyboard (sys::TextManager): the layout "<GetLayoutName()>.bclyt" with the
// text box T_textArea (and T_textArea_guide, shown while the text is empty), the lines P_baseLine_NN, a blinking
// cursor (DecorCursor.bclyt) and highlights (DecorArea_*.bclyt) for the romaji being composed, the selection and the
// multi-tap key, attached to the pane N_decor. The text of the visible lines is rebuilt each frame with color tags
// (char 12) and the line feed symbol. Only the class name is from the RTTI; all other names are ours.
// RTTI N3lyt8TextAreaE @ 0x008D0F9C
// vtable 0x00904574 (vptr 0x0090457C), offset_to_top 0, 14 entries
class TextArea : public ::lyt::Object
{
public:
    // the character the text area writes for a color (two more characters hold the color) / shows for a line feed
    static const u16 CHAR_COLOR = 12;
    static const u16 CHAR_LINE_FEED_SYMBOL = 0xE02F;
    // the drawn text, with the tags
    static const s32 DRAW_BUFFER_LENGTH = 0x201;

    TextArea(u32 unknown, s32 priority, sead::Heap* heap); // 0x0050C33C
    virtual ~TextArea(); // 0x0050C450 slot 0x00
    // 0x0050C43C slot 0x04 (deleting dtor)
    // the text, the cursor and the highlights of the visible lines
    void Calc(bool isAnimate) override; // 0x0050B56C slot 0x08
    // builds the layout from arcReader and the decorations from decorArcReader (NULL: arcReader)
    virtual void Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader); // 0x0050B0AC slot 0x10
    // the index of the character at the position of the lower screen; true if it is behind the end of its line
    virtual bool GetIndexAtPosition(s32* index, const nn::math::VEC2* pos); // 0x0050ADDC slot 0x14
    virtual void vf_0x18(); // 0x0050A7FC slot 0x18
    virtual const char* GetLayoutName() { return "TextArea_01"; } // 0x00747A44 slot 0x1C
    // the decorations of each kind (one per line)
    virtual s32 GetDecorNum() { return 4; } // 0x00747ABC slot 0x20
    // the lines of the text box
    virtual s32 GetLineNum() { return 4; } // 0x00747A58 slot 0x24
    // the lines the cursor stays within when it scrolls
    virtual s32 GetScrollLineNum() { return 2; } // 0x00747AAC slot 0x28
    // the lines shown above the first scrolled line
    virtual s32 GetLineOffset() { return 1; } // 0x00747AB4 slot 0x2C
    // the decoration objects, attached to the pane
    virtual void CreateDecor(ssys::ma::lyt::ArcResAccReader* arcReader, const char* paneName); // 0x0050A800 slot 0x30
    // the drawn text into the text box
    virtual void ApplyText(u16 length); // 0x0050ACD8 slot 0x34

    // (inline) the width of a character with the writer of the text area (0 for control characters)
    DECOMP_EMIT_COPY DECOMP_NOINLINE f32 GetCharWidth(u16 c) // 0x00747A60
    {
        if (c < ' ') {
            return 0.0f;
        }
        if (m_Writer.m_IsWidthFixed) {
            return m_Writer.m_FixedWidth;
        }
        return m_Writer.m_pFont->GetCharWidth(c) * m_Writer.m_ScaleH;
    }

    // the text manager gets the initial text of the keyboard
    DECOMP_NOINLINE void LoadInitialText(); // 0x0050ABBC
    // the cursor is shown at the end of the previous line (it stays so while the cursor is at a line start)
    DECOMP_NOINLINE bool UpdateLineEnd(); // 0x0050AC34
    void SetLineEnd(bool isLineEnd); // 0x0050AD00
    // the line of the cursor
    DECOMP_NOINLINE s32 GetCursorLine(); // 0x0050AD98
    // the first line follows the cursor
    DECOMP_NOINLINE void ScrollToCursor(); // 0x0050AFDC
    // (inline, the parts of Calc that TextAreaOneLine, TextAreaBoard and TextAreaMail share; variant VARIANT_*)
    enum Variant
    {
        VARIANT_TEXT_AREA,
        VARIANT_BOARD,
        VARIANT_MAIL,
    };
    void BuildDrawText(s32 cursor, s32 variant);
    f32 DecorX(s32 variant, f32 x, f32 maskedX);
    void UpdateCursorAnim(s32 cursor);
    // writes the color tag into the text; the characters written (0 without space)
    static s32 WriteColorTag(wchar_t* dst, s32 space, nw::ut::Color8* color); // 0x00507C7C

    nw::lyt::TextBox* m_pTextBox;                  // 0x1B0 T_textArea
    nw::lyt::TextBox* m_pGuideTextBox;             // 0x1B4 T_textArea_guide
    Object* m_pCursor;                             // 0x1B8 DecorCursor
    AnmObject* m_pCursorAnim;                      // 0x1BC DecorCursor_blink
    nw::lyt::Pane* m_pCursorPane;                  // 0x1C0 N_decorCursor
    nw::lyt::Pane* m_pCursorMSPane;                // 0x1C4 P_decorCursorMS
    f32 m_LineTop;                                 // 0x1C8 the y of the first line
    Object* m_pRomanDecors[4];                     // 0x1CC DecorArea_roman (the romaji being composed)
    nw::lyt::Pane* m_pRomanDecorPanes[4];          // 0x1DC
    Object* m_pSelectDecors[4];                    // 0x1EC DecorArea_select (the selection)
    nw::lyt::Pane* m_pSelectDecorPanes[4];         // 0x1FC
    Object* m_pCellPhoneDecor;                     // 0x20C DecorArea_cellphone (the multi-tap key)
    nw::lyt::Pane* m_pCellPhoneDecorPane;          // 0x210
    wchar_t m_DrawBuffer[DRAW_BUFFER_LENGTH];      // 0x214
    s32 m_TopLine;                                 // 0x618
    s32 m_LastCursor;                              // 0x61C
    u32 m_ObjectUnknown;                           // 0x620 (passed to the decorations as Object::m_Unknown0x17C)
    bool m_IsMasked;                               // 0x624 the characters have the fixed width (a password)
    bool m_IsLineEnd;                              // 0x625
    nw::font::TextWriterBase<wchar_t> m_Writer;    // 0x628 measures the characters
    TextAreaTagProcessor m_TagProcessor;           // 0x68C of the text box
    nn::math::MTX44 m_InvMtx;                      // 0x690 from the lower screen into the text box
    nw::ut::Color8 m_TextColor;                    // 0x6D0
    nw::lyt::Pane* m_pBaseLines[8];                // 0x6D4 P_baseLine_NN (two per line)
    nw::lyt::Group* m_pTextAreaGroup;              // 0x6F4 (set by TextAreaOneLineComTitle: G_textArea_N of its box)
};
ASSERT_OFFSET(TextArea, m_DrawBuffer, 0x214);
ASSERT_OFFSET(TextArea, m_Writer, 0x628);
ASSERT_OFFSET(TextArea, m_InvMtx, 0x690);
ASSERT_SIZE(TextArea, 0x6F8);

// a line feed of the keyboard text is shown with the color (written by the game elsewhere, purpose unknown)
extern nw::ut::Color8 g_LineFeedColor;
// the pane of the text areas the decorations are attached to ("N_decor")
extern const char* g_DecorPaneName;
// (set by TextAreaEx/TextAreaMail and the gui text areas; non-zero: the looping cursor animation gets the play mode 5)
extern u32 g_Unknown95F1A4;
} // namespace lyt
