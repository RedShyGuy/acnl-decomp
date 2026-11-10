#pragma once

#include <cstring>

#include "decomp.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_Types.h"
#include "nw/ut/ut_LinkList.h"
#include "ssys/ma/lyt/dLayout.h"

namespace nw {
namespace lyt {
class AnimResource;
class TextBox;
} // namespace lyt
} // namespace nw
namespace sead {
class Heap;
}
namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
}
} // namespace ma
} // namespace ssys

namespace lyt {
class AnmObject;
class TagProcessor;

// A layout of the game on the lower screen: an ssys layout with its animations (AnmObject), in a global list sorted
// by priority that is calculated and drawn each frame, optionally attached to a pane of a parent object (its root
// pane becomes a child of that pane), with helpers for the text boxes (texts of the message file with their style,
// shrinking the font to the width, cutting the text to a number of lines). Only the class name is from the binary
// (RTTI); member and function names are ours.
// RTTI N3lyt6ObjectE @ 0x008D0F88
// vtable 0x00904544 (vptr 0x0090454C), offset_to_top 0, 4 entries
class Object
{
public:
    typedef nw::ut::LinkList<Object, 0x4> List;
    typedef nw::ut::LinkList<Object, 0xC> ChildList;
    typedef nw::ut::LinkList<AnmObject, 0x20> AnmObjectList;

    // the original font size of a text box whose font was shrunk to fit its width
    struct FontSizeEntry
    {
        nw::lyt::TextBox* pTextBox;  // 0x00
        nw::lyt::Size size;          // 0x04
        nw::ut::LinkListNode link;   // 0x0C
    };
    typedef nw::ut::LinkList<FontSizeEntry, 0xC> FontSizeList;

    // the values of m_State
    enum State
    {
        STATE_NONE,
        STATE_1,
        STATE_CHANGED = 2,  // panes were changed: the layout is animated in the next Calc
        STATE_FROZEN = 3    // Calc does nothing
    };
    // objects of this priority or higher do not animate their layout in CalcAll
    static const s32 PRIORITY_NO_ANIMATE = 9999;

    // a callback before DrawAll (with its isRestart)
    typedef void (*DrawCallback)(bool isRestart, void* arg);

    Object(u32 unknown, s32 priority, sead::Heap* heap); // 0x0050A2DC
    virtual ~Object(); // 0x0050A3B0 slot 0x00
    // 0x0050A39C slot 0x04 (deleting dtor)
    // the animations advance; with isAnimate the layout is animated if something changed, else the parent is told
    virtual void Calc(bool isAnimate); // 0x00509D40 slot 0x08
    // the layout is registered for drawing (a child is drawn with its parent)
    virtual void Draw(); // 0x00509C38 slot 0x0C

    // the layout of the archive is built; the object goes into the global list
    void Build(ssys::ma::lyt::ArcResAccReader* arcReader, const char* name, u32 cmdCacheSize); // 0x00509C64

    // all objects: the animations advance / the layouts are drawn (isRestart: from the start of the list; else
    // from where the last call stopped) / all objects are deleted
    static void CalcAll(); // 0x0050A68C
    static void DrawAll(bool isRestart); // 0x0050A57C
    static void DestroyAll(); // 0x0050A60C

    // the panes of the group are shown or hidden
    void SetGroupVisible(const char* groupName, bool visible); // 0x00509650
    // the pane and its children are shown or hidden (the children only if the pane changed)
    DECOMP_NOINLINE void SetPaneVisible(bool visible, nw::lyt::Pane* pane); // 0x00509818
    // an animation of the archive for the panes of a group (NULL: the first group of the animation)
    AnmObject* CreateAnmObject(const char* animName, const char* groupName); // 0x00509884
    // an AnmObject for each pane of the tree (or its material) that has the animation bound; their number
    DECOMP_NOINLINE int CreatePaneAnmObjects(nw::lyt::Pane* pane, const nw::lyt::AnimResource& animRes, AnmObject** anmObjects, int max); // 0x0050A6EC
    // the tag processor of all text boxes below the pane
    DECOMP_NOINLINE void SetTagProcessor(nw::lyt::Pane* pane, nw::font::TagProcessorBase<wchar_t>* tagProcessor); // 0x0050994C
    // the font of the text box is narrowed (to 80 % at least) so that the text fits its width; the original size
    // (kept for the next texts) is returned
    void FitFontSize(nw::lyt::TextBox* textBox, const wchar_t* text, nw::lyt::Size* originalSize); // 0x005099F4
    // the text is copied, cut so that it fits into the lines of the text box
    void TruncateText(wchar_t* dst, int dstLength, const wchar_t* src, nw::lyt::TextBox* textBox, int lineNum); // 0x005096B8
    // the text box gets the text of the label (or the given text) with the line and character space of its style;
    // fitted to the width and cut to lineNum lines (lineNum > 0)
    void SetMessage(const char* label, nw::lyt::TextBox* textBox, const wchar_t* text, bool isFit, int lineNum); // 0x00509E14
    void SetMessage(const char* label, const char* paneName, const wchar_t* text, bool isFit, int lineNum); // 0x0050A044
    // the text box gets the text (length 0: up to the end; fitted if the user data of the box starts with '!')
    void SetText(const wchar_t* text, nw::lyt::TextBox* textBox, bool isFit, u16 length, int lineNum); // 0x0050A184
    void SetText(const wchar_t* text, const char* paneName, bool isFit, u16 length, int lineNum); // 0x0050A0E4

