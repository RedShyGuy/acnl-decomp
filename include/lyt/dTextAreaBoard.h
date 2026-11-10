#pragma once

#include "decomp.h"
#include "lyt/dTextAreaEx.h"
#include "script/dRenderGarden.h"
#include "script/dWordPtr.h"

struct SvBulletinBoardMessage;
namespace sys {
class LineManager;
} // namespace sys
namespace nw {
namespace lyt {
class TextBox;
} // namespace lyt
} // namespace nw

namespace lyt {
// (game code, not decompiled; names are ours) the text of a message of the bulletin board split into the lines of
// its text boxes
class BoardMessageText
{
public:
    BoardMessageText(); // 0x001AF020
    ~BoardMessageText(); // 0x001AF198
    void Format(void* message, nw::lyt::TextBox** textBoxes, s32 lineNum, s32 lineNum2, bool isEditing); // 0x001AECF4
    script::IWord* GetLine(s32 index); // 0x0070BCF4

    u8 m_Data[0x4BC];
};
// (game code, not decompiled) the number of messages of the board / the text of a message as a word
s32 Func118B88(const SvBulletinBoardMessage* messages); // 0x00118B88 (Thumb)
script::WordPtr Func1187AA(void* message); // 0x001187AA (Thumb)
// (a global of the game code: the message being edited lies at +0x434)
extern u8* g_pUnknown9506D4;

// The text area of the bulletin board (board_paper_B): the edited text in T_edit_00 scrolls line by line (a "page" is
// a line) in N_set, the other lines of the message are shown in the boxes of s_TextBoxNames (only the class name is
// from the RTTI; all other names are ours).
// RTTI N3lyt13TextAreaBoardE @ 0x008D0F28
// vtable 0x00904340 (vptr 0x00904348), offset_to_top 0, 25 entries
class TextAreaBoard : public ::lyt::TextAreaEx
{
public:
    // the boxes: six lines, the footer, the date and the two page numbers
    static const s32 TEXT_BOX_NUM = 10;
    static const s32 LINE_NUM = 6;

    // (inline)
    TextAreaBoard(u32 unknown, s32 priority, sead::Heap* heap)
        : TextAreaEx(unknown, priority, heap), m_LineWidth(0.0f), m_pMessage(NULL), m_Lines{56, 56, 56, 56, 56, 56, 56, 11, 3, 3}
    {
    }
    virtual ~TextAreaBoard(); // 0x00506BCC slot 0x00
    // 0x00506B48 slot 0x04 (deleting dtor)
    void Calc(bool isAnimate) override; // 0x00505D50 slot 0x08
    void Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader) override; // 0x005059B0 slot 0x10
    bool GetIndexAtPosition(s32* index, const nn::math::VEC2* pos) override; // 0x0050569C slot 0x14
    const char* GetLayoutName() override { return "board_paper_B"; } // 0x007478B8 slot 0x1C
    // the first line at the top / at most the fifth
    void vf_0x44() override; // 0x0050526C slot 0x44
    void vf_0x48() override; // 0x0050554C slot 0x48
    // isShown: the text is stored in the message and the lines are formatted again; the line boxes are shown or not
    void vf_0x58(bool isShown, bool isEditing) override; // 0x005055BC slot 0x58

    // the line boxes of the layout
    DECOMP_NOINLINE void SetupTextBoxes(); // 0x005050BC
    // the message into the lines of the boxes
    DECOMP_NOINLINE void FormatLines(bool isEditing); // 0x005052DC
    // the first line follows the cursor and the page follows the first line
    DECOMP_NOINLINE void ScrollToCursorPage(); // 0x00505894
    // the typed text into the message
    void StoreText(); // 0x00506AA8
    // (inline) line feeds are allowed until the last line is reached
    void UpdateNewlineAllowed(sys::LineManager* lines, s32 cursor);

    static const char* s_TextBoxNames[TEXT_BOX_NUM];

    f32 m_LineWidth;                                  // 0x74C the width of the line boxes
    void* m_pMessage;                                 // 0x750
    BoardMessageText m_Text;                          // 0x754
    nw::lyt::TextBox* m_pTextBoxes[TEXT_BOX_NUM];     // 0xC10
    script::RenderGarden m_Lines[TEXT_BOX_NUM];       // 0xC38
};
ASSERT_OFFSET(TextAreaBoard, m_Text, 0x754);
ASSERT_OFFSET(TextAreaBoard, m_pTextBoxes, 0xC10);
ASSERT_SIZE(TextAreaBoard, 0x13E0);
} // namespace lyt
