#pragma once

#include "decomp.h"
#include "Other/dSelectWindow.h"
#include "script/dChoiceStandard.h"

namespace script {
class RenderRubySet;

// The choices of the talk in a window (TalkSystem::m_pChoice): the select window of the game and a renderer per
// choice (member names are ours).
// RTTI N6script17ChoiceStandardWinE @ 0x008D3974
// vtable 0x0090A34C (vptr 0x0090A354), offset_to_top 0, 15 entries
class ChoiceStandardWin : public ::script::ChoiceStandard
{
public:
    static const s32 ITEM_NUM = 8;
    static const s32 RENDER_NUM = 6;

    ChoiceStandardWin(); // 0x005E8454 (name after the RTTI)
    virtual ~ChoiceStandardWin(); // 0x005E8574 slot 0x00
    // 0x005E8564 slot 0x04 (deleting dtor)
    virtual void Update(); // 0x007131D4 slot 0x10
    virtual s32 GetCursor() const; // 0x0075C18C slot 0x18
    virtual bool IsCancelPressed() const; // 0x0075C1E0 slot 0x20
    virtual s32 GetCancelValue() const; // 0x00724208 slot 0x24
    virtual void OnSetup(); // 0x005E83F4 slot 0x30
    virtual void OnDecide(); // 0x0020A1E4 slot 0x34
    virtual bool IsReady() const; // 0x0075C178 slot 0x38

    // the texts of the choices into the renderers and the text boxes of the window
    void UpdateTexts(); // 0x005E7FD8
    // the renderers get the text boxes of the window once it is set up; true when they have them
    bool SetupTextBoxes(u32 arg); // 0x005E8230
    // the state machine and the window
    void Run(); // 0x005E82B0

    // the positions of the fixed choices on the slider (the cursor is the first one above it)
    static const f32 s_SliderSteps[7];

    u8 m_IsSetup;                              // 0x0D8
    u8 m_IsTextSet;                            // 0x0D9
    SelectWindow m_Window;                     // 0x0DC
    s32 m_RequestCursor;                       // 0x1270 the choice the net play makes (-1: none)
    RenderRubySet* m_pRenders[RENDER_NUM];     // 0x1274
};
ASSERT_SIZE(ChoiceStandardWin, 0x128C);
} // namespace script
