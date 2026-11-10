#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "List/dTab.h"
#include "script/dWordFix.h"

// The catalog category Furniture (lists of the items of the current player) with four tabs, named after their icon
// panes: P_leaf_00 (item::KIND_DUMMY_FTR), P_wall_00 (wallpapers), P_flr_00 (carpets) and P_takumi_00
// (item::KIND_DUMMY_FTR with the other collection mode of CollectIds: "Takumi" is the code name of Happy Home Designer,
// the furniture that Welcome amiibo adds for it); each tab has its items sorted by the sort table and keeps its
// selection and scroll. Only the class name is from the RTTI; all other names are ours.
// RTTI 7FtrList @ 0x008CD3B4
// vtable 0x008F90F0 (vptr 0x008F90F8), offset_to_top 0, 26 entries
class FtrList : public ::InstSelect<8>
{
public:
    static const s32 TAB_NUM = 4;
    static const s32 WALL_MAX = 159;
    static const s32 FLOOR_MAX = 168;
    static const s32 FTR_MAX = 1773;
    static const s32 UNKNOWN_NUM = 5931;

    FtrList(); // 0x0060D79C
    virtual ~FtrList(); // 0x0060DA4C slot 0x00
    // 0x0060D918 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x0060D280 slot 0x0C
    void Update() override; // 0x0060D2FC slot 0x10
    void Draw() override; // 0x0060D254 slot 0x14
    bool IsDecided() const override // 0x0075E51C slot 0x18
    {
        return m_Control.m_pCurrent != NULL || m_Tab.m_Control.m_pCurrent != NULL;
    }
    bool IsDeciding() override // 0x0075E544 slot 0x1C
    {
        return m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing() || m_Tab.IsBusy();
    }
    s32 GetSelectedId() const override // 0x0075E5B8 slot 0x20
    {
        if (m_Selected == -1) {
            return -1;
        }
        return m_pIds[m_ShownTab][m_Selected];
    }
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x0060D114 slot 0x30
    bool SetupItems() override; // 0x0060D5C4 slot 0x34
    void InitializeScroll() override; // 0x0060D230 slot 0x38
    void UpdateItemNum() override; // 0x0060D1E8 slot 0x44
    void OnEnterIn() override; // 0x0060D578 slot 0x4C
    void OnEnterOut() override; // 0x0060D5A4 slot 0x54
    void UpdateWait() override; // 0x0060D730 slot 0x60
    script::IWord* GetItemWord(s32 index) override { return &m_pNames[m_ShownTab][index]; } // 0x0075E5F4 slot 0x64

    // the icon pane of each tab
    static const char* s_TabPaneNames[TAB_NUM];

    Tab<TAB_NUM> m_Tab;                          // 0x12CC
    script::WordFix<33>* m_pNames[TAB_NUM];      // 0x1F38 the names of each tab
    u16* m_pIds[TAB_NUM];                        // 0x1F48 the items of each tab
    s32 m_ItemNums[TAB_NUM];                     // 0x1F58
    script::WordFix<33> m_WallNames[WALL_MAX];   // 0x1F68 tab 1
    script::WordFix<33> m_FloorNames[FLOOR_MAX]; // 0x588C tab 2
    script::WordFix<33> m_LeafNames[FTR_MAX];    // 0x94EC tab 0
    script::WordFix<33> m_TakumiNames[FTR_MAX];  // 0x31218 tab 3 (Happy Home Designer furniture)
    u16 m_WallIds[WALL_MAX];                     // 0x58F44
    u16 m_FloorIds[FLOOR_MAX];                   // 0x59082
    u16 m_LeafIds[FTR_MAX];                      // 0x591D2
    u16 m_TakumiIds[FTR_MAX];                    // 0x59FAC
    u8 m_Unknown0x5AD88[0x18];                   // 0x5AD88 (not used by the list)
    script::WordFix<33> m_Unknown0x5ADA0[UNKNOWN_NUM]; // 0x5ADA0 (not used by the list)
    u8 m_Unknown0xE0114[0x2E40];                 // 0xE0114 (not used by the list)
    s32 m_ShownTab;                              // 0xE2F54 the tab whose items are shown
};
ASSERT_OFFSET(FtrList, m_pNames, 0x1F38);
ASSERT_OFFSET(FtrList, m_FloorNames, 0x588C);
ASSERT_OFFSET(FtrList, m_TakumiNames, 0x31218);
ASSERT_OFFSET(FtrList, m_WallIds, 0x58F44);
ASSERT_OFFSET(FtrList, m_TakumiIds, 0x59FAC);
ASSERT_OFFSET(FtrList, m_Unknown0x5ADA0, 0x5ADA0);
ASSERT_OFFSET(FtrList, m_ShownTab, 0xE2F54);
ASSERT_SIZE(FtrList, 0xE2F58);
