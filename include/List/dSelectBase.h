#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Button/dButtonActionNode.h"
#include "List/dCatalogBase.h"
#include "List/dContentNode.h"
#include "Item/item_Api.h"
#include "script/dWordFix.h"

class SvFgName;

// (game functions on item names and item lists, purpose unknown; used by the list helpers of SelectBase)
bool Func76B6A0(const SvFgName* name); // 0x0076B6A0
void Func6BA2E8(SvFgName* name); // 0x006BA2E8
bool Func76BE54(const SvFgName* name); // 0x0076BE54
void Func6BBCF0(SvFgName* name); // 0x006BBCF0
bool Func2FE704(const SvFgName* a, const SvFgName* b); // 0x002FE704
bool Func76BA20(const SvFgName* a, const SvFgName* b); // 0x0076BA20
bool Func76A66C(const SvFgName* name); // 0x0076A66C
// a list of item IDs (7104 bytes) built from a bit field of the player
void Func6F05B8(void* list); // 0x006F05B8
void Func6F0438(void* list, const u32* bits, u8 group, u32 value); // 0x006F0438
void Func6F02F4(void* list, const u32* bits, u8 group); // 0x006F02F4
void Func6F0608(void* list, u32 value); // 0x006F0608
s32 Func7712C0(void* list); // 0x007712C0
SvFgName::ID Func771278(void* list, u8 group, s32 index); // 0x00771278

namespace oml {
namespace framework {
class Process;
} // namespace framework
} // namespace oml
// (game code, not decompiled; names are ours) an object that shows an item while it is selected in a list: the
// process that shows it and the item
struct Unknown950338
{
    oml::framework::Process* m_pProcess;  // 0x0
    SvFgName m_Name;                      // 0x4
};
Unknown950338* Func692FD0(); // 0x00692FD0 (returns the object 0x00950338)
void Func692330(Unknown950338* object, const SvFgName& name); // 0x00692330
bool Func6925C0(Unknown950338* object); // 0x006925C0 (stops the process, the item becomes empty)

// A CatalogBase whose rows are buttons (B_list_NN / G_list_NN in the panes N_list_NN): one item is selected with
// a cursor pane (P_slct_00) that moves into the row, and the rows show the items by GetItemWord. The file also has
// the helpers of the item lists derived from it (sorting by Item/Param/Sort<region>.bin, collecting IDs, names).
// Only the class name is from the RTTI; all other names are ours.
// RTTI 10SelectBase @ 0x008CB1D0
// vtable 0x008EC420 (vptr 0x008EC428), offset_to_top 0, 26 entries
class SelectBase : public ::CatalogBase
{
public:
    SelectBase(); // 0x001B087C
    virtual ~SelectBase(); // 0x001B099C slot 0x00
    // 0x001B08E0 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x001B032C slot 0x0C
    void Update() override; // 0x001B03B8 slot 0x10
    bool IsDecided() const override { return m_Control.m_pCurrent != NULL; } // 0x0070BDA0 slot 0x18
    bool IsDeciding() override { return m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing(); } // 0x0070BDB0 slot 0x1C
    script::IWord* GetSelectedWord() override // 0x005EAFF0 slot 0x24 (falls into script::GetEmptyWord)
    {
        if (m_Selected != -1) {
            return GetItemWord(m_Selected);
        }
        return script::GetEmptyWord();
    }
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x001AFA44 slot 0x30
    void OnScroll() override; // 0x001AFF6C slot 0x3C
    void OnSelect(s32 selected, s32 prev) override; // 0x001AF8FC slot 0x40
    void ApplyScroll() override; // 0x001B01E8 slot 0x48
    void UpdateWait() override; // 0x001B073C slot 0x60
    // the name of an item
    virtual script::IWord* GetItemWord(s32 index) = 0; // slot 0x64

    // the button of a row can be chosen (not the last two rows, which are only partly visible)
    void SetRowActive(s32 row, bool active); // 0x001AFA20
    // sorts IDs / names by the sort table
    void SortIds(u16* ids, s32 num); // 0x001AFED0
    void SortNames(SvFgName* names, s32 num); // 0x001B0114
    // adds a name to a list unless an equal one is there
    void AddName(SvFgName* names, s32* num, const SvFgName& name); // 0x001B022C
    // the names of items into texts
    void SetNames(script::WordFix<33>* words, const u16* ids, u32 num); // 0x001B05CC
    // the IDs of the items of a group in the bit field of the current player (unlockedItems); returns the number
    s32 CollectIds(u16* ids, u8 group, bool isSome); // 0x001B062C

    // (inline) the group of an item in the lists sorted by kind: 0 wallpapers, 1 carpets, 2 the others
    static s32 GetKindGroup(const SvFgName& name)
    {
        const item::Kind kind = name.GetKind();
        if (kind == item::KIND_DUMMY_WALL) {
            return 0;
        }
        if (kind == item::KIND_CARPET) {
            return 1;
        }
        return 2;
    }

    // the sort table of the region (Item/Param/Sort<region>.bin)
    static void LoadSortTable(); // 0x001B054C
    static void UnloadSortTable(); // 0x001AFA14

    // (not in the unnamed namespace: the binary never writes them, GCC would fold them there)
    static const char* s_ListPaneName;
    static const char* s_SortFiles[10];

    ContentNode m_LineNode;                // 0x9E0 N_line_00 scrolls with the rows
    ButtonActionNode* m_pNodes;            // 0x9FC one per row (set by the derived classes)
    ButtonActionControl m_Control;         // 0xA00
    nw::lyt::Pane* m_pCursorPane;          // 0xAE4 P_slct_00, a child of the selected row
    s32 m_Selected;                        // 0xAE8 the selected item (-1: none)
};
ASSERT_OFFSET(SelectBase, m_LineNode, 0x9E0);
ASSERT_OFFSET(SelectBase, m_Control, 0xA00);
ASSERT_OFFSET(SelectBase, m_Selected, 0xAE8);
ASSERT_SIZE(SelectBase, 0xAEC);
