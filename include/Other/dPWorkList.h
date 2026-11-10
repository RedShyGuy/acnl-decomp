#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "script/dWordFix.h"

namespace script {
class WordPtr;
} // namespace script

// (game code, not decompiled; names are ours) the 103 public works projects: whether one is offered (two tests),
// its entry in the project tables (252: none), the price of an entry (bells) and its name
bool Func5CACCC(u8 project); // 0x005CACCC
bool Func6CBAC8(u8 project); // 0x006CBAC8
u8 Func5CAE10(u8 project); // 0x005CAE10
u32 Func5CACA4(u8 entry); // 0x005CACA4
void Func56E4B4(script::WordPtr* word, u8 entry); // 0x0056E4B4
// (game code) the object that shows the selected project (pointer at 0x009516B0): show one / hide
struct Unknown9516B0;
extern Unknown9516B0* g_pUnknown9516B0;
void Func229054(Unknown9516B0* object, u8 project); // 0x00229054
void Func2274A4(Unknown9516B0* object); // 0x002274A4

// The list of the public works projects that can be ordered. Only the class name is from the RTTI ("PWork" =
// public works); all other names are ours.
// RTTI 9PWorkList @ 0x008CD7E4
// vtable 0x008FA7C4 (vptr 0x008FA7CC), offset_to_top 0, 26 entries
class PWorkList : public ::InstSelect<8>
{
public:
    static const s32 PROJECT_NUM = 103;

    // (inline, in CatalogBase::Create<PWorkList>)
    PWorkList() { m_TextLength = 38; }
    virtual ~PWorkList(); // 0x006EF868 slot 0x00
    // 0x006EF828 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x006EF6F4 slot 0x0C
    s32 GetSelectedId() const override { return m_Selected != -1 ? m_Ids[m_Selected] : -1; } // 0x00770CD8 slot 0x20
    bool vf_0x28() override { return false; } // 0x00770CD0 slot 0x28
    // the price of the selected project
    u32 vf_0x2C() override; // 0x00770D0C slot 0x2C
    bool SetupItems() override; // 0x006EF714 slot 0x34
    void OnSelect(s32 selected, s32 prev) override; // 0x006EF6B0 slot 0x40
    void OnEnterOut() override; // 0x006EF704 slot 0x54
    void OnEnterWait() override; // 0x006EF824 slot 0x5C
    void UpdateWait() override; // 0x006EF7AC slot 0x60
    script::IWord* GetItemWord(s32 index) override { return &m_Names[index]; } // 0x00770CF4 slot 0x64

    script::WordFix<38> m_Names[PROJECT_NUM];  // 0x12CC
    u8 m_Ids[PROJECT_NUM];                     // 0x3B08 the projects
};
ASSERT_OFFSET(PWorkList, m_Ids, 0x3B08);
ASSERT_SIZE(PWorkList, 0x3B70);
