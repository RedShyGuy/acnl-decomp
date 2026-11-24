#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "state/dMode.h"

namespace nw {
namespace lyt {
class TextBox;
class Pane;
} // namespace lyt
} // namespace nw

// The window of the choices of the talk as a list with a title (member of script::ChoiceStandardItem). Only the layout
// script uses is known; member names are ours. The functions are game code that is not decompiled yet (names are ours,
// after what ChoiceStandardItem does with them).
// RTTI 16ItemSelectWindow @ 0x008CC338
// vtable 0x008F2A74 (vptr 0x008F2A7C), offset_to_top 0, 3 entries
class ItemSelectWindow : public ::state::Mode<ItemSelectWindow>
{
public:
    static const s32 CHOICE_NUM = 6;

    // the text box of a choice, a pane that gets the width of the text, the sound of its button
    struct ChoiceText
    {
        nw::lyt::TextBox* pText;  // 0x0
        nw::lyt::Pane* pPane;     // 0x4
        u32 sound;                // 0x8 SeID
    };

    ItemSelectWindow(); // 0x002BB0CC
    virtual ~ItemSelectWindow(); // 0x002BB248 slot 0x00
    // 0x002BB1D8 slot 0x04 (deleting dtor)
    virtual const char* GetName(Func func) const; // 0x0082BD00 slot 0x08

    // whether the window is prepared without a layout argument
    bool IsSetup(); // 0x002BAD54
    // the window is prepared (true when its layout is ready)
    bool Setup(u32 arg); // 0x002BA9BC
    void Update(); // 0x002BACB8
    // the name pane N_itm_nm_00 (game code that is not decompiled yet; name is ours)
    void SetNameVisible(bool visible); // 0x002BA070
    // the layout is drawn on the bottom screen (game code that is not decompiled yet; name is ours)
    void Draw(); // 0x002BA9A0
    // the window opens (the cancel choice)
    void Open(s32 cancel); // 0x002BB054
    // (the states; names are ours)
    // the window closes
    void State_Close(); // 0x002BA3AC
    // whether the window is closed
    bool IsClosed(); // 0x0071DD70
    // whether the window is still busy
    bool IsBusy(); // 0x0071DDC0

    u8 _unknown0x24[0x3BC - 0x24];
    ChoiceText m_Texts[CHOICE_NUM];   // 0x3BC
    nw::lyt::TextBox* m_pTitleText;   // 0x404
    u8 _unknown0x408[0x4B4 - 0x408];
    nw::lyt::Pane* m_pAttachPane;      // 0x4B4 the pane the window shows at (DragIconControl::UpdateItemWindow; name is ours)
    u8 _unknown0x4B8[0x4BC - 0x4B8];
    s32 m_Cursor;                     // 0x4BC
    u8 _unknown0x4C0[0x4CC - 0x4C0];
    bool m_IsCanceled;                  // 0x4CC
    u8 _unknown0x4CD[0x4CF - 0x4CD];
    bool m_IsNameVisible;               // 0x4CF N_itm_nm_00 (name is ours)
    u8 _unknown0x4D0[0x4D4 - 0x4D0];
    nn::math::VEC3 m_AttachPosition;    // 0x4D4 (name is ours)
    u8 _unknown0x4E0[0xB10 - 0x4E0];
};
ASSERT_SIZE(ItemSelectWindow, 0xB10);
