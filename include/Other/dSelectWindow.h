#pragma once

#include "decomp.h"
#include "state/dMode.h"
#include "Button/dButtonActionControl.h"

namespace nw {
namespace lyt {
class TextBox;
class Pane;
} // namespace lyt
} // namespace nw

// The window of the choices of the talk (member of script::ChoiceStandardWin). Only the layout script uses is known;
// member names are ours. The functions are game code that is not decompiled yet (names are ours, after what
// ChoiceStandardWin does with them).
// RTTI 12SelectWindow @ 0x008CB800
// vtable 0x008EE808 (vptr 0x008EE810), offset_to_top 0, 3 entries
class SelectWindow : public ::state::Mode<SelectWindow>
{
public:
    static const s32 CHOICE_NUM = 6;

    // the text boxes of a choice (text and ruby), a pane that gets the width of the text, the sound of its button
    struct ChoiceText
    {
        nw::lyt::TextBox* pText;   // 0x0
        nw::lyt::TextBox* pRuby;   // 0x4
        nw::lyt::Pane* pPane;      // 0x8
        u32 sound;                 // 0xC SeID
    };
    // the second text boxes of a choice
    struct ChoiceText2
    {
        nw::lyt::TextBox* pText;   // 0x0
        nw::lyt::TextBox* pRuby;   // 0x4
        u8 _unknown0x8[8];
    };

    SelectWindow(); // 0x0020A254
    virtual ~SelectWindow(); // 0x0020A454 slot 0x00
    // 0x0020A3A8 slot 0x04 (deleting dtor)
    virtual const char* GetName(Func func) const; // 0x0082AD88 slot 0x08

    // the window is prepared (true when its layout is ready)
    bool Setup(u32 arg); // 0x00209914
    void Update(); // 0x00209C80
    // the window is released
    void Release(); // 0x00209C48
    // the layout of the window is drawn (registered with the layout manager)
    void Draw(); // 0x002098F8
    // the window opens (the cursor at the choice; -1: none)
    void Open(s32 cursor); // 0x00208E64
    void OpenWithCancel(s32 cancel); // 0x0020A18C
    // the window closes
    void Close(); // 0x002082FC
    // (the states; names are ours)
    // the buttons can be chosen
    void State_Select(); // 0x00209464
    // the window closes
    void State_Close(); // 0x00208CD0
    // whether the window is closed
    bool IsClosed(); // 0x007131DC
    // whether the window is still busy
    bool IsBusy(); // 0x0071322C
    // (inline) whether the ok animation of the chosen button is done
    bool IsSelectDone() const { return m_Control.IsSelectOkDone(); }

    u8 _unknown0x24[0x43C - 0x24];
    ChoiceText m_Texts[CHOICE_NUM];                 // 0x43C
    ChoiceText2 m_Texts2[CHOICE_NUM];               // 0x49C
    u8 _unknown0x4FC[0x504 - 0x4FC];
    s32 m_Cursor;                                   // 0x504 (negative: none)
    u8 _unknown0x508[0x514 - 0x508];
    bool m_IsCanceled;                                // 0x514
    u8 _unknown0x515[0x534 - 0x515];
    f32 m_SliderValue;                              // 0x534 the position of the fixed choices (0..1)
    ButtonActionControl m_Control;                  // 0x538 the buttons of the choices
    u8 _unknown0x61C[0x1192 - 0x61C];
    u8 m_IsNoCancelSound;                           // 0x1192
    u8 m_IsTextChanged;                             // 0x1193 the texts are set again
};
ASSERT_SIZE(SelectWindow, 0x1194);
