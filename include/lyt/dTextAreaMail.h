#pragma once

#include "decomp.h"
#include "Item/dResLoader.h"
#include "Sv/dSvFgName.h"
#include "lyt/dTextAreaEx.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "script/dRenderGarden.h"
#include "script/dWordFix.h"
#include "script/dWordPtr.h"
#include "sys/dSwkbd.h"
#include "Sv/dSvMail.h"

struct SvMail;
namespace sys {
class LineManager;
} // namespace sys
namespace nw {
namespace lyt {
class TextBox;
} // namespace lyt
} // namespace nw

namespace lyt {
// (game code, not decompiled; names are ours) the text of a letter split into the lines of its text boxes
class LetterText
{
public:
    LetterText(); // 0x006AB238
    ~LetterText(); // 0x006AB378
    void Format(SvMail* mail, nw::lyt::TextBox** textBoxes, s32 unknown, bool isEditing); // 0x006AB188
    script::IWord* GetLine(s32 index); // 0x00767B1C

    u8 m_Data[0x3D0];
};
// (game code, not decompiled; names are ours) the check of the text of a letter (UGC)
struct TextCheck
{
    TextCheck(); // 0x00524C68
    bool IsDone(); // 0x00524B80
    void Start(script::IWord* text); // 0x00524BD4

    u32 m_Unknown0x0;   // 0x0
    u32 m_Unknown0x4;   // 0x4
    u8 m_Unknown0x8;    // 0x8
    u8 m_Unknown0x9;    // 0x9
    u8 m_Unknown0xA;    // 0xA
    s32 m_Result;       // 0xC (>= 3: Func5CCF74, else Func5CCFA4)
};
ASSERT_SIZE(TextCheck, 0x10);

// (game code, not decompiled) the header, body and signature of a mail as words; the results of the check
script::WordPtr Func5CCFD4(SvMail* mail); // 0x005CCFD4
script::WordPtr Func5CCF84(SvMail* mail); // 0x005CCF84
script::WordPtr Func5CCFB4(SvMail* mail); // 0x005CCFB4
void Func5CCF74(SvMail* mail); // 0x005CCF74
void Func5CCFA4(SvMail* mail); // 0x005CCFA4
// (game code) the kind of the letter paper, its item name and the color frame of the paper
bool Func75910C(); // 0x0075910C
SvFgName Func6BC8A4(u8 kind, u8 paperId); // 0x006BC8A4
u32 Func76BF8C(const SvFgName* paper); // 0x0076BF8C
s32 Func5D51A8(); // 0x005D51A8
// (a global of the game code: the letter being written lies at +0x118)
extern u8* g_pUnknown950BD0;

// The text area of the letters (ltr_base): the header (with the name of the receiver in it), the six lines of the
// body and the signature are edited one at a time in T_edit_00, page 0 is the header, 1..6 the body, 7 the
// signature; the other parts are shown in the boxes of s_TextBoxNames. The paper is loaded as a texture of the
// letter. Only the class name is from the RTTI; all other names are ours.
// RTTI N3lyt12TextAreaMailE @ 0x008D0F1C
// vtable 0x009042CC (vptr 0x009042D4), offset_to_top 0, 25 entries
class TextAreaMail : public ::lyt::TextAreaEx
{
public:
    // the parts of the letter (m_Mode)
    enum Mode
    {
        MODE_HEADER,
        MODE_BODY,
        MODE_FOOTER,
    };
    // where the cursor is in the header (m_HeaderCursorPart)
    static const s32 HEADER_BEFORE_NAME = 3;
    static const s32 HEADER_AFTER_NAME = 4;
    static const s32 TEXT_BOX_NUM = 8;

    // (inline)
    TextAreaMail(u32 unknown, s32 priority, sead::Heap* heap)
        : TextAreaEx(unknown, priority, heap), m_PaperKind(41), m_pPaperAnim(NULL), m_KeptTop(0), m_Mode(MODE_BODY),
          m_HeaderCursorPart(HEADER_BEFORE_NAME), m_NameLength(0), m_LastLength(0), m_ColorIndex(0), m_LineWidth(0.0f),
          m_HighlightColor(255, 255, 255, 255), m_pMail(NULL), m_Lines{53, 56, 56, 56, 56, 56, 56, 56}
    {
    }
    virtual ~TextAreaMail(); // 0x00504FEC slot 0x00
    // 0x00504F20 slot 0x04 (deleting dtor)
    void Calc(bool isAnimate) override; // 0x00503918 slot 0x08
    void Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader) override; // 0x005034DC slot 0x10
    bool GetIndexAtPosition(s32* index, const nn::math::VEC2* pos) override; // 0x00502FE0 slot 0x14
    const char* GetLayoutName() override { return "ltr_base"; } // 0x007478A4 slot 0x1C
    void ClampCursor(s32* index) override; // 0x00502F38 slot 0x38
    // the page after the first line at once
    void vf_0x44() override; // 0x00501F04 slot 0x44
    void vf_0x48() override; // 0x00502538 slot 0x48
    // isShown: the text is stored in the letter; the parts are shown (all, or all but the edited one)
    void vf_0x58(bool isShown, bool isEditing) override; // 0x005025A0 slot 0x58

