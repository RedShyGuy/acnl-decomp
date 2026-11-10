#pragma once

#include "decomp.h"
#include "gui/dTextArea.h"
#include "lyt/dTextAreaEx.h"

namespace lyt {
class Object;
}

namespace gui {
class EditAreaEx;
class RepeatButtonEx2;
class TchOnButtonSp;

// The controls of a lyt::TextAreaEx (pages): the field (EditAreaEx on G_edit_00), the arrow keys of the key layout
// ("Key_arrow" animations on G_btn_T/G_btn_B, also pressed with the d-pad, scroll a line) and the touch areas of the
// upper and lower part (TchOnButtonSp, enabled on the pages that are not shown in that part). The scroll buttons of
// TextArea are not used. Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui10TextAreaExE @ 0x008D0DE8
// vtable 0x00903D58 (vptr 0x00903D60), offset_to_top 0, 7 entries
class TextAreaEx : public ::gui::TextArea
{
public:
    // the arrow keys and the touch areas
    enum
    {
        BUTTON_TOP,
        BUTTON_BOTTOM,
        BUTTON_NUM
    };

    // (inline; the touch areas are not initialized)
    TextAreaEx() : m_pKeyLayout(NULL), m_pEditAreaEx(NULL)
    {
        m_pArrowButtons[BUTTON_TOP] = NULL;
        m_pArrowButtons[BUTTON_BOTTOM] = NULL;
    }
    ~TextAreaEx() override; // 0x004F7650 slot 0x00
    // 0x004F763C slot 0x04 (deleting dtor)
    // the arrow buttons and the touch areas follow the lines and the page
    void Update() override; // 0x004F73C0 slot 0x0C
    using TextArea::Initialize;
    // the controls of the text area; keyLayout has the arrow keys and the touch areas (the edit field of the text area)
    virtual void Initialize(lyt::TextAreaEx* textArea, lyt::Object* keyLayout); // 0x004F6FC8 slot 0x14
    // the pages of the text (0: the board scrolls by lines only)
    virtual u32 GetPageNum() { return 1; } // 0x004F6F80 slot 0x18

    // the touch area of the lower part is deleted
    void DeleteBottomTouch(); // 0x004F6F20
    // the arrow buttons are reset (without the release)
    void ResetArrows(); // 0x004F6F4C
    void SetArrowsActive(bool active); // 0x004F6F88

    // (inline)
    lyt::TextAreaEx* GetTextAreaEx() { return static_cast<lyt::TextAreaEx*>(m_pTextArea); }

    static const char* s_EditGroupName;
    static const char* s_ArrowGroupNames[BUTTON_NUM];

    lyt::Object* m_pKeyLayout;                        // 0x1C
    EditAreaEx* m_pEditAreaEx;                        // 0x20
    RepeatButtonEx2* m_pArrowButtons[BUTTON_NUM];     // 0x24
    TchOnButtonSp* m_pTouchButtons[BUTTON_NUM];       // 0x2C B_text_T/B_text_B
};
ASSERT_SIZE(TextAreaEx, 0x34);
} // namespace gui
