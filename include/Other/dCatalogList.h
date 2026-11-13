#pragma once

#include "decomp.h"
#include "List/dInstCatalog.h"
#include "List/dTab.h"
#include "script/dWordFix.h"

#include "Sv/dSvPersonalId.h"

struct SvMuseum;
namespace script {
class RenderGarden;
} // namespace script

// (game code, not decompiled; names are ours) the first and the last name and the number of the items of a kind (the
// table 0x878778 has one entry per item::Kind)
struct ItemNameRange
{
    u16 first;  // 0x0
    u16 last;   // 0x2
    u32 count;  // 0x4
    u32 unk_0x8;
};
const ItemNameRange* Func5360A8(u8 kind); // 0x005360A8
// (game code, methods of SvMuseum) the item was given to the museum / by a player of the town (the donor is filled in)
bool Func76D8A8(SvMuseum* museum, const SvFgName* name); // 0x0076D8A8
bool Func76D974(SvMuseum* museum, SvPersonalId* donor, const SvFgName* name); // 0x0076D974

// The list of the donation board outside the museum (Nookipedia "Museum": "a board that lists every exhibit and their
// donors"; not part of the catalog, although its class name and the layouts ctlg_cntnt_01 / ctlg_tab are the
// catalog's): four tabs, named after their icon panes, with the items given to the museum (P_fsl_00 fossils, P_bug_00
// insects, P_fish_00 fish and sea creatures, P_draw_00 paintings and sculptures); the second text per row
// (N_list_base_NN, T_name_NN) is the name of the player who gave the item. Only the class name is from the RTTI; all
// other names are ours.
// RTTI 11CatalogList @ 0x008CB29C
// vtable 0x008ECA18 (vptr 0x008ECA20), offset_to_top 0, 25 entries
class CatalogList : public ::InstCatalog<8>
{
public:
    static const s32 TAB_NUM = 4;
    // the parts of the lists (fossils, insects, fish, paintings, sea creatures, sculptures)
    static const s32 PART_NUM = 6;
    static const s32 FOSSIL_MAX = 67;
    static const s32 INSECT_MAX = 72;
    static const s32 FISH_MAX = 102;
    static const s32 ART_MAX = 33;

    CatalogList(); // 0x001C6ACC
    virtual ~CatalogList(); // 0x001C6D04 slot 0x00
    // 0x001C6CF4 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x001C6228 slot 0x0C
    void Update() override; // 0x001C6300 slot 0x10
    void Draw() override; // 0x001C594C slot 0x14 (falls into CatalogBase::Draw)
    bool IsDecided() const override { return m_Tab.m_Control.m_pCurrent != NULL; } // 0x0070E53C slot 0x18
    bool IsDeciding() override { return m_Tab.IsBusy(); } // 0x0070E554 slot 0x1C
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x001C5D94 slot 0x30
    bool SetupItems() override; // 0x001C6784 slot 0x34
    void InitializeScroll() override; // 0x001C6208 slot 0x38
    void OnScroll() override; // 0x001C6040 slot 0x3C
    void UpdateItemNum() override; // 0x001C61D8 slot 0x44
    void OnEnterIn() override; // 0x001C64E4 slot 0x4C
    void OnEnterOut() override; // 0x001C650C slot 0x54
    void UpdateWait() override; // 0x001C6A84 slot 0x60

    // the items of a kind that the museum has, into a part (names, items, donors); returns the number
    s32 CollectPart(s32 part, u8 kind); // 0x001C6528

    // the icon pane of each tab
    static const char* s_TabPaneNames[TAB_NUM];
    static const char* s_ListPaneName;

    Tab<TAB_NUM> m_Tab;                             // 0xAC0
    ContentNode m_BaseContents[8];                  // 0x172C N_list_base_NN
    script::RenderGarden* m_pDonorRenders;          // 0x180C T_name_NN
    script::WordFix<33>* m_pNames[PART_NUM];        // 0x1810
    u16* m_pIds[PART_NUM];                          // 0x1828
    script::WordFix<10>* m_pDonors[PART_NUM];       // 0x1840
    s32 m_ItemNums[TAB_NUM];                        // 0x1858
    script::WordFix<33> m_FossilNames[FOSSIL_MAX];  // 0x1868
    script::WordFix<33> m_InsectNames[INSECT_MAX];  // 0x307C
    script::WordFix<33> m_FishNames[FISH_MAX];      // 0x4A5C fish and sea creatures
    script::WordFix<33> m_ArtNames[ART_MAX];        // 0x6F04 paintings and sculptures
    script::WordFix<10> m_FossilDonors[FOSSIL_MAX]; // 0x7AE0
    script::WordFix<10> m_InsectDonors[INSECT_MAX]; // 0x8664
    script::WordFix<10> m_FishDonors[FISH_MAX];     // 0x92C4
    script::WordFix<10> m_ArtDonors[ART_MAX];       // 0xA44C
    u16 m_FossilIds[FOSSIL_MAX];                    // 0xA9F8
    u16 m_InsectIds[INSECT_MAX];                    // 0xAA7E
    u16 m_FishIds[FISH_MAX];                        // 0xAB0E
    u16 m_ArtIds[ART_MAX];                          // 0xABDA
    s32 m_ShownTab;                                 // 0xAC1C the tab whose items are shown
    u32 m_DonorLength;                              // 0xAC20 of the donor RenderGardens
};
ASSERT_OFFSET(CatalogList, m_Tab, 0xAC0);
ASSERT_OFFSET(CatalogList, m_BaseContents, 0x172C);
ASSERT_OFFSET(CatalogList, m_pNames, 0x1810);
ASSERT_OFFSET(CatalogList, m_FishNames, 0x4A5C);
ASSERT_OFFSET(CatalogList, m_FossilDonors, 0x7AE0);
ASSERT_OFFSET(CatalogList, m_FossilIds, 0xA9F8);
ASSERT_OFFSET(CatalogList, m_ShownTab, 0xAC1C);
ASSERT_SIZE(CatalogList, 0xAC24);