    // the x of the text: right-aligned text (the signature) is moved by the space left in the line
    DECOMP_NOINLINE f32 AdjustX(f32 x, f32 offset); // 0x00501F6C
    // the typed text into its part of the letter; the lines are formatted again
    void StoreText(bool isEditing); // 0x00502018
    DECOMP_NOINLINE void FormatLines(bool isEditing); // 0x00502350
    // the parts of the letter shown or hidden
    DECOMP_NOINLINE void SetPartsVisible(bool isHeader, bool isBody, bool isFooter); // 0x0050260C
    // the check of the body: done (the result is given to the letter) / started
    bool CheckText(); // 0x005026B8
    void StartTextCheck(); // 0x00502704
    // the edited text into its part (without ending the input)
    DECOMP_NOINLINE void StoreCurrentPart(); // 0x00502754
    // the edit indicator next to the edited part
    DECOMP_NOINLINE void UpdateIndicator(); // 0x00502968
    // the text boxes and the paper of the layout
    DECOMP_NOINLINE void SetupTextBoxes(); // 0x00502A50
    DECOMP_NOINLINE void SetupPaper(); // 0x00502B84
    // the first line follows the cursor and the page follows the first line
    DECOMP_NOINLINE void ScrollToCursorPage(); // 0x005033BC

    // (inline) the header follows the changes of its text before the name; the colors of the name
    void UpdateHeader(s32 cursor);
    // (inline) line feeds only in the body, until its last line
    void UpdateNewlineAllowed(sys::LineManager* lines, s32 cursor);
    // (inline) the length of the header text before the name follows the typed text
    void UpdateHeaderLength();

    static const char* s_TextBoxNames[TEXT_BOX_NUM];

    item::ResLoader m_PaperLoader;                     // 0x74C
    u8 m_PaperKind;                                    // 0x860
    nw::gfx::res::ResImageTexture m_PaperTexture;      // 0x864
    AnmObject* m_pPaperAnim;                           // 0x868 ltr_base_color
    s32 m_KeptTop;                                     // 0x86C the first line of the body
    s32 m_Mode;                                        // 0x870 Mode
    s32 m_HeaderCursorPart;                            // 0x874
    s32 m_NameLength;                                  // 0x878 the name of the receiver
    s32 m_LastLength;                                  // 0x87C
    script::WordFix<9> m_Name;                         // 0x880
    script::WordFix<197> m_Body;                       // 0x8AC (for the check)
    TextCheck m_TextCheck;                             // 0xA50
    s16 m_ColorBounds[4];                              // 0xA60 the indices where the color changes
    s16 m_ColorIndex;                                  // 0xA68
    f32 m_LineWidth;                                   // 0xA6C
    nw::ut::Color8 m_HighlightColor;                   // 0xA70 the name in the header
    SvMail* m_pMail;                                   // 0xA74
    LetterText m_Text;                                 // 0xA78
    nw::lyt::TextBox* m_pTextBoxes[TEXT_BOX_NUM];      // 0xE48
    script::RenderGarden m_Lines[TEXT_BOX_NUM];        // 0xE68
};
ASSERT_OFFSET(TextAreaMail, m_PaperLoader, 0x74C);
ASSERT_OFFSET(TextAreaMail, m_TextCheck, 0xA50);
ASSERT_OFFSET(TextAreaMail, m_pTextBoxes, 0xE48);
ASSERT_SIZE(TextAreaMail, 0x1488);
// (inline) the length of the header text before the name follows the typed text
inline void TextAreaMail::UpdateHeaderLength()
{
    sys::TextManager* manager = sys::g_pTextManager;
    if (m_LastLength != manager->m_Length) {
        if (m_HeaderCursorPart == HEADER_BEFORE_NAME) {
            const s32 indent = manager->m_Length - m_LastLength + m_pMail->receiverNameIndent;
            m_pMail->receiverNameIndent = static_cast<u8>(indent);
            sys::g_pTextManager->m_Unknown0x90 = indent;
        }
        m_LastLength = sys::g_pTextManager->m_Length;
    }
}
} // namespace lyt