    // (inline) the root pane of the child goes below the pane of this layout
    DECOMP_EMIT_COPY void AttachChild(const char* paneName, Object* child) // 0x00747968
    {
        child->GetRootPane()->m_Flag = (child->GetRootPane()->m_Flag & ~nw::lyt::Pane::FLAG_INFLUENCED_ALPHA) | nw::lyt::Pane::FLAG_INFLUENCED_ALPHA;
        std::strncpy(child->m_ParentPaneName, paneName, 16);
        child->m_pParent = this;
        m_pChildren->PushBack(child);
        GetRootPane()->FindPaneByName(paneName, true)->AppendChild(child->GetRootPane());
    }
    // (inline)
    DECOMP_EMIT_COPY void DetachChild(Object* child) // 0x007479D8
    {
        m_pChildren->Erase(child);
        child->ClearParent();
    }
    // (inline)
    DECOMP_EMIT_COPY nw::lyt::Pane* FindPane(const char* name) // 0x00747A30
    {
        return GetRootPane()->FindPaneByName(name, true);
    }
    nw::lyt::Pane* GetRootPane() { return m_Layout.GetRootPane(); }
    // (inline) the object is calculated and drawn or not; an attached one shows or hides its root pane with it
    void SetEnabled(bool enabled)
    {
        if (m_IsEnabled != enabled) {
            m_IsEnabled = enabled;
            if (m_ParentPaneName[0] != '\0') {
                SetPaneVisible(enabled, GetRootPane());
            }
        }
    }

    static List s_List;                                 // the objects by priority (highest first)
    static List::iterator s_DrawIterator;               // where DrawAll goes on
    static DrawCallback s_pDrawCallback;                // (never set in the binary)
    static void* s_DrawCallbackArg;
    static bool s_IsSetTagProcessor;                    // (never set in the binary)
    static TagProcessor s_TagProcessor;                 // with s_IsSetTagProcessor, the tag processor of the text boxes

private:
    // (inline) the root pane leaves the pane of the parent
    void ClearParent()
    {
        char empty[16] = {};
        std::strncpy(m_ParentPaneName, empty, 16);
        m_pParent = NULL;
        GetRootPane()->m_pParent->RemoveChild(GetRootPane());
    }

public:
    nw::ut::LinkListNode m_Link;                 // 0x004 in s_List
    nw::ut::LinkListNode m_ChildLink;            // 0x00C in the child list of the parent
    AnmObjectList m_AnmObjectList;               // 0x014
    ssys::ma::lyt::ArcResAccReader* m_pArcReader; // 0x020
    sead::Heap* m_pHeap;                         // 0x024 the AnmObjects and lists (NULL: the heap of the layouts)
    ssys::ma::lyt::Layout m_Layout;              // 0x028
    ChildList* m_pChildren;                      // 0x174 (made by Build)
    Object* m_pParent;                           // 0x178
    u32 m_Unknown0x17C;                          // 0x17C (from the constructor; TextArea passes a field of itself)
    s32 m_Priority;                              // 0x180
    s32 m_State;                                 // 0x184 State
    bool m_IsEnabled;                            // 0x188 calculated and drawn by CalcAll / DrawAll
    bool m_IsAnimated;                           // 0x189 an animation was applied (by this object or a child)
    u8 m_Unknown0x18A;                           // 0x18A
    char m_ParentPaneName[17];                   // 0x18B the pane of the parent the root pane is attached to
    u32 m_Unknown0x19C;                          // 0x19C
    f32 m_SoundPan;                              // 0x1A0 (the game plays the sounds of its buttons with it)
    FontSizeList m_FontSizeList;                 // 0x1A4
};
ASSERT_OFFSET(Object, m_Layout, 0x28);
ASSERT_OFFSET(Object, m_pChildren, 0x174);
ASSERT_OFFSET(Object, m_ParentPaneName, 0x18B);
ASSERT_OFFSET(Object, m_FontSizeList, 0x1A4);
ASSERT_SIZE(Object, 0x1B0);
} // namespace lyt
