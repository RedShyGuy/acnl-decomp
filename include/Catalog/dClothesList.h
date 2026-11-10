#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "List/dTab.h"
#include "script/dWordFix.h"

// The catalog category Clothing (lists of the items of the current player) with seven tabs, named after their icon
// panes: P_top_00 (tops), P_btm_00 (bottoms), P_drs_00 (one-piece outfits), P_cap_00 (caps), P_acs_00 (accessories),
// P_shoe_00 (shoes and socks) and P_umb_00 (umbrellas); each tab has its items sorted by the sort table and keeps its
// selection and scroll. The items are collected over three frames. Only the class name is from the RTTI; all other
// names are ours.
// RTTI 11ClothesList @ 0x008CB2A8
// vtable 0x008ECA84 (vptr 0x008ECA8C), offset_to_top 0, 26 entries
class ClothesList : public ::InstSelect<8>
{
public:
    static const s32 TAB_NUM = 7;
    // the parts of the lists: the tabs and the socks behind the shoes
    static const s32 PART_NUM = 8;
    static const s32 TOPS_MAX = 493;
    static const s32 BOTTOMS_MAX = 127;
    static const s32 ONE_PIECE_MAX = 107;
    static const s32 CAP_MAX = 234;
    static const s32 ACCE_MAX = 58;
    static const s32 SHOES_MAX = 111;
    static const s32 UMBRELLA_MAX = 37;

    ClothesList(); // 0x001C7AE8
    virtual ~ClothesList(); // 0x001C7DFC slot 0x00
    // 0x001C7C90 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x001C7474 slot 0x0C
    void Update() override; // 0x001C74FC slot 0x10
    void Draw() override; // 0x001C7448 slot 0x14
    bool IsDecided() const override // 0x0070E59C slot 0x18
    {
        return m_Control.m_pCurrent != NULL || m_Tab.m_Control.m_pCurrent != NULL;
    }
    bool IsDeciding() override // 0x0070E5C4 slot 0x1C
    {
        return m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing() || m_Tab.IsBusy();
    }
    s32 GetSelectedId() const override // 0x0070E638 slot 0x20
    {
        if (m_Selected == -1) {
            return -1;
        }
        return m_pIds[m_ShownTab][m_Selected];
    }
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x001C6F48 slot 0x30
    bool SetupItems() override; // 0x001C77CC slot 0x34
    void InitializeScroll() override; // 0x001C7420 slot 0x38
    void UpdateItemNum() override; // 0x001C73D8 slot 0x44
    void OnEnterIn() override; // 0x001C7780 slot 0x4C
    void OnEnterOut() override; // 0x001C77AC slot 0x54
    void UpdateWait() override; // 0x001C7A7C slot 0x60
    script::IWord* GetItemWord(s32 index) override { return &m_pNames[m_ShownTab][index]; } // 0x0070E670 slot 0x64

    // the icon pane of each tab
    static const char* s_TabPaneNames[TAB_NUM];

    Tab<TAB_NUM> m_Tab;                                 // 0x12CC
    script::WordFix<33>* m_pNames[PART_NUM];            // 0x26C4
    u16* m_pIds[PART_NUM];                              // 0x26E4
    s32 m_ItemNums[TAB_NUM];                            // 0x2704
    script::WordFix<33> m_TopsNames[TOPS_MAX];          // 0x2720
    script::WordFix<33> m_BottomsNames[BOTTOMS_MAX];    // 0xD84C
    script::WordFix<33> m_OnePieceNames[ONE_PIECE_MAX]; // 0x105F0
    script::WordFix<33> m_CapNames[CAP_MAX];            // 0x12C64
    script::WordFix<33> m_AcceNames[ACCE_MAX];          // 0x1807C
    script::WordFix<33> m_ShoesNames[SHOES_MAX];        // 0x19554 shoes and socks
    script::WordFix<33> m_UmbrellaNames[UMBRELLA_MAX];  // 0x1BD38
    u16 m_TopsIds[TOPS_MAX];                            // 0x1CA84
    u16 m_BottomsIds[BOTTOMS_MAX];                      // 0x1CE5E
    u16 m_OnePieceIds[ONE_PIECE_MAX];                   // 0x1CF5C
    u16 m_CapIds[CAP_MAX];                              // 0x1D032
    u16 m_AcceIds[ACCE_MAX];                            // 0x1D206
    u16 m_ShoesIds[SHOES_MAX];                          // 0x1D27A
    u16 m_UmbrellaIds[UMBRELLA_MAX];                    // 0x1D358
    s32 m_ShownTab;                                     // 0x1D3A4 the tab whose items are shown
    s32 m_SetupStep;                                    // 0x1D3A8 of SetupItems
};
ASSERT_OFFSET(ClothesList, m_pNames, 0x26C4);
ASSERT_OFFSET(ClothesList, m_ItemNums, 0x2704);
ASSERT_OFFSET(ClothesList, m_TopsNames, 0x2720);
ASSERT_OFFSET(ClothesList, m_UmbrellaNames, 0x1BD38);
ASSERT_OFFSET(ClothesList, m_TopsIds, 0x1CA84);
ASSERT_OFFSET(ClothesList, m_ShownTab, 0x1D3A4);
ASSERT_SIZE(ClothesList, 0x1D3AC);
