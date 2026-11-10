#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "script/dWordFix.h"

// (game code, not decompiled; names are ours) a list of items of the game: OrderList shows them
struct Unknown98C080
{
    static const s32 ITEM_MAX = 128;

    SvFgName items[ITEM_MAX];  // 0x000
    u32 num;                   // 0x200
};
ASSERT_SIZE(Unknown98C080, 0x204);
extern Unknown98C080 g_Unknown98C080;
// (game code) the design argument of item::GetName for the names of OrderList
void* Func6FB8CC(); // 0x006FB8CC
// (game code) a value of the parameters of an item (OrderList::vf_0x28 tests it for 178)
u8 Func769264(const SvFgName* name); // 0x00769264
// (game code) a number of the parameters of an item (OrderList::vf_0x2C returns it * 1.2)
s32 Func3055C8(const SvFgName* name); // 0x003055C8

// The list of the items of g_Unknown98C080: wallpapers, carpets and the other items, each group sorted by the sort
// table; the selected one is shown by the object of Func692FD0. Only the class name is from the RTTI; all other
// names are ours.
// RTTI 9OrderList @ 0x008CD7D8
// vtable 0x008FA754 (vptr 0x008FA75C), offset_to_top 0, 26 entries
class OrderList : public ::InstSelect<8>
{
public:
    static const s32 ITEM_MAX = Unknown98C080::ITEM_MAX;

    // (inline, in CatalogBase::Create<OrderList>)
    OrderList() { m_TextLength = 21; }
    virtual ~OrderList(); // 0x006EF658 slot 0x00
    // 0x006EF5FC slot 0x04 (deleting dtor)
    void Finalize() override; // 0x006EF3E4 slot 0x0C
    // (the index of the selected item, not its ID)
    s32 GetSelectedId() const override { return m_Selected; } // 0x00770C6C slot 0x20
    bool vf_0x28() override; // 0x00770C20 slot 0x28
    u32 vf_0x2C() override; // 0x00770C8C slot 0x2C
    bool SetupItems() override; // 0x006EF410 slot 0x34
    void UpdateWait() override; // 0x006EF590 slot 0x60
    script::IWord* GetItemWord(s32 index) override { return &m_Names[index]; } // 0x00770C74 slot 0x64

    script::WordFix<33> m_Names[ITEM_MAX];  // 0x12CC
    SvFgName m_Items[ITEM_MAX];             // 0x40CC
};
ASSERT_OFFSET(OrderList, m_Items, 0x40CC);
ASSERT_SIZE(OrderList, 0x42CC);
