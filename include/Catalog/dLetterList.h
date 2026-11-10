#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "script/dWordFix.h"

// The catalog category Mail paper: the letter papers of the current player (item::KIND_PAPER in its unlockedItems),
// sorted by the sort table. Only the class name is from the RTTI; all other names are ours.
// RTTI 10LetterList @ 0x008CB0FC
// vtable 0x008EC240 (vptr 0x008EC248), offset_to_top 0, 26 entries
class LetterList : public ::InstSelect<8>
{
public:
    static const s32 ITEM_MAX = 67;

    // (inline, in CatalogBase::Create<LetterList>)
    LetterList()
    {
        m_TextLength = 33;
        m_ScrollBar.m_Unknown0x701 = false;
    }
    virtual ~LetterList(); // 0x001AD488 slot 0x00
    // 0x001AD448 slot 0x04 (deleting dtor)
    s32 GetSelectedId() const override { return m_Selected != -1 ? m_Ids[m_Selected] : -1; } // 0x0070BAA4 slot 0x20
    bool SetupItems() override; // 0x001AD3EC slot 0x34
    script::IWord* GetItemWord(s32 index) override { return &m_Names[index]; } // 0x0070BAC0 slot 0x64

    script::WordFix<33> m_Names[ITEM_MAX];  // 0x12CC
    u16 m_Ids[ITEM_MAX];                    // 0x2AE0
};
ASSERT_OFFSET(LetterList, m_Names, 0x12CC);
ASSERT_OFFSET(LetterList, m_Ids, 0x2AE0);
ASSERT_SIZE(LetterList, 0x2B68);
