#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 9OrderList @ 0x008CD7D8
// vtable 0x008FA754 (vptr 0x008FA75C), offset_to_top 0, 26 entries
class OrderList : public ::InstSelect<8>
{
public:
    OrderList(); // ctor candidate(s) 0x007F4E74 (unverified)
    virtual ~OrderList(); // 0x006EF658 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x006EF5FC slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    void Finalize() override; // 0x006EF3E4 slot 0x0C
    s32 GetSelectedId() const override; // 0x00770C6C slot 0x20
    bool vf_0x28() override; // 0x00770C20 slot 0x28
    u32 vf_0x2C() override; // 0x00770C8C slot 0x2C
    bool SetupItems() override; // 0x006EF410 slot 0x34
    void UpdateWait() override; // 0x006EF590 slot 0x60
    script::IWord* GetItemWord(s32 index) override; // 0x00770C74 slot 0x64
};
