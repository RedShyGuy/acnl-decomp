#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"
#include "script/dWordFix.h"

namespace lyt {
// The list of the tours of the island (layout tour_list_cntnt, rows T_tour_NN): the names are the strings
// STR_Tour_name of the tour IDs (only the class name is from the RTTI; all other names are ours).
// RTTI N3lyt8TourListE @ 0x008D0FA8
// vtable 0x009045B4 (vptr 0x009045BC), offset_to_top 0, 26 entries
class TourList : public ::InstSelect<8>
{
public:
    // (inline)
    TourList()
    {
        m_TextLength = 70;
        m_LayoutName = "tour_list_cntnt";
        m_TextBoxName = "T_tour";
    }
    virtual ~TourList(); // 0x0050C9E0 slot 0x00
    // 0x0050C9A0 slot 0x04 (deleting dtor)
    s32 GetSelectedId() const override { return m_Selected; } // 0x00747AC4 slot 0x20
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x0050C51C slot 0x30
    bool SetupItems() override; // 0x0050C90C slot 0x34
    script::IWord* GetItemWord(s32 index) override { return &m_Names[index]; } // 0x00747ACC slot 0x64

    script::WordFix<70> m_Names[64];  // 0x12CC
};
ASSERT_OFFSET(TourList, m_Names, 0x12CC);
ASSERT_SIZE(TourList, 0x3BCC);

// (the tours of the game code, not decompiled: the manager and its number of tours / the ID of a tour)
extern void* g_pUnknown95D734;
u32 Func76FD4C(void* manager); // 0x0076FD4C
u32 Func76FD78(void* manager, u32 index); // 0x0076FD78
} // namespace lyt
