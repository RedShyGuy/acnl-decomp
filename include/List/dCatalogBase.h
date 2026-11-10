#pragma once

#include "decomp.h"
#include "List/dCommonScrollBar.h"
#include "Other/dLayout.h"
#include "Sv/dSvFgName.h"
#include "script/dInsert.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "state/dMode.h"

namespace nw {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw
namespace sead {
class Heap;
} // namespace sead
namespace script {
class IWord;
class RenderGarden;
} // namespace script
namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
} // namespace lyt
} // namespace ma
} // namespace ssys
class ContentNode;

// (game functions on an item name, purpose unknown; the catalog slots 0x28/0x2C return them for the selected item)
bool Func76D334(const SvFgName* name); // 0x0076D334
u32 Func76AA18(const SvFgName* name); // 0x0076AA18

// The base of the scrolling lists of the menus (catalog, letters, clothes, music, ...): a layout "<name>.bclyt" with
// the animations "<name>_in"/"<name>_out", rows of text boxes "<text box>_NN" with one RenderGarden each, a
// CommonScrollBar for the items, and a state machine (do nothing, enter, exit, wait, list fade-out, list fade-in)
// whose state names are in the binary. Only the class name is from the RTTI; all other names are ours.
// RTTI 11CatalogBase @ 0x008CB290
// vtable 0x008EC9AC (vptr 0x008EC9B4), offset_to_top 0, 25 entries
class CatalogBase : public ::state::Mode<CatalogBase>
{
public:
    CatalogBase(); // 0x001C5B6C
    // a list of the kind T on the heap of the layouts (the menu that shows the lists has a table of these; the
    // instances are made next to the lists)
    template <typename T>
    static CatalogBase* Create()
    {
        return new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) T();
    }
    virtual ~CatalogBase(); // 0x001C5CF4 slot 0x00
    // 0x001C5C50 slot 0x04 (deleting dtor)
    const char* GetName(Func func) const override; // 0x0082AA40 slot 0x08
    virtual void Finalize(); // 0x002F88F4 slot 0x0C
    virtual void Update(); // 0x001C59B0 slot 0x10
    virtual void Draw(); // 0x001C5974 slot 0x14
    // an item was chosen
    virtual bool IsDecided() const { return false; } // 0x0070E470 slot 0x18
    // the buttons of the rows are being chosen
    virtual bool IsDeciding() { return false; } // 0x0070E4D4 slot 0x1C
    // the ID of the selected item (-1: none)
    virtual s32 GetSelectedId() const { return -1; } // 0x0070E4DC slot 0x20
    // the name of the selected item
    virtual script::IWord* GetSelectedWord() { return script::GetEmptyWord(); } // 0x0070E4D0 slot 0x24
    // Func76D334 of the selected item (true without one)
    virtual bool vf_0x28() // 0x0070E478 slot 0x28
    {
        const s32 id = GetSelectedId();
        if (id == -1) {
            return true;
        }
        SvFgName name(static_cast<SvFgName::ID>(id));
        return Func76D334(&name);
    }
    // Func76AA18 of the selected item (0 without one)
    virtual u32 vf_0x2C() // 0x0070E4E4 slot 0x2C
    {
        const s32 id = GetSelectedId();
        if (id == -1) {
            return 0;
        }
        SvFgName name(static_cast<SvFgName::ID>(id));
        return Func76AA18(&name);
    }
    // the panes of the rows (sets m_pContents)
    virtual void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) = 0; // slot 0x30
    // the items of the list; Setup waits until it returns true
    virtual bool SetupItems() = 0; // slot 0x34
    // the scroll at the start
    virtual void InitializeScroll(); // 0x002988A0 slot 0x38
    // the scroll changed (the rows show other items)
    virtual void OnScroll() = 0; // slot 0x3C
    // the selection of the cursor keys changed
    virtual void OnSelect(s32 selected, s32 prev); // 0x001C559C slot 0x40
    // sets m_ItemNum
    virtual void UpdateItemNum(); // 0x001C5948 slot 0x44
    // the scroll bar takes over a new scroll without input
    virtual void ApplyScroll(); // 0x00299F00 slot 0x48
    virtual void OnEnterIn(); // 0x001C5AD4 slot 0x4C
    virtual void UpdateIn(); // 0x001C5A6C slot 0x50
    virtual void OnEnterOut(); // 0x001C5B60 slot 0x54
    virtual void UpdateOut(); // 0x001C5AFC slot 0x58
    virtual void OnEnterWait(); // 0x001C5B68 slot 0x5C
    virtual void UpdateWait(); // 0x001C5B64 slot 0x60

    // loads the layout, the texts and the scroll bar over several frames; true when done
    bool Setup(ssys::ma::lyt::ArcResAccReader* arcReader, sead::Heap* heap); // 0x001C51E0
    // the items are counted again and the list starts at its scroll
    void Reset(); // 0x001C55A0
    void StartIn(); // 0x001C5998
    void StartOut(); // 0x001C5AD8

    // the states (the table 0x0087B05C names them)
    void State_None(); // 0x001C574C
    void Enter_None(); // 0x001C575C
    void State_In(); // 0x001C564C
    void Enter_In(); // 0x001C5658
    void State_Out(); // 0x001C56F8
    void Enter_Out(); // 0x001C5704
    void State_Wait(); // 0x001C5750
    void Enter_Wait(); // 0x001C5760
    void State_ListFadeOut(); // 0x001C57FC
    void Enter_ListFadeOut(); // 0x001C58E8
    void State_ListFadeIn(); // 0x001C576C
    void Enter_ListFadeIn(); // 0x001C57C8

    // the scissor of the layout is the frame of the rows
    DECOMP_NOINLINE void UpdateScissor(); // 0x001C569C

    static void OnScrollCallback(void* container); // 0x001C5AF0
    static void OnSelectCallback(void* container, s32 selected, s32 prev); // 0x001C5590

    const char* m_LayoutName;              // 0x024
    const char* m_TextBoxName;             // 0x028 "T_itm"
    Layout m_Layout;                       // 0x02C
    ssys::ma::lyt::Animation m_InAnim;     // 0x178 <name>_in
    ssys::ma::lyt::Animation m_OutAnim;    // 0x1A0 <name>_out
    ssys::ma::lyt::Animation m_ListOutAnim; // 0x1C8 (set up by the derived classes)
    ssys::ma::lyt::Animation m_ListInAnim; // 0x1F0
    nw::lyt::Pane* m_pFrame;               // 0x218 N_frame_00 (or N_Frame_Pos)
    script::RenderGarden* m_pRenders;      // 0x21C one per row
    ContentNode* m_pContents;              // 0x220 the rows
    CommonScrollBar m_ScrollBar;           // 0x224
    ScrollBarSize m_ScrollBarSize;         // 0x9BC
    s32 m_RowNum;                          // 0x9C0
    s32 m_ItemNum;                         // 0x9C4
    u32 m_TextLength;                      // 0x9C8 of the RenderGardens
    f32 m_RowSpace;                        // 0x9CC the distance of two rows
    bool m_IsAnimating;                    // 0x9D0 in or out
    bool m_IsScrollBarStopped;             // 0x9D1
    bool m_IsListFading;                   // 0x9D2
    bool m_IsListFadeSound;                // 0x9D3
    Func m_ListFadeNext;                   // 0x9D4 the state after the list fade-out
    s32 m_SetupStep;                       // 0x9DC
};
ASSERT_OFFSET(CatalogBase, m_Layout, 0x2C);
ASSERT_OFFSET(CatalogBase, m_ScrollBar, 0x224);
ASSERT_OFFSET(CatalogBase, m_ListFadeNext, 0x9D4);
ASSERT_SIZE(CatalogBase, 0x9E0);
