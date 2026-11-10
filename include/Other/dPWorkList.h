#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 9PWorkList @ 0x008CD7E4
// vtable 0x008FA7C4 (vptr 0x008FA7CC), offset_to_top 0, 26 entries
class PWorkList : public ::InstSelect<8>
{
public:
    PWorkList(); // ctor candidate(s) 0x007F4EE8 (unverified)
    virtual ~PWorkList(); // 0x006EF868 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x006EF828 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    void Finalize() override; // 0x006EF6F4 slot 0x0C
    s32 GetSelectedId() const override; // 0x00770CD8 slot 0x20
    bool vf_0x28() override; // 0x00770CD0 slot 0x28
    u32 vf_0x2C() override; // 0x00770D0C slot 0x2C
    bool SetupItems() override; // 0x006EF714 slot 0x34
    void OnSelect(s32 selected, s32 prev) override; // 0x006EF6B0 slot 0x40
    void OnEnterOut() override; // 0x006EF704 slot 0x54
    void OnEnterWait() override; // 0x006EF824 slot 0x5C
    void UpdateWait() override; // 0x006EF7AC slot 0x60
    script::IWord* GetItemWord(s32 index) override; // 0x00770CF4 slot 0x64
};
