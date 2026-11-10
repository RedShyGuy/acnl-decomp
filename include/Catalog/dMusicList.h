#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "script/dWordFix.h"

// The catalog category Songs: the songs of the current player (item::KIND_MUSIC in its unlockedItems), sorted by the
// sort table. Only the class name is from the RTTI; all other names are ours.
// RTTI 9MusicList @ 0x008CD7B4
// vtable 0x008FA6D4 (vptr 0x008FA6DC), offset_to_top 0, 26 entries
class MusicList : public ::InstSelect<8>
{
public:
    static const s32 ITEM_MAX = 91;

    // (inline, in CatalogBase::Create<MusicList>)
    MusicList()
    {
        m_TextLength = 33;
        m_ScrollBar.m_Unknown0x701 = false;
    }
    virtual ~MusicList(); // 0x006E7890 slot 0x00
    // 0x006E7850 slot 0x04 (deleting dtor)
    s32 GetSelectedId() const override { return m_Selected != -1 ? m_Ids[m_Selected] : -1; } // 0x00770BC4 slot 0x20
    bool SetupItems() override; // 0x006E77FC slot 0x34
    script::IWord* GetItemWord(s32 index) override { return &m_Names[index]; } // 0x00770BE0 slot 0x64

    script::WordFix<33> m_Names[ITEM_MAX];  // 0x12CC
    u16 m_Ids[ITEM_MAX];                    // 0x3380
};
ASSERT_OFFSET(MusicList, m_Ids, 0x3380);
ASSERT_SIZE(MusicList, 0x3438);
