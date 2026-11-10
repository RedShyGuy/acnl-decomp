#pragma once

#include "decomp.h"
#include "gui/dKeySet.h"

namespace gui {
class ButtonBase;
class TchOnButton;

// The key set of the symbols and the European letters ("Keytop_grid"): 36 character keys in a grid plus space, enter,
// backspace and shift; 16 key sets (sign_keytop_00..03, euro_keytop_00..05 and their shifted _c versions). Only the
// class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui10GridKeySetE @ 0x008D0DD0
// vtable 0x00903D00 (vptr 0x00903D08), offset_to_top 0, 9 entries
class GridKeySet : public ::gui::KeySet
{
public:
    explicit GridKeySet(ssys::ma::lyt::ArcResAccReader* arcReader); // 0x004F54E0
    ~GridKeySet() override; // 0x004F5650 slot 0x00
    // 0x004F559C slot 0x04 (deleting dtor)
    bool Initialize(s32 label) override; // 0x004F4224 slot 0x08
    // (the enter key is only enabled while a line feed is allowed or something is being composed)
    void Update() override; // 0x004F4F10 slot 0x0C
    void OnEvent(Widget* sender, s32 event) override; // 0x004F5298 slot 0x10
    void SetVisible(bool visible) override; // 0x004F4100 slot 0x14
    bool IsVisible() override; // 0x004F5288 slot 0x18
    // (the shift key follows; the keys of the key set are enabled)
    void SetLabel(s32 label) override; // 0x004F5040 slot 0x1C
    lyt::ObjectEx* GetLayout() override; // 0x004F5280 slot 0x20

    // the label is one of the shifted key sets (10-15)
    bool IsShiftedLabel(s32 label); // 0x004F40EC
    // the shifted key set of a label (-1: none)
    s32 GetShiftedLabel(s32 label); // 0x004F41D0
    // the page number "page/pageNum" in T_page
    void SetPage(s32 page, s32 pageNum); // 0x004F51B8

    static const char* s_LayoutName;
    static const char* s_AnimN0S1;
    static const char* s_AnimT0S1;
    static const char* s_AnimI0;
    static const char* s_CharLabels[LABEL_NUM];
    // the key sets of the shift key (in dGridKeySetTable.cpp, GCC would fold it)
    static const ShiftEntry s_ShiftLabels[6];

    lyt::ObjectEx* m_pLayout;                       // 0x110
    ButtonBase* m_pButtons[KEY_MAX];                // 0x114
    bool m_IsKeyVisible[KEY_MAX][LABEL_NUM];        // 0x214
    bool m_IsKeyEnabled[KEY_MAX][LABEL_NUM];        // 0x614 (the character is forbidden, a space or missing)
    s32 m_KeyNum;                                   // 0xA14
    s32 m_ShiftKey;                                 // 0xA18 (-1: none)
    TchOnButton* m_pEnterButton;                    // 0xA1C
};
ASSERT_SIZE(GridKeySet, 0xA20);
} // namespace gui
