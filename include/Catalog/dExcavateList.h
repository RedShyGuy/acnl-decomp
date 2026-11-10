#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "List/dTab.h"
#include "script/dWordFix.h"

// The catalog category Gyroids & Fossils (lists of the items of the current player) with two tabs, named after
// their icon panes: P_clay_00 (gyroids, item::KIND_HANIWA) and P_fsl_00 (item::KIND_FOSSIL); each tab has its items
// sorted by the sort table and keeps its selection and scroll. The items are collected in the first frame. Only the
// class name is from the RTTI; all other names are ours.
// RTTI 12ExcavateList @ 0x008CB608
// vtable 0x008EE434 (vptr 0x008EE43C), offset_to_top 0, 26 entries
class ExcavateList : public ::InstSelect<8>
{
public:
    static const s32 TAB_NUM = 2;
    static const s32 HANIWA_MAX = 131;  // gyroids
    static const s32 FOSSIL_MAX = 67;

    ExcavateList(); // 0x00200A30
    virtual ~ExcavateList(); // 0x00200C38 slot 0x00
    // 0x00200B50 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x0020054C slot 0x0C
    void Update() override; // 0x002005D4 slot 0x10
    void Draw() override; // 0x00200520 slot 0x14
    bool IsDecided() const override // 0x00713050 slot 0x18
    {
        return m_Control.m_pCurrent != NULL || m_Tab.m_Control.m_pCurrent != NULL;
    }
    bool IsDeciding() override // 0x00713078 slot 0x1C
    {
        return m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing() || m_Tab.IsBusy();
    }
    s32 GetSelectedId() const override // 0x007130EC slot 0x20
    {
        if (m_Selected == -1) {
            return -1;
        }
        return m_pIds[m_ShownTab][m_Selected];
    }
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x00200038 slot 0x30
    bool SetupItems() override; // 0x002008A8 slot 0x34
    void InitializeScroll() override; // 0x002004FC slot 0x38
    void UpdateItemNum() override; // 0x002004BC slot 0x44
    void OnEnterIn() override; // 0x0020085C slot 0x4C
    void OnEnterOut() override; // 0x00200888 slot 0x54
    void UpdateWait() override; // 0x002009AC slot 0x60
    script::IWord* GetItemWord(s32 index) override { return &m_pNames[m_ShownTab][index]; } // 0x00713124 slot 0x64

    // the icon pane of each tab
    static const char* s_TabPaneNames[TAB_NUM];

    Tab<TAB_NUM> m_Tab;                             // 0x12CC
    script::WordFix<33>* m_pNames[TAB_NUM];         // 0x1A30
    u16* m_pIds[TAB_NUM];                           // 0x1A38
    s32 m_ItemNums[TAB_NUM];                        // 0x1A40
    script::WordFix<33> m_HaniwaNames[HANIWA_MAX];  // 0x1A48 gyroids
    script::WordFix<33> m_FossilNames[FOSSIL_MAX];  // 0x495C
    u16 m_HaniwaIds[HANIWA_MAX];                    // 0x6170
    u16 m_FossilIds[FOSSIL_MAX];                    // 0x6276
    s32 m_ShownTab;                                 // 0x62FC the tab whose items are shown
    s32 m_SetupStep;                                // 0x6300 of SetupItems
};
ASSERT_OFFSET(ExcavateList, m_pNames, 0x1A30);
ASSERT_OFFSET(ExcavateList, m_HaniwaNames, 0x1A48);
ASSERT_OFFSET(ExcavateList, m_FossilIds, 0x6276);
ASSERT_OFFSET(ExcavateList, m_ShownTab, 0x62FC);
ASSERT_SIZE(ExcavateList, 0x6304);
