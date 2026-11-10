#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "script/dWordFix.h"

class ScStage;

// (game code, not decompiled; names are ours) an object of the current scene (NULL on some stages) with items:
// two lists (24 and 16 items) and two single items, a wallpaper and a carpet
void* Func75E9AC(ScStage* scene); // 0x0075E9AC
SvFgName Func71FF94(const void* object, s32 index); // 0x0071FF94
SvFgName Func720184(const void* object, s32 index); // 0x00720184
SvFgName Func2FCC0C(const void* object); // 0x002FCC0C (the item of the name at +2)
SvFgName Func7202FC(const void* object); // 0x007202FC (the item of the name at +4)

// The list of the items of the object of Func75E9AC: wallpapers, carpets and the other items, each group sorted by
// the sort table; the selected one is shown by the object of Func692FD0. Only the class name is from the RTTI; all
// other names are ours.
// RTTI 10TicketList @ 0x008CB208
// vtable 0x008EC584 (vptr 0x008EC58C), offset_to_top 0, 26 entries
class TicketList : public ::InstSelect<8>
{
public:
    static const s32 ITEM_MAX = 42;
    static const s32 LIST0_NUM = 24;
    static const s32 LIST1_NUM = 16;

    // (inline, in CatalogBase::Create<TicketList>)
    TicketList() { m_TextLength = 21; }
    virtual ~TicketList(); // 0x001B7890 slot 0x00
    // 0x001B7834 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x001B74F8 slot 0x0C
    // (the index of the selected item, not its ID)
    s32 GetSelectedId() const override { return m_Selected; } // 0x0070CE18 slot 0x20
    bool vf_0x28() override { return false; } // 0x0070CE10 slot 0x28
    u32 vf_0x2C() override; // 0x0070CE38 slot 0x2C
    bool SetupItems() override; // 0x001B7524 slot 0x34
    void UpdateWait() override; // 0x00692554 slot 0x60
    script::IWord* GetItemWord(s32 index) override { return &m_Names[index]; } // 0x0070CE20 slot 0x64

    script::WordFix<33> m_Names[ITEM_MAX];  // 0x12CC
    SvFgName m_Items[ITEM_MAX];             // 0x21E4
};
ASSERT_OFFSET(TicketList, m_Items, 0x21E4);
ASSERT_SIZE(TicketList, 0x228C);
